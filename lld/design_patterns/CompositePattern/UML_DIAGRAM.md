# Composite Pattern - UML Class Diagram

```
┌─────────────────────────────────────────────────────────────────────┐
│                        Composite Pattern                             │
└─────────────────────────────────────────────────────────────────────┘

                    ┌──────────────────────────┐
                    │      FileSystem          │◄────────────────┐
                    │      (Component)         │                 │
                    ├──────────────────────────┤                 │
                    │                          │                 │
                    ├──────────────────────────┤                 │
                    │ + ls() {abstract}        │                 │
                    └──────────────────────────┘                 │
                               ▲                                 │
                               │ implements                      │
              ┌────────────────┴────────────────┐                │
              │                                 │                │
    ┌─────────┴────────┐           ┌────────────┴───────┐       │
    │      File        │           │     Directory      │───────┘
    │     (Leaf)       │           │    (Composite)     │ children
    ├──────────────────┤           ├────────────────────┤
    │ - fileName: str  │           │ - directoryName:str│
    ├──────────────────┤           │ - children: vector │
    │ + ls()           │           ├────────────────────┤
    │ // print name    │           │ + add(FileSystem)  │
    └──────────────────┘           │ + remove(FileSystem)│
                                   │ + ls()             │
                                   │ // iterate children│
                                   └────────────────────┘


                  ┌──────────────────────────────────────┐
                  │           Tree Structure              │
                  │                                      │
                  │        [Movies] (Directory)          │
                  │       /     |          \             │
                  │   Border   [Comedy]   [Action]       │
                  │   (File)  (Directory) (Directory)    │
                  │              |            |          │
                  │        [Hulchul]      [Dhoom]        │
                  │         (File)        (File)         │
                  └──────────────────────────────────────┘
```

## Sequence Diagram

```
Client          Movies_Dir        Comedy_Dir        File
  │                │                  │               │
  │    ls()        │                  │               │
  │───────────────►│                  │               │
  │                │─── print name    │               │
  │                │                  │               │
  │                │ child.ls()       │               │
  │                │───────────────►File1             │
  │                │                  │  print name   │
  │                │                  │               │
  │                │ child.ls()       │               │
  │                │─────────────────►│               │
  │                │                  │─ print name   │
  │                │                  │               │
  │                │                  │ child.ls()    │
  │                │                  │──────────────►│
  │                │                  │               │─print
```

## Key Components
- **Component (FileSystem)**: Common interface for all
- **Leaf (File)**: End node, no children
- **Composite (Directory)**: Has children, delegates to them
