#include <stdexcept>
#include <cstddef>
#include <ostream>
#include <utility>

class Dequeue {
private:
    int* data;
    size_t capacity;
    size_t front;
    size_t back;
    size_t count;

public:
    explicit Dequeue(size_t cap) : capacity(cap > 0 ? cap + 1 : 1), front(0), back(0), count(0) {
        if (cap == 0) {
            capacity = 1;
        }
        data = new int[capacity];
    }

    ~Dequeue() {
        delete[] data;
    }

    Dequeue(const Dequeue& other) : capacity(other.capacity), front(other.front), back(other.back), count(other.count) {
        data = new int[capacity];
        size_t current_other = other.front;
        for (size_t i = 0; i < count; ++i) {
             data[(front + i) % capacity] = other.data[current_other];
             current_other = (current_other + 1) % other.capacity;
        }
    }

    Dequeue& operator=(const Dequeue& other) {
        if (this != &other) {
            Dequeue temp(other);
            std::swap(data, temp.data);
            std::swap(capacity, temp.capacity);
            std::swap(front, temp.front);
            std::swap(back, temp.back);
            std::swap(count, temp.count);
        }
        return *this;
    }

    Dequeue(Dequeue&& other) noexcept : data(other.data), capacity(other.capacity), front(other.front), back(other.back), count(other.count) {
        other.data = nullptr;
        other.capacity = 0;
        other.front = 0;
        other.back = 0;
        other.count = 0;
    }

    Dequeue& operator=(Dequeue&& other) noexcept {
        if (this != &other) {
            delete[] data;

            data = other.data;
            capacity = other.capacity;
            front = other.front;
            back = other.back;
            count = other.count;

            other.data = nullptr;
            other.capacity = 0;
            other.front = 0;
            other.back = 0;
            other.count = 0;
        }
        return *this;
    }

    bool empty() const {
        return count == 0;
    }

    size_t getSize() const {
        return count;
    }

    size_t getCapacity() const {
        return capacity > 0 ? capacity - 1 : 0;
    }

    void push_front(int value) {
        if (count == getCapacity()) {
            throw std::overflow_error("Dequeue is full");
        }
        front = (front + capacity - 1) % capacity;
        data[front] = value;
        count++;
    }

    void push_back(int value) {
        if (count == getCapacity()) {
            throw std::overflow_error("Dequeue is full");
        }
        data[back] = value;
        back = (back + 1) % capacity;
        count++;
    }

    int pop_front() {
        if (empty()) {
            throw std::out_of_range("Dequeue is empty");
        }
        int value = data[front];
        front = (front + 1) % capacity;
        count--;
        return value;
    }

    int pop_back() {
        if (empty()) {
            throw std::out_of_range("Dequeue is empty");
        }
        back = (back + capacity - 1) % capacity;
        int value = data[back];
        count--;
        return value;
    }

    int getFront() const {
        if (empty()) {
            throw std::out_of_range("Dequeue is empty");
        }
        return data[front];
    }

    int getBack() const {
        if (empty()) {
            throw std::out_of_range("Dequeue is empty");
        }
        size_t last_index = (back + capacity - 1) % capacity;
        return data[last_index];
    }

    void clear() noexcept {
        front = 0;
        back = 0;
        count = 0;
    }

    friend std::ostream& operator<<(std::ostream& os, const Dequeue& dq) {
        os << "[ ";
        if (!dq.empty()) {
            size_t current = dq.front;
            for (size_t i = 0; i < dq.count; ++i) {
                os << dq.data[current] << (i == dq.count - 1 ? "" : " ");
                current = (current + 1) % dq.capacity;
            }
        }
        os << " ]";
        return os;
    }

    int& operator[](size_t index) {
        if (index >= count) {
            throw std::out_of_range("Index out of range");
        }
        size_t actual_index = (front + index) % capacity;
        return data[actual_index];
    }

    const int& operator[](size_t index) const {
        if (index >= count) {
            throw std::out_of_range("Index out of range");
        }
        size_t actual_index = (front + index) % capacity;
        return data[actual_index];
    }
};