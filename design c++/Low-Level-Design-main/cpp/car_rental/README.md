# Car Rental System - Low Level Design (C++)

## Design Patterns Used
- **Strategy Pattern**: For booking selection and pricing strategies
- **Factory Pattern**: For creating vehicles and payment strategies
- **Singleton Pattern**: For managing vehicle inventory (optional)
- **Builder Pattern**: For constructing bookings
- **Repository Pattern**: For managing vehicle and booking data

## Class Diagram

```mermaid
classDiagram
    class VehicleType {
        <<enumeration>>
        CAR
        BIKE
        SUV
        LUXURY
    }

    class BookingStatus {
        <<enumeration>>
        PENDING
        CONFIRMED
        CANCELLED
        COMPLETED
    }

    class PaymentMode {
        <<enumeration>>
        CASH
        CARD
        UPI
    }

    class Vehicle {
        <<abstract>>
        #string vehicleId
        #string brand
        #string model
        #string registrationNumber
        #double pricePerDay
        #bool available
        #VehicleType type
        +getVehicleId() string
        +getBrand() string
        +getModel() string
        +getPricePerDay() double
        +isAvailable() bool
        +setAvailable(bool)
        +getType() VehicleType
    }

    class Car {
        +Car(id, brand, model, regNo, price)
    }

    class Bike {
        +Bike(id, brand, model, regNo, price)
    }

    class SUV {
        +SUV(id, brand, model, regNo, price)
    }

    class LuxuryCar {
        +LuxuryCar(id, brand, model, regNo, price)
    }

    Vehicle <|-- Car
    Vehicle <|-- Bike
    Vehicle <|-- SUV
    Vehicle <|-- LuxuryCar

    class Customer {
        -string customerId
        -string name
        -string email
        -string licenseNumber
        +getCustomerId() string
        +getName() string
        +getEmail() string
        +getLicenseNumber() string
    }

    class Branch {
        -string branchId
        -string name
        -string address
        -vector~Vehicle*~ vehicles
        +addVehicle(Vehicle*)
        +removeVehicle(vehicleId)
        +getAvailableVehicles(type) vector
        +getVehicle(vehicleId) Vehicle*
    }

    class Booking {
        -string bookingId
        -Customer* customer
        -Vehicle* vehicle
        -Branch* branch
        -time_t startDate
        -time_t endDate
        -double totalAmount
        -BookingStatus status
        +confirm()
        +cancel()
        +complete()
        +getTotalAmount() double
    }

    class BookingStrategy {
        <<interface>>
        +selectVehicle(vehicles) Vehicle*
    }

    class CheapestVehicleStrategy {
        +selectVehicle(vehicles) Vehicle*
    }

    class LeastBookedStrategy {
        +selectVehicle(vehicles) Vehicle*
    }

    BookingStrategy <|.. CheapestVehicleStrategy
    BookingStrategy <|.. LeastBookedStrategy

    class PricingStrategy {
        <<interface>>
        +calculatePrice(vehicle, days) double
    }

    class StandardPricing {
        +calculatePrice(vehicle, days) double
    }

    class WeekendPricing {
        +calculatePrice(vehicle, days) double
    }

    class DiscountPricing {
        -double discountPercentage
        +calculatePrice(vehicle, days) double
    }

    PricingStrategy <|.. StandardPricing
    PricingStrategy <|.. WeekendPricing
    PricingStrategy <|.. DiscountPricing

    class BookingService {
        -unordered_map~string,Branch*~ branches
        -vector~Booking*~ bookings
        -BookingStrategy* bookingStrategy
        -PricingStrategy* pricingStrategy
        +addBranch(Branch*)
        +createBooking(customer, branchId, type, start, end) Booking*
        +cancelBooking(bookingId)
        +completeBooking(bookingId)
        +setBookingStrategy(strategy)
        +setPricingStrategy(strategy)
    }

    BookingService --> Branch
    BookingService --> Booking
    BookingService --> BookingStrategy
    BookingService --> PricingStrategy
    Booking --> Customer
    Booking --> Vehicle
    Booking --> Branch
    Branch --> Vehicle
```

## Booking Flow

```mermaid
sequenceDiagram
    participant C as Customer
    participant BS as BookingService
    participant BR as Branch
    participant BST as BookingStrategy
    participant PST as PricingStrategy
    participant B as Booking

    C->>BS: createBooking(customer, branchId, vehicleType, dates)
    BS->>BR: getAvailableVehicles(vehicleType)
    BR-->>BS: List<Vehicle>
    BS->>BST: selectVehicle(vehicles)
    BST-->>BS: selectedVehicle
    BS->>PST: calculatePrice(vehicle, days)
    PST-->>BS: totalAmount
    BS->>B: create(customer, vehicle, amount)
    B->>B: setStatus(CONFIRMED)
    BS->>BR: setVehicleUnavailable(vehicle)
    BS-->>C: Booking confirmed
```

## Strategy Selection

```mermaid
flowchart TD
    A[Select Vehicle] --> B{Which Strategy?}
    
    B -->|Cheapest| C[CheapestVehicleStrategy]
    C --> D[Sort by pricePerDay ascending]
    D --> E[Return first vehicle]
    
    B -->|Least Booked| F[LeastBookedStrategy]
    F --> G[Get booking counts per vehicle]
    G --> H[Return vehicle with least bookings]
    
    I[Calculate Price] --> J{Which Pricing?}
    
    J -->|Standard| K[StandardPricing]
    K --> L[price = pricePerDay × days]
    
    J -->|Weekend| M[WeekendPricing]
    M --> N[price = pricePerDay × days × 1.5]
    
    J -->|Discount| O[DiscountPricing]
    O --> P[price = pricePerDay × days × (1 - discount)]
```

## Booking State Diagram

```mermaid
stateDiagram-v2
    [*] --> PENDING : Create Booking
    PENDING --> CONFIRMED : confirm()
    PENDING --> CANCELLED : cancel()
    CONFIRMED --> COMPLETED : complete()
    CONFIRMED --> CANCELLED : cancel()
    COMPLETED --> [*]
    CANCELLED --> [*]
```

## Vehicle Availability Flow

```mermaid
flowchart TD
    A[Customer Request] --> B{Check Availability}
    B -->|Available| C[Create Booking]
    C --> D[Mark Vehicle Unavailable]
    D --> E[Booking Confirmed]
    
    B -->|Not Available| F[Show Available Alternatives]
    F --> G{Customer Chooses}
    G -->|Yes| C
    G -->|No| H[Request Cancelled]
    
    I[Complete/Cancel Booking] --> J[Mark Vehicle Available]
    J --> K[Vehicle in Pool]
```

## Files Structure
```
car_rental/
├── enums/
│   ├── VehicleType.hpp
│   ├── BookingStatus.hpp
│   └── PaymentMode.hpp
├── model/
│   ├── Vehicle.hpp
│   ├── Car.hpp
│   ├── Bike.hpp
│   ├── SUV.hpp
│   ├── LuxuryCar.hpp
│   ├── Customer.hpp
│   ├── Branch.hpp
│   └── Booking.hpp
├── strategy/
│   ├── booking/
│   │   ├── BookingStrategy.hpp
│   │   ├── CheapestVehicleStrategy.hpp
│   │   └── LeastBookedStrategy.hpp
│   └── pricing/
│       ├── PricingStrategy.hpp
│       ├── StandardPricing.hpp
│       ├── WeekendPricing.hpp
│       └── DiscountPricing.hpp
├── service/
│   └── BookingService.hpp
├── README.md
└── main.cpp
```
