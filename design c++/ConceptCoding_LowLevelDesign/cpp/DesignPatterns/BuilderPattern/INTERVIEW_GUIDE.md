# Builder Pattern - LLD Interview Guide

A step-by-step framework for delivering the Builder pattern low-level design interview.

---

## 📁 Code Reference

| File | Purpose | Key Functions |
|------|---------|---------------|
| [Student.hpp](Student.hpp) | Product class | Getters for all student fields |
| [StudentBuilder.hpp](StudentBuilder.hpp) | Abstract builder | `setName()`, `setAge()`, `setSubjects()`, `setRollNumber()`, `build()` |
| [EngineeringStudentBuilder.hpp](EngineeringStudentBuilder.hpp) | Concrete builder for engineering | `setSubjects()` - sets Math, Physics, CS |
| [MBAStudentBuilder.hpp](MBAStudentBuilder.hpp) | Concrete builder for MBA | `setSubjects()` - sets Marketing, Finance |
| [Director.hpp](Director.hpp) | Controls building process | `buildEngineeringStudent()`, `buildMBAStudent()` |
| [main.cpp](main.cpp) | Demo | Shows builder and director usage |

---

## 🎯 Design Patterns Used

| Pattern | Implementation Location | Purpose |
|---------|------------------------|----------|
| **Builder Pattern** | [StudentBuilder.hpp](StudentBuilder.hpp) with concrete builders | Step-by-step object construction |
| **Fluent Interface** | Builder methods return `*this` for chaining | Method chaining for setters |
| **Director** | [Director.hpp](Director.hpp) | Knows the recipe for building different student types |

---

## 1) Requirements (~5 minutes)

### Clarifying Questions to Ask
- "How many fields does the object have? Are some optional?"
- "Are there different variations of the object?"
- "Should the object be immutable after creation?"
- "Is the construction process complex with multiple steps?"

### Problem Statement

```
Problem:
- Object has many fields (some required, some optional)
- Constructor with many parameters (telescoping constructor anti-pattern)
- Different configurations needed for different use cases
- Construction involves multiple steps

Solution (Builder Pattern):
- Separate object construction from representation
- Step-by-step fluent interface for building
- Same construction process, different representations
- Object is complete and valid only after build()
```

---

## 2) Entities and Relationships (~3 minutes)

### Identify Entities

- **Product** - The complex object being built (Student)
- **Builder** - Abstract interface for building (StudentBuilder)
- **ConcreteBuilder** - Specific builder implementations
- **Director** - Orchestrates building steps (optional)

### Define Relationships

```
Entities:
- Student (product)
- StudentBuilder (abstract builder)
- EngineeringStudentBuilder : StudentBuilder
- MBAStudentBuilder : StudentBuilder
- Director (optional - orchestrates build)

Relationships:
- Builder creates Product
- ConcreteBuilders implement Builder
- Director uses Builder to construct Product
```

---

## 3) Class Design (~10-15 minutes)

### Product (Student)

```
class Student:
  - rollNumber: int
  - age: int
  - name: string
  - fatherName: string
  - motherName: string
  - subjects: vector<string>

  // All fields set by builder, class may be immutable
  + getRollNumber() -> int
  + getName() -> string
  + getSubjects() -> vector<string>
```

### Builder (Abstract)

```
class StudentBuilder:
  # rollNumber: int
  # age: int
  # name: string
  # fatherName: string
  # motherName: string
  # subjects: vector<string>

  + setRollNumber(r: int) -> StudentBuilder&
  + setAge(a: int) -> StudentBuilder&
  + setName(n: string) -> StudentBuilder&
  + setFatherName(f: string) -> StudentBuilder&
  + setMotherName(m: string) -> StudentBuilder&
  + setSubjects() -> StudentBuilder&  // Abstract - varies by type
  + build() -> Student
```

### Concrete Builders

```
class EngineeringStudentBuilder : StudentBuilder
  + setSubjects() -> StudentBuilder&
      subjects = {"DSA", "OS", "Computer Networks", "DBMS"}
      return *this

class MBAStudentBuilder : StudentBuilder
  + setSubjects() -> StudentBuilder&
      subjects = {"Marketing", "Finance", "Operations", "Strategy"}
      return *this
```

### Director (Optional)

```
class Director:
  - builder: StudentBuilder*

  + setBuilder(builder: StudentBuilder*)
      this.builder = builder

  + constructEngineeringStudent() -> Student
      return builder
          .setRollNumber(1)
          .setAge(22)
          .setName("John")
          .setSubjects()
          .build()

  + constructMBAStudent() -> Student
      return builder
          .setRollNumber(2)
          .setAge(25)
          .setName("Jane")
          .setSubjects()
          .build()
```

---

## 4) Implementation (~10 minutes)

### Key Method: Fluent Builder Pattern

