#include "dynamic_array.hpp"

DynamicArray::DynamicArray(int64_t size, int value)
    : size(size), capacity(size) {
    if (capacity > 0) {
        data = new int[capacity];
        std::fill(data, data + size, value);
    } else {
        data = nullptr;
    }
}

DynamicArray::DynamicArray(const std::initializer_list<int>& list)
    : size(list.size()), capacity(list.size()) {
    data = new int[capacity];
    std::copy(list.begin(), list.end(), data);
}

DynamicArray::DynamicArray(const DynamicArray& other)
    : size(other.size), capacity(other.capacity) {
    data = new int[capacity];
    std::copy(other.data, other.data + size, data);
}

DynamicArray& DynamicArray::operator=(const DynamicArray& rhs) {
    if (this != &rhs) {
        int* new_data = new int[rhs.capacity];
        std::copy(rhs.data, rhs.data + rhs.size, new_data);
        delete[] data;
        data = new_data;
        size = rhs.size;
        capacity = rhs.capacity;
    }
    return *this;
}

DynamicArray::~DynamicArray() {
    delete[] data;
}

int64_t DynamicArray::Size() const { return size; }
int64_t DynamicArray::Capacity() const { return capacity; }
bool DynamicArray::empty() const { return size == 0; }

void DynamicArray::push_back(int value) {
    if (size == capacity) {
        int64_t new_capacity = std::max(int64_t(1), capacity * 2);
        int* new_data = new int[new_capacity];
        if (data) {
            std::copy(data, data + size, new_data);
        }
        delete[] data;
        data = new_data;
        capacity = new_capacity;
    }
    data[size++] = value;
}

void DynamicArray::pop_back() {
    if (size > 0) {
        --size;
    } else {
        throw std::out_of_range("Cannot pop from an empty array");
    }
}

void DynamicArray::clear() {
    size = 0;
}

void DynamicArray::erase(int64_t index) {
    if (index < 0 || index >= size) {
        throw std::out_of_range("Index out of range");
    }
    std::copy(data + index + 1, data + size, data + index);
    --size;
}

void DynamicArray::resize(int64_t new_size) {
    if (new_size < 0) {
        throw std::invalid_argument("Size cannot be negative");
    }
    
    if (new_size > capacity) {
        int* new_data = new int[new_size];
        std::copy(data, data + size, new_data);
        delete[] data;
        data = new_data;
        capacity = new_size;
    }
    
    // Инициализация новых элементов нулем
    if (new_size > size) {
        std::fill(data + size, data + new_size, 0);
    }
    
    size = new_size;
}

void DynamicArray::assign(int64_t new_size, int value) {
    if (new_size < 0) {
        throw std::invalid_argument("Size cannot be negative");
    }
    
    if (new_size > capacity) {
        delete[] data;
        capacity = new_size;
        data = new_size > 0 ? new int[capacity] : nullptr;
    }
    std::fill(data, data + new_size, value);
    size = new_size;
}

void DynamicArray::insert(int64_t index, int value) {
    if (index < 0 || index > size) {
        throw std::out_of_range("Index out of range");
    }
    
    if (size == capacity) {
        int64_t new_capacity = std::max(int64_t(1), capacity * 2);
        int* new_data = new int[new_capacity];
        if (data) {
            std::copy(data, data + index, new_data);
            std::copy(data + index, data + size, new_data + index + 1);
        }
        delete[] data;
        data = new_data;
        capacity = new_capacity;
    } else {
        std::copy_backward(data + index, data + size, data + size + 1);
    }
    
    data[index] = value;
    ++size;
}

void DynamicArray::swap(DynamicArray& other) {
    std::swap(size, other.size);
    std::swap(capacity, other.capacity);
    std::swap(data, other.data);
}

int* DynamicArray::begin() { return data; }
int* DynamicArray::end() { return data + size; }

int& DynamicArray::at(int64_t i) const {
    if (i < 0 || i >= size) {
        throw std::out_of_range("Index out of range");
    }
    return data[i];
}

int& DynamicArray::operator[](int64_t i) const { return data[i]; }

bool DynamicArray::operator==(const DynamicArray& rhs) const {
    if (size != rhs.size) return false;
    return std::equal(data, data + size, rhs.data);
}

bool DynamicArray::operator!=(const DynamicArray& rhs) const {
    return !(*this == rhs);
}
