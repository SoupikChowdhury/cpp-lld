# Car Rental System - UML Diagram

## Class Diagram

```
┌─────────────────────────────────────────────────────────────────────────────────────────────────┐
│                                  CAR RENTAL SYSTEM UML                                           │
└─────────────────────────────────────────────────────────────────────────────────────────────────┘

        ┌─────────────────┐     ┌─────────────────┐     ┌─────────────────┐
        │  «enumeration»  │     │  «enumeration»  │     │  «enumeration»  │
        │   VehicleType   │     │  VehicleStatus  │     │ ReservationType │
        ├─────────────────┤     ├─────────────────┤     ├─────────────────┤
        │ CAR             │     │ AVAILABLE       │     │ HOURLY          │
        │ BIKE            │     │ RENTED          │     │ DAILY           │
        │ SUV             │     │ UNDER_MAINT     │     │ WEEKLY          │
        │ VAN             │     │ RESERVED        │     │ MONTHLY         │
        │ TRUCK           │     └─────────────────┘     └─────────────────┘
        └─────────────────┘

┌──────────────────────────────────────────────────────────────────────────────────────────────────┐
│                              VEHICLE RENTAL SYSTEM «Singleton»                                    │
├──────────────────────────────────────────────────────────────────────────────────────────────────┤
│ - stores: map<string, Store>                                                                      │
│ - users: map<string, User>                                                                        │
│ - bills: map<string, Bill>                                                                        │
│ - payments: map<string, Payment>                                                                  │
│ - reservationCounter: int                                                                         │
├──────────────────────────────────────────────────────────────────────────────────────────────────┤
│ + getInstance(): VehicleRentalSystem&                                                             │
│ + addStore(store: Store): void                                                                    │
│ + registerUser(user: User): void                                                                  │
│ + searchAvailableVehicles(type): vector<pair<Store, Vehicle>>                                    │
│ + makeReservation(...): Reservation                                                               │
│ + generateBill(reservation): Bill                                                                 │
│ + processPayment(bill, mode): Payment                                                             │
│ + completeRental(storeId, reservationId, ...): void                                              │
└──────────────────────────────────────────────────────────────────────────────────────────────────┘
            │ 1..*                                    │ 1..*
            ▼                                         ▼
┌────────────────────────────────┐    ┌────────────────────────────────┐
│            STORE               │    │             USER               │
├────────────────────────────────┤    ├────────────────────────────────┤
│ - storeId: string              │    │ - userId: string               │
│ - name: string                 │    │ - name: string                 │
│ - location: Location           │    │ - email: string                │
│ - inventory: VehicleInventory  │    │ - phone: string                │
│ - reservations: map            │    │ - drivingLicense: string       │
├────────────────────────────────┤    │ - isActive: bool               │
│ + addVehicle(vehicle): void    │    ├────────────────────────────────┤
│ + getAvailableVehicles(): vec  │    │ + displayInfo(): void          │
│ + createReservation(...): Res  │    └────────────────────────────────┘
└────────────────────────────────┘
            │ 1                                          ▲
            ▼                                            │
┌────────────────────────────────┐                       │
│          LOCATION              │                       │
├────────────────────────────────┤                       │
│ - locationId: string           │                       │
│ - name: string                 │                       │
│ - address: string              │                       │
│ - city, state, zip: string     │                       │
│ - latitude, longitude: double  │                       │
├────────────────────────────────┤                       │
│ + getFullAddress(): string     │                       │
│ + distanceTo(other): double    │                       │
└────────────────────────────────┘                       │
                                                         │
┌────────────────────────────────────────────────────────┼───────────────────────────────┐
│                           VEHICLE INVENTORY MANAGEMENT  │                               │
├─────────────────────────────────────────────────────────┼───────────────────────────────┤
│ - vehicles: map<string, Vehicle>                        │                               │
│ - vehiclesByType: map<VehicleType, vector<string>>      │                               │
├─────────────────────────────────────────────────────────┼───────────────────────────────┤
│ + addVehicle(vehicle): void                             │                               │
│ + removeVehicle(vehicleId): void                        │                               │
│ + getVehicle(id): Vehicle                               │                               │
│ + getAvailableVehicles(): vector<Vehicle>               │                               │
│ + getAvailableVehiclesByType(type): vector<Vehicle>     │                               │
│ + updateVehicleStatus(id, status): bool                 │                               │
└─────────────────────────────────────────────────────────┼───────────────────────────────┘
            │ 1..*                                        │
            ▼                                             │
┌────────────────────────────────┐                        │
│         VEHICLE (Abstract)     │                        │
├────────────────────────────────┤                        │
│ # vehicleId: string            │                        │
│ # licensePlate: string         │                        │
│ # brand, model: string         │                        │
│ # year: int                    │                        │
│ # dailyRate: double            │                        │
│ # type: VehicleType            │                        │
│ # status: VehicleStatus        │                        │
│ # kmDriven: int                │                        │
├────────────────────────────────┤                        │
│ + getVehicleInfo(): string     │                        │
│ + displayDetails(): void       │                        │
└────────────────────────────────┘                        │
         ▲              ▲                                 │
         │              │                                 │
    ┌────┴────┐    ┌────┴────┐                           │
    │   CAR   │    │  BIKE   │                           │
    ├─────────┤    ├─────────┤                           │
    │- carType│    │-bikeType│                           │
    │-numSeats│    │-engineCC│                           │
    │- hasAC  │    │-hasHelm │                           │
    │-fuelType│    └─────────┘                           │
    └─────────┘                                          │
                                                         │
┌────────────────────────────────────────────────────────┴───────────────────────────────┐
│                                    RESERVATION                                          │
├─────────────────────────────────────────────────────────────────────────────────────────┤
│ - reservationId: string                                                                  │
│ - user: shared_ptr<User>                                                                 │
│ - vehicle: shared_ptr<Vehicle>                                                           │
│ - pickupLocation, dropoffLocation: shared_ptr<Location>                                  │
│ - pickupTime, dropoffTime: time_t                                                        │
│ - reservationType: ReservationType                                                       │
│ - status: ReservationStatus                                                              │
│ - duration: int                                                                          │
├─────────────────────────────────────────────────────────────────────────────────────────┤
│ + confirm(): void                                                                        │
│ + activate(): void                                                                       │
│ + complete(): void                                                                       │
│ + cancel(): void                                                                         │
│ + calculateBaseAmount(): double                                                          │
│ + displayReservation(): void                                                             │
└─────────────────────────────────────────────────────────────────────────────────────────┘
                    │
                    │ 1
                    ▼
┌────────────────────────────────┐         ┌────────────────────────────────┐
│            BILL                │         │          PAYMENT               │
├────────────────────────────────┤         ├────────────────────────────────┤
│ - billId: string               │         │ - paymentId: string            │
│ - reservation: Reservation     │         │ - reservationId: string        │
│ - baseAmount: double           │         │ - amount: double               │
│ - taxes: double                │         │ - mode: PaymentMode            │
│ - insuranceAmount: double      │         │ - status: PaymentStatus        │
│ - lateFeePenalty: double       │         │ - transactionId: string        │
│ - damagePenalty: double        │         ├────────────────────────────────┤
│ - discount: double             │         │ + processPayment(): bool       │
│ - totalAmount: double          │         │ + refund(): bool               │
│ - isPaid: bool                 │         │ + displayPayment(): void       │
├────────────────────────────────┤         └────────────────────────────────┘
│ + addLateFee(hours): void      │
│ + addDamagePenalty(amt): void  │
│ + applyDiscount(amt): void     │
│ + displayBill(): void          │
└────────────────────────────────┘
```

