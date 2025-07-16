#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>

void parseCommandLine(int argc, char* argv[], std::string& inputPath, 
                     std::string& outputPath, std::string& kernelPath) {
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        
        if (arg == "--input" && i + 1 < argc) {
            inputPath = argv[++i];
        } else if (arg == "--output" && i + 1 < argc) {
            outputPath = argv[++i];
        } else if (arg == "--kernel_file" && i + 1 < argc) {
            kernelPath = argv[++i];
        }
    }
    
    if (inputPath.empty() || outputPath.empty() || kernelPath.empty()) {
        std::cerr << "Использование: " << argv[0] << " --input <путь_к_изображению> "
                  << "--output <выходной_файл> --kernel_file <файл_ядра>" << std::endl;
        exit(1);
    }
}

std::vector<std::vector<float>> readKernelFromFile(const std::string& filePath) {
    std::ifstream file(filePath);
    
    if (!file.is_open()) {
        std::cerr << "Ошибка: не удалось открыть файл с ядром " << filePath << std::endl;
        exit(1);
    }
    
    int kernelSize;
    file >> kernelSize;
    
    if (kernelSize <= 0 || kernelSize % 2 == 0) {
        std::cerr << "Ошибка: некорректный размер ядра в файле" << std::endl;
        exit(1);
    }
    
    std::vector<std::vector<float>> kernel(kernelSize, std::vector<float>(kernelSize));
    
    for (int i = 0; i < kernelSize; i++) {
        for (int j = 0; j < kernelSize; j++) {
            if (!(file >> kernel[i][j])) {
                std::cerr << "Ошибка: некорректные данные в файле ядра" << std::endl;
                exit(1);
            }
        }
    }
    
    file.close();
    return kernel;
}

cv::Vec3b getPixelWithMirror(const cv::Mat& image, int x, int y) {
    int width = image.cols;
    int height = image.rows;
    
    if (x < 0) {
        x = -x - 1;
    } else if (x >= width) {
        x = 2 * width - x - 1;
    }
    
    if (y < 0) {
        y = -y - 1;
    } else if (y >= height) {
        y = 2 * height - y - 1;
    }
    
    if (image.channels() == 3) {
        return image.at<cv::Vec3b>(y, x);
    } 
    else if (image.channels() == 1) {
        uchar pixel = image.at<uchar>(y, x);
        return cv::Vec3b(pixel, pixel, pixel);
    }
    
    return cv::Vec3b(0, 0, 0);
}

cv::Mat applyCustomKernel(const cv::Mat& inputImage, const std::vector<std::vector<float>>& kernel) {
    cv::Mat outputImage = inputImage.clone();
    
    int channels = inputImage.channels();
    int kernelSize = kernel.size();
    int halfKernel = kernelSize / 2;
    
    for (int y = 0; y < inputImage.rows; y++) {
        for (int x = 0; x < inputImage.cols; x++) {
            std::vector<float> sumChannels(channels, 0.0f);
            
            for (int ky = 0; ky < kernelSize; ky++) {
                for (int kx = 0; kx < kernelSize; kx++) {
                    int nx = x + (kx - halfKernel);
                    int ny = y + (ky - halfKernel);
                    
                    cv::Vec3b pixel = getPixelWithMirror(inputImage, nx, ny);
                    
                    for (int c = 0; c < channels; c++) {
                        sumChannels[c] += pixel[c] * kernel[ky][kx];
                    }
                }
            }
            
            if (channels == 3) {
                for (int c = 0; c < channels; c++) {
                    sumChannels[c] = std::min(std::max(sumChannels[c], 0.0f), 255.0f);
                }
                
                outputImage.at<cv::Vec3b>(y, x) = cv::Vec3b(
                    static_cast<uchar>(sumChannels[0]),
                    static_cast<uchar>(sumChannels[1]),
                    static_cast<uchar>(sumChannels[2])
                );
            } 
            else if (channels == 1) {
                sumChannels[0] = std::min(std::max(sumChannels[0], 0.0f), 255.0f);
                outputImage.at<uchar>(y, x) = static_cast<uchar>(sumChannels[0]);
            }
        }
    }
    
    return outputImage;
}

void printKernelInfo(const std::vector<std::vector<float>>& kernel) {
    int size = kernel.size();
    
    std::cout << "Применяемое ядро размером " << size << "x" << size << ":" << std::endl;
    
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            std::cout << kernel[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

int main(int argc, char* argv[]) {
    std::string inputPath;
    std::string outputPath;
    std::string kernelPath;
    
    parseCommandLine(argc, argv, inputPath, outputPath, kernelPath);
    
    std::vector<std::vector<float>> kernel = readKernelFromFile(kernelPath);
    printKernelInfo(kernel);
    
    cv::Mat inputImage = cv::imread(inputPath, cv::IMREAD_UNCHANGED);
    
    if (inputImage.empty()) {
        std::cerr << "Ошибка: не удалось загрузить изображение " << inputPath << std::endl;
        return 1;
    }
    
    std::cout << "Применение пользовательского ядра к изображению " << inputPath << std::endl;
    
    cv::Mat outputImage = applyCustomKernel(inputImage, kernel);
    
    bool success = cv::imwrite(outputPath, outputImage);
    
    if (success) {
        std::cout << "Изображение с применённым ядром сохранено в " << outputPath << std::endl;
    } else {
        std::cerr << "Ошибка: не удалось сохранить изображение " << outputPath << std::endl;
        return 1;
    }
    
    return 0;
} 