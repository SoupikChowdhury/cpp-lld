# Cricbuzz - Cricket Scoring System UML Diagram

## Class Diagram

```
┌─────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
│                                        CRICBUZZ - CLASS DIAGRAM                                              │
└─────────────────────────────────────────────────────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
│                                              PLAYER DOMAIN                                                    │
└─────────────────────────────────────────────────────────────────────────────────────────────────────────────┘

┌─────────────────────┐          ┌─────────────────────┐          ┌─────────────────────┐
│      Person         │          │    PlayerType       │          │   PlayerDetails     │
├─────────────────────┤          ├─────────────────────┤          ├─────────────────────┤
│ # name: string      │          │ <<enumeration>>     │          │ - jerseyNumber: str │
│ # age: int          │          │ BATSMAN             │          │ - battingStyle: str │
│ # country: string   │          │ BOWLER              │          │ - bowlingStyle: str │
├─────────────────────┤          │ ALL_ROUNDER         │          │ - totalMatches: int │
│ + getName()         │          │ WICKET_KEEPER       │          │ - totalRuns: int    │
│ + toString()        │          └─────────────────────┘          │ - totalWickets: int │
└─────────────────────┘                                           └─────────────────────┘
         △                                    │                              │
         │                                    │                              │
┌─────────────────────────────────────────────┴──────────────────────────────┴────────────────────────────────┐
│                                               Player                                                          │
├─────────────────────────────────────────────────────────────────────────────────────────────────────────────┤
│ - playerId: string                                                                                            │
│ - playerType: PlayerType                                                                                      │
│ - details: PlayerDetails                                                                                      │
│ - battingScoreCard: unique_ptr<BattingScoreCard>                                                             │
│ - bowlingScoreCard: unique_ptr<BowlingScoreCard>                                                             │
├─────────────────────────────────────────────────────────────────────────────────────────────────────────────┤
│ + getPlayerId(), getPlayerType(), getDetails()                                                               │
│ + getBattingScoreCard(), getBowlingScoreCard()                                                               │
│ + resetScoreCards()                                                                                          │
│ + getScoreSummary(): string                                                                                  │
└─────────────────────────────────────────────────────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
│                                             SCORE CARDS                                                       │
└─────────────────────────────────────────────────────────────────────────────────────────────────────────────┘

┌───────────────────────────────┐               ┌───────────────────────────────┐
│      BattingScoreCard         │               │      BowlingScoreCard         │
├───────────────────────────────┤               ├───────────────────────────────┤
│ - runsScored: int             │               │ - oversBowled: int            │
│ - ballsFaced: int             │               │ - ballsBowled: int            │
│ - fours: int                  │               │ - runsConceded: int           │
│ - sixes: int                  │               │ - wicketsTaken: int           │
│ - isOut: bool                 │               │ - maidens: int                │
│ - dismissalType: string       │               │ - noBalls: int                │
│ - bowlerName: string          │               │ - wides: int                  │
├───────────────────────────────┤               ├───────────────────────────────┤
│ + addRuns(runs: int)          │               │ + addRuns(runs: int)          │
│ + incrementBallsFaced()       │               │ + incrementBalls()            │
│ + setOut(type, bowler)        │               │ + addWicket()                 │
│ + getStrikeRate(): double     │               │ + addNoBall(), addWide()      │
│ + toString(): string          │               │ + getEconomyRate(): double    │
└───────────────────────────────┘               │ + getOversString(): string    │
                                                └───────────────────────────────┘

┌─────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
│                                               TEAM & WICKET                                                   │
└─────────────────────────────────────────────────────────────────────────────────────────────────────────────┘

┌───────────────────────────────┐               ┌───────────────────────────────┐
│           Team                │               │         WicketType            │
├───────────────────────────────┤               ├───────────────────────────────┤
│ - teamId: string              │               │ <<enumeration>>               │
│ - name: string                │               │ BOWLED                        │
│ - country: string             │               │ CAUGHT                        │
│ - players: vector<Player*>    │               │ LBW                           │
│ - captain: Player*            │               │ RUN_OUT                       │
│ - wicketKeeper: Player*       │               │ STUMPED                       │
├───────────────────────────────┤               │ CAUGHT_AND_BOWLED             │
│ + addPlayer(player)           │               │ HIT_WICKET                    │
│ + setCaptain(captain)         │               └───────────────────────────────┘
│ + getPlayer(index): Player*   │                              │
│ + getPlayerById(id): Player*  │               ┌──────────────┴────────────────┐
│ + resetAllScoreCards()        │               │           Wicket              │
│ + getScoreBoard(): string     │               ├───────────────────────────────┤
│ + getBowlingFigures(): string │               │ - wicketType: WicketType      │
└───────────────────────────────┘               │ - batsmanOut: Player*         │
                                                │ - bowler: Player*             │
                                                │ - fielder: Player*            │
                                                │ - overNumber: int             │
                                                │ - teamScore: int              │
                                                └───────────────────────────────┘

┌─────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
│                                            INNING DETAILS                                                     │
└─────────────────────────────────────────────────────────────────────────────────────────────────────────────┘

┌─────────────────────┐     ┌─────────────────────┐     ┌─────────────────────┐     ┌─────────────────────┐
│     BallType        │     │      RunType        │     │    BallDetails      │     │    OverDetails      │
├─────────────────────┤     ├─────────────────────┤     ├─────────────────────┤     ├─────────────────────┤
│ <<enumeration>>     │     │ <<enumeration>>     │     │ - ballNumber: int   │     │ - overNumber: int   │
│ NORMAL              │     │ ZERO                │     │ - ballType: BallType│     │ - bowler: Player*   │
│ WIDE                │     │ ONE, TWO, THREE     │     │ - runsScored: int   │     │ - balls: vector     │
│ NO_BALL             │     │ FOUR                │     │ - runType: RunType  │     │ - runsInOver: int   │
│ DEAD_BALL           │     │ SIX                 │     │ - bowler: Player*   │     │ - wicketsInOver: int│
│ FREE_HIT            │     │ BYE, LEG_BYE        │     │ - striker: Player*  │     │ - isMaiden: bool    │
└─────────────────────┘     │ WIDE, NO_BALL       │     │ - wicket: Wicket*   │     ├─────────────────────┤
                            └─────────────────────┘     │ - isLegalDelivery   │     │ + addBall(ball)     │
                                                        ├─────────────────────┤     │ + getOverSummary()  │
                                                        │ + isWicket(): bool  │     │ + getLegalBallCount │
                                                        │ + toString()        │     └─────────────────────┘
                                                        └─────────────────────┘
                                                                 │
                                                                 ▼
┌─────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
│                                          InningDetails                                                        │
├─────────────────────────────────────────────────────────────────────────────────────────────────────────────┤
│ - inningNumber: int              - totalRuns: int                - striker: Player*                          │
│ - battingTeam: Team*             - totalWickets: int             - nonStriker: Player*                       │
│ - bowlingTeam: Team*             - totalExtras: int              - currentBowler: Player*                    │
│ - overs: vector<OverDetails>     - wides, noBalls, byes, legByes - currentOver: OverDetails*                 │
│ - wickets: vector<Wicket>        - maxOvers: int                 - targetScore: int                          │
├─────────────────────────────────────────────────────────────────────────────────────────────────────────────┤
│ + addRuns(runs, runType, isExtra)    + getRunRate(): double        + getOversString(): string                │
│ + recordWicket(wicket)               + getRequiredRunRate(): double + getScoreSummary(): string              │
│ + completeOver()                     + rotateStrike()               + getFullScoreBoard(): string            │
└─────────────────────────────────────────────────────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
│                                    OBSERVER PATTERN - SCORE UPDATERS                                          │
└─────────────────────────────────────────────────────────────────────────────────────────────────────────────┘

                              ┌───────────────────────────────────────┐
                              │    ScoreUpdaterObserver               │
                              │          <<interface>>                │
                              ├───────────────────────────────────────┤
                              │ + update(ballDetails: BallDetails*)   │
                              │ + getName(): string                   │
                              └───────────────────────────────────────┘
                                               △
                                               │
                        ┌──────────────────────┴──────────────────────┐
                        │                                             │
          ┌─────────────────────────────┐           ┌─────────────────────────────┐
          │    BattingScoreUpdater      │           │    BowlingScoreUpdater      │
          ├─────────────────────────────┤           ├─────────────────────────────┤
          │ + update(ballDetails)       │           │ + update(ballDetails)       │
          │   - Updates batsman's runs  │           │   - Updates bowler's overs  │
          │   - Updates balls faced     │           │   - Updates runs conceded   │
          │   - Records dismissals      │           │   - Updates wickets taken   │
          │ + getName(): string         │           │   - Tracks extras           │
          └─────────────────────────────┘           │ + getName(): string         │
                                                    └─────────────────────────────┘

┌─────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
│                                    TEMPLATE PATTERN - MATCH TYPES                                             │
└─────────────────────────────────────────────────────────────────────────────────────────────────────────────┘

                              ┌───────────────────────────────────────┐
                              │           MatchType                   │
                              │          <<abstract>>                 │
                              ├───────────────────────────────────────┤
                              │ # oversPerInning: int                 │
                              │ # powerPlayOvers: int                 │
                              │ # matchTypeName: string               │
                              ├───────────────────────────────────────┤
                              │ + setupMatch()    // Template Method  │
                              │ + initializeRules()        <<abstract>>│
                              │ + setupFieldRestrictions() <<abstract>>│
                              │ + setupReviewSystem()      <<abstract>>│
                              │ + getFieldersOutsideCirclePowerPlay() │
                              │ + getDRSReviewsPerInnings()           │
                              │ + getDescription(): string            │
                              └───────────────────────────────────────┘
                                               △
                                               │
                        ┌──────────────────────┴──────────────────────┐
                        │                                             │
          ┌─────────────────────────────┐           ┌─────────────────────────────┐
          │       T20MatchType          │           │      OneDayMatchType        │
          ├─────────────────────────────┤           ├─────────────────────────────┤
          │ - hasStrategicTimeout: bool │           │ - drinkBreakInterval: int   │
          │ - oversPerInning = 20       │           │ - oversPerInning = 50       │
          │ - powerPlayOvers = 6        │           │ - powerPlayOvers = 10       │
          ├─────────────────────────────┤           ├─────────────────────────────┤
          │ + initializeRules()         │           │ + initializeRules()         │
          │ + setupFieldRestrictions()  │           │ + setupFieldRestrictions()  │
          │ + setupReviewSystem()       │           │ + setupReviewSystem()       │
          │ + getMaxOversPerBowler() = 4│           │ + getMaxOversPerBowler() = 10│
          └─────────────────────────────┘           │ + isDeathOvers(over): bool  │
                                                    └─────────────────────────────┘

┌─────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
│                                               MATCH                                                           │
└─────────────────────────────────────────────────────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
│                                               Match                                                           │
├─────────────────────────────────────────────────────────────────────────────────────────────────────────────┤
│ - matchId: string                              - innings: vector<InningDetails*>                              │
│ - team1: Team*                                 - currentInning: InningDetails*                               │
│ - team2: Team*                                 - tossWinner: Team*                                           │
│ - matchType: unique_ptr<MatchType>            - tossDecision: string                                         │
│ - venue: string                                - result: MatchResult                                          │
│ - matchDate: string                            - scoreObservers: vector<ScoreUpdaterObserver*>               │
├─────────────────────────────────────────────────────────────────────────────────────────────────────────────┤
│ + registerObserver(observer)                   + startInning(battingTeam, bowlingTeam)                       │
│ + removeObserver(observer)                     + startOver(bowler)                                           │
│ + notifyObservers(ballDetails)                + playBall(ballType, runs, runType, wicket)                    │
│ + conductToss(winner, decision)               + endInning()                                                   │
│ + determineResult()                            + toString(): string                                           │
└─────────────────────────────────────────────────────────────────────────────────────────────────────────────┘
```

