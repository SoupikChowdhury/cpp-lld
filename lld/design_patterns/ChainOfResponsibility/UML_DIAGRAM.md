# Chain of Responsibility Pattern - UML Class Diagram

```
┌─────────────────────────────────────────────────────────────────────┐
│                         Chain of Responsibility                      │
└─────────────────────────────────────────────────────────────────────┘

                    ┌──────────────────────────┐
                    │      LogProcessor        │ ◄─────────────────┐
                    │    (Abstract Handler)    │                   │
                    ├──────────────────────────┤                   │
                    │ - nextLogProcessor: ptr  │───────────────────┘
                    │ + INFO: int = 1          │        next
                    │ + DEBUG: int = 2         │
                    │ + ERROR: int = 3         │
                    ├──────────────────────────┤
                    │ + log(level, message)    │
                    └──────────────────────────┘
                               ▲
                               │ extends
          ┌────────────────────┼────────────────────┐
          │                    │                    │
┌─────────────────┐  ┌─────────────────┐  ┌─────────────────┐
│ InfoLogProcessor│  │DebugLogProcessor│  │ErrorLogProcessor│
│(ConcreteHandler)│  │(ConcreteHandler)│  │(ConcreteHandler)│
├─────────────────┤  ├─────────────────┤  ├─────────────────┤
│                 │  │                 │  │                 │
├─────────────────┤  ├─────────────────┤  ├─────────────────┤
│ + log()         │  │ + log()         │  │ + log()         │
└─────────────────┘  └─────────────────┘  └─────────────────┘


                        ┌────────────┐
                        │   Client   │
                        ├────────────┤
                        │ + main()   │
                        └────────────┘
```

## Sequence Diagram

```
Client          InfoProcessor    DebugProcessor   ErrorProcessor
  │                   │                 │                 │
  │  log(ERROR, msg)  │                 │                 │
  │──────────────────►│                 │                 │
  │                   │  log(ERROR,msg) │                 │
  │                   │────────────────►│                 │
  │                   │                 │  log(ERROR,msg) │
  │                   │                 │────────────────►│
  │                   │                 │                 │──► Handle
  │                   │                 │                 │    (print)
  │                   │                 │                 │
```

## Key Points
- **Handler**: Abstract class defining interface for handling requests
- **Concrete Handlers**: InfoLogProcessor, DebugLogProcessor, ErrorLogProcessor
- **Chain**: Each handler has reference to next handler
- **Client**: Sends request to first handler in chain
