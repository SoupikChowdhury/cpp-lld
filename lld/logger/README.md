# Logger System - Low Level Design (C++)

## Design Patterns Used
- **Singleton Pattern**: Logger class has single instance
- **Chain of Responsibility**: Log handlers chain (Debug → Info → Warn → Error → Fatal)
- **Observer Pattern**: Appenders observe handlers and receive log messages
- **Strategy Pattern**: LogFormatter (PlainTextFormatter, JSONFormatter)

## Class Diagram

```mermaid
classDiagram
    class LogLevel {
        <<enumeration>>
        TRACE
        DEBUG
        INFO
        WARN
        ERROR
        FATAL
    }
    
    class LogMessage {
        -LogLevel level
        -string message
        -long timestamp
        +getLevel() LogLevel
        +getMessage() string
        +getTimestamp() long
    }
    
    class LogFormatter {
        <<interface>>
        +format(LogMessage msg)* string
    }
    
    class PlainTextFormatter {
        +format(LogMessage msg) string
    }
    
    class JSONFormatter {
        +format(LogMessage msg) string
    }
    
    class LogAppender {
        <<interface>>
        +append(string formattedMessage)*
    }
    
    class ConsoleAppender {
        -LogFormatter* formatter
        +append(string formattedMessage)
    }
    
    class FileAppender {
        -LogFormatter* formatter
        -string filePath
        +append(string formattedMessage)
    }
    
    class LogHandler {
        <<abstract>>
        #LogHandler* nextHandler
        #vector~LogAppender*~ appenders
        +setNext(LogHandler* handler)
        +addAppender(LogAppender* appender)
        +handle(LogMessage msg)
        #shouldHandle(LogLevel level)* bool
        #notifyAppenders(LogMessage msg)
    }
    
    class DebugHandler {
        #shouldHandle(LogLevel level) bool
    }
    
    class InfoHandler {
        #shouldHandle(LogLevel level) bool
    }
    
    class WarnHandler {
        #shouldHandle(LogLevel level) bool
    }
    
    class ErrorHandler {
        #shouldHandle(LogLevel level) bool
    }
    
    class FatalHandler {
        #shouldHandle(LogLevel level) bool
    }
    
    class LogHandlerConfiguration {
        -static map~LogLevel, LogHandler*~ handlers
        +build()$ LogHandler*
        +addAppenderForLevel(LogLevel level, LogAppender* appender)$
    }
    
    class Logger {
        -static Logger* instance
        -LogHandler* handlerChain
        +getInstance()$ Logger*
        +log(LogLevel level, string message)
        +debug(string msg)
        +info(string msg)
        +warn(string msg)
        +error(string msg)
        +fatal(string msg)
    }

    LogFormatter <|.. PlainTextFormatter
    LogFormatter <|.. JSONFormatter
    
    LogAppender <|.. ConsoleAppender
    LogAppender <|.. FileAppender
    
    LogHandler <|-- DebugHandler
    LogHandler <|-- InfoHandler
    LogHandler <|-- WarnHandler
    LogHandler <|-- ErrorHandler
    LogHandler <|-- FatalHandler
    
    ConsoleAppender --> LogFormatter
    FileAppender --> LogFormatter
    
    LogHandler --> LogAppender : notifies
    LogHandler --> LogHandler : next
    
    Logger --> LogHandler
    Logger --> LogHandlerConfiguration
    
    LogMessage --> LogLevel
```

## Chain of Responsibility Flow

```mermaid
flowchart LR
    A[Logger] --> B[DebugHandler]
    B --> C[InfoHandler]
    C --> D[WarnHandler]
    D --> E[ErrorHandler]
    E --> F[FatalHandler]
    
    B -.->|TRACE/DEBUG| B1[Appenders]
    C -.->|INFO| C1[Appenders]
    D -.->|WARN| D1[Appenders]
    E -.->|ERROR| E1[Appenders]
    F -.->|FATAL| F1[Appenders]
```

## Sequence Diagram

```mermaid
sequenceDiagram
    participant App
    participant Logger
    participant HandlerChain
    participant InfoHandler
    participant Appenders
    participant Formatter
    
    App->>Logger: info("message")
    Logger->>Logger: create LogMessage
    Logger->>HandlerChain: handle(message)
    HandlerChain->>InfoHandler: handle(message)
    InfoHandler->>InfoHandler: shouldHandle(INFO)?
    Note over InfoHandler: Yes, INFO matches
    InfoHandler->>Appenders: notifyAppenders()
    loop Each Appender
        Appenders->>Formatter: format(message)
        Formatter-->>Appenders: formatted string
        Appenders->>Appenders: append(formatted)
    end
    InfoHandler->>HandlerChain: pass to next
```

## Observer Pattern Detail

```mermaid
flowchart TD
    subgraph Handlers
        A[InfoHandler]
    end
    
    subgraph Appenders[Appenders - Observers]
        B[ConsoleAppender]
        C[FileAppender]
    end
    
    A -->|notifyAppenders| B
    A -->|notifyAppenders| C
    
    B --> D[PlainTextFormatter]
    C --> E[JSONFormatter]
```

## Files Structure
```
logger/
├── enums/
│   └── LogLevel.hpp
├── model/
│   └── LogMessage.hpp
├── formatter/
│   ├── LogFormatter.hpp
│   ├── PlainTextFormatter.hpp
│   └── JSONFormatter.hpp
├── appenders/
│   ├── LogAppender.hpp
│   ├── ConsoleAppender.hpp
│   └── FileAppender.hpp
├── handlers/
│   ├── LogHandler.hpp
│   ├── DebugHandler.hpp
│   ├── InfoHandler.hpp
│   ├── WarnHandler.hpp
│   ├── ErrorHandler.hpp
│   └── FatalHandler.hpp
├── LogHandlerConfiguration.hpp
├── Logger.hpp
└── main.cpp
```
