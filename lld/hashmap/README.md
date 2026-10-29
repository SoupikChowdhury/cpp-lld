# Custom HashMap - Low Level Design (C++)

## Design Patterns Used
- **Data Structure Pattern**: Separate chaining with doubly-linked lists
- **Iterator Pattern**: For traversing the hash map (optional enhancement)

## Implementation Details
- **Initial Size**: 4 buckets
- **Max Capacity**: 2^30 buckets
- **Load Factor**: 0.75 (rehash threshold)
- **Collision Handling**: Separate chaining with doubly-linked lists
- **Sentinel Nodes**: Each bucket has dummy head and tail nodes for easier insertion/deletion

## Class Diagram

```mermaid
classDiagram
    class Node~K, V~ {
        +K key
        +V value
        +Node~K,V~* next
        +Node~K,V~* prev
        +Node(K key, V value)
    }
    
    class CustomHashMap~K, V~ {
        -int capacity
        -int size
        -double loadFactor
        -vector~Node~K,V~*~ buckets
        +CustomHashMap()
        +put(K key, V value)
        +get(K key) V
        +remove(K key)
        +containsKey(K key) bool
        +getSize() int
        +isEmpty() bool
        -hash(K key) int
        -findNode(K key) Node~K,V~*
        -rehash()
    }
    
    CustomHashMap --> Node : contains
```

## Hash Function and Bucket Selection

```mermaid
flowchart TD
    A[put/get/remove Key] --> B[Calculate Hash Code]
    B --> C[hash = hashCode mod capacity]
    C --> D[Get Bucket at Index]
    D --> E[Traverse Linked List]
```

## Put Operation Flow

```mermaid
flowchart TD
    A[put key, value] --> B[Calculate Bucket Index]
    B --> C[Find Node in Bucket]
    C --> D{Node Exists?}
    D -->|Yes| E[Update Value]
    D -->|No| F[Create New Node]
    F --> G[Insert at Head of List]
    G --> H[Increment Size]
    H --> I{Load Factor Exceeded?}
    I -->|Yes| J[Rehash - Double Capacity]
    I -->|No| K[Done]
    J --> K
```

## Rehashing Process

```mermaid
flowchart TD
    A[Trigger Rehash] --> B[Create New Buckets Array]
    B --> C[Double Capacity]
    C --> D[For Each Old Bucket]
    D --> E[For Each Node in Bucket]
    E --> F[Calculate New Index]
    F --> G[Insert into New Bucket]
    G --> H{More Nodes?}
    H -->|Yes| E
    H -->|No| I{More Buckets?}
    I -->|Yes| D
    I -->|No| J[Replace Old Buckets]
```

## Collision Handling with Separate Chaining

```mermaid
graph LR
    subgraph Bucket 0
        H0[Head] --> N1[key1, val1] --> N2[key2, val2] --> T0[Tail]
    end
    
    subgraph Bucket 1
        H1[Head] --> T1[Tail]
    end
    
    subgraph Bucket 2
        H2[Head] --> N3[key3, val3] --> T2[Tail]
    end
    
    subgraph Bucket 3
        H3[Head] --> N4[key4, val4] --> N5[key5, val5] --> N6[key6, val6] --> T3[Tail]
    end
```

## Deletion with Doubly-Linked List

```mermaid
flowchart LR
    A[Prev Node] -->|next| B[Node to Delete]
    B -->|next| C[Next Node]
    B -->|prev| A
    C -->|prev| B
    
    D[After Deletion]
    E[Prev Node] -->|next| F[Next Node]
    F -->|prev| E
```

## Time Complexity Analysis

| Operation | Average Case | Worst Case |
|-----------|--------------|------------|
| put()     | O(1)         | O(n)       |
| get()     | O(1)         | O(n)       |
| remove()  | O(1)         | O(n)       |

*Worst case occurs when all keys hash to the same bucket*

## Files Structure
```
hashmap/
├── CustomHashMap.hpp
└── main.cpp
```
