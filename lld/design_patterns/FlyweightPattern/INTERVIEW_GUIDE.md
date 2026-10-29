# Flyweight Pattern - LLD Interview Guide

A step-by-step framework for delivering the Flyweight pattern low-level design interview.

---

## 📁 Code Reference

| File | Purpose | Key Functions |
|------|---------|---------------|
| [IRobot.hpp](IRobot.hpp) | Flyweight interface | `display(x, y)` - extrinsic state passed in |
| [Sprites.hpp](Sprites.hpp) | Heavy shared data (intrinsic) | Stores sprite images |
| [HumanoidRobot.hpp](HumanoidRobot.hpp) | Concrete flyweight | Stores intrinsic state (type, sprites), receives extrinsic (position) |
| [RoboticDog.hpp](RoboticDog.hpp) | Concrete flyweight | Stores intrinsic state (type, sprites), receives extrinsic (position) |
| [RoboticFactory.hpp](RoboticFactory.hpp) | Flyweight factory with cache | `getRobot()` - returns cached or creates new robot |
| [main.cpp](main.cpp) | Demo | Shows shared robots at different positions |

---

## 🎯 Design Patterns Used

| Pattern | Implementation Location | Purpose |
|---------|------------------------|----------|
| **Flyweight Pattern** | [RoboticFactory.hpp](RoboticFactory.hpp) caches shared robots | Share heavy sprite data across robots |
| **Factory Pattern** | [RoboticFactory.hpp](RoboticFactory.hpp) - `getRobot()` | Manages flyweight pool |

**Key Concepts:**
- **Intrinsic state** (shared, immutable): Type, sprites - stored in flyweight
- **Extrinsic state** (unique, varies): Position x, y - passed at runtime via `display(x, y)`

---

## 1) Requirements (~5 minutes)

### Clarifying Questions to Ask
- "How many similar objects need to be created?"
- "Do these objects share common data?"
- "Is memory a constraint in this system?"
- "Can some data be computed/passed in rather than stored?"

### Problem Statement

```
Problem:
- Need to create many similar objects (e.g., 10,000 game characters)
- Each object has heavy data (e.g., 10MB sprite images)
- Total memory usage is prohibitive (10,000 × 10MB = 100GB)
- Most objects are of the same "type" with different positions

Solution (Flyweight Pattern):
- Separate intrinsic (shared) from extrinsic (unique) state
- Share heavy intrinsic data among similar objects
- Pass lightweight extrinsic data as method parameters
- Dramatically reduce memory footprint
```

---

## 2) Entities and Relationships (~3 minutes)

### Identify Entities

- **Flyweight** - Interface for shared objects (IRobot)
- **ConcreteFlyweight** - Shared implementation (HumanoidRobot)
- **FlyweightFactory** - Creates and caches flyweights (RoboticFactory)
- **Client** - Uses flyweights, provides extrinsic state

### Key Concepts

```
INTRINSIC STATE (Shared, Stored in Flyweight):
- Robot type
- Sprite graphics (heavy!)
- Default behaviors
- Texture data

EXTRINSIC STATE (Unique, Passed In):
- X, Y position
- Current animation frame
- Health points
- Direction facing
```

### Define Relationships

```
Entities:
- IRobot (flyweight interface)
- HumanoidRobot : IRobot (concrete flyweight)
- RoboticFactory (creates/caches flyweights)
- RobotInstance (client - holds extrinsic state + flyweight reference)

Relationships:
- Factory -> Flyweight (1:N, caches by type)
- Many clients share same Flyweight
- Client provides extrinsic state to Flyweight methods
```

---

## 3) Class Design (~10-15 minutes)

### Flyweight Interface

```
class IRobot:
  + display(x: int, y: int)  // Extrinsic state passed in
  + getType() -> string
```

### Concrete Flyweight

```
class HumanoidRobot : IRobot
  - type: string              // Intrinsic - shared
  - body: Sprites*            // Intrinsic - heavy, shared
  - texture: Texture*         // Intrinsic - heavy, shared

  + HumanoidRobot(type: string)
      this.type = type
      this.body = loadSprites(type)  // Heavy data loaded once
      this.texture = loadTexture(type)

  + display(x: int, y: int)
      // Use shared sprites/texture
      // Render at passed-in position (extrinsic)
      body.renderAt(x, y)
```

### Flyweight Factory

```
class RoboticFactory:
  - cache: map<string, shared_ptr<IRobot>>  // Cache by type

  + static getRobot(type: string) -> shared_ptr<IRobot>
      if cache.contains(type)
          return cache[type]  // Return cached flyweight
      
      // Create new flyweight and cache it
      newRobot = make_shared<HumanoidRobot>(type)
      cache[type] = newRobot
      return newRobot
```

### Client (Holds Extrinsic State)

```
class RobotInstance:
  - robot: shared_ptr<IRobot>  // Shared flyweight
  - x: int                      // Extrinsic - unique
  - y: int                      // Extrinsic - unique
  - health: int                 // Extrinsic - unique

  + RobotInstance(type: string, x: int, y: int)
      robot = RoboticFactory::getRobot(type)  // Get shared
      this.x = x
      this.y = y

  + display()
      robot->display(x, y)  // Pass extrinsic state
```

---

## 4) Implementation (~10 minutes)

### Key Method: RoboticFactory.getRobot()

