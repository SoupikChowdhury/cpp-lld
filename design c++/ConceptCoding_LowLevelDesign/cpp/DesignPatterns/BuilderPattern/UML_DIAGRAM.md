# Builder Pattern - UML Class Diagram

```
┌─────────────────────────────────────────────────────────────────────┐
│                          Builder Pattern                             │
└─────────────────────────────────────────────────────────────────────┘

┌──────────────────────────┐          ┌──────────────────────────┐
│        Director          │          │        Student           │
│                          │          │       (Product)          │
├──────────────────────────┤          ├──────────────────────────┤
│ - builder: StudentBuilder│          │ - rollNumber: int        │
├──────────────────────────┤          │ - age: int               │
│ + createStudent()        │          │ - name: string           │
│ - createEngineeringStudent│         │ - fatherName: string     │
│ - createMBAStudent()     │          │ - motherName: string     │
└──────────────────────────┘          │ - subjects: vector       │
            │                         ├──────────────────────────┤
            │ uses                    │ + toString()             │
            ▼                         └──────────────────────────┘
┌──────────────────────────┐                      ▲
│    StudentBuilder        │                      │ creates
│    (Abstract Builder)    │──────────────────────┘
├──────────────────────────┤
│ # rollNumber: int        │
│ # age: int               │
│ # name: string           │
│ # fatherName: string     │
│ # motherName: string     │
│ # subjects: vector       │
├──────────────────────────┤
│ + setRollNumber()        │
│ + setAge()               │
│ + setName()              │
│ + setFatherName()        │
│ + setMotherName()        │
│ + setSubjects() {abstract}│
│ + build(): Student       │
└──────────────────────────┘
            ▲
            │ extends
      ┌─────┴─────┐
      │           │
┌─────┴──────┐  ┌─┴────────────┐
│Engineering │  │    MBA       │
│StudentBldr │  │ StudentBldr  │
├────────────┤  ├──────────────┤
│+ setSubjects()│+ setSubjects()│
│{DSA,OS,...}│  │{Econ,Biz,...}│
└────────────┘  └──────────────┘
```

## Sequence Diagram

```
Client         Director         Builder              Student
  │                │                │                    │
  │ createStudent()│                │                    │
  │───────────────►│                │                    │
  │                │ setRollNumber()│                    │
  │                │───────────────►│                    │
  │                │ setAge()       │                    │
  │                │───────────────►│                    │
  │                │ setName()      │                    │
  │                │───────────────►│                    │
  │                │ setSubjects()  │                    │
  │                │───────────────►│                    │
  │                │ build()        │                    │
  │                │───────────────►│    create          │
  │                │                │───────────────────►│
  │  Student       │                │                    │
  │◄───────────────│                │                    │
```

## Key Components
- **Director**: Knows the construction steps and order
- **Builder (Abstract)**: Declares construction steps
- **Concrete Builders**: Implement steps for specific variants
- **Product**: The complex object being built
