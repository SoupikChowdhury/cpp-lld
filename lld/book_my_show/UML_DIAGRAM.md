# BookMyShow - UML Class Diagram

## ASCII Class Diagram

```
┌─────────────────────────────────────────────────────────────────────────────────────────────────────────┐
│                                    BOOKMYSHOW - MOVIE TICKET BOOKING SYSTEM                              │
└─────────────────────────────────────────────────────────────────────────────────────────────────────────┘

                                           ┌────────────────────┐
                                           │   <<Singleton>>    │
                                           │    BookMyShow      │
                                           ├────────────────────┤
                                           │ - instance         │
                                           │ - movieController  │
                                           │ - theatreController│
                                           │ - allBookings      │
                                           ├────────────────────┤
                                           │ + getInstance()    │
                                           │ + initialize()     │
                                           │ + getMoviesInCity()│
                                           │ + searchMovies()   │
                                           │ + getShowsForMovie()
                                           │ + createBooking()  │
                                           │ + cancelBooking()  │
                                           └─────────┬──────────┘
                                                     │
                              ┌──────────────────────┼──────────────────────┐
                              │                      │                      │
                              ▼                      ▼                      ▼
               ┌────────────────────┐  ┌────────────────────┐  ┌────────────────────┐
               │   <<Singleton>>    │  │   <<Singleton>>    │  │      Booking       │
               │  MovieController   │  │ TheatreController  │  ├────────────────────┤
               ├────────────────────┤  ├────────────────────┤  │ - bookingId        │
               │ - cityMoviesMap    │  │ - cityTheatresMap  │  │ - show             │
               │ - allMovies        │  │ - allTheatres      │  │ - bookedSeats[]    │
               ├────────────────────┤  ├────────────────────┤  │ - payment          │
               │ + getInstance()    │  │ + getInstance()    │  │ - status           │
               │ + addMovie()       │  │ + addTheatre()     │  │ - customerName     │
               │ + getMoviesByCity()│  │ + getTheatresByCity│  ├────────────────────┤
               │ + searchMovies()   │  │ + getShowsForMovie()  │ + makePayment()    │
               └────────┬───────────┘  └────────┬───────────┘  │ + cancelBooking()  │
                        │                       │              │ + generateTicket() │
                        │                       │              └─────────┬──────────┘
                        ▼                       ▼                        │
               ┌────────────────────┐  ┌────────────────────┐           │
               │       Movie        │  │      Theatre       │           │
               ├────────────────────┤  ├────────────────────┤           │
               │ - movieId          │  │ - theatreId        │           │
               │ - movieName        │  │ - theatreName      │           │
               │ - durationInMinutes│  │ - address          │           │
               │ - genre            │  │ - city             │           │
               │ - language         │  │ - screens[]        │           │
               │ - rating           │  │ - shows[]          │           │
               ├────────────────────┤  ├────────────────────┤           │
               │ + getters/setters  │  │ + addScreen()      │           │
               │ + toString()       │  │ + addShow()        │           │
               └────────────────────┘  │ + getShowsForMovie()│          │
                        ▲              └─────────┬──────────┘           │
                        │                        │                      │
                        │           ┌────────────┴────────────┐         │
                        │           │                         │         │
                        │           ▼                         ▼         │
                        │  ┌────────────────────┐   ┌────────────────────┐
                        │  │       Screen       │   │        Show        │◄──────┘
                        │  ├────────────────────┤   ├────────────────────┤
                        │  │ - screenId         │   │ - showId           │
                        │  │ - screenName       │   │ - movie ───────────┘
                        │  │ - seats[]          │   │ - screen ──────────┐
                        │  ├────────────────────┤   │ - startTime        │
                        │  │ + createSeats()    │   │ - bookedSeatIds    │
                        │  │ + getSeatsByCategory  │ - bookingMutex      │
                        │  │ + getTotalSeats()  │   ├────────────────────┤
                        │  └─────────┬──────────┘   │ + bookSeats()      │
                        │            │              │ + cancelSeats()    │
                        │            │              │ + getAvailableSeats│
                        │            │              └────────────────────┘
                        │            ▼                        │
                        │  ┌────────────────────┐             │
                        │  │        Seat        │◄────────────┘
                        │  ├────────────────────┤
                        │  │ - seatId           │
                        │  │ - rowNumber        │
                        │  │ - seatNumber       │
                        │  │ - seatCategory     │
                        │  ├────────────────────┤
                        │  │ + getPrice()       │
                        │  │ + getSeatLabel()   │
                        │  └─────────┬──────────┘
                        │            │
                        │            ▼
                        │  ┌────────────────────┐
                        │  │   <<Enumeration>>  │
                        │  │   SeatCategory     │
                        │  ├────────────────────┤
                        │  │   SILVER           │
                        │  │   GOLD             │
                        │  │   PLATINUM         │
                        │  └────────────────────┘
                        │
       ┌────────────────┘
       │
       │  ┌────────────────────┐       ┌────────────────────┐
       │  │   <<Enumeration>>  │       │      Payment       │
       │  │       City         │       ├────────────────────┤
       │  ├────────────────────┤       │ - paymentId        │
       │  │   BANGALORE        │       │ - amount           │
       │  │   DELHI            │       │ - status           │
       │  │   MUMBAI           │       │ - method           │
       │  │   CHENNAI          │       │ - transactionId    │
       │  │   HYDERABAD        │       ├────────────────────┤
       │  │   KOLKATA          │       │ + processPayment() │
       │  │   PUNE             │       │ + refund()         │
       └──┴────────────────────┘       └────────────────────┘

                                       ┌────────────────────┐
                                       │   <<Enumeration>>  │
                                       │   PaymentMethod    │
                                       ├────────────────────┤
                                       │   CREDIT_CARD      │
                                       │   DEBIT_CARD       │
                                       │   UPI              │
                                       │   NET_BANKING      │
                                       │   WALLET           │
                                       └────────────────────┘

                                       ┌────────────────────┐
                                       │   <<Enumeration>>  │
                                       │   BookingStatus    │
                                       ├────────────────────┤
                                       │   PENDING          │
                                       │   CONFIRMED        │
                                       │   CANCELLED        │
                                       │   EXPIRED          │
                                       └────────────────────┘

                                       ┌────────────────────┐
                                       │   <<Enumeration>>  │
                                       │   PaymentStatus    │
                                       ├────────────────────┤
                                       │   PENDING          │
                                       │   COMPLETED        │
                                       │   FAILED           │
                                       │   REFUNDED         │
                                       └────────────────────┘
```

