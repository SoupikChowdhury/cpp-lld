# Adapter Pattern - LLD Interview Guide

A step-by-step framework for delivering the Adapter pattern low-level design interview.

---

## 📁 Code Reference

| File | Purpose | Key Functions |
|------|---------|---------------|
| [WeightMachine.hpp](WeightMachine.hpp) | Adaptee interface (returns pounds) | `getWeightInPound()` |
| [WeightMachineForBabies.hpp](WeightMachineForBabies.hpp) | Concrete adaptee | `getWeightInPound()` - returns weight in pounds |
| [WeightMachineAdapter.hpp](WeightMachineAdapter.hpp) | Target adapter interface (kg) | `getWeightInKg()` |
| [WeightMachineAdapterImpl.hpp](WeightMachineAdapterImpl.hpp) | Concrete adapter | `getWeightInKg()` - wraps adaptee, converts lbs to kg |
| [main.cpp](main.cpp) | Demo | Shows adapter usage |

---

## 🎯 Design Patterns Used

| Pattern | Implementation Location | Purpose |
|---------|------------------------|----------|
| **Adapter Pattern** | [WeightMachineAdapterImpl.hpp](WeightMachineAdapterImpl.hpp) wraps [WeightMachineForBabies.hpp](WeightMachineForBabies.hpp) | Converts pounds interface to kilograms |

**Key Conversion:** `weight_kg = weight_lbs * 0.45`

---

## 1) Requirements (~5 minutes)

### Clarifying Questions to Ask
- "What interfaces are incompatible?"
- "Can we modify the existing class, or is it third-party/legacy?"
- "What does the client expect vs what does the existing class provide?"
- "Is this a one-time conversion or ongoing integration?"

### Problem Statement

```
Problem:
- Client expects interface A (e.g., getWeightInKg())
- Existing class provides interface B (e.g., getWeightInPound())
- Cannot modify existing class (third-party library, legacy code)
- Need to make them work together

Solution (Adapter Pattern):
- Create adapter that implements target interface
- Adapter wraps the existing class (adaptee)
- Adapter translates calls from target to adaptee
- Client uses target interface, unaware of adapter
```

---

## 2) Entities and Relationships (~3 minutes)

### Identify Entities

- **Target** - Interface the client expects (WeightMachineAdapter)
- **Adaptee** - Existing class with incompatible interface (WeightMachine)
- **Adapter** - Converts adaptee interface to target (WeightMachineAdapterImpl)
- **Client** - Uses target interface

### Define Relationships

```
Entities:
- WeightMachineAdapter (target interface)
- WeightMachine (adaptee - existing)
- WeightMachineForBabies : WeightMachine
- WeightMachineAdapterImpl : WeightMachineAdapter (adapter)

Relationships:
- Adapter implements Target interface
- Adapter has-a Adaptee (composition)
- Client uses Target interface
```

---

## 3) Class Design (~10-15 minutes)

### Adaptee (Existing Class - Cannot Modify)

```
class WeightMachine:
  + getWeightInPound() -> double  // Existing interface

class WeightMachineForBabies : WeightMachine
  + getWeightInPound() -> double
      return 28.0  // Returns weight in pounds
```

### Target (What Client Needs)

```
class WeightMachineAdapter:
  + getWeightInKg() -> double  // Client expects this
```

### Adapter (Bridges the Gap)

```
class WeightMachineAdapterImpl : WeightMachineAdapter
  - machine: WeightMachine*  // Composition - has adaptee

  + WeightMachineAdapterImpl(machine: WeightMachine*)
      this.machine = machine

  + getWeightInKg() -> double
      // Convert adaptee's output to target's expected format
      poundWeight = machine.getWeightInPound()
      return poundWeight * 0.45359237  // Pound to kg conversion
```

### Client Code

```
// Client uses target interface, unaware of adaptation
void processWeight(WeightMachineAdapter* adapter) {
    double kg = adapter->getWeightInKg();
    cout << "Weight: " << kg << " kg" << endl;
}

// Usage
WeightMachine* machine = new WeightMachineForBabies();
WeightMachineAdapter* adapter = new WeightMachineAdapterImpl(machine);
processWeight(adapter);  // Client doesn't know about pounds
```

