#pragma once

#include <vector>
#include <list>
#include <memory>
#include <stdexcept>
#include <functional>
#include <optional>
#include <iostream>

namespace DataStructures {

/**
 * @brief Custom HashMap implementation using chaining for collision resolution
 * 
 * Features:
 * - Separate chaining with linked lists
 * - Dynamic resizing when load factor exceeds threshold
 * - Generic key-value storage
 * - Custom hash function support
 * 
 * @tparam K Key type (must be hashable)
 * @tparam V Value type
 */
template<typename K, typename V>
class MyHashMap {
private:
    /**
     * @brief Node structure for storing key-value pairs
     */
    struct HashNode {
        K key;
        V value;
        
        HashNode(const K& k, const V& v) : key(k), value(v) {}
    };

    // Bucket type - linked list for chaining
    using Bucket = std::list<HashNode>;
    
    std::vector<Bucket> buckets_;
    size_t size_;           // Number of key-value pairs
    size_t capacity_;       // Number of buckets
    float loadFactorThreshold_;
    
    // Hash function
    std::hash<K> hasher_;

    /**
     * @brief Get bucket index for a key
     */
    size_t getBucketIndex(const K& key) const {
        return hasher_(key) % capacity_;
    }

    /**
     * @brief Resize and rehash when load factor exceeded
     */
    void rehash() {
        size_t newCapacity = capacity_ * 2;
        std::vector<Bucket> newBuckets(newCapacity);
        
        // Rehash all existing elements
        for (auto& bucket : buckets_) {
            for (auto& node : bucket) {
                size_t newIndex = hasher_(node.key) % newCapacity;
                newBuckets[newIndex].emplace_back(node.key, node.value);
            }
        }
        
        buckets_ = std::move(newBuckets);
        capacity_ = newCapacity;
        
        std::cout << "[HashMap] Rehashed to capacity: " << capacity_ << "\n";
    }

    /**
     * @brief Check if rehashing is needed
     */
    bool needsRehash() const {
        return static_cast<float>(size_) / capacity_ > loadFactorThreshold_;
    }

public:
    /**
     * @brief Constructor
     * @param initialCapacity Initial number of buckets
     * @param loadFactor Threshold for rehashing (default 0.75)
     */
    explicit MyHashMap(size_t initialCapacity = 16, float loadFactor = 0.75f)
        : size_(0)
        , capacity_(initialCapacity)
        , loadFactorThreshold_(loadFactor) {
        
        buckets_.resize(capacity_);
    }

    /**
     * @brief Insert or update a key-value pair
     * @param key The key
     * @param value The value
     * Time Complexity: O(1) average, O(n) worst case
     */
    void put(const K& key, const V& value) {
        size_t index = getBucketIndex(key);
        Bucket& bucket = buckets_[index];
        
        // Check if key already exists
        for (auto& node : bucket) {
            if (node.key == key) {
                node.value = value;  // Update existing
                return;
            }
        }
        
        // Insert new node
        bucket.emplace_back(key, value);
        size_++;
        
        // Check if rehashing is needed
        if (needsRehash()) {
            rehash();
        }
    }

    /**
     * @brief Get value for a key
     * @param key The key to look up
     * @return Optional containing value if found
     * Time Complexity: O(1) average, O(n) worst case
     */
    std::optional<V> get(const K& key) const {
        size_t index = getBucketIndex(key);
        const Bucket& bucket = buckets_[index];
        
        for (const auto& node : bucket) {
            if (node.key == key) {
                return node.value;
            }
        }
        
        return std::nullopt;  // Key not found
    }

    /**
     * @brief Get value with default if not found
     */
    V getOrDefault(const K& key, const V& defaultValue) const {
        auto result = get(key);
        return result.value_or(defaultValue);
    }

    /**
     * @brief Check if key exists
     * Time Complexity: O(1) average
     */
    bool containsKey(const K& key) const {
        return get(key).has_value();
    }

    /**
     * @brief Remove a key-value pair
     * @param key The key to remove
     * @return true if removed, false if not found
     * Time Complexity: O(1) average, O(n) worst case
     */
    bool remove(const K& key) {
        size_t index = getBucketIndex(key);
        Bucket& bucket = buckets_[index];
        
        for (auto it = bucket.begin(); it != bucket.end(); ++it) {
            if (it->key == key) {
                bucket.erase(it);
                size_--;
                return true;
            }
        }
        
        return false;  // Key not found
    }

