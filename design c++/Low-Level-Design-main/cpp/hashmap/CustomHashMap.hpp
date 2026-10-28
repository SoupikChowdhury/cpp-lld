#ifndef CUSTOM_HASH_MAP_HPP
#define CUSTOM_HASH_MAP_HPP

#include <vector>
#include <functional>
#include <optional>
#include <stdexcept>

template<typename K, typename V>
class CustomHashMap {
private:
    // Node structure for doubly-linked list
    struct Node {
        K key;
        V value;
        Node* next;
        Node* prev;

        Node() : next(nullptr), prev(nullptr) {}  // Sentinel constructor
        Node(const K& key, const V& value) 
            : key(key), value(value), next(nullptr), prev(nullptr) {}
    };

    static constexpr int INITIAL_CAPACITY = 4;
    static constexpr int MAX_CAPACITY = 1 << 30;
    static constexpr double LOAD_FACTOR = 0.75;

    int capacity;
    int size_;
    std::vector<Node*> buckets;  // Each bucket has head and tail sentinels

    int hash(const K& key) const {
        return std::abs(static_cast<int>(std::hash<K>{}(key))) % capacity;
    }

    void initBucket(int index) {
        Node* head = new Node();
        Node* tail = new Node();
        head->next = tail;
        tail->prev = head;
        buckets[index] = head;
    }

    Node* getTail(int index) const {
        Node* current = buckets[index];
        while (current->next != nullptr) {
            current = current->next;
        }
        return current;
    }

    Node* findNode(const K& key) const {
        int index = hash(key);
        if (buckets[index] == nullptr) return nullptr;

        Node* current = buckets[index]->next;  // Skip head sentinel
        Node* tail = getTail(index);

        while (current != tail) {
            if (current->key == key) {
                return current;
            }
            current = current->next;
        }
        return nullptr;
    }

    void insertNode(int index, Node* node) {
        if (buckets[index] == nullptr) {
            initBucket(index);
        }

        Node* head = buckets[index];
        Node* firstNode = head->next;

        // Insert after head
        head->next = node;
        node->prev = head;
        node->next = firstNode;
        firstNode->prev = node;
    }

    void removeNode(Node* node) {
        Node* prevNode = node->prev;
        Node* nextNode = node->next;
        prevNode->next = nextNode;
        nextNode->prev = prevNode;
        delete node;
    }

    void rehash() {
        int newCapacity = capacity * 2;
        if (newCapacity > MAX_CAPACITY) {
            newCapacity = MAX_CAPACITY;
            if (capacity == MAX_CAPACITY) return;  // Already at max
        }

        std::vector<Node*> oldBuckets = buckets;
        int oldCapacity = capacity;

        capacity = newCapacity;
        buckets = std::vector<Node*>(capacity, nullptr);

        // Rehash all existing nodes
        for (int i = 0; i < oldCapacity; i++) {
            if (oldBuckets[i] == nullptr) continue;

            Node* head = oldBuckets[i];
            Node* current = head->next;

            while (current->next != nullptr) {  // Until tail
                Node* next = current->next;
                
                // Recalculate index and insert
                int newIndex = hash(current->key);
                insertNode(newIndex, current);
                
                current = next;
            }

            // Delete old head and tail sentinels
            delete head;
            delete current;  // This is the tail
        }
    }

public:
    CustomHashMap() : capacity(INITIAL_CAPACITY), size_(0) {
        buckets = std::vector<Node*>(capacity, nullptr);
    }

    ~CustomHashMap() {
        for (int i = 0; i < capacity; i++) {
            if (buckets[i] != nullptr) {
                Node* current = buckets[i];
                while (current != nullptr) {
                    Node* next = current->next;
                    delete current;
                    current = next;
                }
            }
        }
    }

    void put(const K& key, const V& value) {
        Node* existing = findNode(key);
        
        if (existing != nullptr) {
            existing->value = value;  // Update existing
            return;
        }

        // Insert new node
        int index = hash(key);
        Node* newNode = new Node(key, value);
        insertNode(index, newNode);
        size_++;

        // Check if rehashing needed
        if (size_ > capacity * LOAD_FACTOR) {
            rehash();
        }
    }

    std::optional<V> get(const K& key) const {
        Node* node = findNode(key);
        if (node != nullptr) {
            return node->value;
        }
        return std::nullopt;
    }

    V getOrDefault(const K& key, const V& defaultValue) const {
        Node* node = findNode(key);
        return (node != nullptr) ? node->value : defaultValue;
    }

    void remove(const K& key) {
        Node* node = findNode(key);
        if (node != nullptr) {
            removeNode(node);
            size_--;
        }
    }

    bool containsKey(const K& key) const {
        return findNode(key) != nullptr;
    }

    int getSize() const { return size_; }
    bool isEmpty() const { return size_ == 0; }
    int getCapacity() const { return capacity; }

    // Print for debugging
    void print() const {
        std::cout << "HashMap (size=" << size_ << ", capacity=" << capacity << "):" << std::endl;
        for (int i = 0; i < capacity; i++) {
            if (buckets[i] != nullptr) {
                std::cout << "  Bucket " << i << ": ";
                Node* current = buckets[i]->next;
                while (current->next != nullptr) {
                    std::cout << "[" << current->key << "=" << current->value << "] ";
                    current = current->next;
                }
                std::cout << std::endl;
            }
        }
    }
};

#endif // CUSTOM_HASH_MAP_HPP
