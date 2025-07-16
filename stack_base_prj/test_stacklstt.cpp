#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../dynamic_array/doctest.h"
#include "stack_lst_t.hpp"
#include <string>
#include <sstream>

TEST_CASE("StackLstT - базовые операции с int") {
    StackLstT<int> stack;
    
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

TEST_CASE("StackLstT - конструкторы и операторы присваивания") {
    SUBCASE("Конструктор копирования") {
        StackLstT<int> stack1;
        stack1.push(1);
        stack1.push(2);
        
        StackLstT<int> stack2(stack1);
        CHECK(stack2.size() == 2);
        CHECK(stack2.top() == 2);
        stack2.pop();
        CHECK(stack2.top() == 1);
        CHECK(stack1.size() == 2); // Оригинальный стек не изменился
    }
    
    SUBCASE("Конструктор перемещения") {
        StackLstT<int> stack1;
        stack1.push(1);
        stack1.push(2);
        
        StackLstT<int> stack2(std::move(stack1));
        CHECK(stack2.size() == 2);
        CHECK(stack2.top() == 2);
        CHECK(stack1.empty()); // Оригинальный стек пуст после перемещения
    }
    
    SUBCASE("Initializer list конструктор") {
        StackLstT<int> stack{1, 2, 3};
        CHECK(stack.size() == 3);
        CHECK(stack.top() == 3);
    }
}

TEST_CASE("StackLstT - работа с разными типами данных") {
    SUBCASE("Работа со строками") {
        StackLstT<std::string> stack;
        stack.push("Hello");
        stack.push("World");
        
        CHECK(stack.size() == 2);
        CHECK(stack.top() == "World");
        stack.pop();
        CHECK(stack.top() == "Hello");
    }
    
    SUBCASE("Работа с double") {
        StackLstT<double> stack;
        stack.push(1.5);
        stack.push(2.7);
        
        CHECK(stack.size() == 2);
        CHECK(stack.top() == doctest::Approx(2.7));
    }
}

TEST_CASE("StackLstT - операции swap и merge") {
    SUBCASE("swap") {
        StackLstT<int> stack1{1, 2};
        StackLstT<int> stack2{3, 4, 5};
        
        stack1.swap(stack2);
        CHECK(stack1.size() == 3);
        CHECK(stack2.size() == 2);
        CHECK(stack1.top() == 5);
        CHECK(stack2.top() == 2);
    }
    
    SUBCASE("merge") {
        StackLstT<int> stack1{1, 2};
        StackLstT<int> stack2{3, 4};
        
        stack1.merge(stack2);
        CHECK(stack1.size() == 4);
        CHECK(stack2.empty());
        CHECK(stack1.top() == 4);
    }
}

TEST_CASE("StackLstT - операторы сравнения") {
    StackLstT<int> stack1{1, 2, 3};
    StackLstT<int> stack2{1, 2, 3};
    StackLstT<int> stack3{1, 2, 4};
    
    CHECK(stack1 == stack2);
    CHECK(stack1 != stack3);
}

TEST_CASE("StackLstT - оператор вывода") {
    StackLstT<int> stack{1, 2, 3};
    std::ostringstream oss;
    oss << stack;
    CHECK(oss.str() == "[3, 2, 1]");
    
    StackLstT<std::string> strStack;
    strStack.push("hello");
    strStack.push("world");
    std::ostringstream oss2;
    oss2 << strStack;
    CHECK(oss2.str() == "[world, hello]");
} 