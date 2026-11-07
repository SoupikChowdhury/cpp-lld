# C++ Low Level Design

Low level design interview questions and design patterns, implemented in C++17. Each folder has a README written so that reading it end to end is enough to deliver that question in an interview. Everything in a README describes the code in its folder, and its verification section comes from actually running that code.

## How each README is organized

The interview questions follow the order you'd use in the room:

```text
Requirements -> Entities -> Class Design -> Implementation -> Extensibility
```

Each one also has the gaps in that code with the fix to say, what's expected at each level, a self-test, and the command to run it. Read the gaps table before the interview: raising a weakness yourself, with its fix, is a strong signal.

## Interview questions

| Question | Patterns | The part to get right |
|---|---|---|
| [Parking Lot](./parking_lot/) | Singleton, Strategy, Factory, Builder | Finding a spot, and pricing |
| [Car Rental](./car_rental/) | Strategy, state-like guards | Availability over time, double booking |
| [Book My Show](./book_my_show/) | Facade, Singleton, lock per object | Two people choosing the same seat |
| [Doctor Appointment](./doctors_appointment/) | Strategy, Repository | Overlaps, and handing a freed slot to the waitlist |
| [Customer Support](./customer_support/) | Strategy, Repository | Routing, and never stranding an issue |
| [Order Management](./order_management_system/) | Strategy, Facade, Singleton | When stock and money change |
| [Splitwise](./splitwise/) | Strategy, Factory, Repository | Balances, and simplifying debts |
| [Cricbuzz](./cricbuzz/) | Observer, Template Method | What one delivery changes |
| [Elevator](./elevator/) | Singleton, Facade | The LOOK scheduling algorithm |
| [ATM](./atm/) | State, Chain of Responsibility | The state machine, and dispensing notes |
| [Vending Machine](./vending_machine/) | State | Which actions each state allows |
| [Logger](./logger/) | Singleton, Chain of Responsibility, Observer, Strategy | Levels and multiple outputs |
| [Rate Limiter](./rate_limiter/) | Strategy, Factory | Token bucket vs sliding window |
| [Snakes and Ladder](./snakes_and_ladder/) | Factory, composition | Simple modeling |
| [Tic Tac Toe](./tic_tac_toe/) | Composition | Checking for a win |
| [HashMap](./hashmap/) | Data structure | Separate chaining, load factor, rehashing |

## Second implementations

The same question built a different way. To learn the question, read the main folder; these READMEs compare the two designs and cover their own gaps.

| Folder | Compare with |
|---|---|
| [ConceptCoding_atm](./ConceptCoding_atm/) | [atm](./atm/) |
| [ConceptCoding_car_rental](./ConceptCoding_car_rental/) | [car_rental](./car_rental/) |
| [ConceptCoding_hashmap](./ConceptCoding_hashmap/) | [hashmap](./hashmap/) |
| [ConceptCoding_snakes_and_ladder](./ConceptCoding_snakes_and_ladder/) | [snakes_and_ladder](./snakes_and_ladder/) |
| [ConceptCoding_splitwise](./ConceptCoding_splitwise/) | [splitwise](./splitwise/) |

## Design patterns

Short explainers, each with its example, when not to use it, and how it differs from similar patterns.

| Pattern | Idea | Example |
|---|---|---|
| [Adapter](./design_patterns/AdapterPattern/) | Make a class usable through the interface you expect | Pounds to kilograms |
| [Builder](./design_patterns/BuilderPattern/) | Construct a valid object step by step | Student |
| [Chain of Responsibility](./design_patterns/ChainOfResponsibility/) | Pass a request along until someone handles it | Log levels |
| [Composite](./design_patterns/CompositePattern/) | Treat one item and a group the same way | Files and directories |
| [Flyweight](./design_patterns/FlyweightPattern/) | Share what's common across many objects | Robot sprites |
| [Null Object](./null_object/) | Return an object that does nothing, not `nullptr` | Missing vehicle |

## C++ fundamentals

| Topic | What it covers |
|---|---|
| [Smart pointers](./smart_pointers/) | `UniquePtr` and a thread-safe `SharedPtr`: RAII, moves, atomics |
| [String](./string/) | The rule of five and copy and swap |

## Choosing a pattern

| If you notice... | Consider |
|---|---|
| Behavior depends on what step you're in | State |
| Several interchangeable ways to do one thing | Strategy |
| Several things must react to one event | Observer |
| A request should go to whichever handler fits | Chain of Responsibility |
| One simple entry point over several classes | Facade |
| A part-whole tree | Composite |
| An object with many optional fields | Builder |
| The wrong interface on a class you can't change | Adapter |

Don't force a pattern. Several READMEs here point out where a class looks like a pattern and isn't really one, which is worth being able to say too.

## Running the code

Every README ends with its exact command. Most folders need only `main.cpp`:

```bash
cd lld/parking_lot
g++ -std=c++17 main.cpp -o parking_lot
./parking_lot
```

A few have a second source file, and it must be included:

```bash
g++ -std=c++17 main.cpp ATMImplementation.cpp -o atm                 # atm
g++ -std=c++17 main.cpp ATM.cpp -o atm_cc                            # ConceptCoding_atm
g++ -std=c++17 main.cpp BookMyShow.cpp -o book_my_show               # book_my_show
g++ -std=c++17 main.cpp VendingMachine.cpp -o vending_machine        # vending_machine
```

On Linux, add `-pthread` for code that starts threads.

[`README_GUIDE.md`](./README_GUIDE.md) explains how these READMEs are written, for anyone adding a new one.

## License

MIT License - Feel free to use for learning and interviews!

Educational purposes only. Use for interview preparation.
