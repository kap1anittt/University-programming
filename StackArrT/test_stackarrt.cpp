#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../dynamic_array/doctest.h"
#include "stackarrt.hpp"
#include <string>

TEST_CASE("StackArrT - базовые операции с int") {
    StackArrT<int> stack;
    
    SUBCASE("Пустой стек") {
        CHECK(stack.empty());
        CHECK(stack.size() == 0);
        CHECK_THROWS_AS(stack.pop(), std::out_of_range);
        CHECK_THROWS_AS(stack.top(), std::out_of_range);
    }
    
    SUBCASE("Операции push/pop") {
        stack.push(1);
        CHECK_FALSE(stack.empty());
        CHECK(stack.size() == 1);
        CHECK(stack.top() == 1);
        
        stack.push(2);
        CHECK(stack.size() == 2);
        CHECK(stack.top() == 2);
        
        stack.pop();
        CHECK(stack.size() == 1);
        CHECK(stack.top() == 1);
        
        stack.pop();
        CHECK(stack.empty());
    }
}

TEST_CASE("StackArrT - конструкторы и операторы присваивания") {
    SUBCASE("Конструктор копирования") {
        StackArrT<int> stack1;
        stack1.push(1);
        stack1.push(2);
        
        StackArrT<int> stack2(stack1);
        CHECK(stack2.size() == 2);
        CHECK(stack2.top() == 2);
        stack2.pop();
        CHECK(stack2.top() == 1);
        CHECK(stack1.size() == 2); // Оригинальный стек не изменился
    }
    
    SUBCASE("Конструктор перемещения") {
        StackArrT<int> stack1;
        stack1.push(1);
        stack1.push(2);
        
        StackArrT<int> stack2(std::move(stack1));
        CHECK(stack2.size() == 2);
        CHECK(stack2.top() == 2);
        CHECK(stack1.empty()); // Оригинальный стек пуст после перемещения
    }
    
    SUBCASE("Initializer list конструктор") {
        StackArrT<int> stack{1, 2, 3};
        CHECK(stack.size() == 3);
        CHECK(stack.top() == 3);
    }
}

TEST_CASE("StackArrT - работа с разными типами данных") {
    SUBCASE("Работа со строками") {
        StackArrT<std::string> stack;
        stack.push("Hello");
        stack.push("World");
        
        CHECK(stack.size() == 2);
        CHECK(stack.top() == "World");
        stack.pop();
        CHECK(stack.top() == "Hello");
    }
    
    SUBCASE("Работа с double") {
        StackArrT<double> stack;
        stack.push(1.5);
        stack.push(2.7);
        
        CHECK(stack.size() == 2);
        CHECK(stack.top() == doctest::Approx(2.7));
    }
}

TEST_CASE("StackArrT - операции swap и merge") {
    SUBCASE("swap") {
        StackArrT<int> stack1{1, 2};
        StackArrT<int> stack2{3, 4, 5};
        
        stack1.swap(stack2);
        CHECK(stack1.size() == 3);
        CHECK(stack2.size() == 2);
        CHECK(stack1.top() == 5);
        CHECK(stack2.top() == 2);
    }
    
    SUBCASE("merge") {
        StackArrT<int> stack1{1, 2};
        StackArrT<int> stack2{3, 4};
        
        stack1.merge(stack2);
        CHECK(stack1.size() == 4);
        CHECK(stack2.empty());
        CHECK(stack1.top() == 4);
    }
}

TEST_CASE("StackArrT - операторы сравнения") {
    StackArrT<int> stack1{1, 2, 3};
    StackArrT<int> stack2{1, 2, 3};
    StackArrT<int> stack3{1, 2, 4};
    
    CHECK(stack1 == stack2);
    CHECK(stack1 != stack3);
} 