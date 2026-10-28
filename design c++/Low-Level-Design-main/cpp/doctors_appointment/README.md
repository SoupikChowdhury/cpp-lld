# Doctor Appointment System - Low Level Design (C++)

## Design Patterns Used
- **Strategy Pattern**: For slot ranking/prioritization strategies
- **Factory Pattern**: For creating appointments
- **Repository Pattern**: For managing doctors, patients, and appointments
- **Observer Pattern**: For waitlist notifications (optional)

## Class Diagram

```mermaid
classDiagram
    class Specialty {
        <<enumeration>>
        GENERAL
        CARDIOLOGY
        DERMATOLOGY
        ORTHOPEDICS
        PEDIATRICS
        NEUROLOGY
    }

    class AppointmentStatus {
        <<enumeration>>
        AVAILABLE
        BOOKED
        CANCELLED
        COMPLETED
    }

    class Doctor {
        -string doctorId
        -string name
        -Specialty specialty
        -double rating
        -double consultationFee
        +getDoctorId() string
        +getName() string
        +getSpecialty() Specialty
        +getRating() double
        +getConsultationFee() double
    }

    class Patient {
        -string patientId
        -string name
        -string email
        -string phone
        +getPatientId() string
        +getName() string
    }

    class TimeSlot {
        -time_t startTime
        -time_t endTime
        -int durationMinutes
        +getStartTime() time_t
        +getEndTime() time_t
        +overlaps(TimeSlot) bool
    }

    class Appointment {
        -string appointmentId
        -Doctor* doctor
        -Patient* patient
        -TimeSlot slot
        -AppointmentStatus status
        +book(Patient*)
        +cancel()
        +complete()
        +isAvailable() bool
    }

    class WaitlistEntry {
        -string waitlistId
        -Patient* patient
        -Doctor* doctor
        -time_t requestedTime
        +getPatient() Patient*
        +getDoctor() Doctor*
    }

    class SlotRankingStrategy {
        <<interface>>
        +rankSlots(slots) vector~Appointment*~
    }

    class EarliestSlotStrategy {
        +rankSlots(slots) vector~Appointment*~
    }

    class HighestRatedDoctorStrategy {
        +rankSlots(slots) vector~Appointment*~
    }

    class CheapestSlotStrategy {
        +rankSlots(slots) vector~Appointment*~
    }

    SlotRankingStrategy <|.. EarliestSlotStrategy
    SlotRankingStrategy <|.. HighestRatedDoctorStrategy
    SlotRankingStrategy <|.. CheapestSlotStrategy

    class DoctorRepository {
        -map~string,Doctor*~ doctors
        +addDoctor(Doctor*)
        +getDoctor(doctorId) Doctor*
        +getDoctorsBySpecialty(Specialty) vector
    }

    class AppointmentRepository {
        -map~string,Appointment*~ appointments
        +createAppointment(doctor, slot) Appointment*
        +getAppointment(appointmentId) Appointment*
        +getAvailableSlots(doctorId) vector
    }

    class AppointmentService {
        -DoctorRepository doctorRepo
        -AppointmentRepository appointmentRepo
        -SlotRankingStrategy* rankingStrategy
        -vector~WaitlistEntry*~ waitlist
        +bookAppointment(patient, doctorId, slotId) Appointment*
        +cancelAppointment(appointmentId)
        +searchAvailableSlots(specialty) vector
        +addToWaitlist(patient, doctorId)
        +setRankingStrategy(strategy)
    }

    AppointmentService --> DoctorRepository
    AppointmentService --> AppointmentRepository
    AppointmentService --> SlotRankingStrategy
    AppointmentService --> WaitlistEntry
    AppointmentRepository --> Appointment
    DoctorRepository --> Doctor
    Appointment --> Doctor
    Appointment --> Patient
    Appointment --> TimeSlot
    WaitlistEntry --> Patient
    WaitlistEntry --> Doctor
```

## Appointment Booking Flow

```mermaid
sequenceDiagram
    participant P as Patient
    participant AS as AppointmentService
    participant DR as DoctorRepository
    participant AR as AppointmentRepository
    participant RS as RankingStrategy
    participant A as Appointment

    P->>AS: searchAvailableSlots(specialty)
    AS->>DR: getDoctorsBySpecialty(specialty)
    DR-->>AS: List<Doctor>
    AS->>AR: getAvailableSlots(doctorIds)
    AR-->>AS: List<Appointment>
    AS->>RS: rankSlots(appointments)
    RS-->>AS: rankedSlots
    AS-->>P: Available Slots

    P->>AS: bookAppointment(patientId, appointmentId)
    AS->>AR: getAppointment(appointmentId)
    AR-->>AS: Appointment
    alt Slot Available
        AS->>A: book(patient)
        A->>A: setStatus(BOOKED)
        AS-->>P: Booking Confirmed
    else Slot Taken
        AS-->>P: Slot Unavailable
    end
```

## Cancellation and Waitlist Processing

```mermaid
flowchart TD
    A[Cancel Appointment] --> B[Mark CANCELLED]
    B --> C[Mark Slot AVAILABLE]
    C --> D{Waitlist for Doctor?}
    D -->|Yes| E[Get First Waitlist Entry]
    E --> F[Notify Patient]
    F --> G[Auto-book if Configured]
    G --> H[Remove from Waitlist]
    D -->|No| I[Slot Available for Others]
```

## Slot Ranking Strategies

```mermaid
flowchart TD
    A[Available Slots] --> B{Which Strategy?}
    
    B -->|Earliest| C[EarliestSlotStrategy]
    C --> D[Sort by startTime ascending]
    
    B -->|Highest Rated| E[HighestRatedDoctorStrategy]
    E --> F[Sort by doctor.rating descending]
    
    B -->|Cheapest| G[CheapestSlotStrategy]
    G --> H[Sort by consultationFee ascending]
```

## Appointment State Diagram

```mermaid
stateDiagram-v2
    [*] --> AVAILABLE : Create Slot
    AVAILABLE --> BOOKED : book()
    BOOKED --> COMPLETED : complete()
    BOOKED --> CANCELLED : cancel()
    CANCELLED --> AVAILABLE : Waitlist auto-book
    CANCELLED --> [*] : No waitlist
    COMPLETED --> [*]
```

## Time Slot Management

```mermaid
flowchart LR
    subgraph Doctor Schedule
        S1[9:00-9:30] --> S2[9:30-10:00]
        S2 --> S3[10:00-10:30]
        S3 --> S4[10:30-11:00]
    end
    
    S1 -.->|BOOKED| P1[Patient A]
    S2 -.->|AVAILABLE| Available
    S3 -.->|BOOKED| P2[Patient B]
    S4 -.->|AVAILABLE| Available
```

## Files Structure
```
doctors_appointment/
├── enums/
│   ├── Specialty.hpp
│   └── AppointmentStatus.hpp
├── model/
│   ├── Doctor.hpp
│   ├── Patient.hpp
│   ├── TimeSlot.hpp
│   ├── Appointment.hpp
│   └── WaitlistEntry.hpp
├── strategy/
│   ├── SlotRankingStrategy.hpp
│   ├── EarliestSlotStrategy.hpp
│   ├── HighestRatedDoctorStrategy.hpp
│   └── CheapestSlotStrategy.hpp
├── repository/
│   ├── DoctorRepository.hpp
│   └── AppointmentRepository.hpp
├── service/
│   └── AppointmentService.hpp
├── README.md
└── main.cpp
```