---

## 4) Implementation (~10 minutes)

### Key Method: Adapter.getWeightInKg()

```
getWeightInKg() -> double
    // Step 1: Get data from adaptee in its format
    poundWeight = machine.getWeightInPound()
    
    // Step 2: Convert/transform to target format
    kgWeight = poundWeight * 0.45359237
    
    // Step 3: Return in format client expects
    return kgWeight
```

### Real-World Example: XML to JSON Adapter

```
// Existing service returns XML
class LegacyDataService:
  + getData() -> XMLDocument

// Client expects JSON
class DataAdapter:
  + getData() -> JSONObject

// Adapter implementation
class XMLToJSONAdapter : DataAdapter
  - legacyService: LegacyDataService

  + getData() -> JSONObject
      xml = legacyService.getData()
      json = convertXMLToJSON(xml)  // Transformation
      return json
```

### Verification: Walk Through a Scenario

```
Scenario: Baby weighing station

1. WeightMachineForBabies measures baby
   - getWeightInPound() → 28.0 pounds

2. Hospital system expects kilograms
   - Target: getWeightInKg()

3. Adapter bridges the gap:
   - adapter = new WeightMachineAdapterImpl(babyMachine)
   - adapter.getWeightInKg()
     → babyMachine.getWeightInPound() = 28.0
     → 28.0 * 0.45359237 = 12.7 kg
     → return 12.7

4. Hospital system receives: 12.7 kg
   - Doesn't know original was in pounds!
```

---

## 5) Extensibility (~5 minutes)

### "Object Adapter vs Class Adapter?"

| Type | Mechanism | Pros | Cons |
|------|-----------|------|------|
| **Object** | Composition (has-a) | Flexible, works with subclasses | Extra indirection |
| **Class** | Multiple inheritance | Slightly faster | Tight coupling, language limitations |

> "Object Adapter (composition) is preferred. More flexible, works with any subclass of adaptee, and doesn't require multiple inheritance which isn't available in all languages."

### "How is Adapter different from other patterns?"

| Pattern | Purpose | Interface |
|---------|---------|-----------|
| **Adapter** | Makes incompatible interfaces work together | Changes interface |
| **Decorator** | Adds behavior without changing interface | Same interface |
| **Proxy** | Controls access without changing interface | Same interface |
| **Facade** | Simplifies complex subsystem | New simplified interface |

### "When would you NOT use Adapter?"

> "If you can modify the original class, direct modification might be simpler. Adapter adds a layer of indirection. Use it when the adaptee is unchangeable (third-party, legacy) or when you need to work with multiple incompatible classes."

### Real-World Adapter Examples

1. **Power adapters** - US to EU plug
2. **Memory card readers** - SD card to USB
3. **Database drivers** - JDBC adapts to different databases
4. **Payment gateways** - Unified interface to Stripe, PayPal, etc.
5. **Legacy system integration** - Modern API to old mainframe

---

## Design Patterns Summary

| Pattern | Where | Why |
|---------|-------|-----|
| **Adapter** | WeightMachineAdapterImpl | Bridge incompatible interfaces |

---

## Adapter Pattern Template

```
// Step 1: Target Interface (what client wants)
class Target:
  + request() -> Result

// Step 2: Adaptee (existing incompatible class)
class Adaptee:
  + specificRequest() -> DifferentResult

// Step 3: Adapter (bridges the gap)
class Adapter : Target
  - adaptee: Adaptee

  + Adapter(adaptee: Adaptee)
      this.adaptee = adaptee

  + request() -> Result
      rawResult = adaptee.specificRequest()
      return transform(rawResult)  // Convert to expected format

// Client uses Target interface
void clientCode(Target* target) {
    Result r = target->request();
    // Process result
}
```

---

## When to Use Adapter

### Use When:
- Existing class interface doesn't match requirements
- Working with third-party/legacy code you can't modify
- Want to create reusable class that cooperates with unrelated interfaces
- Need to use several existing subclasses but impractical to adapt each

### Don't Use When:
- You can modify the original class directly
- The transformation is trivial (just write the conversion inline)
- You're overcomplicating simple interface differences
