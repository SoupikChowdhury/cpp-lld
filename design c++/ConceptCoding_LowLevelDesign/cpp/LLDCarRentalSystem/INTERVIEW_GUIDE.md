# Car Rental System - LLD Interview Guide

A step-by-step framework for delivering the car rental system low-level design interview.

---

## 📁 Code Reference

| File | Purpose | Key Functions |
|------|---------|---------------|
| [VehicleRentalSystem.hpp](VehicleRentalSystem.hpp) | Main system (Singleton) | `addStore()`, `registerUser()`, `searchAvailableVehicles()`, `makeReservation()` |
| [Store.hpp](Store.hpp) | Rental store location | `addVehicle()`, `getAvailableVehicles()`, `createReservation()` |
| [VehicleInventoryManagement.hpp](VehicleInventoryManagement.hpp) | Vehicle inventory per store | `addVehicle()`, `removeVehicle()`, `getVehiclesByType()` |
| [Reservation.hpp](Reservation.hpp) | Booking record | `confirm()`, `activate()`, `complete()`, `cancel()`, `getReservationId()` |
| [ReservationStatus.hpp](ReservationStatus.hpp) | Status enum | `PENDING`, `CONFIRMED`, `ACTIVE`, `COMPLETED`, `CANCELLED` |
| [ReservationType.hpp](ReservationType.hpp) | Duration type enum | `HOURLY`, `DAILY`, `WEEKLY`, `MONTHLY` |
| [Bill.hpp](Bill.hpp) | Bill generation | `calculateBill()`, `generateInvoice()` |
| [Payment.hpp](Payment.hpp) | Payment processing | `processPayment()` |
| [PaymentMode.hpp](PaymentMode.hpp) | Payment interface | `pay()` |
| [User.hpp](User.hpp) | Customer entity | `getName()`, `getLicense()` |
| [Location.hpp](Location.hpp) | Location entity | `getCity()`, `getAddress()` |
| [Product/Vehicle.hpp](Product/Vehicle.hpp) | Abstract vehicle base | `getVehicleId()`, `getType()`, `getStatus()`, `setStatus()` |
| [Product/Car.hpp](Product/Car.hpp) | Car subclass | Extended with carType, seats, AC, fuel |
| [Product/Bike.hpp](Product/Bike.hpp) | Bike subclass | Extended with bikeType, CC |
| [Product/VehicleType.hpp](Product/VehicleType.hpp) | Vehicle type enum | `CAR`, `BIKE`, `SUV` |
| [Product/Status.hpp](Product/Status.hpp) | Vehicle status enum | `AVAILABLE`, `RESERVED`, `RENTED` |

---

## 🎯 Design Patterns Used

| Pattern | Implementation Location | Purpose |
|---------|------------------------|----------|
| **Singleton** | [VehicleRentalSystem.hpp](VehicleRentalSystem.hpp) | Single system instance |
| **Inheritance/Polymorphism** | [Product/Vehicle.hpp](Product/Vehicle.hpp) → Car, Bike | Different vehicle types |
| **Composition** | Store contains VehicleInventoryManagement and Reservations | Object relationships |

---

## 1) Requirements (~5 minutes)

### Clarifying Questions to Ask
- "Are there multiple store locations? Can users pick up at one and return at another?"
- "What vehicle types should we support? Cars, bikes, SUVs?"
- "What rental duration options? Hourly, daily, weekly?"
- "How should billing work? Base rate plus taxes, insurance, penalties?"
- "Should we handle vehicle maintenance status?"

### Requirements

```
Requirements:
1. Multiple stores across different locations.
2. Each store maintains its own vehicle inventory.
3. Users can search for available vehicles by type and dates.
4. Users can make reservations specifying pickup/dropoff locations and times.
5. System generates bills with base amount, taxes, insurance, and late fees.
6. Support different vehicle types (Car, Bike, etc.).
7. Track reservation status through lifecycle.

Out of Scope:
- User authentication and membership tiers
- Vehicle damage assessment
- Insurance claim processing
- GPS/vehicle tracking
- Dynamic pricing based on demand
- Loyalty/rewards program
```

---

## 2) Entities and Relationships (~3 minutes)

### Identify Entities

Scan the requirements and pull out meaningful nouns:

- **VehicleRentalSystem** - Central management (Singleton)
- **Store** - Rental location
- **Vehicle** - Car, Bike (abstract hierarchy)
- **VehicleInventoryManagement** - Manages vehicles per store
- **User** - Customer with license info
- **Reservation** - Booking record
- **Bill** - Invoice with breakdown
- **Payment** - Payment processing
- **Location** - Address with coordinates

### Define Relationships

