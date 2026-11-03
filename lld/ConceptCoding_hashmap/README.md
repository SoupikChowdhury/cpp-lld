# HashMap (Concept && Coding version) - C++

A second implementation of the same question. **To learn how to deliver the question in an interview, read [`../hashmap/README.md`](../hashmap/README.md)**: requirements, chaining vs open addressing, load factor, amortized resizing and the follow-ups are the same.

This README covers only what's different here: how this design compares, what it does better, where it breaks, and how to run it. Everything matches the C++ code in this folder.

---

## 1. How this version differs

| Topic | Main version (`hashmap`) | This version (`MyHashMap`) |
|---|---|---|
| Bucket | Hand-written doubly linked list with head/tail sentinels | **`std::list<HashNode>`** |
| Memory | Raw `new` / `delete` | Standard containers own everything (**rule of zero**) |
| Copying | Compiles, then corrupts the heap | **Correct deep copy** (tested: the copy and the original are independent) |
| Requirements on `V` | `K` and `V` must be default-constructible (sentinels) | Only `operator[]` needs a default `V`, and only if you call it |
| Hash to index | `std::abs(static_cast<int>(hash)) % capacity` | **`hasher_(key) % capacity_` in `size_t`** (no truncation, no `abs(INT_MIN)`) |
| Starting size, load factor | Fixed: 4 and 0.75 | **Constructor arguments** (default 16, 0.75), not validated |
| Rehash | Relinks existing nodes (no copies, addresses stay valid) | **Copies every entry** into new lists (addresses change) |
| API | put, get, getOrDefault, remove, containsKey | Plus **`operator[]`, `keys()`, `values()`, `entries()`, `forEach`, `clear`, `printStats`**; `remove` returns `bool` |
| Lookup cost | `findNode` walks the bucket twice | One walk |
| Output | Silent | Rehash prints `[HashMap] Rehashed to capacity: N` |

**Which to present in an interview?** This one, with two changes: move the nodes during rehash instead of copying them (`std::list::splice`), and validate the constructor arguments. Using `std::list` for buckets is fine unless the interviewer explicitly wants you to write the linked list yourself; then the main version shows how.

---

## 2. Design at a glance

```cpp
template<typename K, typename V>
class MyHashMap {
    struct HashNode { K key; V value; };
    using Bucket = std::list<HashNode>;
    std::vector<Bucket> buckets_;
    size_t size_, capacity_;
    float loadFactorThreshold_;
    std::hash<K> hasher_;
public:
    explicit MyHashMap(size_t initialCapacity = 16, float loadFactor = 0.75f);
    void put(const K& key, const V& value);
    std::optional<V> get(const K& key) const;
    V getOrDefault(const K& key, const V& defaultValue) const;
    bool remove(const K& key);
    V& operator[](const K& key);              // inserts V{} if missing
    std::vector<K> keys() const;  std::vector<V> values() const;
    std::vector<std::pair<K, V>> entries() const;
    void forEach(std::function<void(const K&, V&)> func);
    void clear();
};
// StringHashMap<V>: just MyHashMap<std::string, V> with a constructor (no custom hash, despite its comment)
```

---

## 3. Key code

### Index and rehash

```cpp
// MyHashMap.hpp
size_t getBucketIndex(const K& key) const {
    return hasher_(key) % capacity_;
}

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
    // ... prints "[HashMap] Rehashed to capacity: N"
}
```

`emplace_back(node.key, node.value)` builds a **new** node from copies; the old lists are then destroyed. With `std::list::splice` the existing nodes could be moved into their new buckets with no copying and no new allocations.

### operator[] has to look again after a rehash

```cpp
// MyHashMap.hpp
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
```

Because rehash rebuilds every list, the node just added and the `bucket` reference no longer exist afterwards, so the code searches again. That's the same reason a reference taken from `operator[]` earlier dangles once a later insert triggers a rehash (see gaps).

---

## 4. Verification

The outputs below are from actually running the code.

```text
main.cpp's own tests (assert-based): all pass
    put/get/containsKey/update, remove (true/false), operator[], getOrDefault,
    keys/values/entries, forEach, clear
    rehash test: capacity 4 -> "Rehashed to capacity: 8" -> 16 after 10 inserts
    collision test: capacity 2, load factor 10, keys 0, 2, 4, 6 -> max chain length 4
        (all even, so all in bucket 0)
    10,000 inserts from capacity 16 -> 10 rehashes, up to 16384

Separate test program:
300,000 random put / remove / get vs std::unordered_map   -> 0 mismatches
20,000 int keys i: 4 ms;  keys i << 16: 671 ms            (same low-bit clustering)
copy, then change the copy                                -> original unchanged
1000 puts of a value type that counts copies, from capacity 4 -> 2542 copies
reference to map["k"] across rehashes                     -> address changed
load factor 0: capacity 64 after 2 puts ... 67,108,864 after 22 puts (1.1 s)
capacity 0: put(1, 1) -> crash 0xC0000094 (integer divide by zero)
```

