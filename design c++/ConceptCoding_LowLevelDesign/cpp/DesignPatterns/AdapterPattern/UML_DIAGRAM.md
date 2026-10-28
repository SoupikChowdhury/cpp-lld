# Adapter Pattern - UML Class Diagram

```
┌─────────────────────────────────────────────────────────────────────┐
│                         Adapter Pattern                              │
└─────────────────────────────────────────────────────────────────────┘

┌──────────────────────────┐           ┌──────────────────────────┐
│    WeightMachineAdapter  │           │      WeightMachine       │
│    (Target Interface)    │           │  (Adaptee Interface)     │
├──────────────────────────┤           ├──────────────────────────┤
│ + getWeightInKg(): double│           │ + getWeightInPound():    │
│   {abstract}             │           │   double {abstract}      │
└──────────────────────────┘           └──────────────────────────┘
            ▲                                      ▲
            │ implements                           │ implements
            │                                      │
┌───────────┴──────────────┐           ┌──────────┴───────────────┐
│ WeightMachineAdapterImpl │           │  WeightMachineForBabies  │
│      (Adapter)           │           │      (Concrete Adaptee)  │
├──────────────────────────┤           ├──────────────────────────┤
│ - weightMachine: ptr     │───────────│                          │
├──────────────────────────┤  uses     ├──────────────────────────┤
│ + getWeightInKg(): double│           │ + getWeightInPound():    │
│   // converts lb to kg   │           │   double { return 28; }  │
└──────────────────────────┘           └──────────────────────────┘


┌────────────┐
│   Client   │───────► Uses WeightMachineAdapter (Kg interface)
├────────────┤
│ + main()   │
└────────────┘
```

## Sequence Diagram

```
Client          Adapter                  Adaptee
  │                │                        │
  │ getWeightInKg()│                        │
  │───────────────►│                        │
  │                │   getWeightInPound()   │
  │                │───────────────────────►│
  │                │                        │
  │                │       28.0 (lb)        │
  │                │◄───────────────────────│
  │                │                        │
  │  12.6 (kg)     │  // 28 * 0.45 = 12.6   │
  │◄───────────────│                        │
  │                │                        │
```

## Key Components
- **Target (WeightMachineAdapter)**: Interface client expects
- **Adaptee (WeightMachine)**: Existing interface with incompatible methods
- **Adapter (WeightMachineAdapterImpl)**: Wraps adaptee, implements target
- **Client**: Works with target interface
