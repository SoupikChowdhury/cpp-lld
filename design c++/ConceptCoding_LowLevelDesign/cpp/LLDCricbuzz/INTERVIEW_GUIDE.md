# Cricbuzz - LLD Interview Guide

A step-by-step framework for delivering the cricket scoring system low-level design interview.

---

## 📁 Code Reference

| File | Purpose | Key Functions |
|------|---------|---------------|
| [Match.hpp](Match.hpp) | Match entity | `startInning()`, `playBall()`, `startMatch()`, `getWinner()` |
| [MatchType.hpp](MatchType.hpp) | Abstract match type | `setupMatch()`, `getOversPerInnings()`, `getDRSReviewsPerInnings()` |
| [T20MatchType.hpp](T20MatchType.hpp) | T20 rules | `getOversPerInnings()` returns 20 |
| [OneDayMatchType.hpp](OneDayMatchType.hpp) | ODI rules | `getOversPerInnings()` returns 50 |
| [Team/Team.hpp](Team/Team.hpp) | Team entity | `addPlayer()`, `getPlayer()`, `getPlayers()` |
| [Team/Player.hpp](Team/Player.hpp) | Player entity | `getName()`, `getPlayerDetails()` |
| [Team/PlayerDetails.hpp](Team/PlayerDetails.hpp) | Player info | `getBattingScoreCard()`, `getBowlingScoreCard()` |
| [Team/Wicket.hpp](Team/Wicket.hpp) | Wicket details | `getDismissalType()`, `getBowler()` |
| [Team/Score/BattingScoreCard.hpp](Team/Score/BattingScoreCard.hpp) | Batting stats | `addRuns()`, `getRuns()`, `getBalls()` |
| [Team/Score/BowlingScoreCard.hpp](Team/Score/BowlingScoreCard.hpp) | Bowling stats | `addWicket()`, `addRuns()`, `getWickets()` |
| [Inning/InningDetails.hpp](Inning/InningDetails.hpp) | Inning management | `recordBall()`, `getScore()`, `handleWicket()` |
| [Inning/OverDetails.hpp](Inning/OverDetails.hpp) | Over details | `addBall()`, `getBalls()`, `isComplete()` |
| [Inning/BallDetails.hpp](Inning/BallDetails.hpp) | Ball-by-ball | `getRuns()`, `getBallType()`, `getWicket()` |
| [Inning/BallType.hpp](Inning/BallType.hpp) | Ball type enum | `NORMAL`, `WIDE`, `NO_BALL`, `LEG_BYE` |
| [Inning/RunType.hpp](Inning/RunType.hpp) | Run types | `SINGLE`, `DOUBLE`, `TRIPLE`, `FOUR`, `SIX` |
| [ScoreUpdater/ScoreUpdaterObserver.hpp](ScoreUpdater/ScoreUpdaterObserver.hpp) | Observer interface | `update()` |
| [ScoreUpdater/BattingScoreUpdater.hpp](ScoreUpdater/BattingScoreUpdater.hpp) | Batting observer | `update()` - updates batsman stats |
| [ScoreUpdater/BowlingScoreUpdater.hpp](ScoreUpdater/BowlingScoreUpdater.hpp) | Bowling observer | `update()` - updates bowler stats |

---

## 🎯 Design Patterns Used

| Pattern | Implementation Location | Purpose |
|---------|------------------------|----------|
| **Observer Pattern** | [ScoreUpdater/](ScoreUpdater/) - ScoreUpdaterObserver notifies batting/bowling updates | Real-time score updates |
| **Template Method Pattern** | [MatchType.hpp](MatchType.hpp) with hooks for T20/ODI rules | Different match formats |
| **Strategy Pattern** | T20MatchType, OneDayMatchType | Different match type rules |

---

## 1) Requirements (~5 minutes)

### Clarifying Questions to Ask
- "What match formats should we support? T20, ODI, Test?"
- "Is this live scoring or post-match entry?"
- "Should we track ball-by-ball data?"
- "What statistics need to be calculated? Batting, bowling averages?"
- "Should multiple observers receive score updates in real-time?"

