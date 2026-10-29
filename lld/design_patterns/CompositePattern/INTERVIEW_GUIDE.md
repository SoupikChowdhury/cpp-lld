# Composite Pattern - LLD Interview Guide

A step-by-step framework for delivering the Composite pattern low-level design interview.

---

## 📁 Code Reference

| File | Purpose | Key Functions |
|------|---------|---------------|
| [FileSystem.hpp](FileSystem.hpp) | Component interface | `ls()` - abstract method |
| [File.hpp](File.hpp) | Leaf implementation | `ls()` - prints file name |
| [Directory.hpp](Directory.hpp) | Composite implementation | `add()`, `ls()` - prints directory and recursively prints children |
| [main.cpp](main.cpp) | Demo | Tree structure creation and traversal |

---

## 🎯 Design Patterns Used

| Pattern | Implementation Location | Purpose |
|---------|------------------------|----------|
| **Composite Pattern** | [FileSystem.hpp](FileSystem.hpp) with File (leaf) and Directory (composite) | Tree structure for files and directories |

**Key Behavior:** Directory's `ls()` recursively calls `ls()` on all children, providing uniform treatment of files and directories.

---

## 1) Requirements (~5 minutes)

### Clarifying Questions to Ask
- "Is there a tree/hierarchical structure?"
- "Should individual items and groups be treated uniformly?"
- "What operations need to work on both leaves and composites?"
- "Can containers contain other containers (recursive structure)?"

### Problem Statement

```
Problem:
- Tree/hierarchical structure (files and folders, UI components)
- Want to treat individual objects and compositions uniformly
- Same operation (size, display, count) on both leaf and composite
- Recursive composition - containers can contain other containers

Solution (Composite Pattern):
- Common interface for both leaf and composite
- Leaf implements operation directly
- Composite delegates operation to children
- Client treats all objects through common interface
```

---

## 2) Entities and Relationships (~3 minutes)

### Identify Entities

- **Component** - Common interface (FileSystem)
- **Leaf** - End node, no children (File)
- **Composite** - Contains children (Directory)

### Define Relationships

```
Entities:
- FileSystem (component interface)
- File : FileSystem (leaf)
- Directory : FileSystem (composite)

Relationships:
- Directory contains FileSystem objects (children)
- Children can be Files or other Directories
- Both File and Directory implement FileSystem interface
```

---

## 3) Class Design (~10-15 minutes)

### Component (Interface)

```
class FileSystem:
  + ls() -> void  // List contents
  + getSize() -> int  // Get total size
  + getName() -> string
  
  // Optional: Child management (can be in Composite only)
  + add(child: FileSystem*)
  + remove(child: FileSystem*)
```

### Leaf (File)

```
class File : FileSystem
  - name: string
  - size: int

  + ls()
      cout << name << endl

  + getSize() -> int
      return size  // Just this file's size

  + getName() -> string
      return name

  // add/remove throw exception (files can't have children)
  + add(child: FileSystem*)
      throw "Cannot add to file"
```

### Composite (Directory)

```
class Directory : FileSystem
  - name: string
  - children: vector<FileSystem*>

  + ls()
      cout << name << "/" << endl
      for child in children:
          child->ls()  // Delegate to children

  + getSize() -> int
      total = 0
      for child in children:
          total += child->getSize()  // Aggregate from children
      return total

  + getName() -> string
      return name

  + add(child: FileSystem*)
      children.push_back(child)

  + remove(child: FileSystem*)
      children.erase(find(child))
```

---

## 4) Implementation (~10 minutes)

### Key Method: Directory.ls() - Recursive Listing

```
ls()
    cout << getName() << "/" << endl
    for child in children:
        // Polymorphic call - works for both File and Directory
        child->ls()
```

### Key Method: Directory.getSize() - Aggregate Size

```
getSize() -> int
    total = 0
    for child in children:
        // If child is File: returns file size
        // If child is Directory: returns sum of its children
        total += child->getSize()
    return total
```

### Key Method: Building the Tree

