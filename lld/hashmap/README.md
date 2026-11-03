# HashMap - Low Level Design (C++)

A generic hash map written from scratch: `CustomHashMap<K, V>` with `put`, `get`, `getOrDefault`, `remove` and `containsKey`. Keys are spread over an array of buckets by their hash; keys that land in the same bucket are kept in a doubly linked list (separate chaining); and the array doubles when it gets more than 75% full.

This README walks the question the way you should deliver it in an interview: **Requirements -> Entities -> Class Design -> Implementation -> Extensibility**. Everything here matches the C++ code in this folder.

| Step | Time (45 min interview) | Section |
|---|---|---|
| Pin down the API and guarantees | 5 min | [1. Requirements](#1-requirements) |
| Pick the pieces | 3 min | [2. Entities](#2-entities-and-relationships) |
| Collisions, buckets, resizing, hashing | 10 min | [3. Class design](#3-class-design) |
| Write put / get / remove / rehash | 15-20 min | [4. Implementation](#4-implementation), [5. Verification](#5-verification) |
| "What if..." | 5+ min | [6. Extensibility](#6-extensibility) |

---

## 0. Understanding the problem

"Implement a HashMap" sounds like a data-structures question, but in an LLD round it's a design question: which collision strategy, what each bucket looks like, when to grow, how a hash becomes a bucket index, what the API returns for a missing key, and who owns the memory. The interviewer usually opens with:

> "Implement a HashMap without using the standard library's map. Support put, get and remove."

The answer should explain **why the operations are O(1) on average, and when they aren't**.

---

## 1. Requirements

### Clarifying questions

Group your questions around: **what the system does**, **what can go wrong**, **what's in scope**, and **what might change later**.

**You:** "Which operations?"
**Interviewer:** "put, get, remove, containsKey, size."

**You:** "Any key and value types, or just strings and ints?"
**Interviewer:** "Generic: any key that can be hashed and compared."
> A template `CustomHashMap<K, V>` using `std::hash<K>` and `operator==`.

**You:** "What does `put` do with a key that's already there?"
**Interviewer:** "Replace the value."

**You:** "What should `get` return for a missing key?"
**Interviewer:** "Make it obvious that it's missing."
> Options: throw, return a default, or return `std::optional<V>`. We use `optional` for `get` and add `getOrDefault`.

**You:** "Should it grow, and shrink?"
**Interviewer:** "Grow automatically so operations stay fast. Shrinking isn't needed."

**You:** "Iteration, thread safety, ordering?"
**Interviewer:** "Out of scope. Single-threaded, no ordering."

### Final requirements

Write this on the board before moving on:

```text
REQUIREMENTS
1. Generic CustomHashMap<K, V>; K hashable (std::hash<K>) and comparable (==)
2. put(key, value): insert, or replace the value if the key exists
3. get(key) -> optional<V>;  getOrDefault(key, default);  containsKey(key)
4. remove(key): no-op if the key isn't there
5. getSize(), isEmpty(), getCapacity()
6. Average O(1) for put / get / remove: grow the table when it gets too full

OUT OF SCOPE
- Iteration, ordering, shrinking, thread safety
```

---

## 2. Entities and relationships

A data structure has few entities; the interesting part is their shape.

- **Node** - one key-value pair, linked to its neighbours in the same bucket (`next`, `prev`).
- **Bucket** - a linked list of nodes whose keys hash to the same index. In our code each bucket has two **sentinel** nodes (a dummy head and a dummy tail) so inserting and removing never special-case "first" or "last".
- **Bucket array** - `std::vector<Node*>`, one head sentinel per bucket, `nullptr` until the bucket is first used.
- **The map** - owns the array and all nodes, tracks size and capacity, decides when to grow.

```text
buckets (capacity 8, illustrative)
 [0] -> null
 [1] -> HEAD <-> ("kiwi", 90) <-> ("apple", 100) <-> TAIL
 [2] -> null
 [3] -> HEAD <-> ("date", 40) <-> TAIL
 ...
```

| Entity | Responsibility |
|---|---|
| `CustomHashMap<K, V>` | Public API; owns buckets and nodes; hashing, lookup, insert, remove, rehash. |
| `Node` (private struct) | key, value, `next`, `prev`. Sentinels are nodes with default-constructed key and value. |

---

## 3. Class design

```cpp
template<typename K, typename V>
class CustomHashMap {
    struct Node { K key; V value; Node* next; Node* prev; };

    static constexpr int INITIAL_CAPACITY = 4;
    static constexpr int MAX_CAPACITY = 1 << 30;
    static constexpr double LOAD_FACTOR = 0.75;

    int capacity;
    int size_;
    std::vector<Node*> buckets;          // head sentinel per bucket, or nullptr

    int hash(const K& key) const;        // key -> bucket index
    Node* findNode(const K& key) const;
    void insertNode(int index, Node* node);
    void removeNode(Node* node);
    void rehash();                       // double the capacity, relink every node
public:
    void put(const K& key, const V& value);
    std::optional<V> get(const K& key) const;
    V getOrDefault(const K& key, const V& defaultValue) const;
    void remove(const K& key);
    bool containsKey(const K& key) const;
    int getSize() const;  bool isEmpty() const;  int getCapacity() const;
};
```

> [!NOTE]
> **Decision: how are collisions handled?**
> - **Good: open addressing.** All entries live in the array itself; on a collision, probe the next slot (linear or quadratic). Cache-friendly and no per-node allocations, but deletion needs "tombstones" and performance drops sharply as the table fills.
> - **Our choice (Great for an interview): separate chaining.** Each bucket is a list of the entries that hash there. Simple to explain, deletion is trivial, and it degrades gently when full. `std::unordered_map` uses chaining too.

> [!TIP]
> **Say in the interview:** "I use separate chaining: each bucket is a linked list. Average O(1) as long as the load factor stays bounded, which resizing guarantees."

> [!NOTE]
> **Decision: what does a bucket look like?**
> - **Good: a singly linked list.** One pointer per node; removal remembers the previous node while searching.
> - **Our choice: a doubly linked list with head and tail sentinels.** Insert-after-head and unlink-a-node are each four pointer assignments with no "is this the first node?" checks. The cost: two extra nodes per used bucket and a `prev` pointer per entry. (The tail sentinel isn't strictly needed: the code also stops at `next == nullptr`.)

> [!NOTE]
> **Decision: when and how does it grow?**
> - **Our choice (Great): load factor 0.75, double the capacity.** After an insert, if `size > capacity * 0.75`, allocate twice the buckets and move every node to its new bucket. Doubling makes the total cost of all resizes O(n), so `put` stays O(1) **amortized**. Capacity stays a power of two (4, 8, 16, ...), capped at 2^30.
> - It never shrinks: after removing everything, the capacity stays where it peaked.

> [!TIP]
> **Say in the interview:** "When the load factor passes 0.75 I double the table and rehash. Each resize costs O(n), but doubling means it happens rarely enough that put is O(1) amortized."

> [!NOTE]
> **Decision: how does a hash become a bucket index?**
> - **Our choice (Good): `std::abs((int)std::hash<K>{}(key)) % capacity`.** Short, and works for typical keys.
> - **Great: mix the hash bits first, in `size_t`.** With a power-of-two capacity, `% capacity` keeps only the **low** bits. For `int` keys, `std::hash` is the identity in libstdc++, so keys that share their low bits all collide: in our tests 20,000 keys that were multiples of 65,536 took **1,595 ms** instead of 7 ms. Mixing (e.g. `h ^ (h >> 16)`, as Java does) or a prime capacity spreads them out, and staying in `size_t` avoids `std::abs(INT_MIN)`, which is undefined.

> [!NOTE]
> **Decision: what does `get` return for a missing key?**
> - **Good: throw** (like `std::map::at`) or **return a default** (like `operator[]` inserting one).
> - **Great (our choice): `std::optional<V>`**, plus `getOrDefault`. "Not found" is part of the type, so the caller can't forget to check, and no exception is used for a normal outcome.

### Complexity

| Operation | Average | Worst case |
|---|---|---|
| `put` | O(1) amortized | O(n): all keys in one bucket, or a resize |
| `get`, `containsKey` | O(1) | O(n): all keys in one bucket |
| `remove` | O(1) | O(n) |
| `rehash` | O(n) | O(n) |
| Space | O(n + capacity) | plus 2 sentinel nodes per used bucket |

---

## 4. Implementation

Before writing code, ask the interviewer: real code, pseudocode, or talk it through? Write `hash`, `findNode`, `put` (with the resize check), `rehash` and `remove`.

### From key to bucket

```cpp
// CustomHashMap.hpp
int hash(const K& key) const {
    return std::abs(static_cast<int>(std::hash<K>{}(key))) % capacity;
}
```

`std::hash<K>` returns a `size_t`; the cast keeps the low 32 bits, `abs` makes it non-negative (except for `INT_MIN`, see gaps), and `% capacity` picks the bucket.

### Finding a key

**Core logic:** hash to the bucket; if the bucket was never used it's `nullptr`; otherwise walk from the node after the head sentinel up to the tail sentinel, comparing keys with `==`.

```cpp
// CustomHashMap.hpp
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
```

`get`, `getOrDefault`, `containsKey` and `remove` all start with `findNode`. Note that it walks the bucket twice: once to find the tail, once to search.

### put

**Core logic**
1. If the key exists, replace its value and stop.
2. Otherwise create a node and insert it right after the bucket's head sentinel (creating the bucket's sentinels on first use).
3. Count it; if `size > capacity * 0.75`, rehash.

```cpp
// CustomHashMap.hpp
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
```

With capacity 4 the threshold is 3, so the **4th** insert triggers the first resize (to 8).

### rehash

**Core logic:** double the capacity, make a fresh array of empty buckets, and move every existing node into the bucket for its new index. Nodes are **relinked, not copied**, so no key or value is reallocated. The old sentinels are deleted.

```cpp
// CustomHashMap.hpp
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
```

`next` is saved before `insertNode`, because relinking the node overwrites its `next`. `hash` already uses the new `capacity`, since it was assigned first.

### remove

```cpp
// CustomHashMap.hpp
void removeNode(Node* node) {
    Node* prevNode = node->prev;
    Node* nextNode = node->next;
    prevNode->next = nextNode;
    nextNode->prev = prevNode;
    delete node;
}

void remove(const K& key) {
    Node* node = findNode(key);
    if (node != nullptr) {
        removeNode(node);
        size_--;
    }
}
```

Thanks to the sentinels, `prevNode` and `nextNode` always exist: no special case for the first or last real node.

### C++ talking points

- **Template + `std::hash<K>`**: any key with a `std::hash` specialization and `operator==` works; you could also take the hasher as a template parameter, like `std::unordered_map` does.
- **`std::optional<V>`** for `get` makes "missing" part of the return type (C++17).
- **Sentinels need default-constructible `K` and `V`.** `Node()` default-constructs `key` and `value`, so `CustomHashMap<int, NoDefault>` doesn't compile ("no matching function for call to `NoDefault::NoDefault()`"). A key-less base node type for sentinels removes the requirement.
- **Rule of three.** The class owns raw `new`ed nodes and frees them in the destructor, but copying isn't deleted or defined: a copy shares the nodes and both free them (see gaps).
- **Rehash relinks nodes**, so a node's address never changes: the same property that lets `std::unordered_map` keep references to elements valid across a rehash.
- **`static constexpr double LOAD_FACTOR`** is allowed in-class because it's `constexpr`.
- **`int` vs `size_t`**: sizes, capacity and the hash are `int` here; `size_t` avoids the truncation and the `abs` problem.

### Gaps in our code (know these before the interviewer finds them)

I tested each of these by running the code, except where marked "by reading". Raising them yourself, with the fix, is a strong signal.

| Gap | What happens | Fix to say |
|---|---|---|
| **Keys sharing low bits all collide** | 20,000 `int` keys `i << 16` took **1,595 ms** vs **7 ms** for `i`: every key is a multiple of 65,536, so `key % 32768 == 0` and they all chain in bucket 0 (O(n) per operation). | Mix the hash before taking the index (`h ^ (h >> 16)`, or multiply by a large odd constant), or use a prime capacity. |
| **Copying corrupts the heap** | `CustomHashMap b = a;` copies the bucket pointers; when `b` is destroyed it frees `a`'s nodes. The test then read freed memory and crashed with `0xC0000374` (heap corruption). | Delete the copy constructor and copy assignment (or write a deep copy), and add move operations. |
| **`K` and `V` must have default constructors** | A value type without one doesn't compile, only because of the sentinel nodes. | Sentinels as a separate node type without key and value; or `std::list` per bucket. |
| **`std::abs(static_cast<int>(hash))`** | Truncates a 64-bit hash to 32 bits, and `std::abs(INT_MIN)` is undefined. Key `INT_MIN` worked in our test only because `INT_MIN % (power of two) == 0`. | Do it all in `size_t`: `std::hash<K>{}(key) % capacity`. |
| **Never shrinks** | After 10,000 puts and 10,000 removes: size 0, capacity 16,384; emptied buckets also keep their two sentinels. | Halve the capacity when the load factor drops below, say, 0.25. |
| **`findNode` walks each bucket twice** (by reading) | `getTail` walks the whole list before the search loop does. | Loop until `current->next == nullptr` (the tail) and drop `getTail`. |
| **No iteration, no thread safety** (by reading) | You can't loop over the entries, and concurrent use is a data race. | An iterator over buckets and nodes; a mutex (or a lock per bucket group) if shared. |

---

## 5. Verification

Trace the code on concrete scenarios and say the state out loud after each step. The outputs below are from actually running the code.

**1. The demo**
```text
new map                               size 0, capacity 4
put apple, banana, cherry             size 3 (3 > 3? no)
put date                              size 4 > 4 x 0.75 = 3 -> rehash -> capacity 8
put elderberry                        size 5 (5 > 6? no), capacity 8
get apple = 10, banana = 20, grape = not found;  getOrDefault(grape, 0) = 0
put apple 100                         existing node updated, size unchanged
remove banana                         containsKey(banana) = No, size 4
put fig, guava, honeydew, kiwi        7th entry: 7 > 6 -> rehash -> capacity 16; final size 8
```
(The demo's comment says the 5th insert "may trigger rehash"; it's actually the 4th.)

**2. Correctness and edge cases** (separate test program)
```text
300,000 random put / remove / get on keys 0..4999, compared with std::unordered_map
                                       -> 0 mismatches; final size 2517, capacity 4096
20,000 int keys i         -> 7 ms        20,000 int keys i << 16 -> 1,595 ms (one bucket)
key INT_MIN               -> stored and found (by luck, see gaps)
10,000 puts + 10,000 removes -> size 0, capacity 16384
copy, then destroy the copy  -> crash 0xC0000374 (heap corruption)
value type without a default constructor -> compile error
```

---

## 6. Extensibility

If there's time left, expect "what if" questions. You usually explain where the change goes rather than code it.

### 1. "Make it thread-safe"

> "Simplest: one `std::shared_mutex`; `get` and `containsKey` take a shared lock, `put` and `remove` an exclusive one, and `rehash` runs under the exclusive lock. For more concurrency, lock striping: split buckets into N groups, each with its own mutex, so writers to different groups don't block each other. A resize then has to take all the stripes."

### 2. "Iterate over the entries"

> "An iterator that holds a bucket index and a node pointer: `++` moves to the next node, and at a bucket's tail jumps to the next non-empty bucket. Modifying the map invalidates it, just as with `std::unordered_map` after a rehash."

### 3. "Protect against bad hash functions or attacks"

> "Mix the hash bits, and if a bucket's chain grows long (Java uses 8), turn that bucket into a balanced tree so the worst case is O(log n) instead of O(n). Randomly seeded hashing stops attackers from choosing colliding keys."

### 4. "Use open addressing instead"

> "One flat array of slots. On a collision probe forward (linear probing, or Robin Hood hashing to even out probe lengths). Deletion marks a tombstone so later searches keep probing. Resize at a lower load factor, around 0.5-0.7. Fewer allocations and better cache behaviour."

### 5. "Build an LRU cache on top"

> "A hash map from key to a node in a doubly linked list ordered by recency: `get` moves the node to the front, `put` inserts at the front and evicts from the back when full. Both O(1). The sentinel-based list in this map is exactly the list you'd need."

### 6. "Shrink when it empties"

> "After a remove, if `size < capacity * 0.25` and capacity is above the initial size, halve it and rehash. The gap between 0.75 and 0.25 avoids resizing back and forth around one size."

---

## 7. What is expected at each level

**Junior.** A bucket array, chaining with linked lists, correct put/get/remove including "update an existing key", and an explanation of the average O(1).

**Mid-level.** Load factor and doubling with amortized analysis, correct rehash (saving `next` before relinking), a deliberate API for missing keys (`optional`), and complexity including the worst case. Discuss open addressing vs chaining.

**Senior.** Hash quality and how a power-of-two capacity interacts with weak hashes (the clustering test), signed/unsigned pitfalls in the index, ownership and the rule of three, sentinel trade-offs (default-constructible types), shrinking, thread safety by striping, and treeified buckets against attacks.

---

## 8. Test yourself

<details><summary>1. Why is <code>get</code> O(1) on average but O(n) in the worst case?</summary>

On average each bucket holds about `load factor` entries, because the table grows to keep that bounded. In the worst case every key hashes to the same bucket and you walk a list of n nodes (our clustered-keys test).
</details>

<details><summary>2. Why does doubling make <code>put</code> O(1) amortized even though a rehash is O(n)?</summary>

Resizes happen when the size passes 0.75 x capacity (at 4, 7, 13, 25, ... entries), each one moving all current entries: the total work is a geometric series, at most about 2n. Spread over n inserts, that's O(1) each.
</details>

<details><summary>3. Why are the head and tail sentinels useful, and what do they cost?</summary>

Insert-after-head and unlinking a node never need "is this the first/last node?" checks. Cost: two extra nodes per used bucket, and `K` and `V` must be default-constructible because the sentinels hold a key and a value.
</details>

<details><summary>4. In <code>rehash</code>, why is <code>next</code> saved before calling <code>insertNode</code>?</summary>

`insertNode` relinks the node into its new bucket and overwrites its `next`. Without saving it first, the loop would continue in the new bucket instead of the old one.
</details>

<details><summary>5. Why did keys that were multiples of 65,536 make the map 230x slower?</summary>

`std::hash<int>` is the identity, and with a power-of-two capacity `% capacity` keeps only the low bits. All those keys have the same low bits (zero), so they all chain in bucket 0. Mix the hash bits first.
</details>

<details><summary>6. What goes wrong when you copy a <code>CustomHashMap</code>?</summary>

The default copy copies the bucket pointers, so both maps share the nodes and both destructors free them: heap corruption. Delete the copy operations or implement a deep copy (rule of three).
</details>

<details><summary>7. Chaining vs open addressing in one sentence each?</summary>

Chaining: lists per bucket, simple deletion, graceful when full, an allocation per entry. Open addressing: everything in one array, cache-friendly, but needs tombstones for deletion and slows sharply at high load.
</details>

<details><summary>8. Why return <code>std::optional&lt;V&gt;</code> from <code>get</code>?</summary>

A missing key is a normal outcome, not an error. `optional` puts it in the type so the caller must check, without exceptions and without a magic default value.
</details>

<details><summary>9. What's wrong with <code>std::abs(static_cast&lt;int&gt;(hash))</code>?</summary>

It throws away the upper 32 bits of the hash, and `std::abs(INT_MIN)` is undefined behaviour. Use `size_t` throughout: `hash % capacity` is then always a valid index.
</details>

<details><summary>10. How would you build an LRU cache with this?</summary>

Map each key to a node in a recency-ordered doubly linked list; on access move the node to the front, on insert add at the front and evict the tail when over capacity. Both operations O(1).
</details>

---

## 9. Run it

```bash
cd lld/hashmap
g++ -std=c++17 main.cpp -o hashmap
./hashmap
```

Expected output (shortened):

```text
--- Initial State ---
Size: 0
Capacity: 4
Is Empty: Yes
...
After 4 insertions:
Size: 4
Capacity: 8
...
apple: 10
banana: 20
grape: not found
...
Removed: banana
Contains 'banana': No
Size: 4
...
Final Size: 8
Final Capacity: 16
```

### Files

```text
hashmap/
├── main.cpp            demo: inserts, resize, get/getOrDefault, update, remove, more inserts
└── CustomHashMap.hpp   the whole map: Node, buckets with sentinels, hash, put/get/remove, rehash
```