### Requirements

```
Requirements:
1. Support multiple match types (T20, ODI, Test).
2. Track teams with their players.
3. Record ball-by-ball scoring with run types and wickets.
4. Maintain batting and bowling scorecards.
5. Update multiple components when a ball is played (batting stats, bowling stats, innings total).
6. Calculate match results based on format-specific rules.
7. Handle overs, extras, and different dismissal types.

Out of Scope:
- User authentication and accounts
- Live streaming integration
- Historical statistics aggregation
- DLS calculations for rain-affected matches
- Player auction/team management
- Commentary and highlights
```

---

## 2) Entities and Relationships (~3 minutes)

### Identify Entities

Scan the requirements and pull out meaningful nouns:

- **Match** - The game being played
- **Team** - Collection of players
- **Player** - Individual with score cards
- **InningDetails** - One team's batting turn
- **OverDetails** - 6 balls bowled
- **BallDetails** - Single delivery with outcome
- **BattingScoreCard** - Batsman's performance
- **BowlingScoreCard** - Bowler's performance
- **MatchType** - T20, ODI rules (Template Method)
- **ScoreUpdaterObserver** - Components that update on ball events

### Define Relationships

```
Entities:
- Match
- Team
- Player
- InningDetails
- OverDetails
- BallDetails
- BattingScoreCard
- BowlingScoreCard
- MatchType (abstract: T20MatchType, OneDayMatchType)
- ScoreUpdaterObserver (abstract: BattingScoreUpdater, BowlingScoreUpdater)

Relationships:
- Match -> Team (1:2)
- Match -> InningDetails (1:2 or 1:4 for Test)
- Match -> MatchType (1:1)
- Team -> Player (1:N)
- Player -> BattingScoreCard (1:1 per innings)
- Player -> BowlingScoreCard (1:1 per innings)
- InningDetails -> OverDetails (1:N)
- OverDetails -> BallDetails (1:6)
- BallDetails notifies ScoreUpdaterObservers
```

---

## 3) Class Design (~10-15 minutes)

### Match (Orchestrator)

| Requirement | State Needed |
|-------------|--------------|
| Teams playing | team1, team2 |
| Match format | matchType |
| Both innings | innings: vector |
| Match outcome | result |

```
class Match:
  - matchId: string
  - team1: Team
  - team2: Team
  - matchType: MatchType
  - innings: vector<InningDetails>
  - venue: string
  - matchDate: date
  - result: string

  + startMatch()
  + startInnings(battingTeam, bowlingTeam)
  + getCurrentInnings() -> InningDetails
  + getResult() -> string
```

### InningDetails

| Requirement | State Needed |
|-------------|--------------|
| Track overs | overs: vector<OverDetails> |
| Track runs and wickets | totalRuns, totalWickets |
| Current batsmen | striker, nonStriker |
| Target (if chasing) | targetScore |

```
class InningDetails:
  - inningsNumber: int
  - battingTeam: Team
  - bowlingTeam: Team
  - overs: vector<OverDetails>
  - totalRuns: int
  - totalWickets: int
  - striker: Player
  - nonStriker: Player
  - currentBowler: Player
  - maxOvers: int
  - targetScore: int?

  + startOver(bowler: Player)
  + playBall(ballDetails: BallDetails)
  + getTotalRuns() -> int
  + getTotalWickets() -> int
  + getCurrentOver() -> OverDetails
  + isInningsComplete() -> bool
```

### BallDetails

```
class BallDetails:
  - ballNumber: int
  - bowler: Player
  - striker: Player
  - nonStriker: Player
  - ballType: BallType (NORMAL, WIDE, NO_BALL, WICKET)
  - runsScored: int
  - wicket: Wicket?

  + getRunsScored() -> int
  + isWicket() -> bool
  + getBallType() -> BallType
```

### BattingScoreCard