## Relationship Summary

```
┌──────────────────────────────────────────────────────────────────────────────────────────┐
│                              RELATIONSHIPS                                                │
├──────────────────────────────────────────────────────────────────────────────────────────┤
│                                                                                          │
│  BookMyShow ─────────────────► MovieController      (1 : 1)  Uses Singleton              │
│  BookMyShow ─────────────────► TheatreController    (1 : 1)  Uses Singleton              │
│  BookMyShow ─────────────────► Booking              (1 : *)  Creates and manages         │
│                                                                                          │
│  MovieController ────────────► Movie                (1 : *)  Manages                     │
│  MovieController ────────────► City                 (M : N)  City-Movie mapping          │
│                                                                                          │
│  TheatreController ──────────► Theatre              (1 : *)  Manages                     │
│  TheatreController ──────────► City                 (M : N)  City-Theatre mapping        │
│                                                                                          │
│  Theatre ────────────────────► Screen               (1 : *)  Composition                 │
│  Theatre ────────────────────► Show                 (1 : *)  Aggregation                 │
│  Theatre ────────────────────► City                 (1 : 1)  Association                 │
│                                                                                          │
│  Screen ─────────────────────► Seat                 (1 : *)  Composition                 │
│                                                                                          │
│  Show ───────────────────────► Movie                (1 : 1)  Association                 │
│  Show ───────────────────────► Screen               (1 : 1)  Association                 │
│  Show ───────────────────────► Seat (booked)        (1 : *)  Track booked seat IDs       │
│                                                                                          │
│  Booking ────────────────────► Show                 (1 : 1)  Association                 │
│  Booking ────────────────────► Seat                 (1 : *)  Association (booked seats)  │
│  Booking ────────────────────► Payment              (1 : 1)  Composition                 │
│                                                                                          │
│  Seat ───────────────────────► SeatCategory         (1 : 1)  Has-a                       │
│                                                                                          │
│  Payment ────────────────────► PaymentStatus        (1 : 1)  Has-a                       │
│  Payment ────────────────────► PaymentMethod        (1 : 1)  Has-a                       │
│                                                                                          │
└──────────────────────────────────────────────────────────────────────────────────────────┘
```

