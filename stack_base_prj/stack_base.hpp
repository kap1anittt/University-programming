#ifndef STACK_BASE_HPP
#define STACK_BASE_HPP

#include <cstddef>
#include <initializer_list>
#include <ostream>
#include <stdexcept>
#include <algorithm>

template <typename T>
class IStackBase {
public:
    virtual ~IStackBase() = default;
    
    // Абстрактные методы, которые дочерние классы должны реализовать
    virtual void push_impl(const T& value) = 0;
    virtual void pop_impl() = 0;
    virtual T& top_impl() const = 0;
    virtual bool empty_impl() const = 0;
    virtual std::ptrdiff_t size_impl() const = 0;
    virtual IStackBase<T>* create_instance() const = 0; // Factory метод для создания экземпляра того же типа
    
    // Основные методы стека с общей реализацией
    void push(const T& value) {
        push_impl(value);
    }
    
    void pop() {
        if (empty()) {
            throw std::out_of_range("Stack is empty");
        }
        pop_impl();
    }
    
    T& top() const {
        if (empty()) {
            throw std::out_of_range("Stack is empty");
        }
        return top_impl();
    }
    
    // Информационные методы с общей реализацией
    bool empty() const {
        return empty_impl();
    }
    
    std::ptrdiff_t size() const {
        return size_impl();
    }
    
    // Методы для работы с другими стеками
    virtual void swap_impl(IStackBase<T>& other) = 0;
    virtual void merge_impl(IStackBase<T>& other) = 0;
    
    void swap(IStackBase<T>& other) {
        if (typeid(*this) != typeid(other)) {
            throw std::invalid_argument("Can only swap with the same stack type");
        }
        swap_impl(other);
    }
    
    void merge(IStackBase<T>& other) {
        if (typeid(*this) != typeid(other)) {
            throw std::invalid_argument("Can only merge with the same stack type");
        }
        merge_impl(other);
    }
    
    // Операторы сравнения
    virtual bool compare_impl(const IStackBase<T>& rhs) const = 0;
    
    bool operator==(const IStackBase<T>& rhs) const {
        if (typeid(*this) != typeid(rhs) || size() != rhs.size()) {
            return false;
        }
        return compare_impl(rhs);
    }
    
    bool operator!=(const IStackBase<T>& rhs) const {
        return !(*this == rhs);
    }
    
    // Метод для вывода в поток
    virtual void printToStream(std::ostream& os) const = 0;
};

// Глобальный оператор вывода (реализован один раз в базовом классе без friend)
template <typename T>
std::ostream& operator<<(std::ostream& os, const IStackBase<T>& stack) {
    stack.printToStream(os);
    return os;
}

#endif // STACK_BASE_HPP