```
class BattingScoreCard:
  - player: Player
  - runs: int
  - balls: int
  - fours: int
  - sixes: int
  - isOut: bool
  - dismissalType: DismissalType?
  - dismissedBy: Player?

  + addRuns(runs: int)
  + incrementBalls()
  + addBoundary(runs: int)
  + setOut(dismissalType, dismissedBy)
  + getStrikeRate() -> double
```

### BowlingScoreCard

```
class BowlingScoreCard:
  - player: Player
  - overs: int
  - ballsInCurrentOver: int
  - maidens: int
  - runsGiven: int
  - wickets: int
  - wides: int
  - noBalls: int

  + addBall()
  + addRuns(runs: int)
  + addWicket()
  + addExtra(extraType)
  + getEconomyRate() -> double
```

### MatchType (Abstract - Template Method Pattern)

```
class MatchType:
  # Template method
  + setupMatch()
      initializeRules()
      setupFieldRestrictions()
      setupReviewSystem()

  + initializeRules()  // Abstract
  + setupFieldRestrictions()  // Abstract
  + getMaxOvers() -> int  // Abstract

class T20MatchType : MatchType
  + getMaxOvers() -> 20
  + initializeRules()  // T20-specific rules

class OneDayMatchType : MatchType
  + getMaxOvers() -> 50
  + initializeRules()  // ODI-specific rules
```

### ScoreUpdaterObserver (Abstract - Observer Pattern)

```
class ScoreUpdaterObserver:
  + update(ballDetails: BallDetails)

class BattingScoreUpdater : ScoreUpdaterObserver
  + update(ballDetails)
      // Update striker's batting card

class BowlingScoreUpdater : ScoreUpdaterObserver
  + update(ballDetails)
      // Update bowler's bowling card

class InningsScoreUpdater : ScoreUpdaterObserver
  + update(ballDetails)
      // Update innings total
```

---

## 4) Implementation (~10 minutes)

### Key Method: InningDetails.playBall()

```
playBall(ballDetails: BallDetails) -> void
    // Add ball to current over
    currentOver = overs.back()
    currentOver.addBall(ballDetails)
    
    // Notify all observers
    for observer in scoreUpdaters:
        observer.update(ballDetails)
    
    // Handle wicket
    if ballDetails.isWicket()
        totalWickets++
        striker.getBattingCard().setOut(ballDetails.getWicket())
        // Need new batsman
    
    // Add runs
    totalRuns += ballDetails.getRunsScored()
    
    // Rotate strike for odd runs
    if ballDetails.getRunsScored() % 2 == 1
        swapStrike()
    
    // Check over complete (ignore wides/no-balls)
    if currentOver.isComplete() and not isInningsComplete()
        startNewOver()
        swapStrike()  // Change of ends
```

### Key Method: BattingScoreUpdater.update()

```
update(ballDetails: BallDetails) -> void
    striker = ballDetails.getStriker()
    battingCard = striker.getBattingScoreCard()
    
    // Increment balls faced (not for wides)
    if ballDetails.getBallType() != WIDE
        battingCard.incrementBalls()
    
    // Add runs
    runs = ballDetails.getRunsScored()
    battingCard.addRuns(runs)
    
    // Track boundaries
    if runs == 4
        battingCard.incrementFours()
    else if runs == 6
        battingCard.incrementSixes()
```

### Key Method: BowlingScoreUpdater.update()

```
update(ballDetails: BallDetails) -> void
    bowler = ballDetails.getBowler()
    bowlingCard = bowler.getBowlingScoreCard()
    
    // Add legal ball
    if ballDetails.getBallType() == NORMAL or ballDetails.isWicket()
        bowlingCard.addBall()
    
    // Add extras
    if ballDetails.getBallType() == WIDE
        bowlingCard.addWide()
    else if ballDetails.getBallType() == NO_BALL
        bowlingCard.addNoBall()
    
    // Add runs conceded
    bowlingCard.addRuns(ballDetails.getRunsScored())
    
    // Add wicket (if dismissal credited to bowler)
    if ballDetails.isWicket() and isBowlerWicket(ballDetails.getWicket())
        bowlingCard.addWicket()
```

