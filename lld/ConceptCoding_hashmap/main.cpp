#include <iostream>
#include <string>
#include <cassert>
#include "MyHashMap.hpp"

using namespace DataStructures;

void testBasicOperations() {
    std::cout << "\n========== Test: Basic Operations ==========\n";
    
    MyHashMap<std::string, int> map;
    
    // Test put and get
    map.put("apple", 100);
    map.put("banana", 200);
    map.put("cherry", 300);
    
    assert(map.get("apple").value() == 100);
    assert(map.get("banana").value() == 200);
    assert(map.get("cherry").value() == 300);
    assert(!map.get("grape").has_value());
    
    std::cout << "✓ Put and Get operations work correctly\n";
    
    // Test size
    assert(map.size() == 3);
    std::cout << "✓ Size is correct: " << map.size() << "\n";
    
    // Test containsKey
    assert(map.containsKey("apple"));
    assert(!map.containsKey("grape"));
    std::cout << "✓ ContainsKey works correctly\n";
    
    // Test update
    map.put("apple", 150);
    assert(map.get("apple").value() == 150);
    assert(map.size() == 3);  // Size should not change
    std::cout << "✓ Update existing key works\n";
    
    map.printAll();
}

void testRemoveOperation() {
    std::cout << "\n========== Test: Remove Operation ==========\n";
    
    MyHashMap<int, std::string> map;
    
    map.put(1, "one");
    map.put(2, "two");
    map.put(3, "three");
    
    assert(map.size() == 3);
    
    // Remove existing key
    bool removed = map.remove(2);
    assert(removed);
    assert(map.size() == 2);
    assert(!map.containsKey(2));
    std::cout << "✓ Remove existing key works\n";
    
    // Remove non-existing key
    removed = map.remove(99);
    assert(!removed);
    assert(map.size() == 2);
    std::cout << "✓ Remove non-existing key returns false\n";
    
    map.printAll();
}

void testRehashing() {
    std::cout << "\n========== Test: Rehashing ==========\n";
    
    MyHashMap<int, int> map(4, 0.75f);  // Small capacity to trigger rehash
    
    std::cout << "Initial capacity: " << map.capacity() << "\n";
    
    // Insert enough elements to trigger rehash
    for (int i = 0; i < 10; i++) {
        map.put(i, i * 10);
    }
    
    std::cout << "After insertions - Capacity: " << map.capacity() << "\n";
    
    // Verify all elements are still accessible
    for (int i = 0; i < 10; i++) {
        assert(map.get(i).value() == i * 10);
    }
    std::cout << "✓ All elements accessible after rehashing\n";
    
    map.printStats();
}

void testOperatorBracket() {
    std::cout << "\n========== Test: Operator[] ==========\n";
    
    MyHashMap<std::string, int> map;
    
    // Insert using operator[]
    map["x"] = 10;
    map["y"] = 20;
    map["z"] = 30;
    
    assert(map["x"] == 10);
    assert(map["y"] == 20);
    assert(map["z"] == 30);
    std::cout << "✓ Operator[] for insertion works\n";
    
    // Modify using operator[]
    map["x"] = 100;
    assert(map["x"] == 100);
    std::cout << "✓ Operator[] for modification works\n";
    
    // Access non-existing creates default
    int val = map["new_key"];  // Should create with default value 0
    assert(val == 0);
    assert(map.containsKey("new_key"));
    std::cout << "✓ Operator[] creates default for missing key\n";
}

void testGetOrDefault() {
    std::cout << "\n========== Test: GetOrDefault ==========\n";
    
    MyHashMap<std::string, int> map;
    map.put("a", 1);
    
    assert(map.getOrDefault("a", -1) == 1);
    assert(map.getOrDefault("missing", -1) == -1);
    std::cout << "✓ GetOrDefault works correctly\n";
}

