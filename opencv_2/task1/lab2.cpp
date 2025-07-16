#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>

void parseCommandLine(int argc, char* argv[], std::string& inputPath, 
                     std::string& outputPath, int& kernelSize) {
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        
        if (arg == "--input" && i + 1 < argc) {
            inputPath = argv[++i];
        } else if (arg == "--output" && i + 1 < argc) {
            outputPath = argv[++i];
        } else if (arg == "--kernel" && i + 1 < argc) {
            kernelSize = std::stoi(argv[++i]);
            
            if (kernelSize % 2 == 0) {
                std::cerr << "Ошибка: размер ядра должен быть нечетным числом" << std::endl;
                exit(1);
            }
        }
    }
    
    if (inputPath.empty() || outputPath.empty() || kernelSize <= 0) {
        std::cerr << "Использование: " << argv[0] << " --input <путь_к_изображению> "
                  << "--output <выходной_файл> --kernel <размер_ядра>" << std::endl;
        exit(1);
    }
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

cv::Mat applyBlurFilter(const cv::Mat& inputImage, int kernelSize) {
    cv::Mat outputImage = inputImage.clone();
    
    int channels = inputImage.channels();
    
    int halfKernel = kernelSize / 2;
    
    for (int y = 0; y < inputImage.rows; y++) {
        for (int x = 0; x < inputImage.cols; x++) {
            std::vector<int> sumChannels(channels, 0);
            
            for (int ky = -halfKernel; ky <= halfKernel; ky++) {
                for (int kx = -halfKernel; kx <= halfKernel; kx++) {
                    int nx = x + kx;
                    int ny = y + ky;
                    
                    cv::Vec3b pixel = getPixelWithMirror(inputImage, nx, ny);
                    
                    for (int c = 0; c < channels; c++) {
                        sumChannels[c] += pixel[c];
                    }
                }
            }
            
            int totalPixels = kernelSize * kernelSize;
            
            if (channels == 3) {
                outputImage.at<cv::Vec3b>(y, x) = cv::Vec3b(
                    sumChannels[0] / totalPixels,
                    sumChannels[1] / totalPixels,
                    sumChannels[2] / totalPixels
                );
            } 
            else if (channels == 1) {
                outputImage.at<uchar>(y, x) = sumChannels[0] / totalPixels;
            }
        }
    }
    
    return outputImage;
}

int main(int argc, char* argv[]) {
    std::string inputPath;
    std::string outputPath;
    int kernelSize = 3;
    
    parseCommandLine(argc, argv, inputPath, outputPath, kernelSize);
    
    cv::Mat inputImage = cv::imread(inputPath, cv::IMREAD_UNCHANGED);
    
    if (inputImage.empty()) {
        std::cerr << "Ошибка: не удалось загрузить изображение " << inputPath << std::endl;
        return 1;
    }
    
    std::cout << "Применение фильтра размытия с ядром " << kernelSize << "x" << kernelSize 
              << " к изображению " << inputPath << std::endl;
    
    cv::Mat outputImage = applyBlurFilter(inputImage, kernelSize);
    
    bool success = cv::imwrite(outputPath, outputImage);
    
    if (success) {
        std::cout << "Изображение с применённым фильтром размытия сохранено в " << outputPath << std::endl;
    } else {
        std::cerr << "Ошибка: не удалось сохранить изображение " << outputPath << std::endl;
        return 1;
    }
    
    return 0;
}