```
getRobot(type: string) -> shared_ptr<IRobot>
    // Check cache first
    if cache.contains(type)
        return cache[type]  // O(1) lookup, return shared instance
    
    // Cache miss - create new flyweight
    shared_ptr<IRobot> newRobot
    
    if type == "humanoid"
        newRobot = make_shared<HumanoidRobot>(type)
    else if type == "robotic_dog"
        newRobot = make_shared<RoboticDog>(type)
    
    // Store in cache for future use
    cache[type] = newRobot
    
    return newRobot
```

### Key Method: Flyweight.display()

```
// In HumanoidRobot
display(x: int, y: int)
    // Use INTRINSIC data (stored in object)
    sprite = body.getCurrentFrame()
    
    // Use EXTRINSIC data (passed in)
    screen.draw(sprite, x, y)
    
    // The flyweight doesn't store x, y - it receives them
```

### Client Usage

```
// Create 10,000 robots - only 5 flyweight objects created!
vector<RobotInstance> robots;

for i = 0 to 10000:
    type = selectRandomType()  // "humanoid", "robotic_dog", etc.
    x = random(0, screenWidth)
    y = random(0, screenHeight)
    
    robots.push_back(RobotInstance(type, x, y))

// Render all robots
for robot in robots:
    robot.display()
```

### Verification: Walk Through Memory Calculation

```
BEFORE Flyweight (naive approach):
- 10,000 robots
- Each robot stores: sprite (10MB) + texture (500KB) + position (16 bytes)
- Total: 10,000 × 10.5MB ≈ 105 GB ❌ (impossible!)

AFTER Flyweight:
- 5 flyweight objects (one per robot type)
  - Stored: 5 × 10.5MB = 52.5 MB
- 10,000 robot instances
  - Stored: position (16 bytes) + flyweight pointer (8 bytes) = 24 bytes each
  - Total: 10,000 × 24 bytes = 240 KB
  
- TOTAL: 52.5 MB + 240 KB ≈ 53 MB ✅

SAVINGS: 105 GB → 53 MB = 99.95% reduction!
```

---

## 5) Extensibility (~5 minutes)

### "How to make Flyweight Factory thread-safe?"

> "Add synchronization around cache access. Use mutex for simple cases, or lock-free concurrent map for high-performance scenarios."

```
class ThreadSafeRoboticFactory:
  - cache: map<string, shared_ptr<IRobot>>
  - mutex: std::mutex

  + static getRobot(type: string) -> shared_ptr<IRobot>
      lock_guard<mutex> lock(mtx)
      
      if cache.contains(type)
          return cache[type]
      
      // Create and cache
      cache[type] = make_shared<HumanoidRobot>(type)
      return cache[type]
```

### "Flyweight vs Object Pool?"

| Aspect | Flyweight | Object Pool |
|--------|-----------|-------------|
| Sharing | SAME object used by many | Objects reused one at a time |
| State | Immutable shared state | Mutable, reset between uses |
| Lifecycle | Multiple concurrent references | Checkout/return mechanism |
| Use case | Many similar read-only objects | Expensive-to-create objects |

### "How to handle extrinsic state that changes frequently?"

> "Keep extrinsic state in the client (RobotInstance). The flyweight methods receive it as parameters. For animations, the client tracks frame number and passes it to display()."

### Real-World Examples

1. **Game characters** - NPCs sharing sprites
2. **Word processors** - Character glyphs (font rendering)
3. **String interning** - Java String pool
4. **Database connections** - Connection pooling (related concept)
5. **CSS styles** - Browsers share computed styles

---

## Design Patterns Summary

| Pattern | Where | Why |
|---------|-------|-----|
| **Flyweight** | IRobot shared instances | Share heavy data |
| **Factory** | RoboticFactory | Create and cache flyweights |
| **Singleton** | Factory (optional) | Single cache instance |

---

## Flyweight Pattern Template

```
// Step 1: Flyweight Interface
class Flyweight:
  + operation(extrinsicState: State)

// Step 2: Concrete Flyweight (shared, immutable intrinsic state)
class ConcreteFlyweight : Flyweight
  - intrinsicState: HeavyData  // SHARED

  + operation(extrinsicState: State)
      // Use both intrinsic (stored) and extrinsic (passed in)

// Step 3: Flyweight Factory (creates and caches)
class FlyweightFactory:
  - cache: map<Key, Flyweight*>

  + get(key: Key) -> Flyweight*
      if not cache.contains(key)
          cache[key] = new ConcreteFlyweight(key)
      return cache[key]

// Step 4: Client (holds extrinsic state)
class Client:
  - flyweight: Flyweight*  // Reference to shared
  - extrinsicState: State  // Unique per client

  + doSomething()
      flyweight->operation(extrinsicState)
```

---

## When to Use Flyweight

### Use When:
- Application uses large number of similar objects
- Objects have substantial shared (intrinsic) state
- Object identity is not important (can share references)
- Extrinsic state can be computed or passed in
- Memory is a constraint

### Don't Use When:
- Few objects needed
- All state is unique per object
- Objects need distinct identity
- Shared state is minimal (overhead not worth it)
- Objects are mutable and need independent state

---

## Intrinsic vs Extrinsic State

| Intrinsic (Shared) | Extrinsic (Unique) |
|--------------------|-------------------|
| Stored in flyweight | Passed to methods |
| Immutable | Can change |
| Same for all instances of type | Different per instance |
| Heavy data (images, textures) | Light data (position, state) |
| Examples: sprite, font glyph | Examples: x/y coords, color |