```
Entities:
- VehicleRentalSystem (Singleton, Facade)
- Store
- Vehicle (abstract: Car, Bike)
- VehicleInventoryManagement
- User
- Reservation
- Bill
- Payment
- Location

Relationships:
- VehicleRentalSystem -> Store (1:N)
- Store -> VehicleInventoryManagement (1:1)
- Store -> Reservation (1:N)
- Store -> Location (1:1)
- VehicleInventoryManagement -> Vehicle (1:N)
- Reservation -> User (N:1)
- Reservation -> Vehicle (N:1)
- Reservation -> Location (pickup, dropoff)
- Reservation -> Bill (1:1)
- Bill -> Payment (1:1)
```

---

## 3) Class Design (~10-15 minutes)

### VehicleRentalSystem (Singleton, Facade)

| Requirement | State Needed |
|-------------|--------------|
| Central management | Singleton instance |
| Manage all stores | stores: vector<Store> |
| Manage all users | users: vector<User> |

```
class VehicleRentalSystem:
  - instance: VehicleRentalSystem* (static)
  - stores: vector<Store>
  - users: vector<User>

  + getInstance() -> VehicleRentalSystem*
  + addStore(store: Store)
  + searchAvailableVehicles(city, vehicleType, startDate, endDate) -> vector<Vehicle>
  + makeReservation(user, vehicle, pickupLoc, dropoffLoc, startDate, endDate) -> Reservation
  + completeReservation(reservationId) -> Bill
```

### Vehicle (Abstract)

| Requirement | State Needed |
|-------------|--------------|
| Vehicle identity | vehicleId, licensePlate |
| Vehicle details | brand, model, year |
| Rental info | dailyRate |
| Availability | status |

```
class Vehicle:
  - vehicleId: string
  - licensePlate: string
  - brand: string
  - model: string
  - year: int
  - dailyRate: double
  - status: VehicleStatus (AVAILABLE, RENTED, UNDER_MAINTENANCE)
  - kmDriven: int

  + getDailyRate() -> double
  + getStatus() -> VehicleStatus
  + setStatus(status: VehicleStatus)

class Car : Vehicle
  - numSeats: int
  - transmissionType: string

class Bike : Vehicle
  - engineCC: int
```

### Store

```
class Store:
  - storeId: string
  - name: string
  - location: Location
  - inventory: VehicleInventoryManagement
  - reservations: vector<Reservation>

  + getAvailableVehicles(vehicleType, startDate, endDate) -> vector<Vehicle>
  + createReservation(user, vehicle, pickupLoc, dropoffLoc, dates) -> Reservation
  + completeReservation(reservationId) -> Bill
```

### Reservation

| Requirement | State Needed |
|-------------|--------------|
| Booking details | user, vehicle, dates, locations |
| Status tracking | status |
| Rental type | reservationType |

```
class Reservation:
  - reservationId: string
  - user: User
  - vehicle: Vehicle
  - pickupLocation: Location
  - dropoffLocation: Location
  - pickupTime: datetime
  - dropoffTime: datetime
  - reservationType: ReservationType (HOURLY, DAILY, WEEKLY, MONTHLY)
  - status: ReservationStatus (PENDING, CONFIRMED, ACTIVE, COMPLETED, CANCELLED)
  - createdAt: datetime

  + getDuration() -> int
  + getBaseAmount() -> double
  + confirm()
  + activate()
  + complete() -> Bill
  + cancel()
```

### Bill

```
class Bill:
  - billId: string
  - reservation: Reservation
  - baseAmount: double
  - taxes: double
  - insurance: double
  - lateFee: double
  - damageCharges: double
  - discount: double
  - totalAmount: double
  - isPaid: bool

  + calculateTotal() -> double
  + generateInvoice() -> string
  + markPaid()
```

### VehicleInventoryManagement

```
class VehicleInventoryManagement:
  - vehicles: vector<Vehicle>

  + addVehicle(vehicle: Vehicle)
  + removeVehicle(vehicleId: string)
  + getVehiclesByType(type: VehicleType) -> vector<Vehicle>
  + getAvailableVehicles(type, startDate, endDate) -> vector<Vehicle>
  + updateVehicleStatus(vehicleId, status)
```

---

## 4) Implementation (~10 minutes)

### Key Method: Store.createReservation()

```
createReservation(user, vehicle, pickupLoc, dropoffLoc, startTime, endTime) -> Reservation
    // Validate vehicle availability
    if vehicle.getStatus() != AVAILABLE
        throw "Vehicle not available"
    
    // Check for conflicting reservations
    for reservation in reservations:
        if reservation.vehicle == vehicle and
           reservation.overlaps(startTime, endTime)
            throw "Vehicle already reserved for these dates"
    
    // Create reservation
    reservation = new Reservation(
        reservationId = generateId(),
        user = user,
        vehicle = vehicle,
        pickupLocation = pickupLoc,
        dropoffLocation = dropoffLoc,
        pickupTime = startTime,
        dropoffTime = endTime,
        status = PENDING
    )
    
    reservations.add(reservation)
    
    return reservation
```

### Key Method: Reservation.complete()

