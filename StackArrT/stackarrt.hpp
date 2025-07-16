#ifndef STACKARRT_HPP
#define STACKARRT_HPP

#include <initializer_list>
#include <cstddef>
#include <stdexcept>
#include <algorithm>

template <typename T>
class StackArrT {
public:
    StackArrT() = default;
    
    ~StackArrT() {
        delete[] data_;
    }
    
    StackArrT(const StackArrT<T>& other) 
        : size_(other.size_)
        , i_top_(other.i_top_) {
        data_ = new T[size_];
        for (std::ptrdiff_t i = 0; i <= i_top_; ++i) {
            data_[i] = other.data_[i];
        }
    }
    
    StackArrT(StackArrT<T>&& other)
        : size_(other.size_)
        , i_top_(other.i_top_)
        , data_(other.data_) {
        other.size_ = 0;
        other.i_top_ = -1;
        other.data_ = nullptr;
    }
    
    StackArrT(const std::initializer_list<T>& list)
        : size_(list.size())
        , i_top_(list.size() - 1) {
        data_ = new T[size_];
        std::ptrdiff_t i = 0;
        for (const auto& item : list) {
            data_[i++] = item;
        }
    }
    
    void push(const T& value) {
        if (i_top_ + 1 >= size_) {
            std::ptrdiff_t new_size = (size_ == 0) ? 1 : size_ * 2;
            T* new_data = new T[new_size];
            for (std::ptrdiff_t i = 0; i <= i_top_; ++i) {
                new_data[i] = data_[i];
            }
            delete[] data_;
            data_ = new_data;
            size_ = new_size;
        }
        data_[++i_top_] = value;
    }
    
    void pop() {
        if (empty()) {
            throw std::out_of_range("Stack is empty");
        }
        --i_top_;
    }
    
    T& top() const {
        if (empty()) {
            throw std::out_of_range("Stack is empty");
        }
        return data_[i_top_];
    }
    
    void swap(StackArrT<T>& other) {
        std::swap(size_, other.size_);
        std::swap(i_top_, other.i_top_);
        std::swap(data_, other.data_);
    }
    
    void merge(StackArrT<T>& other) {
        if (other.empty()) {
            return;
        }
        
        std::ptrdiff_t new_size = size_ + other.size_;
        T* new_data = new T[new_size];
        
        // Копируем элементы текущего стека
        for (std::ptrdiff_t i = 0; i <= i_top_; ++i) {
            new_data[i] = data_[i];
        }
        
        // Копируем элементы другого стека
        for (std::ptrdiff_t i = 0; i <= other.i_top_; ++i) {
            new_data[i_top_ + 1 + i] = other.data_[i];
        }
        
        delete[] data_;
        data_ = new_data;
        size_ = new_size;
        i_top_ = i_top_ + other.size_;
        
        // Очищаем другой стек
        other.clear();
    }
    
    bool empty() const {
        return i_top_ < 0;
    }
    
    std::ptrdiff_t size() const {
        return i_top_ + 1;
    }
    
    bool operator==(const StackArrT<T>& rhs) const {
        if (size() != rhs.size()) {
            return false;
        }
        for (std::ptrdiff_t i = 0; i <= i_top_; ++i) {
            if (data_[i] != rhs.data_[i]) {
                return false;
            }
        }
        return true;
    }
    
    bool operator!=(const StackArrT<T>& rhs) const {
        return !(*this == rhs);
    }
    
    StackArrT<T>& operator=(const StackArrT<T>& rhs) noexcept {
        if (this != &rhs) {
            StackArrT<T> tmp(rhs);
            swap(tmp);
        }
        return *this;
    }
    
    StackArrT<T>& operator=(StackArrT<T>&& other) {
        if (this != &other) {
            delete[] data_;
            size_ = other.size_;
            i_top_ = other.i_top_;
            data_ = other.data_;
            other.size_ = 0;
            other.i_top_ = -1;
            other.data_ = nullptr;
        }
        return *this;
    }
    
private:
    void clear() {
        delete[] data_;
        data_ = nullptr;
        size_ = 0;
        i_top_ = -1;
    }
    
    std::ptrdiff_t size_ = 0;   //!< число элементов в буфере
    std::ptrdiff_t i_top_ = -1; //!< индекс top элемента
    T* data_ = nullptr;   //!< элементы стека
};

#endif // STACKARRT_HPP 