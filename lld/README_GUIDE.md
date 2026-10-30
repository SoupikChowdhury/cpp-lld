# README Guide

How every question's `README.md` in `lld/` is written, and which ones are done.
Goal: reading one README end to end should be enough to walk into an interview and deliver that question.

## The interview framework

Every README follows the same five steps, in this order:

```
1. Requirements  ->  2. Entities  ->  3. Class Design  ->  4. Implementation  ->  5. Extensibility
```

| Step | Rough time in a 45 min interview | What you leave on the whiteboard |
|---|---|---|
| 1. Requirements | 5 min | Numbered final requirements + "out of scope" list |
| 2. Entities | 3-5 min | Entity / responsibility table, relationships |
| 3. Class Design | 10 min | State + methods for each class, key decisions |
| 4. Implementation | 15-20 min | Core methods with edge cases, then a dry run |
| 5. Extensibility | 5 min (more for senior) | How the design absorbs 2-3 "what if" questions |

## Section template

Copy this skeleton for each README. Headings stay the same across questions so they are easy to compare.

```markdown
# <Question> - Low Level Design (C++)

<one-paragraph summary + "How to use this README" table (step, time, section link)>

## 0. Understanding the problem
What the system is, in 3-4 sentences. The one-line prompt the interviewer gives.

## 1. Requirements
### Clarifying questions        <- You / Interviewer dialogue, each answer followed by why it mattered
### Final requirements          <- numbered list + out of scope, as you'd write it on the board

## 2. Entities and relationships
Walk the nouns: which become classes, which stay enums/data, and why.
Entity | Responsibility table. Small relationship diagram.

## 3. Class design
Per class: "requirement -> what it must track" table, then methods, then the C++ declaration.
Design decisions as Good / Great (no "Bad" options), with the one we chose and why.
Final class design block + design patterns table (pattern, where, why, one line to say).

## 4. Implementation
Per key method: core logic steps, edge cases, then the real code from this folder.
C++ talking points (ownership, smart pointers, atomics, const).
Gaps in our code: what an interviewer could catch, and the fix to say out loud.

## 5. Verification
Dry run of real scenarios with state after each step. Use output from actually running main.cpp.

## 6. Extensibility
The common follow-ups. For each: what to say (a quote you can reuse), and a code sketch.

## 7. What is expected at each level
Junior / Mid / Senior.

## 8. Test yourself
8-12 questions with answers hidden in <details>.

## 9. Run it
Compile command, expected output, file map.
```

## Rules for writing one

1. **Describe our C++ code, not the reference page.** Read every file in the folder first. Class names, fields, method signatures and numbers in the README must match the code.
2. **Run it before writing.** Compile `main.cpp`, run it, and test edge cases (full, invalid input, double operations) with a small scratch program. The Verification section and the "gaps" list come from those runs, not from reading.
   - Compiler: GCC 16 from WinLibs (`...\WinGet\Packages\BrechtSanders.WinLibs.POSIX.UCRT_...\mingw64\bin`). Put that folder first on `PATH` when compiling **and** running; otherwise the program picks up the old `C:\MinGW` runtime and exits with code 127.
   - Also check every `cpp` excerpt against its source file line by line, and list README identifiers that don't exist in the code to confirm each one is deliberate.
3. **Code excerpts are copied exactly** from the source files (trimming with `// ...` is fine). No rewritten "cleaner" versions presented as ours.
4. **Always include "Gaps in our code".** If the code rounds wrong, races, leaks or dangles, say so with the one-line fix. The code itself is left as is unless asked; the README is where the interview answer lives.
   - Exception: if `main.cpp` itself hangs or crashes, ask the user before writing. If they want it fixed, make the smallest fix, re-run every demo, and add a short "The bug this code used to have" section (as in `elevator`).