### Key Method: Match.startMatch()

```
startMatch() -> void
    // Setup match based on type
    matchType.setupMatch()
    
    // Toss (simplified)
    battingFirst = toss()
    bowlingFirst = (battingFirst == team1) ? team2 : team1
    
    // Start first innings
    innings1 = new InningDetails(
        inningsNumber = 1,
        battingTeam = battingFirst,
        bowlingTeam = bowlingFirst,
        maxOvers = matchType.getMaxOvers()
    )
    
    innings.add(innings1)
```

### Verification: Walk Through a Scenario

```
Setup: T20 Match - Team A vs Team B, Team A batting first

1. startMatch() → T20 rules applied, maxOvers = 20

2. startInnings(Team A, Team B)
   - striker = Player1 (Team A)
   - nonStriker = Player2 (Team A)
   - currentBowler = Bowler1 (Team B)

3. playBall(BallDetails: NORMAL, runsScored=4)
   - Observers notified:
     - BattingScoreUpdater: Player1 balls=1, runs=4, fours=1
     - BowlingScoreUpdater: Bowler1 balls=1, runs=4
     - InningsScoreUpdater: totalRuns=4
   - No strike rotation (4 runs = even)

4. playBall(BallDetails: NORMAL, runsScored=1)
   - Player1 balls=2, runs=5
   - Bowler1 balls=2, runs=5
   - totalRuns=5
   - Strike rotated: striker=Player2, nonStriker=Player1

5. playBall(BallDetails: WICKET, runsScored=0, dismissal=BOWLED)
   - Player2 OUT BOWLED by Bowler1
   - Bowler1 wickets=1
   - totalWickets=1
   - New batsman needed

6. After 20 overs: Team A = 165/7
7. startInnings(Team B, Team A, targetScore=166)
   - Team B chases 166 to win
```

---

## 5) Extensibility (~5 minutes)

### "How would you handle live score updates to thousands of users?"

> "The Observer pattern is already in place. Add a LiveScorePublisher observer that publishes to a message queue (Kafka). Client apps subscribe to the queue via WebSockets for real-time updates."

```
class LiveScorePublisher : ScoreUpdaterObserver
  + update(ballDetails)
      jsonUpdate = serializeToJSON(ballDetails)
      messageQueue.publish("match-updates", jsonUpdate)
```

### "How would you add DLS calculations for rain-affected matches?"

> "Create a DLSCalculator class that takes first innings score, overs played, overs lost to rain, and wickets to calculate revised target. Integrate with InningDetails when rain interruption is recorded."

```
class DLSCalculator:
  + calculateRevisedTarget(firstInningsScore, oversPlayed, oversLost, wickets) -> int
      // DLS resource table lookup
      // Calculate proportional target
```

### "How would you implement super over for tied matches?"

> "Create a SuperOver class that creates mini-innings with 1 over each. Reuse existing InningDetails with maxOvers=1. If super over also ties, use boundary count as tiebreaker."

### "How would you add player career statistics?"

> "Create a PlayerCareerStats class that aggregates data across matches. After each match completion, update career stats. Use a separate database table for efficient querying."

---

## Design Patterns Summary

| Pattern | Where | Why |
|---------|-------|-----|
| **Observer** | ScoreUpdaterObserver | Notify multiple components on ball events |
| **Template Method** | MatchType | Common setup with format-specific rules |
| **Composition** | Match → Innings → Overs → Balls | Build complex structure from simple parts |
| **Factory** | Create scorecards per innings | Centralized initialization |

---

## Ball Event Flow

```
Ball Played
     ↓
 BallDetails created (runs, ballType, wicket)
     ↓
 Notify Observers:
     ├── BattingScoreUpdater → Update striker's card
     ├── BowlingScoreUpdater → Update bowler's card
     └── InningsScoreUpdater → Update innings total
     ↓
 Check innings complete (all out / overs done / target reached)
     ↓
 If complete → Start next innings or End match
```
