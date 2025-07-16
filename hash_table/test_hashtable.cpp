#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "hash_table.hpp"
#include <string>
#include <sstream>
#include <set>

TEST_CASE("Базовые операции хеш-таблицы") {
    HashTable<int, std::string> ht;
    
    SUBCASE("Вставка и поиск") {
        ht.insert(1, "один");
        ht.insert(2, "два");
        ht.insert(3, "три");
        
        CHECK(ht.contains(1));
        CHECK(ht.contains(2));
        CHECK(ht.contains(3));
        CHECK_FALSE(ht.contains(4));
        
        CHECK(ht[1] == "один");
        CHECK(ht[2] == "два");
        CHECK(ht[3] == "три");
    }
    
    SUBCASE("Перезапись существующего ключа") {
        ht.insert(1, "один");
        ht.insert(1, "новое значение");
        
        CHECK(ht[1] == "новое значение");
    }
    
    SUBCASE("Оператор доступа - создание нового элемента") {
        CHECK_FALSE(ht.contains(5));
        ht[5] = "пять";
        CHECK(ht.contains(5));
        CHECK(ht[5] == "пять");
    }
    
    SUBCASE("Размер таблицы") {
        CHECK(ht.size() == 0);
        ht.insert(1, "один");
        CHECK(ht.size() == 1);
        ht.insert(2, "два");
        CHECK(ht.size() == 2);
        ht.insert(1, "новое значение");
        CHECK(ht.size() == 2);
    }
    
    SUBCASE("Пустая таблица") {
        CHECK(ht.empty());
        ht.insert(1, "один");
        CHECK_FALSE(ht.empty());
    }
}

TEST_CASE("Итераторы хеш-таблицы") {
    HashTable<int, std::string> ht;
    
    SUBCASE("Пустая таблица") {
        CHECK(ht.begin() == ht.end());
    }
    
    SUBCASE("Перебор элементов") {
        ht.insert(1, "один");
        ht.insert(2, "два");
        ht.insert(3, "три");
        
        std::set<int> keys;
        std::set<std::string> values;
        
        for (const auto& pair : ht) {
            keys.insert(pair.first);
            values.insert(pair.second);
        }
        
        CHECK(keys.size() == 3);
        CHECK(keys.count(1) == 1);
        CHECK(keys.count(2) == 1);
        CHECK(keys.count(3) == 1);
        
        CHECK(values.size() == 3);
        CHECK(values.count("один") == 1);
        CHECK(values.count("два") == 1);
        CHECK(values.count("три") == 1);
    }
    
    SUBCASE("Модификация через индексный оператор") {
        HashTable<int, std::string> modHt;
        modHt.insert(1, "один");
        modHt.insert(2, "два");
        
        std::vector<int> keys;
        for (const auto& pair : modHt) {
            keys.push_back(pair.first);
        }
        
        for (int key : keys) {
            if (key == 1) {
                modHt[key] = "ONE";
            } else if (key == 2) {
                modHt[key] = "TWO";
            }
        }
        
        CHECK(modHt[1] == "ONE");
        CHECK(modHt[2] == "TWO");
    }
}

TEST_CASE("Коллизии в хеш-таблице") {
    HashTable<int, std::string> ht(1);
    
    ht.insert(1, "один");
    ht.insert(2, "два");
    ht.insert(3, "три");
    
    CHECK(ht.contains(1));
    CHECK(ht.contains(2));
    CHECK(ht.contains(3));
    
    CHECK(ht[1] == "один");
    CHECK(ht[2] == "два");
    CHECK(ht[3] == "три");
}

TEST_CASE("Копирование хеш-таблицы") {
    HashTable<int, std::string> ht1;
    ht1.insert(1, "один");
    ht1.insert(2, "два");
    
    HashTable<int, std::string> ht2(ht1);
    CHECK(ht2.size() == 2);
    CHECK(ht2[1] == "один");
    CHECK(ht2[2] == "два");
    
    HashTable<int, std::string> ht3;
    ht3 = ht1;
    CHECK(ht3.size() == 2);
    CHECK(ht3[1] == "один");
    CHECK(ht3[2] == "два");
    
    ht1.insert(3, "три");
    CHECK(ht1.contains(3));
    CHECK_FALSE(ht2.contains(3));
    CHECK_FALSE(ht3.contains(3));
}

TEST_CASE("Автоматическое увеличение размера") {
    HashTable<int, int> ht(2);
    
    for (int i = 0; i < 10; ++i) {
        ht.insert(i, i * 10);
    }
    
    for (int i = 0; i < 10; ++i) {
        CHECK(ht.contains(i));
        CHECK(ht[i] == i * 10);
    }
}

TEST_CASE("Сложные типы данных") {
    HashTable<std::string, std::vector<int>> ht;
    
    ht.insert("один", {1, 11, 111});
    ht.insert("два", {2, 22, 222});
    
    CHECK(ht["один"].size() == 3);
    CHECK(ht["один"][0] == 1);
    CHECK(ht["один"][1] == 11);
    CHECK(ht["один"][2] == 111);
    
    CHECK(ht["два"].size() == 3);
    CHECK(ht["два"][0] == 2);
    CHECK(ht["два"][1] == 22);
    CHECK(ht["два"][2] == 222);
    
    ht["три"] = {3, 33, 333};
    CHECK(ht.contains("три"));
    CHECK(ht["три"].size() == 3);
    CHECK(ht["три"][0] == 3);
    
    ht["один"].push_back(1111);
    CHECK(ht["один"].size() == 4);
    CHECK(ht["один"][3] == 1111);
} 