# Flyweight Pattern - UML Class Diagram

```
┌─────────────────────────────────────────────────────────────────────┐
│                        Flyweight Pattern                             │
└─────────────────────────────────────────────────────────────────────┘

┌──────────────────────────┐
│     RoboticFactory       │
│   (Flyweight Factory)    │
├──────────────────────────┤
│ - cache: Map<str, IRobot>│
├──────────────────────────┤
│ + createRobot(type):     │
│   IRobot                 │───────────────┐
│ + getCacheSize(): int    │               │
└──────────────────────────┘               │
                                           │ creates/returns
                                           ▼
                    ┌──────────────────────────┐
                    │        IRobot            │
                    │     (Flyweight)          │
                    ├──────────────────────────┤
                    │                          │
                    ├──────────────────────────┤
                    │ + display(x, y): void    │  ◄── Extrinsic state
                    │   {abstract}             │      (passed in)
                    └──────────────────────────┘
                               ▲
                               │ implements
              ┌────────────────┴────────────────┐
              │                                 │
    ┌─────────┴──────────┐         ┌───────────┴────────┐
    │   HumanoidRobot    │         │    RoboticDog      │
    │ (ConcreteFlyweight)│         │ (ConcreteFlyweight)│
    ├────────────────────┤         ├────────────────────┤
    │ - type: string     │ ◄─────  │ - type: string     │ ◄── Intrinsic
    │ - body: Sprites    │ shared  │ - body: Sprites    │     (shared)
    ├────────────────────┤         ├────────────────────┤
    │ + display(x, y)    │         │ + display(x, y)    │
    └────────────────────┘         └────────────────────┘
              │                               │
              └───────────┬───────────────────┘
                          │ uses
                          ▼
                 ┌─────────────────┐
                 │    Sprites      │
                 │  (Heavy Object) │
                 ├─────────────────┤
                 │ - graphicData   │  ~10MB each
                 └─────────────────┘
```

## Memory Comparison

```
WITHOUT Flyweight:
┌────────────────────────────────────────────────────────┐
│  1000 Humanoid Robots                                   │
│  Each has own Sprites (~10MB)                           │
│  Total: 1000 × 10MB = 10GB memory                       │
└────────────────────────────────────────────────────────┘

WITH Flyweight:
┌────────────────────────────────────────────────────────┐
│  1000 Humanoid Robots                                   │
│  All share 1 Sprites object (~10MB)                     │
│  Positions stored externally                            │
│  Total: 10MB + 1000 × 8 bytes = ~10MB memory           │
└────────────────────────────────────────────────────────┘
```

## Key Concepts
- **Intrinsic State**: Shared, immutable (type, sprites)
- **Extrinsic State**: Unique per instance (x, y position)
- **Factory**: Manages flyweight pool, returns cached objects
