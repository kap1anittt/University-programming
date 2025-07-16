#!/bin/bash

# Скрипт для конвертации PPM в PNG с помощью ImageMagick

# Директория для сохранения изображений
IMG_DIR="images"

# Создаем директорию, если она не существует
mkdir -p $IMG_DIR

# Устанавливаем значение по умолчанию для имени файла
INPUT_FILE="chessboard"
OUTPUT_FILE="chessboard"

# Проверяем, были ли переданы аргументы
if [ $# -ge 1 ]; then
    INPUT_FILE="$1"
fi

# Устанавливаем имя исходного файла с расширением
if [[ ! "$INPUT_FILE" =~ \.ppm$ ]]; then
    INPUT_FILE_WITH_EXT="${INPUT_FILE}.ppm"
else
    INPUT_FILE_WITH_EXT="$INPUT_FILE"
    # Удаляем расширение .ppm для базового имени файла
    INPUT_FILE="${INPUT_FILE%.*}"
fi

# Полные пути к файлам
INPUT_PATH="${IMG_DIR}/${INPUT_FILE_WITH_EXT}"
OUTPUT_PATH="${IMG_DIR}/${INPUT_FILE}.png"

# Проверяем, установлен ли ImageMagick
if ! command -v convert &> /dev/null; then
    echo "ImageMagick не установлен. Установите его командой:"
    echo "  brew install imagemagick    # для macOS"
    echo "  sudo apt install imagemagick    # для Ubuntu/Debian Linux"
    exit 1
fi

# Проверяем, существует ли исходный файл
if [ ! -f "$INPUT_PATH" ]; then
    echo "Файл $INPUT_PATH не найден. Сначала запустите ./lab1 для его создания."
    exit 1
fi

# Выполняем конвертацию
echo "Конвертация $INPUT_PATH в $OUTPUT_PATH..."
convert "$INPUT_PATH" "$OUTPUT_PATH"

# Проверяем результат
if [ -f "$OUTPUT_PATH" ]; then
    echo "Конвертация успешно завершена."
    echo "Открываю файл $OUTPUT_PATH..."
    
    # Определяем операционную систему и открываем файл с помощью подходящей команды
    case "$(uname -s)" in
        Darwin*)    # macOS
            open "$OUTPUT_PATH"
            ;;
        Linux*)     # Linux
            if command -v xdg-open &> /dev/null; then
                xdg-open "$OUTPUT_PATH"
            elif command -v display &> /dev/null; then
                display "$OUTPUT_PATH"
            else
                echo "Не удалось открыть файл. Установите ImageMagick полностью или используйте другую программу просмотра изображений."
            fi
            ;;
        *)          # Другие ОС
            echo "Файл $OUTPUT_PATH создан в директории $IMG_DIR."
            ;;
    esac
else
    echo "Ошибка при конвертации."
fi