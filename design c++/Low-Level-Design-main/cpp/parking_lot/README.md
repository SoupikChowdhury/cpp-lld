# Parking Lot System - Low Level Design (C++)

## Design Patterns Used
- **Singleton Pattern**: ParkingLot class has single instance
- **Factory Pattern**: VehicleFactory, PricingStrategyFactory, PaymentStrategyFactory
- **Strategy Pattern**: PricingStrategy (TimeBasedPricing, EventBasedPricing), PaymentStrategy (Cash, Card, UPI)
- **Builder Pattern**: Ticket construction
- **Template Method**: Vehicle abstract class with getType() as abstract method

## Class Diagram

```mermaid
classDiagram
    class Vehicle {
        <<abstract>>
        #string licensePlate
        +getLicensePlate() string
        +getType()* VehicleType
    }
    
    class Car {
        +getType() VehicleType
    }
    
    class Bike {
        +getType() VehicleType
    }
    
    class Truck {
        +getType() VehicleType
    }
    
    class ParkingSpot {
        -string id
        -VehicleType allowedType
        -atomic~bool~ occupied
        +getId() string
        +getAllowedType() VehicleType
        +tryOccupy() bool
        +release()
    }
    
    class ParkingFloor {
        -string id
        -map~string, ParkingSpot*~ spots
        +getId() string
        +addSpot(ParkingSpot* spot)
        +findAvailableSpot(VehicleType type) ParkingSpot*
    }
    
    class Ticket {
        -string id
        -Vehicle* vehicle
        -ParkingSpot* spot
        -LocalDateTime entryTime
        -LocalDateTime exitTime
        -double fee
        +getId() string
        +getVehicle() Vehicle*
        +getSpot() ParkingSpot*
    }
    
    class Gate {
        <<abstract>>
        #string id
        +getId() string
        +getType()* GateType
    }
    
    class EntryGate {
        +getType() GateType
        +parkVehicle(Vehicle* vehicle, DateTime entryTime) Ticket*
    }
    
    class ExitGate {
        +getType() GateType
        +unparkVehicle(string ticketId, DateTime exitTime, PaymentMode mode)
    }
    
    class PricingStrategy {
        <<interface>>
        +calculateFee(VehicleType type, long hours)* double
    }
    
    class TimeBasedPricing {
        +calculateFee(VehicleType type, long hours) double
    }
    
    class EventBasedPricing {
        +calculateFee(VehicleType type, long hours) double
    }
    
    class PaymentStrategy {
        <<interface>>
        +processPayment(double amount)* bool
    }
    
    class CashPayment {
        +processPayment(double amount) bool
    }
    
    class CardPayment {
        +processPayment(double amount) bool
    }
    
    class UPIPayment {
        +processPayment(double amount) bool
    }
    
    class PaymentService {
        +processPayment(double amount, PaymentStrategy* strategy) bool
    }
    
    class ParkingLot {
        -static ParkingLot* instance
        -map~string, ParkingFloor*~ floors
        -map~string, Ticket*~ activeTickets
        -PricingStrategy* pricingStrategy
        +getInstance()$ ParkingLot*
        +addFloor(ParkingFloor* floor)
        +parkVehicle(Vehicle* vehicle, DateTime entry) Ticket*
        +unparkVehicle(string ticketId, DateTime exit, PaymentMode mode)
        +setPricingStrategy(PricingStrategy* strategy)
    }
    
    class VehicleFactory {
        +create(VehicleType type, string plate)$ Vehicle*
    }
    
    class PricingStrategyFactory {
        +get(PricingStrategyType type)$ PricingStrategy*
    }
    
    class PaymentStrategyFactory {
        +get(PaymentMode mode)$ PaymentStrategy*
    }

    Vehicle <|-- Car
    Vehicle <|-- Bike
    Vehicle <|-- Truck
    
    Gate <|-- EntryGate
    Gate <|-- ExitGate
    
    PricingStrategy <|.. TimeBasedPricing
    PricingStrategy <|.. EventBasedPricing
    
    PaymentStrategy <|.. CashPayment
    PaymentStrategy <|.. CardPayment
    PaymentStrategy <|.. UPIPayment
    
    ParkingLot --> ParkingFloor
    ParkingFloor --> ParkingSpot
    ParkingLot --> Ticket
    ParkingLot --> PricingStrategy
    
    Ticket --> Vehicle
    Ticket --> ParkingSpot
    
    EntryGate --> ParkingLot
    ExitGate --> ParkingLot
    
    PaymentService --> PaymentStrategy
```

## Flow Diagram

```mermaid
flowchart TD
    A[Vehicle Arrives] --> B[EntryGate]
    B --> C{Find Available Spot}
    C -->|Found| D[Create Ticket]
    C -->|Not Found| E[Display Parking Full]
    D --> F[Mark Spot Occupied]
    F --> G[Return Ticket]
    
    H[Vehicle Exits] --> I[ExitGate]
    I --> J[Get Ticket]
    J --> K[Calculate Fee]
    K --> L[Process Payment]
    L --> M{Payment Success?}
    M -->|Yes| N[Release Spot]
    M -->|No| O[Retry Payment]
    N --> P[Exit Complete]
```

## Sequence Diagram

```mermaid
sequenceDiagram
    participant V as Vehicle
    participant EG as EntryGate
    participant PL as ParkingLot
    participant PF as ParkingFloor
    participant PS as ParkingSpot
    participant T as Ticket
    
    V->>EG: arrive()
    EG->>PL: parkVehicle(vehicle, entryTime)
    PL->>PF: findAvailableSpot(vehicleType)
    PF->>PS: tryOccupy()
    PS-->>PF: true (atomic)
    PF-->>PL: parkingSpot
    PL->>T: create(vehicle, spot, entryTime)
    PL-->>EG: ticket
    EG-->>V: ticket
    
    Note over V,T: Later - Vehicle Exits
    
    V->>ExitGate: exit(ticketId)
    ExitGate->>PL: unparkVehicle(ticketId, exitTime, paymentMode)
    PL->>PricingStrategy: calculateFee(type, duration)
    PL->>PaymentService: processPayment(fee, strategy)
    PaymentService-->>PL: success
    PL->>PS: release()
    PL-->>ExitGate: success
```

## Files Structure
```
parking_lot/
├── enums/
│   ├── VehicleType.hpp
│   ├── GateType.hpp
│   ├── PaymentMode.hpp
│   ├── PaymentStatus.hpp
│   └── PricingStrategyType.hpp
├── model/
│   ├── Vehicle.hpp
│   ├── Car.hpp
│   ├── Bike.hpp
│   ├── Truck.hpp
│   ├── ParkingSpot.hpp
│   ├── ParkingFloor.hpp
│   ├── Ticket.hpp
│   ├── Gate.hpp
│   ├── EntryGate.hpp
│   └── ExitGate.hpp
├── strategy/
│   ├── pricing/
│   │   ├── PricingStrategy.hpp
│   │   ├── TimeBasedPricing.hpp
│   │   └── EventBasedPricing.hpp
│   └── payment/
│       ├── PaymentStrategy.hpp
│       ├── CashPayment.hpp
│       ├── CardPayment.hpp
│       └── UPIPayment.hpp
├── factory/
│   ├── VehicleFactory.hpp
│   ├── PricingStrategyFactory.hpp
│   └── PaymentStrategyFactory.hpp
├── service/
│   ├── ParkingLot.hpp
│   └── PaymentService.hpp
└── main.cpp
```