```
complete() -> Bill
    // Mark vehicle as returned
    vehicle.setStatus(AVAILABLE)
    
    // Calculate actual duration
    actualDropoffTime = now()
    
    // Generate bill
    bill = new Bill(reservation = this)
    
    // Base amount
    duration = calculateDuration(pickupTime, actualDropoffTime)
    bill.baseAmount = duration * vehicle.getDailyRate()
    
    // Check for late return
    if actualDropoffTime > dropoffTime
        hoursLate = diff(actualDropoffTime, dropoffTime)
        bill.lateFee = hoursLate * LATE_FEE_PER_HOUR
    
    // Add taxes and insurance
    bill.taxes = bill.baseAmount * TAX_RATE
    bill.insurance = duration * INSURANCE_PER_DAY
    
    // Calculate total
    bill.calculateTotal()
    
    // Update status
    status = COMPLETED
    
    return bill
```

### Key Method: VehicleRentalSystem.searchAvailableVehicles()

```
searchAvailableVehicles(city, vehicleType, startDate, endDate) -> vector<Vehicle>
    results = []
    
    for store in stores:
        if store.location.city == city
            available = store.inventory.getAvailableVehicles(
                vehicleType, startDate, endDate)
            results.addAll(available)
    
    return results
```

### Key Method: Bill.calculateTotal()

```
calculateTotal() -> double
    totalAmount = baseAmount
               + taxes
               + insurance
               + lateFee
               + damageCharges
               - discount
    
    return totalAmount
```

### Verification: Walk Through a Scenario

```
Setup:
- Store in Seattle with: Car A ($50/day), Bike B ($25/day)
- User "Alice"

1. searchAvailableVehicles("Seattle", CAR, Jan1, Jan3)
   - Store Seattle found
   - Car A available (no conflicts)
   - Return [Car A]

2. makeReservation(Alice, Car A, Seattle, Seattle, Jan1, Jan3)
   - Vehicle available ✓
   - No conflicts ✓
   - Create Reservation RES-001
   - Status = PENDING
   
3. reservation.confirm()
   - Status = CONFIRMED
   - Car A status = RESERVED

4. reservation.activate() (on Jan 1)
   - Status = ACTIVE
   - Car A status = RENTED

5. reservation.complete() (on Jan 3, returned on time)
   - Duration = 2 days
   - Bill:
     - baseAmount = 2 × $50 = $100
     - taxes = $100 × 0.1 = $10
     - insurance = 2 × $5 = $10
     - lateFee = $0
     - totalAmount = $120
   - Car A status = AVAILABLE
   - Status = COMPLETED
```

---

## 5) Extensibility (~5 minutes)

### "How would you handle one-way rentals (different pickup/dropoff)?"

> "Already supported! Reservation has separate pickupLocation and dropoffLocation. For billing, add a one-way surcharge based on distance between locations."

```
calculateOneWaySurcharge() -> double
    if pickupLocation != dropoffLocation
        distance = pickupLocation.distanceTo(dropoffLocation)
        return distance * RATE_PER_KM
    return 0
```

### "How would you implement dynamic pricing?"

> "Use Strategy pattern for pricing. Create PricingStrategy interface with implementations like StandardPricing, WeekendSurge, HolidayPricing. Select strategy based on booking dates."

```
class PricingStrategy:
  + calculateRate(vehicle, startDate, endDate) -> double

class SurgePricing : PricingStrategy
  + calculateRate(vehicle, startDate, endDate)
      demandRatio = getAvailableCount() / getTotalCount()
      if demandRatio < 0.2
          return vehicle.getDailyRate() * 2.0  // 2x surge
      return vehicle.getDailyRate()
```

### "How would you add vehicle maintenance scheduling?"

> "Add VehicleStatus.UNDER_MAINTENANCE. Track kmDriven and schedule maintenance after threshold. Maintenance blocks the vehicle from new reservations."

```
checkMaintenanceNeeded(vehicle) -> bool
    if vehicle.kmDriven > MAINTENANCE_THRESHOLD
        vehicle.setStatus(UNDER_MAINTENANCE)
        scheduleMaintenance(vehicle)
        return true
    return false
```

### "How would you handle concurrent reservation attempts?"

> "Add mutex at the store level. Lock when checking availability and creating reservation to prevent double-booking."

---

## Design Patterns Summary

| Pattern | Where | Why |
|---------|-------|-----|
| **Singleton** | VehicleRentalSystem | Single system instance |
| **Factory** | VehicleFactory | Create vehicle types |
| **Strategy** | PricingStrategy (extension) | Different pricing algorithms |
| **State** | ReservationStatus | Clean status transitions |
| **Facade** | VehicleRentalSystem | Simplified interface |

---

## Reservation Status Flow

```
PENDING → CONFIRMED → ACTIVE → COMPLETED
    ↓         ↓          ↓
CANCELLED  CANCELLED   CANCELLED
```

| Transition | Trigger |
|------------|---------|
| PENDING → CONFIRMED | User confirms booking |
| CONFIRMED → ACTIVE | User picks up vehicle |
| ACTIVE → COMPLETED | User returns vehicle |
| Any → CANCELLED | User cancels / no-show |