## Sequence Diagram - Rental Flow

```
┌──────┐  ┌─────────────────────┐  ┌───────┐  ┌─────────────┐  ┌─────────┐  ┌──────┐
│ User │  │VehicleRentalSystem │  │ Store │  │ Inventory   │  │Reserv.  │  │ Bill │
└──┬───┘  └─────────┬───────────┘  └───┬───┘  └──────┬──────┘  └────┬────┘  └──┬───┘
   │                │                  │             │              │          │
   │ searchVehicles │                  │             │              │          │
   │───────────────>│                  │             │              │          │
   │                │ getAvailable()   │             │              │          │
   │                │─────────────────>│             │              │          │
   │                │                  │getAvailable │              │          │
   │                │                  │────────────>│              │          │
   │                │                  │<────────────│              │          │
   │                │<─────────────────│             │              │          │
   │<───────────────│                  │             │              │          │
   │                │                  │             │              │          │
   │ makeReservation│                  │             │              │          │
   │───────────────>│                  │             │              │          │
   │                │createReservation │             │              │          │
   │                │─────────────────>│             │              │          │
   │                │                  │  create()   │              │          │
   │                │                  │───────────────────────────>│          │
   │                │                  │             │  confirm()   │          │
   │                │                  │───────────────────────────>│          │
   │                │<─────────────────│             │              │          │
   │<───────────────│                  │             │              │          │
   │                │                  │             │              │          │
   │ completeRental │                  │             │              │          │
   │───────────────>│                  │             │              │          │
   │                │ generateBill()   │             │              │          │
   │                │────────────────────────────────────────────────────────>│
   │                │                  │             │              │   calc() │
   │                │<────────────────────────────────────────────────────────│
   │<───────────────│                  │             │              │          │
```

