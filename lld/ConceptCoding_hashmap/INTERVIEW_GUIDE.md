# HashMap Implementation - LLD Interview Guide

A step-by-step framework for delivering the HashMap data structure low-level design interview.

---

## 📁 Code Reference

| File | Purpose | Key Functions |
|------|---------|---------------|
| [MyHashMap.hpp](MyHashMap.hpp) | Custom HashMap implementation | `put()`, `get()`, `remove()`, `containsKey()`, `clear()`, `size()`, `hash()`, `resize()` |
| [main.cpp](main.cpp) | Test/demo file | Usage examples |

---

## 🎯 Design Patterns & Techniques

| Pattern/Technique | Implementation Location | Purpose |
|-------------------|------------------------|----------|
| **Template Pattern** | `MyHashMap<K, V>` class | Generic key-value storage |
| **Chaining** | `std::list<std::pair<K,V>>` at each bucket | Collision resolution |
| **Dynamic Resizing** | `resize()` method | Maintain O(1) average performance |
| **Hash Function** | `hash()` using `std::hash<K>` | Convert keys to bucket indices |

---

## 1) Requirements (~5 minutes)

### Clarifying Questions to Ask
- "What types of keys and values should the HashMap support?"
- "Should we handle collisions? What method is preferred?"
- "Do we need dynamic resizing (rehashing)?"
- "Should it be thread-safe?"
- "What's the expected load factor threshold?"

### Requirements

```
Requirements:
1. Store key-value pairs with O(1) average lookup, insert, and delete.
2. Handle hash collisions using chaining (linked lists at each bucket).
3. Support dynamic resizing when load factor exceeds threshold (0.75).
4. Provide standard operations: put, get, remove, containsKey.
5. Handle duplicate keys by updating the value.
6. Support iteration over keys and values.

Out of Scope:
- Thread safety (unless asked)
- Custom hash functions (use default)
- Ordered iteration
- Null key handling
- Treeification of buckets (Java 8+ feature)
```

---

## 2) Entities and Relationships (~3 minutes)

### Identify Entities

Scan the requirements and pull out meaningful nouns:

- **HashMap** - The main container
- **HashNode** - Key-value pair stored in buckets
- **Bucket** - List of nodes at each index (for chaining)
- **Hash Function** - Converts keys to bucket indices

### Define Relationships

```
Entities:
- MyHashMap<K, V> (main class)
- HashNode<K, V> (entry)
- Bucket = list<HashNode> (collision chain)

Relationships:
- HashMap -> Bucket (1:N, array of buckets)
- Bucket -> HashNode (1:N, linked list)
```

---

## 3) Class Design (~10-15 minutes)

### HashNode

| Requirement | State Needed |
|-------------|--------------|
| Store key-value | key: K, value: V |

```
class HashNode<K, V>:
  - key: K
  - value: V

  + HashNode(key: K, value: V)
  + getKey() -> K
  + getValue() -> V
  + setValue(value: V)
```

### MyHashMap

| Requirement | State Needed |
|-------------|--------------|
| Store buckets | buckets: vector<list<HashNode>> |
| Track count | size: int |
| Track capacity | capacity: int |
| Load factor threshold | loadFactorThreshold: float (0.75) |

```
class MyHashMap<K, V>:
  - buckets: vector<Bucket>
  - size: int
  - capacity: int
  - loadFactorThreshold: float = 0.75
  - hasher: hash<K>

  + put(key: K, value: V)
  + get(key: K) -> V?
  + remove(key: K) -> bool
  + containsKey(key: K) -> bool
  + getSize() -> int
  + isEmpty() -> bool
  + keys() -> vector<K>
  + values() -> vector<V>
  + clear()
```

---

## 4) Implementation (~10 minutes)

### Key Method: getBucketIndex()

```
getBucketIndex(key: K) -> int
    hashCode = hasher(key)
    return hashCode % capacity
```

### Key Method: put()

```
put(key: K, value: V) -> void
    // Check if rehashing needed
    if loadFactor() > loadFactorThreshold
        rehash()
    
    index = getBucketIndex(key)
    bucket = buckets[index]
    
    // Check if key exists (update value)
    for node in bucket:
        if node.key == key
            node.value = value
            return
    
    // Key doesn't exist, add new node
    bucket.push_back(HashNode(key, value))
    size++
```

### Key Method: get()

```
get(key: K) -> V?
    index = getBucketIndex(key)
    bucket = buckets[index]
    
    for node in bucket:
        if node.key == key
            return node.value
    
    return null  // Key not found
```

### Key Method: remove()

```
remove(key: K) -> bool
    index = getBucketIndex(key)
    bucket = buckets[index]
    
    for it = bucket.begin(); it != bucket.end(); it++:
        if it->key == key
            bucket.erase(it)
            size--
            return true
    
    return false  // Key not found
```