---

## 5. Gaps in this code

| Gap | What happens | Fix to say |
|---|---|---|
| **Constructor arguments aren't validated** | `MyHashMap(0)` crashes on the first `put` (`hash % 0`, `0xC0000094`), and doubling 0 stays 0. A load factor of 0 rehashes on **every** insert: 67 million buckets after 22 puts. | Require `initialCapacity >= 1` and `loadFactor > 0` (throw `std::invalid_argument`). |
| **Rehash copies every entry** | 1000 puts made 2542 value copies; the address of every value changes on each rehash, so references from `operator[]` or `forEach` dangle afterwards. | `splice` the existing nodes into their new buckets (no copies, addresses stay valid), or move `key`/`value`. |
| **Keys sharing low bits all collide** | 20,000 `int` keys `i << 16` took 671 ms vs 4 ms for `i`: identity hash plus power-of-two capacity. | Mix the hash bits before `%`, or use a prime capacity. |
| **The data structure prints** | Every rehash writes "[HashMap] Rehashed to capacity: N" to `std::cout`. | Leave logging to the caller (or behind a debug flag). |
| **Never shrinks** (by reading) | `remove` and `clear` keep the capacity: after removing half of 10,000 entries it stays at 16384. | Halve when the load factor drops below about 0.25. |
| **`StringHashMap` adds nothing** (by reading) | Its comment says "custom hash", but it only forwards the constructor. | Remove it, or give `MyHashMap` a `Hash` template parameter like `std::unordered_map`. |
| **No thread safety** (by reading) | Concurrent use is a data race. | A `std::shared_mutex`, or lock striping (see the main README). |

---

## 6. Test yourself

<details><summary>1. Why does copying this map work when copying the main version crashes?</summary>

All memory is owned by `std::vector` and `std::list`, whose copy constructors deep-copy. The class needs no destructor or copy operations of its own (rule of zero). The main version owns raw `new`ed nodes but doesn't define copying (rule of three).
</details>

<details><summary>2. Why does <code>operator[]</code> search again after calling <code>rehash()</code>?</summary>

Rehash builds brand-new lists and destroys the old ones, so the node just inserted and the `bucket` reference are gone. The value now lives in a different node in a different bucket.
</details>

<details><summary>3. How would you rehash without copying any entries?</summary>

Move each node into its new bucket with `std::list::splice` (it relinks the node, so nothing is copied and references stay valid), or at least `std::move` the key and value.
</details>

<details><summary>4. What happens with <code>MyHashMap(0)</code> or a load factor of 0?</summary>

Capacity 0: the first `put` computes `hash % 0` and crashes. Load factor 0: `size / capacity > 0` is always true, so every insert doubles the table (67 million buckets after 22 puts). Validate both in the constructor.
</details>

<details><summary>5. Why is <code>hasher_(key) % capacity_</code> safer than the main version's index calculation?</summary>

It stays in `size_t`, so the full hash is used and the result is never negative. The main version casts to `int` (losing the upper bits) and calls `std::abs`, which is undefined for `INT_MIN`.
</details>

<details><summary>6. Is using <code>std::list</code> for buckets acceptable in an interview?</summary>

Usually yes, and it avoids memory bugs. If the interviewer wants the list written by hand, the main version shows that; either way, explain why a node-based list keeps removal O(1) once the node is found.
</details>

---

## 7. Run it

```bash
cd lld/ConceptCoding_hashmap
g++ -std=c++17 main.cpp -o hashmap_cc
./hashmap_cc
```

`main.cpp` is a set of `assert`-based tests; each prints a ✓ line. Expected key lines (shortened):

```text
========== Test: Basic Operations ==========
✓ Put and Get operations work correctly
...
========== Test: Rehashing ==========
Initial capacity: 4
[HashMap] Rehashed to capacity: 8
[HashMap] Rehashed to capacity: 16
After insertions - Capacity: 16
✓ All elements accessible after rehashing
...
========== Test: Large Dataset ==========
...
✓ Removed half the elements
...
ALL TESTS PASSED SUCCESSFULLY!
```

Compiling with `-DNDEBUG` turns the `assert`s off, so build without it to actually check anything. The banner uses box-drawing characters, which may look odd on a Windows console.

### Files

```text
ConceptCoding_hashmap/
├── main.cpp        assert-based tests: basic ops, remove, rehash, operator[], keys/values, forEach, clear, collisions, 10,000 entries
└── MyHashMap.hpp   MyHashMap<K, V> (vector of std::list buckets) + StringHashMap
```
