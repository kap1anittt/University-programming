#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sys/stat.h>
#include <cstdlib> // для system()

// Проверка существования папки
bool directoryExists(const std::string& path) {
    struct stat info;
    return stat(path.c_str(), &info) == 0 && (info.st_mode & S_IFDIR);
}

void printUsage() {
    std::cout << "Как использовать: ./chessboard --rows <число строк> --cols <число столбцов> --cell <размер клетки> [--output <имя_файла>]" << std::endl;
}

//argc - количество аргументов
//argv - массив аргументов
int main(int argc, char* argv[]) {
    // Значения по умолчанию
    int rows = 8;
    int cols = 8;
    int cellSize = 50;
    std::string outputFile = "chessboard.ppm";
    std::string outputDir = "images";

    // Создаем папку images, если она не существует
    if (!directoryExists(outputDir)) {
        #ifdef _WIN32
        _mkdir(outputDir.c_str());
        #else
        mkdir(outputDir.c_str(), 0777);
        #endif
        std::cout << "Создана папка '" << outputDir << "' для хранения изображений." << std::endl;
    }

    // Разбор аргументов командной строки
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];

        if (arg == "--rows" || arg == "--cols" || arg == "--cell" || arg == "--output") {
            if (i + 1 >= argc) {
                std::cout << "Ошибка: не указан аргумент для " << arg << std::endl;
                printUsage();
                return 1;
            }
            
            if (arg == "--rows") {
                rows = std::stoi(argv[i + 1]);
                i++;
            } else if (arg == "--cols") {
                cols = std::stoi(argv[i + 1]);
                i++;
            } else if (arg == "--cell") {
                cellSize = std::stoi(argv[i + 1]);
                i++;
            } else if (arg == "--output") {
                outputFile = argv[i + 1];
                // Добавляем расширение .ppm, если его нет
                if (outputFile.find(".ppm") == std::string::npos) {
                    outputFile += ".ppm";
                }
                i++;
            }
        } else {
            std::cout << "Ошибка: неизвестный аргумент " << arg << std::endl;
            printUsage();
            return 1;
        }
    }

    if (rows <= 0 || cols <= 0 || cellSize <= 0) {
        std::cout << "Ошибка: размеры должны быть положительными" << std::endl;
        printUsage();
        return 1;
    }

    // Формируем полный путь к файлу
    std::string fullPath = outputDir + "/" + outputFile;

    // Создание изображения в формате PPM (Portable Pixmap Format)
    // Это простой текстовый формат изображения
    std::ofstream out(fullPath);
    if (!out) {
        std::cout << "Ошибка: не удалось создать файл " << fullPath << std::endl;
        return 1;
    }

    int width = cols * cellSize;
    int height = rows * cellSize;
    
    // Заголовок PPM файла (формат P3 - ASCII)
    out << "P3" << std::endl;
    out << width << " " << height << std::endl;
    out << "255" << std::endl; // максимальное значение цвета

    // Генерация шахматной доски
    for (int i = 0; i < rows; ++i) {
        for (int y = 0; y < cellSize; ++y) { // строки внутри клетки
            for (int j = 0; j < cols; ++j) {
                // Определение цвета клетки: черный (0,0,0) или белый (255,255,255)
                int color = ((i + j) % 2 == 0) ? 255 : 0;
                
                for (int x = 0; x < cellSize; ++x) { // столбцы внутри клетки
                    // Запись RGB значений пикселя
                    out << color << " " << color << " " << color << " ";
                }
            }
            out << std::endl; // перевод строки после каждой строки пикселей
        }
    }

    out.close();
    std::cout << "Шахматная доска размером " << rows << "x" << cols << 
              " с клетками " << cellSize << "x" << cellSize << 
              " сохранена в файл " << fullPath << std::endl;
    
    // Автоматическая конвертация в PNG
    std::string baseName = outputFile;
    if (baseName.find(".ppm") != std::string::npos) {
        baseName = baseName.substr(0, baseName.find(".ppm"));
    }
    
    // Запуск конвертации через ImageMagick
    std::string cmd = "convert " + fullPath + " " + outputDir + "/" + baseName + ".png";
    int result = system(cmd.c_str());
    
    if (result == 0) {
        std::cout << "Файл успешно конвертирован в PNG: " << outputDir << "/" << baseName << ".png" << std::endl;
    } else {
        std::cout << "Ошибка при конвертации в PNG. Возможно, ImageMagick не установлен." << std::endl;
        std::cout << "Для установки ImageMagick используйте команду:" << std::endl;
        std::cout << "  brew install imagemagick    # для macOS" << std::endl;
        std::cout << "  sudo apt install imagemagick    # для Ubuntu/Debian Linux" << std::endl;
    }
    
    return 0;
}