    /**
     * @brief Clear all entries
     */
    void clear() {
        for (auto& bucket : buckets_) {
            bucket.clear();
        }
        size_ = 0;
    }

    /**
     * @brief Get number of key-value pairs
     */
    size_t size() const { return size_; }

    /**
     * @brief Check if empty
     */
    bool isEmpty() const { return size_ == 0; }

    /**
     * @brief Get current capacity (number of buckets)
     */
    size_t capacity() const { return capacity_; }

    /**
     * @brief Get current load factor
     */
    float loadFactor() const {
        return static_cast<float>(size_) / capacity_;
    }

    /**
     * @brief Get all keys
     */
    std::vector<K> keys() const {
        std::vector<K> result;
        result.reserve(size_);
        
        for (const auto& bucket : buckets_) {
            for (const auto& node : bucket) {
                result.push_back(node.key);
            }
        }
        
        return result;
    }

    /**
     * @brief Get all values
     */
    std::vector<V> values() const {
        std::vector<V> result;
        result.reserve(size_);
        
        for (const auto& bucket : buckets_) {
            for (const auto& node : bucket) {
                result.push_back(node.value);
            }
        }
        
        return result;
    }

    /**
     * @brief Get all key-value pairs
     */
    std::vector<std::pair<K, V>> entries() const {
        std::vector<std::pair<K, V>> result;
        result.reserve(size_);
        
        for (const auto& bucket : buckets_) {
            for (const auto& node : bucket) {
                result.emplace_back(node.key, node.value);
            }
        }
        
        return result;
    }

    /**
     * @brief Apply function to each entry
     */
    void forEach(std::function<void(const K&, V&)> func) {
        for (auto& bucket : buckets_) {
            for (auto& node : bucket) {
                func(node.key, node.value);
            }
        }
    }

    /**
     * @brief Operator[] for convenient access
     * Creates entry with default value if not exists
     */
    V& operator[](const K& key) {
        size_t index = getBucketIndex(key);
        Bucket& bucket = buckets_[index];
        
        for (auto& node : bucket) {
            if (node.key == key) {
                return node.value;
            }
        }
        
        // Insert new with default value
        bucket.emplace_back(key, V{});
        size_++;
        
        if (needsRehash()) {
            rehash();
            // Need to find the node again after rehash
            index = getBucketIndex(key);
            for (auto& node : buckets_[index]) {
                if (node.key == key) {
                    return node.value;
                }
            }
        }
        
        return bucket.back().value;
    }

    /**
     * @brief Print hash map statistics
     */
    void printStats() const {
        std::cout << "\n=== HashMap Statistics ===\n";
        std::cout << "Size: " << size_ << "\n";
        std::cout << "Capacity: " << capacity_ << "\n";
        std::cout << "Load Factor: " << loadFactor() << "\n";
        
        // Bucket distribution
        size_t emptyBuckets = 0;
        size_t maxChainLength = 0;
        
        for (const auto& bucket : buckets_) {
            if (bucket.empty()) {
                emptyBuckets++;
            }
            maxChainLength = std::max(maxChainLength, bucket.size());
        }
        
        std::cout << "Empty Buckets: " << emptyBuckets << "/" << capacity_ << "\n";
        std::cout << "Max Chain Length: " << maxChainLength << "\n";
        std::cout << "===========================\n";
    }

    /**
     * @brief Print all entries (for debugging)
     */
    void printAll() const {
        std::cout << "\n{";
        bool first = true;
        for (const auto& bucket : buckets_) {
            for (const auto& node : bucket) {
                if (!first) std::cout << ", ";
                std::cout << node.key << ": " << node.value;
                first = false;
            }
        }
        std::cout << "}\n";
    }
};

/**
 * @brief Specialized HashMap for string keys with custom hash
 */
template<typename V>
class StringHashMap : public MyHashMap<std::string, V> {
public:
    StringHashMap(size_t initialCapacity = 16)
        : MyHashMap<std::string, V>(initialCapacity) {}
};

} // namespace DataStructures
