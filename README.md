# C++ Low Level Design

Low level design interview questions and design patterns, implemented in C++17 and written up so that reading one README is enough to deliver that question in an interview.

**[Browse everything in the full index →](./lld/README.md)**

## What's inside

| Section | Contents | Start with |
|---|---|---|
| **Interview questions** | 16 systems, from Parking Lot to Cricbuzz | [Parking Lot](./lld/parking_lot/) |
| **Second implementations** | 5 of those questions built a different way, compared with the main version | [ConceptCoding_atm](./lld/ConceptCoding_atm/) |
| **Design patterns** | Adapter, Builder, Chain of Responsibility, Composite, Flyweight, Null Object | [Composite](./lld/design_patterns/CompositePattern/) |
| **C++ fundamentals** | Hand-written smart pointers and a string class | [Smart pointers](./lld/smart_pointers/) |

## How each question is written

Every interview question follows the order you'd use in the room:

```text
Requirements -> Entities -> Class Design -> Implementation -> Extensibility
```

Each README describes the code in its own folder, and its results come from actually running that code. It also covers:

- **Gaps in the code,** with the fix to say, so you can raise weaknesses before the interviewer does
- **Bugs the code used to have,** where there were any, and what they teach
- **What's expected** at junior, mid-level and senior
- **A self-test,** with the answers hidden until you click

## Run one

You need a compiler that supports C++17.

```bash
cd lld/parking_lot
g++ -std=c++17 main.cpp -o parking_lot
./parking_lot
```

Each README ends with its exact command, because a few folders have a second source file.
