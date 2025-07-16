#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "dynamic_array.hpp"

TEST_SUITE("Динамический массив") {
    TEST_CASE("Конструкторы") {
        {
            auto arr = DynamicArray();
            REQUIRE(arr.Size() == 0);
            REQUIRE(arr.Capacity() == 0);
            REQUIRE(arr.empty());
        }
        
        {
            const int size = 5, value = 42;
            auto arr = DynamicArray(size, value);
            
            REQUIRE(arr.Size() == size);
            REQUIRE(arr.Capacity() >= size);
            
            for (int i = 0; i < size; ++i)
                REQUIRE(arr[i] == value);
        }
        
        {
            auto arr = DynamicArray{10, 20, 30, 40, 50};
            
            REQUIRE(arr.Size() == 5);
            
            for (int i = 0; i < 5; ++i)
                REQUIRE(arr[i] == (i+1)*10);
        }
        
        {
            auto original = DynamicArray{5, 6, 7};
            auto copy = DynamicArray(original);
            
            REQUIRE(copy.Size() == original.Size());
            REQUIRE(copy.Capacity() == original.Capacity());
            REQUIRE(copy == original);
        }
    }

    TEST_CASE("Модификаторы") {
        {
            auto arr = DynamicArray{};
            
            arr.push_back(100);
            REQUIRE(arr.Size() == 1);
            REQUIRE(arr[0] == 100);
            
            for (int i = 0; i < 5; ++i)
                arr.push_back(i);
                
            REQUIRE(arr.Size() == 6);
            REQUIRE(arr[arr.Size()-1] == 4);
        }
        
        {
            auto arr = DynamicArray{1, 2, 3, 4};
            
            arr.pop_back();
            REQUIRE(arr.Size() == 3);
            REQUIRE(arr[arr.Size()-1] == 3);
            
            auto emptyArr = DynamicArray{};
            REQUIRE_THROWS_AS(emptyArr.pop_back(), std::out_of_range);
        }
        
        {
            auto arr = DynamicArray{5, 4, 3, 2, 1};
            REQUIRE_FALSE(arr.empty());
            
            arr.clear();
            REQUIRE(arr.empty());
            REQUIRE(arr.Size() == 0);
        }
        
        {
            auto arr = DynamicArray{10, 30, 40};
            
            arr.insert(1, 20);
            REQUIRE(arr.Size() == 4);
            REQUIRE(arr[0] == 10);
            REQUIRE(arr[1] == 20);
            REQUIRE(arr[2] == 30);
            REQUIRE(arr[3] == 40);
            
            REQUIRE_THROWS_AS(arr.insert(-1, 0), std::out_of_range);
            REQUIRE_THROWS_AS(arr.insert(arr.Size()+1, 0), std::out_of_range);
        }
        
        {
            auto arr = DynamicArray{5, 10, 15, 20};
            
            arr.erase(1);
            REQUIRE(arr.Size() == 3);
            REQUIRE(arr[0] == 5);
            REQUIRE(arr[1] == 15);
            REQUIRE(arr[2] == 20);
            
            REQUIRE_THROWS_AS(arr.erase(-5), std::out_of_range);
            REQUIRE_THROWS_AS(arr.erase(5), std::out_of_range);
        }
        
        {
            auto arr = DynamicArray{1, 2, 3};
            
            arr.resize(5);
            REQUIRE(arr.Size() == 5);
            REQUIRE(arr[0] == 1);
            REQUIRE(arr[1] == 2);
            REQUIRE(arr[2] == 3);
            REQUIRE(arr[3] == 0);
            REQUIRE(arr[4] == 0);
            
            arr.resize(2);
            REQUIRE(arr.Size() == 2);
            REQUIRE(arr[0] == 1);
            REQUIRE(arr[1] == 2);
            
            REQUIRE_THROWS_AS(arr.resize(-10), std::invalid_argument);
        }
        
        {
            auto arr = DynamicArray{9, 8, 7};
            
            arr.assign(4, 123);
            REQUIRE(arr.Size() == 4);
            for (int i = 0; i < arr.Size(); ++i)
                REQUIRE(arr[i] == 123);
                
            REQUIRE_THROWS_AS(arr.assign(-2, 0), std::invalid_argument);
        }
    }
    TEST_CASE("Доступ к элементам") {
        auto arr = DynamicArray{11, 22, 33};
        
        SUBCASE("Оператор индексации") {
            for (int i = 0; i < arr.Size(); ++i)
                REQUIRE(arr[i] == (i+1)*11);
        }
        
        SUBCASE("Метод at() с проверкой границ") {
            REQUIRE(arr.at(0) == 11);
            REQUIRE(arr.at(1) == 22);
            REQUIRE(arr.at(2) == 33);
            
            REQUIRE_THROWS_AS(arr.at(-1), std::out_of_range);
            REQUIRE_THROWS_AS(arr.at(10), std::out_of_range);
        }
    }

    TEST_CASE("Операторы сравнения и присваивания") {
        {
            auto a1 = DynamicArray{1, 2, 3};
            auto a2 = DynamicArray{1, 2, 3};
            auto a3 = DynamicArray{1, 2, 3, 4};
            auto a4 = DynamicArray{1, 2, 4};
            
            REQUIRE(a1 == a2);
            REQUIRE(a1 != a3);
            REQUIRE(a1 != a4);
            REQUIRE(a3 != a4);
        }
        
        {
            auto src = DynamicArray{42, 43, 44};
            auto dst = DynamicArray{};
            
            dst = src;
            
            REQUIRE(dst == src);
            REQUIRE(dst.Size() == src.Size());
            REQUIRE(dst.Capacity() == src.Capacity());
        }
    }

    TEST_CASE("Обмен значениями и итераторы") {
        {
            auto a = DynamicArray{1, 2, 3};
            auto b = DynamicArray{10, 20};
            
            auto a_size = a.Size();
            auto b_size = b.Size();
            
            a.swap(b);
            
            REQUIRE(a.Size() == b_size);
            REQUIRE(b.Size() == a_size);
            REQUIRE(a[0] == 10);
            REQUIRE(a[1] == 20);
            REQUIRE(b[0] == 1);
            REQUIRE(b[1] == 2);
            REQUIRE(b[2] == 3);
        }
        
        {
            auto arr = DynamicArray{5, 10, 15, 20, 25};
            
            int total = 0;
            for (auto it = arr.begin(); it != arr.end(); ++it) {
                total += *it;
            }
            
            REQUIRE(total == 75);
            
            int* first = arr.begin();
            int* last = arr.end();
            int product = 1;
            
            while (first != last) {
                product *= *first++;
            }
            
            REQUIRE(product == 5*10*15*20*25);
        }
    }
}