### Key Method: rehash()

```
rehash() -> void
    newCapacity = capacity * 2
    newBuckets = vector<Bucket>(newCapacity)
    
    // Redistribute all entries
    for bucket in buckets:
        for node in bucket:
            newIndex = hasher(node.key) % newCapacity
            newBuckets[newIndex].push_back(node)
    
    buckets = newBuckets
    capacity = newCapacity
```

### Key Method: loadFactor()

```
loadFactor() -> float
    return (float)size / capacity
```

### Verification: Walk Through a Scenario

```
Initial: capacity = 4, size = 0, buckets = [[], [], [], []]

1. put("apple", 1)
   - hash("apple") % 4 = 2
   - buckets[2].push({"apple", 1})
   - size = 1
   - buckets = [[], [], [apple:1], []]

2. put("banana", 2)
   - hash("banana") % 4 = 1
   - buckets[1].push({"banana", 2})
   - size = 2
   - buckets = [[], [banana:2], [apple:1], []]

3. put("cherry", 3)  // Assume collision with "apple" at index 2
   - hash("cherry") % 4 = 2
   - buckets[2].push({"cherry", 3})
   - size = 3
   - buckets = [[], [banana:2], [apple:1, cherry:3], []]

4. get("cherry")
   - hash("cherry") % 4 = 2
   - Search bucket[2]: apple != cherry, cherry == cherry ✓
   - Return 3

5. put("date", 4)  // Triggers rehash (loadFactor = 4/4 = 1.0 > 0.75)
   - rehash(): capacity = 8
   - Redistribute all entries with new indices
   - size = 4
```

---

## 5) Extensibility (~5 minutes)

### "How would you make this thread-safe?"

> "For simple thread safety, add a mutex and lock on every operation. For better concurrency, use bucket-level locks or a concurrent hash map with lock striping."

```
class ConcurrentHashMap<K, V>:
  - bucketLocks: vector<mutex>
  
  + put(key, value)
      index = getBucketIndex(key)
      lock_guard<mutex> lock(bucketLocks[index])
      // Insert logic
  
  + get(key) -> V?
      index = getBucketIndex(key)
      shared_lock lock(bucketLocks[index])  // Read lock
      // Lookup logic
```

### "What makes a good hash function?"

> "A good hash function is deterministic (same input → same output), provides uniform distribution across buckets, is fast to compute, and minimizes collisions. For strings, polynomial rolling hash works well."

```
// Good hash function for strings
hash(s: string) -> int
    h = 0
    for c in s:
        h = h * 31 + c
    return h
```

### "How would you implement LRU Cache using HashMap?"

> "Combine HashMap with a doubly-linked list. HashMap provides O(1) lookup, list maintains access order. On access, move node to front. On capacity overflow, evict from back."

```
class LRUCache<K, V>:
  - cache: list<pair<K, V>>
  - map: HashMap<K, list_iterator>
  - capacity: int
  
  + get(key) -> V
      // Move to front on access
  
  + put(key, value)
      // Add to front, evict from back if over capacity
```

### "How to handle very long collision chains?"

> "Java 8+ converts chains to red-black trees when length exceeds 8. This improves worst case from O(n) to O(log n). Implementation: when chain length > threshold, convert bucket to TreeMap."

---

## Design Patterns Summary

| Pattern | Where | Why |
|---------|-------|-----|
| **Strategy** | Hash function (extension) | Allow custom hashing |
| **Iterator** | Key/value iteration | Standard traversal interface |

---

## Time/Space Complexity

| Operation | Average | Worst Case |
|-----------|---------|------------|
| put(key, value) | O(1) | O(n) |
| get(key) | O(1) | O(n) |
| remove(key) | O(1) | O(n) |
| containsKey(key) | O(1) | O(n) |
| keys() / values() | O(n) | O(n) |
| rehash() | O(n) | O(n) |

**Space Complexity**: O(n) where n = number of entries

**Worst case** occurs when all keys hash to the same bucket (degenerate to linked list).

---

## Common Interview Questions

### "Why is default load factor 0.75?"

> Trade-off between time and space. Lower (0.5) means fewer collisions but more memory waste. Higher (0.9) means more collisions but less memory. 0.75 is empirically a good balance.

### "HashMap vs TreeMap?"

| Feature | HashMap | TreeMap |
|---------|---------|---------|
| Ordering | Unordered | Sorted by key |
| Lookup | O(1) average | O(log n) |
| Implementation | Hash table | Red-Black Tree |
| Use case | Fast lookup | Range queries, ordered iteration |

### "What if we need the insertion order preserved?"

> Use LinkedHashMap - maintains a doubly-linked list of entries in insertion order while keeping O(1) access.
