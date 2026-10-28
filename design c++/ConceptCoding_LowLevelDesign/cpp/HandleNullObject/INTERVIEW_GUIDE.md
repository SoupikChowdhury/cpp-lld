# Null Object Pattern - LLD Interview Guide

A step-by-step framework for delivering the Null Object pattern low-level design interview.

---

## 📁 Code Reference

| File | Purpose | Key Functions |
|------|---------|---------------|
| [Vehicle.hpp](Vehicle.hpp) | Abstract interface | `start()`, `drive()`, `stop()`, `isNull()` |
| [Car.hpp](Car.hpp) | Concrete vehicle implementation | `start()`, `drive()`, `stop()` - real behavior |
| [NullVehicle.hpp](NullVehicle.hpp) | Null object implementation | `start()`, `drive()`, `stop()` - do nothing, `isNull()` returns true |
| [VehicleFactory.hpp](VehicleFactory.hpp) | Factory returning NullVehicle for unknown types | `createVehicle()` - returns NullVehicle instead of nullptr |
| [main.cpp](main.cpp) | Demo showing no null checks needed | Usage examples |

---

## 🎯 Design Patterns Used

| Pattern | Implementation Location | Purpose |
|---------|------------------------|----------|
| **Null Object Pattern** | [NullVehicle.hpp](NullVehicle.hpp) | Safe default behavior instead of nullptr |
| **Factory Pattern** | [VehicleFactory.hpp](VehicleFactory.hpp) - `createVehicle()` | Create vehicles, never returns nullptr |

---

## 1) Requirements (~5 minutes)

### Clarifying Questions to Ask
- "What happens when a requested object doesn't exist?"
- "Should we throw exceptions or return a default behavior?"
- "Is 'absence of an object' meaningful in this context?"
- "Do we need explicit null checks throughout the codebase?"

### Problem Statement

```
Problem:
- Factory returns nullptr when object not found
- Every call site needs null checks
- Null pointer exceptions if checks are missed
- Violates Open/Closed Principle (every new usage needs checks)

Solution (Null Object Pattern):
- Return a special "null" object instead of nullptr
- Null object implements same interface with neutral behavior
- Call sites use objects uniformly without null checks
- "Absence" has a safe, do-nothing implementation
```

---

## 2) Entities and Relationships (~3 minutes)

### Identify Entities

- **Vehicle** - Abstract interface
- **Car** - Concrete real implementation
- **NullVehicle** - Concrete null implementation
- **VehicleFactory** - Returns Vehicle (never nullptr)

### Define Relationships

```
Entities:
- Vehicle (abstract interface)
- Car : Vehicle (real implementation)
- Bike : Vehicle (real implementation)
- NullVehicle : Vehicle (null implementation)
- VehicleFactory

Relationships:
- VehicleFactory -> Vehicle (returns, never nullptr)
- Car, Bike, NullVehicle all implement Vehicle
- NullVehicle is often Singleton (stateless)
```

---

## 3) Class Design (~10-15 minutes)

### Vehicle (Interface)

```
class Vehicle:
  + getType() -> string
  + start()
  + stop()
  + isNull() -> bool  // Key method to check if null object
```

### Car (Concrete Implementation)

```
class Car : Vehicle
  - brand: string
  - model: string

  + getType() -> "Car"
  + start()
      cout << "Car starting..."
  + stop()
      cout << "Car stopping..."
  + isNull() -> false
```

### NullVehicle (Null Object Implementation)

```
class NullVehicle : Vehicle
  // Stateless - can be singleton

  + getType() -> "No Vehicle"
  + start()
      // Do nothing - neutral behavior
  + stop()
      // Do nothing - neutral behavior
  + isNull() -> true
```

### VehicleFactory

```
class VehicleFactory:
  + getVehicle(type: string) -> shared_ptr<Vehicle>
      if type == "car"
          return make_shared<Car>()
      if type == "bike"
          return make_shared<Bike>()
      // Unknown type - return Null Object, NOT nullptr
      return make_shared<NullVehicle>()
```

---

## 4) Implementation (~10 minutes)

