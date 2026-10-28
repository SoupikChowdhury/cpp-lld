# Chain of Responsibility Pattern - LLD Interview Guide

A step-by-step framework for delivering the Chain of Responsibility pattern low-level design interview.

---

## 📁 Code Reference

| File | Purpose | Key Functions |
|------|---------|---------------|
| [LogProcessor.hpp](LogProcessor.hpp) | Abstract handler base | `log()` - passes to next if not handled, `setNextProcessor()` |
| [InfoLogProcessor.hpp](InfoLogProcessor.hpp) | INFO level handler | `log()` - handles INFO, passes others to next |
| [DebugLogProcessor.hpp](DebugLogProcessor.hpp) | DEBUG level handler | `log()` - handles DEBUG, passes others to next |
| [ErrorLogProcessor.hpp](ErrorLogProcessor.hpp) | ERROR level handler | `log()` - handles ERROR, passes others to next |
| [main.cpp](main.cpp) | Demo | Chain creation and usage |

---

## 🎯 Design Patterns Used

| Pattern | Implementation Location | Purpose |
|---------|------------------------|----------|
| **Chain of Responsibility** | [LogProcessor.hpp](LogProcessor.hpp) with Info→Debug→Error chain | Log processors form a chain, each handles its level |

**Chain Order:** Info → Debug → Error (each handler only processes its level, passes others)

---

## 1) Requirements (~5 minutes)

### Clarifying Questions to Ask
- "Can multiple handlers process the same request?"
- "Is the handler determined at runtime or compile time?"
- "What happens if no handler can process the request?"
- "Should handlers be easily added/removed from the chain?"

### Problem Statement

```
Problem:
- Request can be handled by one of several handlers
- Handler isn't known beforehand (determined at runtime)
- Want to decouple sender from receiver
- Want flexibility to add/remove handlers dynamically

Solution (Chain of Responsibility Pattern):
- Chain of handler objects
- Each handler decides to process or pass to next
- Sender doesn't know which handler will process
- Easy to add/remove handlers without changing existing code
```

---

## 2) Entities and Relationships (~3 minutes)

### Identify Entities

- **Handler** - Abstract base with next pointer (LogProcessor)
- **ConcreteHandlers** - Specific handlers (InfoLogProcessor, DebugLogProcessor, ErrorLogProcessor)
- **Request** - Data being processed (log message with level)

### Define Relationships

```
Entities:
- LogProcessor (abstract handler)
- InfoLogProcessor : LogProcessor
- DebugLogProcessor : LogProcessor
- ErrorLogProcessor : LogProcessor

Relationships:
- Handler has-a next Handler (linked list)
- Each handler implements abstract handle method
- Client sends to first handler in chain
```

---

## 3) Class Design (~10-15 minutes)

### Handler (Abstract)

```
class LogProcessor:
  # nextLogProcessor: LogProcessor*
  # INFO: int = 1
  # DEBUG: int = 2
  # ERROR: int = 3

  + LogProcessor(next: LogProcessor*)
      this.nextLogProcessor = next

  + log(logLevel: int, message: string)
      if nextLogProcessor != null
          nextLogProcessor.log(logLevel, message)
```

### Concrete Handlers

```
class InfoLogProcessor : LogProcessor
  + log(logLevel: int, message: string)
      if logLevel == INFO
          cout << "INFO: " << message << endl
      else
          LogProcessor::log(logLevel, message)  // Pass to next

class DebugLogProcessor : LogProcessor
  + log(logLevel: int, message: string)
      if logLevel == DEBUG
          cout << "DEBUG: " << message << endl
      else
          LogProcessor::log(logLevel, message)

class ErrorLogProcessor : LogProcessor
  + log(logLevel: int, message: string)
      if logLevel == ERROR
          cout << "ERROR: " << message << endl
      else
          LogProcessor::log(logLevel, message)
```

### Building the Chain

```
// Build chain: Info → Debug → Error → null
LogProcessor* chain = new InfoLogProcessor(
    new DebugLogProcessor(
        new ErrorLogProcessor(nullptr)
    )
);

// Client uses the chain
chain->log(DEBUG, "This is a debug message");
```

---

## 4) Implementation (~10 minutes)

### Key Method: Handler.log() - Base Implementation

```
log(logLevel: int, message: string)
    // Default: pass to next handler
    if nextLogProcessor != null
        nextLogProcessor.log(logLevel, message)
    // If no next handler and we reach here, request was unhandled
```

### Key Method: ConcreteHandler.log() - Specific Implementation

```
// In InfoLogProcessor
log(logLevel: int, message: string)
    if logLevel == INFO
        // This handler CAN handle it
        cout << "INFO: " << message << endl
        // Optionally pass to next for logging to multiple destinations
    else
        // This handler CANNOT handle, pass to next
        LogProcessor::log(logLevel, message)
```

