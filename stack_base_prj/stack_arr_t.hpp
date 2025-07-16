#ifndef STACKARRT_HPP
#define STACKARRT_HPP

#include <initializer_list>
#include <cstddef>
#include <stdexcept>
#include <algorithm>
#include <ostream>
#include "stack_base.hpp"

template <typename T>
class StackArrT : public IStackBase<T> {
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
    
    // Реализация абстрактных методов базового класса
    void push_impl(const T& value) override {
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
    
    void pop_impl() override {
        --i_top_;
    }
    
    T& top_impl() const override {
        return data_[i_top_];
    }
    
    bool empty_impl() const override {
        return i_top_ < 0;
    }
    
    std::ptrdiff_t size_impl() const override {
        return i_top_ + 1;
    }
    
    IStackBase<T>* create_instance() const override {
        return new StackArrT<T>();
    }
    
    void swap_impl(IStackBase<T>& other) override {
        StackArrT<T>* other_arr = dynamic_cast<StackArrT<T>*>(&other);
        if (other_arr) {
            std::swap(size_, other_arr->size_);
            std::swap(i_top_, other_arr->i_top_);
            std::swap(data_, other_arr->data_);
        }
    }
    
    void merge_impl(IStackBase<T>& other) override {
        StackArrT<T>* other_arr = dynamic_cast<StackArrT<T>*>(&other);
        if (!other_arr) {
            return;
        }
        
        if (other_arr->empty()) {
            return;
        }
        
        std::ptrdiff_t new_size = size_ + other_arr->size();
        T* new_data = new T[new_size];
        
        // Копируем элементы текущего стека
        for (std::ptrdiff_t i = 0; i <= i_top_; ++i) {
            new_data[i] = data_[i];
        }
        
        // Копируем элементы другого стека
        for (std::ptrdiff_t i = 0; i <= other_arr->i_top_; ++i) {
            new_data[i_top_ + 1 + i] = other_arr->data_[i];
        }
        
        delete[] data_;
        data_ = new_data;
        size_ = new_size;
        i_top_ = i_top_ + other_arr->size();
        
        // Очищаем другой стек
        other_arr->clear();
    }
    
    bool compare_impl(const IStackBase<T>& rhs) const override {
        const StackArrT<T>* rhs_arr = dynamic_cast<const StackArrT<T>*>(&rhs);
        if (!rhs_arr) {
            return false;
        }
        
        for (std::ptrdiff_t i = 0; i <= i_top_; ++i) {
            if (data_[i] != rhs_arr->data_[i]) {
                return false;
            }
        }
        return true;
    }
    
    void printToStream(std::ostream& os) const override {
        os << "[";
        for (std::ptrdiff_t i = 0; i <= i_top_; ++i) {
            os << data_[i];
            if (i < i_top_) {
                os << ", ";
            }
        }
        os << "]";
    }
    
    StackArrT<T>& operator=(const StackArrT<T>& rhs) noexcept {
        if (this != &rhs) {
            StackArrT<T> tmp(rhs);
            this->swap(tmp);
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