```
// Builder methods return *this for chaining
setRollNumber(r: int) -> StudentBuilder&
    this.rollNumber = r
    return *this

setAge(a: int) -> StudentBuilder&
    this.age = a
    return *this

setName(n: string) -> StudentBuilder&
    this.name = n
    return *this
```

### Key Method: build()

```
build() -> Student
    // Validate required fields
    if name.empty()
        throw "Name is required"
    if rollNumber <= 0
        throw "Roll number must be positive"
    
    // Create immutable product
    return Student(rollNumber, age, name, fatherName, 
                   motherName, subjects)
```

### Key Method: EngineeringStudentBuilder.setSubjects()

```
setSubjects() -> StudentBuilder&
    subjects.clear()
    subjects.push_back("Data Structures")
    subjects.push_back("Algorithms")
    subjects.push_back("Operating Systems")
    subjects.push_back("Computer Networks")
    return *this
```

### Client Usage

```
// Without Director (client controls steps)
EngineeringStudentBuilder builder;
Student student = builder
    .setRollNumber(101)
    .setAge(20)
    .setName("Alice")
    .setFatherName("Bob")
    .setSubjects()
    .build();

// With Director (encapsulated recipes)
Director director;
director.setBuilder(new EngineeringStudentBuilder());
Student engStudent = director.constructEngineeringStudent();

director.setBuilder(new MBAStudentBuilder());
Student mbaStudent = director.constructMBAStudent();
```

### Verification: Walk Through a Scenario

```
Building an Engineering Student:

1. Create EngineeringStudentBuilder
   - All fields initially empty/default

2. setRollNumber(101) → rollNumber = 101, returns builder

3. setAge(20) → age = 20, returns builder

4. setName("Alice") → name = "Alice", returns builder

5. setSubjects() → subjects = ["DSA", "OS", "Networks", "DBMS"]
   - Engineering-specific subjects set

6. build()
   - Validate: name ✓, rollNumber ✓
   - Create Student(101, 20, "Alice", "", "", ["DSA"...])
   - Return immutable Student object

7. Student is ready to use with all fields set correctly
```

---

## 5) Extensibility (~5 minutes)

### "Builder vs Factory Pattern?"

| Aspect | Builder | Factory |
|--------|---------|---------|
| Construction | Multi-step | Single step |
| Complexity | Complex objects | Simpler objects |
| Control | Client controls steps | Factory controls creation |
| Focus | HOW to construct | WHAT to construct |

> "Use Builder when construction involves many steps or optional parameters. Use Factory when you just need to select which type to create."

### "When is Director useful?"

> "Director is useful when you have predefined 'recipes' for building objects that are reused across the application. It encapsulates the construction sequence. Without Director, clients must know the build steps."

### "How to make the built object immutable?"

> "Make Product constructor private, only accessible to Builder (friend class). All Product fields are const or have only getters. Once build() returns, the object cannot be modified."

```
class Student:
  friend class StudentBuilder;  // Only builder can construct

  - Student(...)  // Private constructor

  + getName() -> string const
  // No setters
```

### "How to handle optional vs required fields?"

> "Required fields can be validated in build(). Optional fields use default values. Consider using a separate 'required' builder step or making required fields constructor parameters of the builder."

```
class StudentBuilder:
  + StudentBuilder(name: string)  // Required in constructor
      this.name = name

  + setOptionalAge(a: int) -> StudentBuilder&  // Optional
```

---

## Design Patterns Summary

| Pattern | Where | Why |
|---------|-------|-----|
| **Builder** | StudentBuilder | Step-by-step construction |
| **Template Method** | setSubjects() in subclasses | Variant-specific behavior |

---

## Builder Pattern Template

```
// Step 1: Product
class Product:
  - partA: string
  - partB: int
  - partC: vector<string>

// Step 2: Abstract Builder
class Builder:
  # partA: string
  # partB: int
  # partC: vector<string>

  + buildPartA(a: string) -> Builder&
      this.partA = a
      return *this
  + buildPartB(b: int) -> Builder&
      this.partB = b
      return *this
  + buildPartC() -> Builder&  // Abstract
  + build() -> Product

// Step 3: Concrete Builder
class ConcreteBuilder : Builder
  + buildPartC() -> Builder&
      partC = {"specific", "values"}
      return *this

// Step 4: Director (optional)
class Director:
  + construct(builder: Builder) -> Product
      return builder.buildPartA("x").buildPartB(1).buildPartC().build()

// Usage
ConcreteBuilder builder;
Product p = builder.buildPartA("x").buildPartB(1).buildPartC().build();
```

---

## When to Use Builder

### Use When:
- Object has many constructor parameters (especially optional ones)
- Object construction is complex with multiple steps
- Same construction process needs different representations
- Object should be immutable after creation
- Avoiding "telescoping constructor" anti-pattern

### Don't Use When:
- Object is simple with few fields
- All fields are required and straightforward
- No variants needed
- Adds unnecessary complexity for simple cases