void testKeysValuesEntries() {
    std::cout << "\n========== Test: Keys, Values, Entries ==========\n";
    
    MyHashMap<int, std::string> map;
    map.put(1, "one");
    map.put(2, "two");
    map.put(3, "three");
    
    auto keys = map.keys();
    assert(keys.size() == 3);
    std::cout << "Keys: ";
    for (const auto& k : keys) std::cout << k << " ";
    std::cout << "\n";
    
    auto values = map.values();
    assert(values.size() == 3);
    std::cout << "Values: ";
    for (const auto& v : values) std::cout << v << " ";
    std::cout << "\n";
    
    auto entries = map.entries();
    assert(entries.size() == 3);
    std::cout << "Entries: ";
    for (const auto& [k, v] : entries) std::cout << "(" << k << ":" << v << ") ";
    std::cout << "\n";
    
    std::cout << "✓ Keys, Values, Entries work correctly\n";
}

void testForEach() {
    std::cout << "\n========== Test: ForEach ==========\n";
    
    MyHashMap<std::string, int> map;
    map.put("a", 1);
    map.put("b", 2);
    map.put("c", 3);
    
    // Double all values
    map.forEach([](const std::string& key, int& value) {
        value *= 2;
    });
    
    assert(map.get("a").value() == 2);
    assert(map.get("b").value() == 4);
    assert(map.get("c").value() == 6);
    std::cout << "✓ ForEach modifies values correctly\n";
    
    map.printAll();
}

void testClearAndEmpty() {
    std::cout << "\n========== Test: Clear and Empty ==========\n";
    
    MyHashMap<int, int> map;
    
    assert(map.isEmpty());
    std::cout << "✓ New map is empty\n";
    
    map.put(1, 100);
    map.put(2, 200);
    
    assert(!map.isEmpty());
    
    map.clear();
    
    assert(map.isEmpty());
    assert(map.size() == 0);
    assert(!map.containsKey(1));
    std::cout << "✓ Clear works correctly\n";
}

void testCollisionHandling() {
    std::cout << "\n========== Test: Collision Handling ==========\n";
    
    // Use small capacity to force collisions
    MyHashMap<int, std::string> map(2, 10.0f);  // High load factor to prevent rehash
    
    // These keys will likely collide in a 2-bucket map
    map.put(0, "zero");
    map.put(2, "two");
    map.put(4, "four");
    map.put(6, "six");
    
    // Verify all elements are accessible
    assert(map.get(0).value() == "zero");
    assert(map.get(2).value() == "two");
    assert(map.get(4).value() == "four");
    assert(map.get(6).value() == "six");
    
    std::cout << "✓ Collision handling (chaining) works correctly\n";
    
    map.printStats();
}

void testLargeDataset() {
    std::cout << "\n========== Test: Large Dataset ==========\n";
    
    MyHashMap<int, int> map;
    const int N = 10000;
    
    // Insert N elements
    for (int i = 0; i < N; i++) {
        map.put(i, i * i);
    }
    
    assert(map.size() == N);
    std::cout << "✓ Inserted " << N << " elements\n";
    
    // Verify all elements
    for (int i = 0; i < N; i++) {
        assert(map.get(i).value() == i * i);
    }
    std::cout << "✓ All elements verified\n";
    
    // Remove half
    for (int i = 0; i < N; i += 2) {
        map.remove(i);
    }
    
    assert(map.size() == N / 2);
    std::cout << "✓ Removed half the elements\n";
    
    map.printStats();
}

int main() {
    std::cout << "╔═══════════════════════════════════════════════════════════╗\n";
    std::cout << "║          CUSTOM HASHMAP - COMPREHENSIVE TESTS             ║\n";
    std::cout << "╚═══════════════════════════════════════════════════════════╝\n";

    testBasicOperations();
    testRemoveOperation();
    testRehashing();
    testOperatorBracket();
    testGetOrDefault();
    testKeysValuesEntries();
    testForEach();
    testClearAndEmpty();
    testCollisionHandling();
    testLargeDataset();

    std::cout << "\n╔═══════════════════════════════════════════════════════════╗\n";
    std::cout << "║              ALL TESTS PASSED SUCCESSFULLY!                ║\n";
    std::cout << "╚═══════════════════════════════════════════════════════════╝\n";

    return 0;
}
