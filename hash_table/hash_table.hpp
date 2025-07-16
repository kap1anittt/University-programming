#pragma once

#include <vector>
#include <functional>
#include <utility>
#include <stdexcept>

template<typename KeyType, typename ValueType>
class HashTable {
private:
    struct Node {
        KeyType key;
        ValueType value;
        Node* next;

        Node(const KeyType& k, const ValueType& v) : key(k), value(v), next(nullptr) {}
    };

    std::vector<Node*> table;
    std::hash<KeyType> hash_function;
    size_t size_;

public:
    HashTable(size_t capacity = 16) : table(capacity, nullptr), size_(0) {}

    ~HashTable() {
        clear();
    }

    HashTable(const HashTable& other) : table(other.table.size(), nullptr), hash_function(other.hash_function), size_(0) {
        for (auto it = other.begin(); it != other.end(); ++it) {
            const auto& pair = *it;
            insert(pair.first, pair.second);
        }
    }

    HashTable& operator=(const HashTable& other) {
        if (this != &other) {
            clear();
            table.resize(other.table.size(), nullptr);
            size_ = 0;
            for (auto it = other.begin(); it != other.end(); ++it) {
                const auto& pair = *it;
                insert(pair.first, pair.second);
            }
        }
        return *this;
    }

    void clear() {
        for (auto& head : table) {
            while (head) {
                Node* temp = head;
                head = head->next;
                delete temp;
            }
        }
        std::fill(table.begin(), table.end(), nullptr);
        size_ = 0;
    }

    void insert(const KeyType& key, const ValueType& value) {
        size_t index = hash_function(key) % table.size();
        
        Node* current = table[index];
        while (current) {
            if (current->key == key) {
                current->value = value;
                return;
            }
            current = current->next;
        }
        
        Node* new_node = new Node(key, value);
        new_node->next = table[index];
        table[index] = new_node;
        size_++;
        
        if (size_ > table.size() * 0.75) {
            rehash();
        }
    }

    bool contains(const KeyType& key) const {
        size_t index = hash_function(key) % table.size();
        Node* current = table[index];
        
        while (current) {
            if (current->key == key) {
                return true;
            }
            current = current->next;
        }
        
        return false;
    }

    ValueType& operator[](const KeyType& key) {
        size_t index = hash_function(key) % table.size();
        Node* current = table[index];
        
        while (current) {
            if (current->key == key) {
                return current->value;
            }
            current = current->next;
        }
        
        Node* new_node = new Node(key, ValueType());
        new_node->next = table[index];
        table[index] = new_node;
        size_++;
        
        if (size_ > table.size() * 0.75) {
            rehash();
            index = hash_function(key) % table.size();
            current = table[index];
            while (current) {
                if (current->key == key) {
                    return current->value;
                }
                current = current->next;
            }
        }
        
        return table[index]->value;
    }

    size_t size() const {
        return size_;
    }

    bool empty() const {
        return size_ == 0;
    }

private:
    void rehash() {
        size_t new_capacity = table.size() * 2;
        std::vector<Node*> new_table(new_capacity, nullptr);
        
        for (auto& head : table) {
            while (head) {
                Node* node = head;
                head = head->next;
                
                size_t new_index = hash_function(node->key) % new_capacity;
                node->next = new_table[new_index];
                new_table[new_index] = node;
            }
        }
        
        table = std::move(new_table);
    }

public:
    using value_type = std::pair<const KeyType&, ValueType&>;

    class Iterator {
    private:
        size_t index;
        Node* current;
        std::vector<Node*>& table;

    public:
        Iterator(size_t i, Node* node, const std::vector<Node*>& tbl)
            : index(i), current(node), table(const_cast<std::vector<Node*>&>(tbl)) {
            if (current == nullptr && index < table.size()) {
                advance();
            }
        }

        Iterator& operator++() {
            if (current != nullptr) {
                current = current->next;
                if (current == nullptr) {
                    index++;
                    advance();
                }
            }
            return *this;
        }

        bool operator!=(const Iterator& other) const {
            return index != other.index || current != other.current;
        }
        
        bool operator==(const Iterator& other) const {
            return index == other.index && current == other.current;
        }

        value_type operator*() const {
            if (current == nullptr) {
                throw std::out_of_range("Iterator is out of range");
            }
            return {current->key, current->value};
        }

    private:
        void advance() {
            while (index < table.size() && table[index] == nullptr) {
                index++;
            }
            if (index < table.size()) {
                current = table[index];
            } else {
                current = nullptr;
            }
        }
    };

    Iterator begin() const {
        return Iterator(0, table.empty() ? nullptr : table[0], table);
    }

    Iterator end() const {
        return Iterator(table.size(), nullptr, table);
    }
}; 