```
// Create structure:
// root/
//   ├── file1.txt (100 bytes)
//   ├── docs/
//   │   ├── readme.md (50 bytes)
//   │   └── images/
//   │       └── logo.png (200 bytes)
//   └── file2.txt (75 bytes)

Directory* root = new Directory("root");
Directory* docs = new Directory("docs");
Directory* images = new Directory("images");

File* file1 = new File("file1.txt", 100);
File* file2 = new File("file2.txt", 75);
File* readme = new File("readme.md", 50);
File* logo = new File("logo.png", 200);

root->add(file1);
root->add(docs);
root->add(file2);
docs->add(readme);
docs->add(images);
images->add(logo);
```

### Verification: Walk Through a Scenario

```
Scenario: Get total size of root directory

root.getSize()
├── file1.getSize() → 100
├── docs.getSize()
│   ├── readme.getSize() → 50
│   └── images.getSize()
│       └── logo.getSize() → 200
│   └── Total: 50 + 200 = 250
├── file2.getSize() → 75
└── Total: 100 + 250 + 75 = 425 bytes

Scenario: List contents (ls)

root.ls()
→ "root/"
  file1.ls() → "file1.txt"
  docs.ls()
  → "docs/"
    readme.ls() → "readme.md"
    images.ls()
    → "images/"
      logo.ls() → "logo.png"
  file2.ls() → "file2.txt"
```

---

## 5) Extensibility (~5 minutes)

### "Where should child management methods be?"

| Approach | Description | Trade-off |
|----------|-------------|-----------|
| **In Component** | Both Leaf and Composite have add/remove | Transparency (uniform), but Leaf throws exceptions |
| **In Composite only** | Only Composite has add/remove | Safety (type-checked), but need type checking |

> "I prefer putting add/remove in Composite only for type safety. Clients must know they're working with a Directory to add children, which is realistic."

### "How to navigate back to parent?"

> "Add a parent pointer to FileSystem. Use weak_ptr to avoid circular references. Useful for operations like 'get full path' or 'move up directory'."

```
class FileSystem:
  # parent: weak_ptr<Directory>

  + getParent() -> Directory*
      return parent.lock().get()

  + getFullPath() -> string
      if parent == null
          return getName()
      return parent.getFullPath() + "/" + getName()
```

### "How to add new operations without modifying classes?"

> "Use Visitor pattern with Composite. Create a FileSystemVisitor interface with visit(File) and visit(Directory). Each class accepts the visitor. New operations = new visitor implementations."

### Real-World Examples

1. **File systems** - Files and directories
2. **UI frameworks** - Components, containers, panels
3. **Organization charts** - Employees, departments
4. **Arithmetic expressions** - Operands and operators
5. **HTML DOM** - Elements containing elements

---

## Design Patterns Summary

| Pattern | Where | Why |
|---------|-------|-----|
| **Composite** | FileSystem hierarchy | Treat leaf and composite uniformly |
| **Visitor** | Operations (extension) | Add operations without modifying |
| **Iterator** | Traversal (extension) | Standard way to walk tree |

---

## Composite Pattern Template

```
// Step 1: Component Interface
class Component:
  + operation()  // Works on both leaf and composite
  + add(child: Component*)  // Optional here
  + remove(child: Component*)  // Optional here

// Step 2: Leaf (no children)
class Leaf : Component
  + operation()
      // Do actual work

// Step 3: Composite (has children)
class Composite : Component
  - children: vector<Component*>

  + operation()
      for child in children:
          child->operation()  // Delegate

  + add(child: Component*)
      children.push_back(child)

// Client uses Component interface uniformly
void process(Component* c) {
    c->operation();  // Works for Leaf or Composite
}
```

---

## When to Use Composite

### Use When:
- Part-whole hierarchies (tree structure)
- Want to treat individual and composite objects uniformly
- Clients should ignore the difference between compositions and individuals
- Recursive structures (directories, org charts, UI)

### Don't Use When:
- Flat structure (no hierarchy)
- Leaf and composite behaviors are fundamentally different
- Type checking is required throughout
- Over-generalization of unrelated objects

---

## Composite + Other Patterns

| Combination | Purpose |
|-------------|---------|
| **Composite + Iterator** | Walk the tree structure |
| **Composite + Visitor** | Add operations without modifying |
| **Composite + Decorator** | Add behavior to components |
| **Composite + Flyweight** | Share leaf objects |
