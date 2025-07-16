#ifndef STACKLSTT_HPP
#define STACKLSTT_HPP

#include <initializer_list>
#include <cstddef>
#include <stdexcept>
#include <ostream>
#include "stack_base.hpp"

template <typename T>
class StackLstT : public IStackBase<T> {
public:
    StackLstT() = default;
    
    ~StackLstT() {
        clear();
    }
    
    StackLstT(const StackLstT<T>& other) : StackLstT() {
        if (other.empty()) {
            return;
        }
        
        Node* curr_other = other.head_;
        head_ = new Node{curr_other->value, nullptr};
        Node* curr = head_;
        size_ = 1;
        
        curr_other = curr_other->next;
        while (curr_other) {
            curr->next = new Node{curr_other->value, nullptr};
            curr = curr->next;
            curr_other = curr_other->next;
            size_++;
        }
    }
    
    StackLstT(StackLstT<T>&& other) : head_(other.head_), size_(other.size_) {
        other.head_ = nullptr;
        other.size_ = 0;
    }
    
    StackLstT(const std::initializer_list<T>& list) : StackLstT() {
        for (const auto& item : list) {
            this->push(item);
        }
    }
  
    void push_impl(const T& value) override {
        head_ = new Node{value, head_};
        size_++;
    }
    
    void pop_impl() override {
        Node* temp = head_;
        head_ = head_->next;
        delete temp;
        size_--;
    }
    
    T& top_impl() const override {
        return head_->value;
    }
    
    bool empty_impl() const override {
        return head_ == nullptr;
    }
    
    std::ptrdiff_t size_impl() const override {
        return size_;
    }
    
    IStackBase<T>* create_instance() const override {
        return new StackLstT<T>();
    }
    
    void swap_impl(IStackBase<T>& other) override {
        StackLstT<T>* other_lst = dynamic_cast<StackLstT<T>*>(&other);
        if (other_lst) {
            std::swap(head_, other_lst->head_);
            std::swap(size_, other_lst->size_);
        }
    }
    
    void merge_impl(IStackBase<T>& other) override {
        StackLstT<T>* other_lst = dynamic_cast<StackLstT<T>*>(&other);
        if (!other_lst) {
            return;
        }
        
        if (other_lst->empty()) {
            return;
        }
        
        // Если текущий стек пуст, просто забираем содержимое другого
        if (this->empty()) {
            head_ = other_lst->head_;
            size_ = other_lst->size_;
            other_lst->head_ = nullptr;
            other_lst->size_ = 0;
            return;
        }
        
        // Сохраняем указатель на голову другого стека
        Node* other_head = other_lst->head_;
        
        // Обновляем размер
        size_ += other_lst->size_;
        
        // Добавляем элементы другого стека в начало текущего
        Node* last_other = other_head;
        while (last_other->next) {
            last_other = last_other->next;
        }
        
        last_other->next = head_;
        head_ = other_head;
        
        // Очищаем другой стек
        other_lst->head_ = nullptr;
        other_lst->size_ = 0;
    }
    
    bool compare_impl(const IStackBase<T>& rhs) const override {
        const StackLstT<T>* rhs_lst = dynamic_cast<const StackLstT<T>*>(&rhs);
        if (!rhs_lst) {
            return false;
        }
        
        Node* curr1 = head_;
        Node* curr2 = rhs_lst->head_;
        
        while (curr1 && curr2) {
            if (curr1->value != curr2->value) {
                return false;
            }
            curr1 = curr1->next;
            curr2 = curr2->next;
        }
        
        return true;
    }
    
    void printToStream(std::ostream& os) const override {
        os << "[";
        Node* curr = head_;
        bool first = true;
        
        while (curr) {
            if (!first) {
                os << ", ";
            }
            os << curr->value;
            first = false;
            curr = curr->next;
        }
        
        os << "]";
    }

    StackLstT<T>& operator=(const StackLstT<T>& rhs) noexcept {
        if (this != &rhs) {
            StackLstT<T> tmp(rhs);
            this->swap(tmp);
        }
        return *this;
    }
    
    StackLstT<T>& operator=(StackLstT<T>&& other) {
        if (this != &other) {
            clear();
            head_ = other.head_;
            size_ = other.size_;
            other.head_ = nullptr;
            other.size_ = 0;
        }
        return *this;
    }

private:
    void clear() {
        while (!this->empty()) {
            this->pop();
        }
    }
    
    struct Node {
        T value;
        Node* next = nullptr;
    };

    Node* head_ = nullptr;
    std::ptrdiff_t size_ = 0;
};

#endif // STACKLSTT_HPP