### Before: Without Null Object Pattern

```
// Error-prone - requires null checks everywhere
Vehicle* vehicle = factory.getVehicle(type);

if (vehicle != nullptr) {
    vehicle->start();
    cout << vehicle->getType() << endl;
} else {
    cout << "No vehicle found" << endl;
}

// Another location - might forget to check!
vehicle->start();  // CRASH if nullptr
```

### After: With Null Object Pattern

```
// Clean and safe - no null checks needed
auto vehicle = factory.getVehicle(type);

vehicle->start();  // Safe even for NullVehicle
cout << vehicle->getType() << endl;

// When null status matters (rare)
if (vehicle->isNull()) {
    logWarning("Vehicle not found for type: " + type);
}
```

### Key Method: VehicleFactory.getVehicle()

```
getVehicle(type: string) -> shared_ptr<Vehicle>
    if type == "car"
        return make_shared<Car>("Toyota", "Camry")
    if type == "bike"
        return make_shared<Bike>("Honda", "CBR")
    
    // CRITICAL: Return Null Object, not nullptr
    return NullVehicle::getInstance()  // Singleton
```

### Singleton NullVehicle (Optimization)

```
class NullVehicle : Vehicle
  - static instance: shared_ptr<NullVehicle>

  + static getInstance() -> shared_ptr<Vehicle>
      if instance == null
          instance = make_shared<NullVehicle>()
      return instance
```

### Verification: Walk Through a Scenario

```
Scenario 1: Valid vehicle type
1. factory.getVehicle("car") → Returns Car object
2. car.start() → "Car starting..."
3. car.getType() → "Car"
4. car.isNull() → false

Scenario 2: Invalid vehicle type
1. factory.getVehicle("spaceship") → Returns NullVehicle
2. nullVehicle.start() → (nothing happens)
3. nullVehicle.getType() → "No Vehicle"
4. nullVehicle.isNull() → true

// Both scenarios work without null checks!
```

---

## 5) Extensibility (~5 minutes)

### "When should you NOT use Null Object Pattern?"

> "When null has specific meaning that requires different handling—like 'database connection failed' needs retry logic, not silent failure. Also avoid when performance is critical (extra object creation) or when errors must be explicitly reported."

### "How is this different from Optional/Maybe?"

| Feature | Null Object | Optional |
|---------|-------------|----------|
| Has behavior | Yes (default) | No |
| Methods callable | All methods safe | Must check first |
| Semantics | "Does nothing" | "Might not exist" |
| Use case | Polymorphism | Value presence/absence |

### "Can you combine Null Object with other patterns?"

> "Yes! Often used with Factory (factory returns null object), Strategy (null strategy does nothing), and Observer (null observer ignores events)."

### Real-World Examples

1. **NullLogger** - Discards all log messages (testing)
2. **NullCommand** - Does nothing (disabled menu items)
3. **Collections.emptyList()** - Empty but usable list
4. **NullEventHandler** - Ignores events
5. **NullRepository** - Returns empty results (testing)

---

## Design Patterns Summary

| Pattern | Where | Why |
|---------|-------|-----|
| **Null Object** | NullVehicle | Avoid null checks |
| **Singleton** | NullVehicle instance | Stateless, single instance |
| **Factory** | VehicleFactory | Returns null object for unknown types |

---

## Comparison: Null Handling Approaches

| Approach | Pros | Cons |
|----------|------|------|
| **Return nullptr** | Simple | Crash risk, verbose checks |
| **Throw exception** | Explicit error | Disruptive flow |
| **Return Optional** | Type-safe | Still needs checking |
| **Null Object** | Uniform usage, safe | Extra class, hides absence |

---

## When to Use Null Object

### Use When:
- "Absence" has a sensible default behavior
- Want to eliminate null checks from client code
- Processing collections uniformly (no special cases)
- Following "Tell, Don't Ask" principle

### Don't Use When:
- Null has specific meaning requiring different handling
- Performance is critical (avoid object creation)
- Error conditions must be explicitly reported
- Clients genuinely need to know about absence
