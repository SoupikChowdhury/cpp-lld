# BookMyShow - LLD Interview Guide

A step-by-step framework for delivering the movie ticket booking system low-level design interview.

---

## 📁 Code Reference

| File | Purpose | Key Functions |
|------|---------|---------------|
| [BookMyShow.hpp](BookMyShow.hpp) | Main facade (Singleton) | `getBookMyShowObject()`, `getMoviesInCity()`, `getShowsForMovie()`, `createBooking()` |
| [BookMyShow.cpp](BookMyShow.cpp) | Implementation of facade | All facade methods |
| [MovieController.hpp](MovieController.hpp) | Movie management (Singleton) | `addMovie()`, `getMoviesByCity()`, `searchMovies()` |
| [TheatreController.hpp](TheatreController.hpp) | Theatre management (Singleton) | `addTheatre()`, `getTheatresByCity()`, `getShowsForMovie()` |
| [Theatre.hpp](Theatre.hpp) | Theatre entity with screens | `addScreen()`, `getScreens()`, `getShows()` |
| [Screen.hpp](Screen.hpp) | Screen with seats | `getSeats()`, `getAvailableSeats()` |
| [Show.hpp](Show.hpp) | Show with thread-safe booking | `bookSeats()`, `areSeatsAvailable()`, `cancelSeats()`, `getAvailableSeats()` |
| [Seat.hpp](Seat.hpp) | Seat with category and pricing | `getSeatCategory()`, `getPrice()`, `isAvailable()` |
| [Booking.hpp](Booking.hpp) | Booking record | `confirmBooking()`, `cancelBooking()`, `makePayment()` |
| [Movie.hpp](Movie.hpp) | Movie entity | `getTitle()`, `getDuration()` |
| [Payment.hpp](Payment.hpp) | Payment processing | `processPayment()` |
| [Enums/City.hpp](Enums/City.hpp) | City enumeration | `DELHI`, `MUMBAI`, `BANGALORE` |
| [Enums/SeatCategory.hpp](Enums/SeatCategory.hpp) | Seat category enum | `SILVER`, `GOLD`, `PLATINUM` |

---

## 🎯 Design Patterns Used

| Pattern | Implementation Location | Purpose |
|---------|------------------------|----------|
| **Singleton** | [BookMyShow.hpp](BookMyShow.hpp), [MovieController.hpp](MovieController.hpp), [TheatreController.hpp](TheatreController.hpp) | Single instance for controllers |
| **Facade** | [BookMyShow.hpp](BookMyShow.hpp) | Unified interface for movie booking |
| **Composition** | Theatre→Screen→Seat hierarchy | Object composition for theatre structure |
| **Thread-Safety** | [Show.hpp](Show.hpp) - `std::mutex` for `bookSeats()` | Prevent double-booking of seats |

---

## 1) Requirements (~5 minutes)

### Clarifying Questions to Ask
- "What entities are we booking? Movies, events, or both?"
- "Do we need to support multiple cities and theatres?"
- "How should seat selection work? Pick specific seats or just quantity?"
- "What payment methods should we support?"
- "Should we handle concurrent booking attempts for the same seats?"

### Requirements

```
Requirements:
1. Users can browse movies by city.
2. Users can view theatres showing a specific movie with available showtimes.
3. Users can select specific seats from a seat map.
4. Seats are categorized (Silver, Gold, Platinum) with different pricing.
5. Booking is confirmed only after successful payment.
6. Two users cannot book the same seat for the same show.
7. Users can cancel bookings (with refund policy).

Out of Scope:
- User authentication/registration
- Food/beverage ordering
- Reviews and ratings
- Notifications and reminders
- Waitlist for sold-out shows
- Dynamic pricing
```

---

## 2) Entities and Relationships (~3 minutes)

### Identify Entities

Scan the requirements and pull out meaningful nouns:

- **Movie** - The content being shown
- **Theatre** - Physical location with screens
- **Screen** - Contains seats, hosts shows
- **Seat** - Individual bookable unit with category
- **Show** - Movie + Screen + Time, tracks booked seats
- **Booking** - Transaction record linking user, show, seats
- **Payment** - Handles payment processing

### Define Relationships

```
Entities:
- Movie
- Theatre
- Screen
- Seat
- Show
- Booking
- Payment
- MovieController (Singleton)
- TheatreController (Singleton)
- BookMyShow (Facade)

Relationships:
- Theatre -> Screen (1:N, composition)
- Theatre -> Show (1:N, aggregation)
- Screen -> Seat (1:N, composition)
- Show -> Movie (N:1)
- Show -> Screen (N:1)
- Booking -> Show (N:1)
- Booking -> Seat (1:N)
- Booking -> Payment (1:1)
```

---

## 3) Class Design (~10-15 minutes)

### Show (Critical - Where booking logic lives)

| Requirement | State Needed |
|-------------|--------------|
| Track which seats are booked | bookedSeatIds: set<int> |
| Prevent concurrent double-booking | bookingMutex: mutex |
| Link to movie and screen | movie, screen references |
| Showtime info | startTime: time_t |

```
class Show:
  - showId: int
  - movie: Movie*
  - screen: Screen*
  - startTime: time_t
  - bookedSeatIds: set<int>
  - bookingMutex: mutex

  + areSeatsAvailable(seats: vector<Seat>) -> bool
  + bookSeats(seats: vector<Seat>) -> bool
  + cancelSeats(seats: vector<Seat>)
  + getAvailableSeats() -> vector<Seat>
```

### Seat

| Requirement | State Needed |
|-------------|--------------|
| Seat identity | seatId, rowNumber, seatNumber |
| Pricing tier | seatCategory: SeatCategory |

