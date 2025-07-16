#!/bin/bash

# Скрипт для компиляции, генерации и конвертации шахматной доски

# Директория для сохранения изображений
IMG_DIR="images"

# Создаем директорию, если она не существует
mkdir -p $IMG_DIR

# Значения по умолчанию
ROWS=8
COLS=8
CELL_SIZE=50
OUTPUT_FILE="chessboard"

# Функция для вывода помощи
print_help() {
    echo "Использование: $0 [параметры]"
    echo "Параметры:"
    echo "  --rows N      Количество строк (по умолчанию: 8)"
    echo "  --cols N      Количество столбцов (по умолчанию: 8)"
    echo "  --cell N      Размер клетки в пикселях (по умолчанию: 50)"
    echo "  --output FILE Имя выходного файла без расширения (по умолчанию: chessboard)"
    echo "  --help        Показать эту справку"
    echo ""
    echo "Все изображения сохраняются в директорию $IMG_DIR/"
}

# Обработка аргументов командной строки
while [[ $# -gt 0 ]]; do
    case "$1" in
        --rows)
            ROWS="$2"
            shift 2
            ;;
        --cols)
            COLS="$2"
            shift 2
            ;;
        --cell)
            CELL_SIZE="$2"
            shift 2
            ;;
        --output)
            OUTPUT_FILE="$2"
            shift 2
            ;;
        --help)
            print_help
            exit 0
            ;;
        *)
            echo "Неизвестный параметр: $1"
            print_help
            exit 1
            ;;
    esac
done

# Шаг 1: Компиляция программы
echo "Компиляция программы..."
g++ -std=c++11 chessboard.cpp -o chessboard

if [ $? -ne 0 ]; then
    echo "Ошибка компиляции. Выход."
    exit 1
fi

echo "Компиляция успешно завершена."

# Шаг 2: Генерация шахматной доски
echo "Генерация шахматной доски ${ROWS}x${COLS} с клетками ${CELL_SIZE}x${CELL_SIZE}..."
./chessboard --rows "$ROWS" --cols "$COLS" --cell "$CELL_SIZE" --output "$OUTPUT_FILE"

if [ $? -ne 0 ]; then
    echo "Ошибка при генерации шахматной доски. Выход."
    exit 1
fi

# Полные пути к файлам
PPM_FILE="${IMG_DIR}/${OUTPUT_FILE}.ppm"
PNG_FILE="${IMG_DIR}/${OUTPUT_FILE}.png"

# Шаг 3: Конвертация в PNG
echo "Конвертация в PNG формат..."

# Проверяем, установлен ли ImageMagick
if ! command -v convert &> /dev/null; then
    echo "ImageMagick не установлен. Конвертация в PNG невозможна."
    echo "Установите ImageMagick командой:"
    echo "  brew install imagemagick    # для macOS"
    echo "  sudo apt install imagemagick    # для Ubuntu/Debian Linux"
    echo "Шахматная доска сохранена только в формате PPM: ${PPM_FILE}"
    exit 0
fi

# Конвертация в PNG
convert "${PPM_FILE}" "${PNG_FILE}"

if [ $? -ne 0 ]; then
    echo "Ошибка при конвертации в PNG. Доступен только файл ${PPM_FILE}"
    exit 1
fi

echo "Шахматная доска успешно создана и конвертирована в PNG."
echo "Результаты:"
echo "  - PPM формат: ${PPM_FILE}"
echo "  - PNG формат: ${PNG_FILE}"

# Открытие результата
echo "Открываю файл ${PNG_FILE}..."

# Определяем операционную систему и открываем файл с помощью подходящей команды
case "$(uname -s)" in
    Darwin*)    # macOS
        open "${PNG_FILE}"
        ;;
    Linux*)     # Linux
        if command -v xdg-open &> /dev/null; then
            xdg-open "${PNG_FILE}"
        elif command -v display &> /dev/null; then
            display "${PNG_FILE}"
        else
            echo "Не удалось автоматически открыть файл. Используйте графический просмотрщик изображений."
        fi
        ;;
    *)          # Другие ОС
        echo "Для просмотра используйте графический просмотрщик изображений."
        ;;
esac

exit 0 