### Alternative: ATM Cash Dispenser Example

```
class CashWithdrawProcessor:
  - nextProcessor: CashWithdrawProcessor*

  + withdraw(atm: ATM*, amount: int)
      if nextProcessor != null
          nextProcessor.withdraw(atm, amount)

class TwoThousandWithdrawProcessor : CashWithdrawProcessor
  + withdraw(atm: ATM*, amount: int)
      // Handle 2000-denomination notes
      requiredNotes = amount / 2000
      availableNotes = atm.getNoOf2000Notes()
      notesToDispense = min(requiredNotes, availableNotes)
      
      atm.dispense2000Notes(notesToDispense)
      remaining = amount - (notesToDispense * 2000)
      
      // Pass remainder to next handler
      if remaining > 0
          CashWithdrawProcessor::withdraw(atm, remaining)

class FiveHundredWithdrawProcessor : CashWithdrawProcessor
  + withdraw(atm: ATM*, amount: int)
      // Similar logic for 500-denomination
```

### Verification: Walk Through a Scenario

```
Chain: Info → Debug → Error → null

Scenario 1: log(INFO, "Starting app")
1. InfoLogProcessor.log(INFO, "Starting app")
   - logLevel == INFO ✓
   - Output: "INFO: Starting app"
   - Stop (handled)

Scenario 2: log(DEBUG, "Variable x = 5")
1. InfoLogProcessor.log(DEBUG, "Variable x = 5")
   - logLevel != INFO
   - Pass to next
2. DebugLogProcessor.log(DEBUG, "Variable x = 5")
   - logLevel == DEBUG ✓
   - Output: "DEBUG: Variable x = 5"
   - Stop (handled)

Scenario 3: log(ERROR, "Connection failed")
1. InfoLogProcessor → pass
2. DebugLogProcessor → pass
3. ErrorLogProcessor.log(ERROR, "Connection failed")
   - logLevel == ERROR ✓
   - Output: "ERROR: Connection failed"
   - Stop (handled)

Scenario 4: log(FATAL, "System crash")  // Unknown level
1. InfoLogProcessor → pass
2. DebugLogProcessor → pass
3. ErrorLogProcessor → pass
4. nullptr → Request unhandled (may need default handler)
```

---

## 5) Extensibility (~5 minutes)

### "How is this different from Decorator?"

| Aspect | Chain of Responsibility | Decorator |
|--------|-------------------------|-----------|
| Handler count | ONE handler processes | ALL decorators process |
| Flow | Can break chain | Always continues |
| Purpose | Find right handler | Add behavior |
| Coupling | Loose (unknown handler) | Wrapping layers |

### "What if no handler processes the request?"

> "Three options: 1) Have a default catch-all handler at end of chain. 2) Throw exception for unhandled requests. 3) Silently ignore (depends on use case). For logging, silent ignore might be okay. For payments, exception is better."

```
class DefaultLogProcessor : LogProcessor
  + log(logLevel, message)
      // Catch-all handler
      cout << "UNKNOWN[" << logLevel << "]: " << message << endl
```

### "How to ensure order of handlers?"

> "Order is determined during chain construction. The client/configuration builds the chain in specific order. For priority-based ordering, sort handlers before building chain."

### Real-World Examples

1. **Logging frameworks** - Log4j, SLF4j handler chains
2. **Servlet filters** - Request processing pipeline
3. **Event handling** - GUI event bubbling
4. **Approval workflows** - Manager → Director → VP chain
5. **ATM cash dispensing** - Different denomination handlers

---

## Design Patterns Summary

| Pattern | Where | Why |
|---------|-------|-----|
| **Chain of Responsibility** | LogProcessor chain | Decouple sender from receiver |

---

## Chain of Responsibility Template

```
// Step 1: Abstract Handler
class Handler:
  # next: Handler*

  + Handler(next: Handler*)
      this.next = next

  + handle(request: Request)
      if next != null
          next.handle(request)

// Step 2: Concrete Handler
class ConcreteHandler : Handler
  + handle(request: Request)
      if canHandle(request)
          // Process request
      else
          Handler::handle(request)  // Pass to next

  - canHandle(request) -> bool
      // Check if this handler can process

// Step 3: Build Chain
Handler* chain = new HandlerA(
    new HandlerB(
        new HandlerC(nullptr)
    )
);

// Step 4: Client uses chain
chain->handle(request);
```

---

## When to Use Chain of Responsibility

### Use When:
- Multiple objects may handle a request
- Handler isn't known beforehand
- Want to issue request without specifying receiver explicitly
- Set of handlers should be dynamic (add/remove at runtime)
- Want to decouple sender from receiver

### Don't Use When:
- Handler is known at compile time
- Only one handler ever needed
- Request must always be handled (unless you add catch-all)
- Performance critical and chain is very long