```
class Seat:
  - seatId: int
  - rowNumber: int
  - seatNumber: int
  - seatCategory: SeatCategory (SILVER, GOLD, PLATINUM)

  + getPrice() -> double
  + getSeatLabel() -> string  // "A1", "B5"
```

**Design Decision:** Seat doesn't track booking state. Why? Same seat can be available for one show and booked for another. Booking state is show-specific.

### Booking

| Requirement | State Needed |
|-------------|--------------|
| Link to show and seats | show, bookedSeats |
| Payment info | payment |
| Booking lifecycle | status: BookingStatus |

```
class Booking:
  - bookingId: int
  - show: Show*
  - bookedSeats: vector<Seat*>
  - payment: Payment*
  - status: BookingStatus (PENDING, CONFIRMED, CANCELLED)
  - customerName: string
  - customerPhone: string

  + calculateTotalAmount() -> double
  + makePayment(method: PaymentMethod) -> bool
  + cancelBooking() -> bool
  + generateTicket() -> string
```

### Theatre

```
class Theatre:
  - theatreId: int
  - name: string
  - address: string
  - city: City
  - screens: vector<Screen*>
  - shows: vector<Show*>

  + addShow(show: Show*)
  + getShowsForMovie(movieId: int) -> vector<Show*>
```

### BookMyShow (Facade)

```
class BookMyShow:
  - movieController: MovieController*
  - theatreController: TheatreController*
  - allBookings: vector<Booking*>

  + getMoviesInCity(city: City) -> vector<Movie>
  + getShowsForMovie(movieName: string, city: City) -> map<Theatre, vector<Show>>
  + createBooking(show: Show, seats: vector<Seat>, customer, payment) -> Booking
  + cancelBooking(bookingId: int) -> bool
```

---

## 4) Implementation (~10 minutes)

### Key Method: Show.bookSeats() - Thread-safe booking

```
bookSeats(seats: vector<Seat>) -> bool
    lock_guard<mutex> lock(bookingMutex)  // RAII lock
    
    // Step 1: Verify ALL seats are available
    for seat in seats:
        if bookedSeatIds.contains(seat.getId())
            return false  // At least one seat taken
    
    // Step 2: Book ALL seats atomically
    for seat in seats:
        bookedSeatIds.insert(seat.getId())
    
    return true
```

### Key Method: Booking.makePayment()

```
makePayment(method: PaymentMethod) -> bool
    // Step 1: Try to lock seats first (optimistic)
    if not show.bookSeats(bookedSeats)
        return false  // Seats already taken
    
    // Step 2: Process payment
    payment = new Payment(calculateTotalAmount(), method)
    if not payment.processPayment()
        // Rollback: Release seats
        show.cancelSeats(bookedSeats)
        return false
    
    // Step 3: Confirm booking
    status = BookingStatus.CONFIRMED
    return true
```

### Key Method: Booking.calculateTotalAmount()

```
calculateTotalAmount() -> double
    total = 0
    for seat in bookedSeats:
        total += seat.getPrice()
    
    // Add convenience fee, taxes
    total += CONVENIENCE_FEE
    total *= (1 + TAX_RATE)
    
    return total
```

### Verification: Walk Through a Scenario

```
Initial: Show for "Inception" at 7PM, Screen has 100 seats, all available

User A flow:
1. getShowsForMovie("Inception", BANGALORE) → Returns Show
2. show.getAvailableSeats() → 100 seats
3. User selects seats A1, A2 (Gold category)
4. createBooking(show, [A1, A2], userA, CREDIT_CARD)
   - show.bookSeats([A1, A2]) → Locks mutex, checks available, books, returns true
   - payment.processPayment() → Returns true
   - status = CONFIRMED
5. show.getAvailableSeats() → 98 seats

User B (concurrent):
1. Tries to book A1, A3
2. show.bookSeats([A1, A3]) → A1 already in bookedSeatIds → Returns false
3. Booking fails, user prompted to select different seats
```

---

## 5) Extensibility (~5 minutes)

### "How would you handle concurrent booking attempts?"

> "The Show class uses a mutex around the bookSeats() method. This ensures atomic check-and-book. For distributed systems, I'd use Redis distributed locks or database-level optimistic locking with version numbers."

### "How would you add offers/discounts?"

> "I'd use the Strategy pattern for pricing. Create a PricingStrategy interface with implementations like WeekdayDiscount, CouponDiscount, MembershipDiscount. The Booking class uses the strategy to calculate final amount."

```
class PricingStrategy:
  + calculatePrice(booking: Booking) -> double

class WeekdayDiscount : PricingStrategy
class CouponDiscount : PricingStrategy
```

### "How would you add a waitlist for sold-out shows?"

> "Add a WaitlistController that maintains a queue per show. When a booking is cancelled, notify the first person in the waitlist. Use Observer pattern—Waitlist observers the Show for cancellation events."

### "How would you scale for millions of concurrent users?"

> "Database sharding by city/theatre, Redis cache for show availability, message queues for async booking confirmation. Read replicas for browsing, write to primary for bookings."

---

## Design Patterns Summary

| Pattern | Where | Why |
|---------|-------|-----|
| **Singleton** | Controllers | Single source of truth |
| **Facade** | BookMyShow | Simplified interface to subsystem |
| **Factory** | Screen.createSeats() | Complex seat initialization |
| **Strategy** | Pricing (extension) | Different discount algorithms |
| **Observer** | Notifications (extension) | Booking status updates |

---

## Booking Flow

```
1. Browse movies by city
          ↓
2. Select movie → Get theatres and shows
          ↓
3. Select show → View seat map
          ↓
4. Select seats → Create pending booking
          ↓
5. Process payment
          ↓
   ├── Success → Lock seats → Confirm booking → Generate ticket
   └── Failure → Release seats → Show error
```
