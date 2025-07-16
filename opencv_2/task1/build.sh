#!/bin/bash

# Создаем директорию для сборки если её нет
mkdir -p build

# Переходим в директорию сборки
cd build

# Конфигурируем проект с помощью CMake
cmake ..

# Собираем проект
make

# Возвращаемся в исходную директорию
cd ..

echo "Сборка завершена. Для запуска используйте команду:"
echo "./build/lab2 --input image.jpg --output blur_kernel5.jpg --kernel 5" 