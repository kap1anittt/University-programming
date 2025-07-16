#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>
#include <string>
#include <sys/stat.h>

bool directoryExists(const std::string& path) {
    struct stat info;
    return stat(path.c_str(), &info) == 0 && (info.st_mode & S_IFDIR);
}

cv::Scalar parseColor(const std::string& colorStr) {
    std::stringstream ss(colorStr);
    std::string item;
    std::vector<int> rgb;
    
    while (std::getline(ss, item, ',')) {
        rgb.push_back(std::stoi(item));
    }
    
    if (rgb.size() != 3) {
        return cv::Scalar(0, 0, 255);
    }
    
    return cv::Scalar(rgb[2], rgb[1], rgb[0]);
}

void printUsage() {
    std::cout << "Использование: ./spiral_generator --width <ширина> --height <высота> "
              << "--thick <толщина> --step <шаг> --color R,G,B" << std::endl;
}

void drawPixel(cv::Mat& image, int x, int y, int thickness, const cv::Vec3b& color) {
    int halfThick = thickness / 2;
    
    for (int dy = -halfThick; dy <= halfThick; dy++) {
        for (int dx = -halfThick; dx <= halfThick; dx++) {
            int nx = x + dx;
            int ny = y + dy;
            
            if (nx >= 0 && nx < image.cols && ny >= 0 && ny < image.rows) {
                image.at<cv::Vec3b>(ny, nx) = color;
            }
        }
    }
}

void drawLine(cv::Mat& image, int x1, int y1, int x2, int y2, int thickness, const cv::Vec3b& color) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;
    
    while (true) {
        drawPixel(image, x1, y1, thickness, color);
        
        if (x1 == x2 && y1 == y2) {
            break;
        }
        
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

int main(int argc, char* argv[]) {
    int width = 800;
    int height = 600;
    int thickness = 2;
    int step = 20;
    std::string colorStr = "0,0,255";
    std::string outputDir = "images_spiral";

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--width" && i + 1 < argc) {
            width = std::stoi(argv[++i]);
        } else if (arg == "--height" && i + 1 < argc) {
            height = std::stoi(argv[++i]);
        } else if (arg == "--thick" && i + 1 < argc) {
            thickness = std::stoi(argv[++i]);
        } else if (arg == "--step" && i + 1 < argc) {
            step = std::stoi(argv[++i]);
        } else if (arg == "--color" && i + 1 < argc) {
            colorStr = argv[++i];
        }
    }

    if (width <= 0 || height <= 0 || thickness <= 0 || step <= 0) {
        std::cout << "Ошибка: все размеры должны быть положительными" << std::endl;
        printUsage();
        return 1;
    }

    if (!directoryExists(outputDir)) {
        #ifdef _WIN32
            _mkdir(outputDir.c_str());
        #else
            mkdir(outputDir.c_str(), 0777);
        #endif
    }

    cv::Mat image(height, width, CV_8UC3, cv::Scalar(255, 255, 255));
    cv::Point center(width / 2, height / 2);
    
    cv::Scalar cvColor = parseColor(colorStr);
    cv::Vec3b color(cvColor[0], cvColor[1], cvColor[2]);

    int maxDimension = std::max(width, height);
    int turns = maxDimension / (2 * step);
    
    std::vector<cv::Point> points;
    
    points.push_back(center);
    
    int x = center.x;
    int y = center.y;
    int direction = 0;
    int currentLength = step;
    int segmentCount = 0;
    
    drawPixel(image, x, y, thickness, color);
    
    for (int i = 0; i < turns * 4; i++) {
        int dx = 0, dy = 0;
        
        switch (direction) {
            case 0: dx = currentLength; break;
            case 1: dy = currentLength; break;
            case 2: dx = -currentLength; break;
            case 3: dy = -currentLength; break;
        }
        
        int newX = x + dx;
        int newY = y + dy;
        
        if (newX < 0 || newX >= width || newY < 0 || newY >= height) {
            break;
        }
        
        drawLine(image, x, y, newX, newY, thickness, color);
        
        x = newX;
        y = newY;
        
        points.push_back(cv::Point(x, y));
        
        direction = (direction + 1) % 4;
        
        segmentCount++;
        if (segmentCount % 2 == 0) {
            currentLength += step;
        }
    }
    
    std::string filename = outputDir + "/square_spiral_" + 
                          std::to_string(width) + "x" + 
                          std::to_string(height) + "_" +
                          std::to_string(thickness) + "_" +
                          std::to_string(step) + ".png";

    bool success = cv::imwrite(filename, image);
    
    if (success) {
        std::cout << "Квадратная спираль успешно сохранена в " << filename << std::endl;
    } else {
        std::cout << "Ошибка при сохранении изображения!" << std::endl;
        return 1;
    }

    return 0;
} 