## Data Flow Diagram

```
┌─────────────────────────────────────────────────────────────────────────────────────────┐
│                              BOOKING FLOW                                                │
└─────────────────────────────────────────────────────────────────────────────────────────┘

     ┌─────────┐
     │  User   │
     └────┬────┘
          │
          ▼
┌─────────────────────┐    ┌──────────────────┐    ┌──────────────────┐
│  1. Select City     │───►│  2. View Movies  │───►│  3. Select Movie │
└─────────────────────┘    └──────────────────┘    └──────────────────┘
                                                            │
          ┌─────────────────────────────────────────────────┘
          ▼
┌─────────────────────┐    ┌──────────────────┐    ┌──────────────────┐
│  4. View Shows in   │───►│  5. Select Show  │───►│  6. View Seats   │
│     Theatres        │    │  (Theatre+Time)  │    │  Availability    │
└─────────────────────┘    └──────────────────┘    └──────────────────┘
                                                            │
          ┌─────────────────────────────────────────────────┘
          ▼
┌─────────────────────┐    ┌──────────────────┐    ┌──────────────────┐
│  7. Select Seats    │───►│  8. Enter User   │───►│  9. Make Payment │
│  (Check Category)   │    │  Details         │    │                  │
└─────────────────────┘    └──────────────────┘    └────────┬─────────┘
                                                            │
          ┌─────────────────────────────────────────────────┘
          ▼
┌─────────────────────┐    ┌──────────────────┐
│ 10. Booking Created │───►│ 11. Generate     │
│  (Seats Locked)     │    │     Ticket       │
└─────────────────────┘    └──────────────────┘
```

## Concurrency Handling

```
┌─────────────────────────────────────────────────────────────────────────────────────────┐
│                         THREAD-SAFE SEAT BOOKING                                         │
└─────────────────────────────────────────────────────────────────────────────────────────┘

    User A                              Show                              User B
      │                                  │                                  │
      │  1. Request seats [A1, A2]       │                                  │
      │─────────────────────────────────►│                                  │
      │                                  │   2. Request seats [A2, A3]      │
      │                                  │◄─────────────────────────────────│
      │                                  │                                  │
      │                    ┌─────────────┴─────────────┐                    │
      │                    │    bookingMutex.lock()    │                    │
      │                    │    (One thread gets lock) │                    │
      │                    └─────────────┬─────────────┘                    │
      │                                  │                                  │
      │  3. Lock acquired               │                                  │
      │◄─────────────────────────────────│                                  │
      │                                  │                                  │
      │  4. Seats [A1, A2] booked        │                                  │
      │                                  │                                  │
      │                    ┌─────────────┴─────────────┐                    │
      │                    │   bookingMutex.unlock()   │                    │
      │                    └─────────────┬─────────────┘                    │
      │                                  │                                  │
      │                                  │   5. Lock acquired               │
      │                                  │─────────────────────────────────►│
      │                                  │                                  │
      │                                  │   6. A2 already booked!          │
      │                                  │   Only A3 available - FAIL       │
      │                                  │─────────────────────────────────►│
      │                                  │                                  │
```
