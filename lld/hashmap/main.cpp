// Custom HashMap - Main Entry Point
// Demonstrates Data Structure implementation with separate chaining

#include <iostream>
#include <string>
#include "CustomHashMap.hpp"

int main() {
    std::cout << "=== Custom HashMap Demo ===" << std::endl;
    std::cout << std::endl;

    CustomHashMap<std::string, int> map;

    std::cout << "--- Initial State ---" << std::endl;
    std::cout << "Size: " << map.getSize() << std::endl;
    std::cout << "Capacity: " << map.getCapacity() << std::endl;
    std::cout << "Is Empty: " << (map.isEmpty() ? "Yes" : "No") << std::endl;

    std::cout << std::endl << "--- Inserting Elements ---" << std::endl;
    
    map.put("apple", 10);
    std::cout << "Inserted: apple -> 10" << std::endl;
    
    map.put("banana", 20);
    std::cout << "Inserted: banana -> 20" << std::endl;
    
    map.put("cherry", 30);
    std::cout << "Inserted: cherry -> 30" << std::endl;
    
    map.put("date", 40);
    std::cout << "Inserted: date -> 40" << std::endl;

    std::cout << std::endl << "After 4 insertions:" << std::endl;
    std::cout << "Size: " << map.getSize() << std::endl;
    std::cout << "Capacity: " << map.getCapacity() << std::endl;

    // This should trigger rehash (load factor > 0.75)
    map.put("elderberry", 50);
    std::cout << "\nInserted: elderberry -> 50 (may trigger rehash)" << std::endl;
    std::cout << "Size: " << map.getSize() << std::endl;
    std::cout << "Capacity: " << map.getCapacity() << std::endl;

    std::cout << std::endl << "--- Retrieving Elements ---" << std::endl;
    
    auto appleVal = map.get("apple");
    std::cout << "apple: " << (appleVal.has_value() ? std::to_string(*appleVal) : "not found") << std::endl;
    
    auto bananaVal = map.get("banana");
    std::cout << "banana: " << (bananaVal.has_value() ? std::to_string(*bananaVal) : "not found") << std::endl;
    
    auto grapeVal = map.get("grape");
    std::cout << "grape: " << (grapeVal.has_value() ? std::to_string(*grapeVal) : "not found") << std::endl;

    std::cout << std::endl << "--- Using getOrDefault ---" << std::endl;
    std::cout << "grape (default 0): " << map.getOrDefault("grape", 0) << std::endl;
    std::cout << "cherry (default 0): " << map.getOrDefault("cherry", 0) << std::endl;

    std::cout << std::endl << "--- Updating Element ---" << std::endl;
    map.put("apple", 100);
    std::cout << "Updated apple -> 100" << std::endl;
    std::cout << "apple: " << map.getOrDefault("apple", 0) << std::endl;

    std::cout << std::endl << "--- Checking containsKey ---" << std::endl;
    std::cout << "Contains 'apple': " << (map.containsKey("apple") ? "Yes" : "No") << std::endl;
    std::cout << "Contains 'grape': " << (map.containsKey("grape") ? "Yes" : "No") << std::endl;

    std::cout << std::endl << "--- Removing Element ---" << std::endl;
    map.remove("banana");
    std::cout << "Removed: banana" << std::endl;
    std::cout << "Contains 'banana': " << (map.containsKey("banana") ? "Yes" : "No") << std::endl;
    std::cout << "Size: " << map.getSize() << std::endl;

    std::cout << std::endl << "--- Adding More Elements (Testing Collisions) ---" << std::endl;
    map.put("fig", 60);
    map.put("guava", 70);
    map.put("honeydew", 80);
    map.put("kiwi", 90);
    
    std::cout << "Added: fig, guava, honeydew, kiwi" << std::endl;
    std::cout << "Final Size: " << map.getSize() << std::endl;
    std::cout << "Final Capacity: " << map.getCapacity() << std::endl;

    std::cout << std::endl << "=== Demo Complete ===" << std::endl;

    return 0;
}