5. **Write in our own words.** The reference pages are for structure and ideas; don't paste their text.
6. **Design decisions show Good and Great only.** Leave out "Bad" options (user preference). The exported PDFs keep only the box headings, so the reasoning has to be written here.
7. **Formatting for GitHub:**
   - Design decisions go in a `> [!NOTE]` box; the "Say in the interview" line goes in its own `> [!TIP]` box right after (blank line between them).
   - Real C++ goes in ` ```cpp ` fences; pseudo-code and class outlines go in ` ```text ` so they aren't colored as C++.
   - `mermaid` blocks for diagrams, `<details><summary>` for quiz answers.
   - Check before committing: `gh api markdown -f mode=gfm -F text=@README.md` and confirm no raw `[!NOTE]` or `|---|` is left in the HTML.

## Reference material

Exported reference pages live outside the repo in `C:\Users\user\Downloads\hello interview`:

| Reference PDF | Our folder |
|---|---|
| Parking Lot | `parking_lot` |
| Elevator | `elevator` |
| Rate Limiter | `rate_limiter` |
| Amazon Locker | none yet |
| Connect Four | none yet |

To read them as text: `pdftotext -layout "<file>.pdf" out.txt` (`pdftotext` ships with Git for Windows).

## Progress

Status: `done` = rewritten in this format, `todo` = not yet.

| Folder | Current docs | Status |
|---|---|---|
| parking_lot | README.md | done |
| atm | README.md | todo |
| car_rental | README.md | todo |
| customer_support | README.md | todo |
| doctors_appointment | README.md | todo |
| hashmap | README.md | todo |
| logger | README.md | done |
| rate_limiter | README.md | done |
| snakes_and_ladder | README.md | done |
| splitwise | README.md | done |
| elevator | README.md | done |
| book_my_show | INTERVIEW_GUIDE.md, UML_DIAGRAM.md | todo |
| cricbuzz | INTERVIEW_GUIDE.md, UML_DIAGRAM.md | todo |
| order_management_system | INTERVIEW_GUIDE.md, UML_DIAGRAM.md | todo |
| tic_tac_toe | INTERVIEW_GUIDE.md, UML_DIAGRAM.md | todo |
| vending_machine | INTERVIEW_GUIDE.md, UML_DIAGRAM.md | todo |
| null_object | INTERVIEW_GUIDE.md | todo |
| ConceptCoding_atm | INTERVIEW_GUIDE.md, UML_DIAGRAM.md | todo |
| ConceptCoding_car_rental | INTERVIEW_GUIDE.md, UML_DIAGRAM.md | todo |
| ConceptCoding_hashmap | INTERVIEW_GUIDE.md | todo |
| ConceptCoding_snakes_and_ladder | INTERVIEW_GUIDE.md, UML_DIAGRAM.md | todo |
| ConceptCoding_splitwise | INTERVIEW_GUIDE.md, UML_DIAGRAM.md | todo |
| design_patterns | per-pattern INTERVIEW_GUIDE.md | todo |
| smart_pointers | none | todo |
| string | none | todo |

### Plan for folders that have `INTERVIEW_GUIDE.md`

Those guides follow the five steps but describe an idealized design (methods that don't exist, pseudocode only, no gaps, levels or self-test), so they are reworked, using them as raw material:

| Group | Folders | Plan |
|---|---|---|
| Interview questions | elevator, book_my_show, tic_tac_toe, vending_machine, order_management_system, cricbuzz | Full rework into `README.md` in this format. |
| ConceptCoding duplicates | ConceptCoding_atm, _car_rental, _hashmap, _snakes_and_ladder, _splitwise | Lighter `README.md`: how this design differs from the main folder's, its own gaps, how to run it. The main folder's README teaches the question. |
| Pattern explainers | design_patterns/*, null_object | Lightest: check against the code, fix wrong names, add a short gaps list and self-test. |

Once a folder's `README.md` is written, delete its old `INTERVIEW_GUIDE.md` and `UML_DIAGRAM.md` in the same commit (user decision, made on `elevator`), so a folder never has two documents that disagree.