## Sequence Diagram - Ball Delivery Flow

```
┌────────┐   ┌───────┐   ┌────────────┐   ┌──────────────────┐   ┌─────────────────┐   ┌─────────────────┐
│ Main   │   │ Match │   │InningDetails│   │BattingScoreUpdater│   │BowlingScoreUpdater│   │  Player        │
└───┬────┘   └───┬───┘   └─────┬──────┘   └────────┬─────────┘   └────────┬────────┘   └───────┬─────────┘
    │            │             │                   │                      │                    │
    │ playBall() │             │                   │                      │                    │
    │───────────>│             │                   │                      │                    │
    │            │ create      │                   │                      │                    │
    │            │ BallDetails │                   │                      │                    │
    │            │─────────────│                   │                      │                    │
    │            │             │                   │                      │                    │
    │            │ notifyObservers(ballDetails)    │                      │                    │
    │            │─────────────────────────────────>                      │                    │
    │            │             │                   │                      │                    │
    │            │             │  update(ballDetails)                     │                    │
    │            │             │   │               │                      │                    │
    │            │             │   │  getBattingScoreCard()               │                    │
    │            │             │   │───────────────────────────────────────────────────────────>
    │            │             │   │               │                      │                    │
    │            │             │   │  addRuns()    │                      │                    │
    │            │             │   │───────────────────────────────────────────────────────────>
    │            │             │   │               │                      │                    │
    │            │─────────────────────────────────────────────────────────>                   │
    │            │             │                   │     update(ballDetails)                   │
    │            │             │                   │                      │                    │
    │            │             │                   │  getBowlingScoreCard()                    │
    │            │             │                   │───────────────────────────────────────────>
    │            │             │                   │                      │                    │
    │            │             │                   │     addRuns(), incrementBalls()           │
    │            │             │                   │───────────────────────────────────────────>
    │            │             │                   │                      │                    │
    │            │ addRuns()   │                   │                      │                    │
    │            │────────────>│                   │                      │                    │
    │            │             │                   │                      │                    │
    │            │ check over complete             │                      │                    │
    │            │────────────>│                   │                      │                    │
    │            │             │                   │                      │                    │
    │   result   │             │                   │                      │                    │
    │<───────────│             │                   │                      │                    │
```

## Design Patterns Used

| Pattern | Implementation | Purpose |
|---------|---------------|---------|
| **Observer** | `ScoreUpdaterObserver`, `BattingScoreUpdater`, `BowlingScoreUpdater` | Notify multiple score updaters when a ball is played |
| **Template Method** | `MatchType`, `T20MatchType`, `OneDayMatchType` | Define match setup algorithm with customizable steps |
| **Composition** | `Player` contains `BattingScoreCard` and `BowlingScoreCard` | Compose player with score tracking capabilities |

## Key Relationships

1. **Match** has two **Teams** and a **MatchType**
2. **Team** contains multiple **Players**
3. **Player** extends **Person** and has score cards
4. **Match** maintains a list of **InningDetails**
5. **InningDetails** contains multiple **OverDetails**
6. **OverDetails** contains multiple **BallDetails**
7. **Match** notifies **ScoreUpdaterObserver** instances on each ball