## Component Diagram

```
┌─────────────────────────────────────────────────────────────────────────────────────┐
│                           CAR RENTAL SYSTEM COMPONENTS                               │
└─────────────────────────────────────────────────────────────────────────────────────┘

     ┌─────────────────────────────────────────────────────────────────────┐
     │                           «component»                                │
     │                     VEHICLE RENTAL SYSTEM                            │
     │  ┌──────────────────┐  ┌──────────────────┐  ┌──────────────────┐   │
     │  │   Store Mgmt     │  │   User Mgmt      │  │  Billing System  │   │
     │  └──────────────────┘  └──────────────────┘  └──────────────────┘   │
     └─────────────────────────────────────────────────────────────────────┘
                     │                    │                    │
                     ▼                    ▼                    ▼
     ┌────────────────────┐   ┌────────────────────┐   ┌────────────────────┐
     │   «component»      │   │   «component»      │   │   «component»      │
     │      STORE         │   │      USER          │   │     PAYMENT        │
     │  ┌──────────────┐  │   │  ┌──────────────┐  │   │  ┌──────────────┐  │
     │  │  Inventory   │  │   │  │ User Profile │  │   │  │   Gateway    │  │
     │  ├──────────────┤  │   │  └──────────────┘  │   │  ├──────────────┤  │
     │  │ Reservation  │  │   │                    │   │  │   Billing    │  │
     │  └──────────────┘  │   │                    │   │  └──────────────┘  │
     └────────────────────┘   └────────────────────┘   └────────────────────┘
              │                         
              ▼                         
     ┌────────────────────┐
     │   «component»      │
     │     VEHICLE        │
     │  ┌──────────────┐  │
     │  │     Car      │  │
     │  ├──────────────┤  │
     │  │    Bike      │  │
     │  └──────────────┘  │
     └────────────────────┘
```

## State Diagram - Reservation

```
                                    ┌─────────────────┐
                                    │                 │
        ┌───────────────────────────│    PENDING      │
        │                           │                 │
        │                           └────────┬────────┘
        │                                    │
        │                           confirmed│
        │                                    ▼
        │                           ┌─────────────────┐
        │                           │                 │
        │                           │   CONFIRMED     │
        │                           │                 │
        │                           └────────┬────────┘
        │                                    │
        │                            pickup  │
        │                                    ▼
    cancelled                       ┌─────────────────┐
        │                           │                 │
        │                           │    ACTIVE       │
        │                           │                 │
        │                           └────────┬────────┘
        │                                    │
        │                            return  │
        ▼                                    ▼
┌─────────────────┐                 ┌─────────────────┐
│                 │                 │                 │
│   CANCELLED     │                 │   COMPLETED     │
│                 │                 │                 │
└─────────────────┘                 └─────────────────┘
```

## State Diagram - Vehicle

```
                    ┌─────────────────┐
                    │                 │
                    │   AVAILABLE     │◄────────────────┐
                    │                 │                 │
                    └────────┬────────┘                 │
                             │                          │
              ┌──────────────┼──────────────┐           │
              │              │              │           │
         reserved      maintenance    returned          │
              │              │              │           │
              ▼              ▼              │           │
    ┌─────────────────┐  ┌─────────────────┐│           │
    │                 │  │                 ││           │
    │    RESERVED     │  │ UNDER_MAINT     │┼───────────┘
    │                 │  │                 ││
    └────────┬────────┘  └─────────────────┘│
             │                              │
        picked up                           │
             │                              │
             ▼                              │
    ┌─────────────────┐                     │
    │                 │                     │
    │     RENTED      │─────────────────────┘
    │                 │
    └─────────────────┘
```

## Design Patterns Used

| Pattern | Where Used | Purpose |
|---------|------------|---------|
| **Singleton** | VehicleRentalSystem | Single system instance managing all operations |
| **Factory** | Can extend Vehicle creation | Create different vehicle types |
| **Strategy** | ReservationType pricing | Different pricing for hourly/daily/weekly/monthly |
| **State** | VehicleStatus, ReservationStatus | Manage state transitions |
| **Repository** | VehicleInventoryManagement | Abstract data access layer |
