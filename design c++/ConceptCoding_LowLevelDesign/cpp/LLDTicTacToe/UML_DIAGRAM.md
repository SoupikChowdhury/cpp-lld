# Tic Tac Toe Game - UML Diagram

## Class Diagram

```
┌─────────────────────────────────────────────────────────────────────────────────────────────────┐
│                                    TIC TAC TOE GAME UML                                          │
└─────────────────────────────────────────────────────────────────────────────────────────────────┘

                                    ┌─────────────────┐
                                    │  «enumeration»  │
                                    │    PieceType    │
                                    ├─────────────────┤
                                    │ X               │
                                    │ O               │
                                    │ EMPTY           │
                                    └─────────────────┘

┌──────────────────────────────────────────────────────────────────────────────────────────────────┐
│                                       TIC TAC TOE GAME                                            │
├──────────────────────────────────────────────────────────────────────────────────────────────────┤
│ - board: unique_ptr<Board>                                                                        │
│ - players: deque<unique_ptr<Player>>                                                              │
│ - gameOver: bool                                                                                  │
│ - winner: Player*                                                                                 │
│ - moveCount: int                                                                                  │
├──────────────────────────────────────────────────────────────────────────────────────────────────┤
│ + TicTacToeGame(boardSize: int = 3)                                                              │
│ + initializePlayers(player1Name: string, player2Name: string): void                              │
│ + addPlayer(player: unique_ptr<Player>): void                                                    │
│ + makeMove(row: int, col: int): bool                                                             │
│ + playWithMoves(moves: vector<pair<int,int>>): Player*                                           │
│ + isGameOver(): bool                                                                              │
│ + isDraw(): bool                                                                                  │
│ + getWinner(): Player*                                                                            │
│ + getCurrentPlayer(): Player*                                                                     │
│ + reset(): void                                                                                   │
│ - checkWinner(row: int, col: int, piece: PieceType): bool                                        │
│ - isWinningLine(line: vector<PieceType>): bool                                                   │
└──────────────────────────────────────────────────────────────────────────────────────────────────┘
            │                                   │
            │ 1                                 │ 2
            ▼                                   ▼
┌─────────────────────────────┐      ┌─────────────────────────────┐
│           BOARD             │      │           PLAYER            │
├─────────────────────────────┤      ├─────────────────────────────┤
│ - size: int                 │      │ - name: string              │
│ - grid: vector<vector>      │      │ - playingPiece: unique_ptr  │
│ - filledCells: int          │      ├─────────────────────────────┤
├─────────────────────────────┤      │ + getName(): string         │
│ + getSize(): int            │      │ + getPlayingPiece(): Piece* │
│ + isCellEmpty(r,c): bool    │      │ + getPieceType(): PieceType │
│ + placePiece(r,c,p): bool   │      │ + getSymbol(): char         │
│ + getPiece(r,c): PieceType  │      └─────────────────────────────┘
│ + isFull(): bool            │                  │
│ + displayBoard(): void      │                  │ 1
│ + reset(): void             │                  ▼
│ + getRow(r): vector         │      ┌─────────────────────────────┐
│ + getColumn(c): vector      │      │       PLAYING PIECE         │
│ + getMainDiagonal(): vector │      │        «abstract»           │
│ + getAntiDiagonal(): vector │      ├─────────────────────────────┤
└─────────────────────────────┘      │ # pieceType: PieceType      │
                                     ├─────────────────────────────┤
                                     │ + getPieceType(): PieceType │
                                     │ + getSymbol(): char         │
                                     │ + getName(): string         │
                                     └─────────────────────────────┘
                                              ▲          ▲
                                              │          │
                              ┌───────────────┴──┐  ┌────┴───────────────┐
                              │  PLAYING PIECE X │  │  PLAYING PIECE O   │
                              ├──────────────────┤  ├────────────────────┤
                              │ + getSymbol(): 'X'│  │ + getSymbol(): 'O' │
                              │ + getName(): "X"  │  │ + getName(): "O"   │
                              └──────────────────┘  └────────────────────┘
```

## Sequence Diagram - Making a Move

```
┌──────┐  ┌───────────────┐  ┌────────┐  ┌─────────┐
│ main │  │TicTacToeGame  │  │ Board  │  │ Player  │
└──┬───┘  └───────┬───────┘  └───┬────┘  └────┬────┘
   │              │              │            │
   │ makeMove(r,c)│              │            │
   │─────────────>│              │            │
   │              │              │            │
   │              │ getPieceType()            │
   │              │───────────────────────────>│
   │              │<───────────────────────────│
   │              │              │            │
   │              │ placePiece(r,c,piece)     │
   │              │─────────────>│            │
   │              │<─────────────│            │
   │              │ success      │            │
   │              │              │            │
   │              │ displayBoard()            │
   │              │─────────────>│            │
   │              │              │            │
   │              │ checkWinner(r,c,piece)    │
   │              │──────┐       │            │
   │              │      │       │            │
   │              │<─────┘       │            │
   │              │ getRow(r)    │            │
   │              │─────────────>│            │
   │              │<─────────────│            │
   │              │ getColumn(c) │            │
   │              │─────────────>│            │
   │              │<─────────────│            │
   │              │              │            │
   │<─────────────│              │            │
   │ result       │              │            │
```

## Component Diagram

```
┌─────────────────────────────────────────────────────────────────────────────────────┐
│                              TIC TAC TOE COMPONENTS                                  │
└─────────────────────────────────────────────────────────────────────────────────────┘

                    ┌─────────────────────────────────────────┐
                    │              «component»                 │
                    │           TIC TAC TOE GAME               │
                    │  ┌─────────────────────────────────────┐│
                    │  │         Game Controller             ││
                    │  │  - Move validation                  ││
                    │  │  - Win detection                    ││
                    │  │  - Turn management                  ││
                    │  └─────────────────────────────────────┘│
                    └─────────────────────────────────────────┘
                              │                    │
                              ▼                    ▼
               ┌──────────────────────┐  ┌──────────────────────┐
               │    «component»       │  │    «component»       │
               │       BOARD          │  │      PLAYER          │
               │ ┌──────────────────┐ │  │ ┌──────────────────┐ │
               │ │    Grid Data     │ │  │ │   Player Info    │ │
               │ ├──────────────────┤ │  │ ├──────────────────┤ │
               │ │ Win Check Utils  │ │  │ │  Playing Piece   │ │
               │ └──────────────────┘ │  │ └──────────────────┘ │
               └──────────────────────┘  └──────────────────────┘
                                                   │
                                                   ▼
                                        ┌──────────────────────┐
                                        │    «component»       │
                                        │   PLAYING PIECES     │
                                        │ ┌──────────────────┐ │
                                        │ │    Piece X       │ │
                                        │ ├──────────────────┤ │
                                        │ │    Piece O       │ │
                                        │ └──────────────────┘ │
                                        └──────────────────────┘
```

## State Diagram - Game State

```
                    ┌─────────────────┐
                    │                 │
                    │   INITIALIZED   │
                    │                 │
                    └────────┬────────┘
                             │
                    addPlayers()
                             │
                             ▼
                    ┌─────────────────┐
                    │                 │
                    │     READY       │
                    │  (P1's Turn)    │
                    └────────┬────────┘
                             │
                       makeMove()
                             │
         ┌───────────────────┼───────────────────┐
         │                   │                   │
   invalid move         valid move         valid move
   (stay same)          (no win)            (WIN!)
         │                   │                   │
         │                   ▼                   │
         │          ┌─────────────────┐          │
         │          │                 │          │
         └─────────>│  IN PROGRESS    │──────────┤
                    │  (P2's Turn)    │          │
                    └────────┬────────┘          │
                             │                   │
                    ┌────────┴────────┐          │
                    │                 │          │
              more moves         board full      │
                    │                 │          │
                    ▼                 ▼          ▼
           ┌─────────────┐   ┌─────────────┐  ┌─────────────┐
           │  Continue   │   │    DRAW     │  │  GAME WON   │
           │  (P1 Turn)  │   │             │  │             │
           └─────────────┘   └─────────────┘  └─────────────┘
```

## Class Relationships

```
┌──────────────────────────────────────────────────────────────────────────────────┐
│                              RELATIONSHIP DIAGRAM                                 │
└──────────────────────────────────────────────────────────────────────────────────┘

    TicTacToeGame ──────── has-a ────────> Board
           │
           │
           └──────────── has-many ───────> Player
                                              │
                                              │
                                              └──── has-a ────> PlayingPiece
                                                                     │
                                                                     │ «abstract»
                                                                     │
                                                     ┌───────────────┼───────────────┐
                                                     │               │               │
                                              PlayingPieceX    PlayingPieceO    (extensible)
```

## Win Detection Algorithm

```
┌─────────────────────────────────────────────────────────────────────────────────────┐
│                              WIN CHECK ALGORITHM                                     │
└─────────────────────────────────────────────────────────────────────────────────────┘

After placing piece at (row, col):

    ┌───────────────────────────────────────────────────────────────┐
    │                    CHECK ROW                                   │
    │   Get all pieces in row[row]                                  │
    │   If all same and not EMPTY → WIN                             │
    └───────────────────────────────────────────────────────────────┘
                              │
                              ▼
    ┌───────────────────────────────────────────────────────────────┐
    │                   CHECK COLUMN                                 │
    │   Get all pieces in column[col]                               │
    │   If all same and not EMPTY → WIN                             │
    └───────────────────────────────────────────────────────────────┘
                              │
                              ▼
    ┌───────────────────────────────────────────────────────────────┐
    │                CHECK MAIN DIAGONAL                             │
    │   If row == col (piece is on main diagonal):                  │
    │     Get all pieces on main diagonal                           │
    │     If all same and not EMPTY → WIN                           │
    └───────────────────────────────────────────────────────────────┘
                              │
                              ▼
    ┌───────────────────────────────────────────────────────────────┐
    │                CHECK ANTI DIAGONAL                             │
    │   If row + col == size - 1 (piece is on anti-diagonal):       │
    │     Get all pieces on anti-diagonal                           │
    │     If all same and not EMPTY → WIN                           │
    └───────────────────────────────────────────────────────────────┘
                              │
                              ▼
    ┌───────────────────────────────────────────────────────────────┐
    │                   NO WIN FOUND                                 │
    │   Check if board is full → DRAW                               │
    │   Otherwise → Continue game                                   │
    └───────────────────────────────────────────────────────────────┘
```

## Design Patterns Used

| Pattern | Where Used | Purpose |
|---------|------------|---------|
| **Factory** | PlayingPieceX/PlayingPieceO creation | Create different piece types |
| **Strategy** | Win checking algorithm | Flexible win condition checking |
| **State** | Game state (playing, won, draw) | Manage game states |
| **Template Method** | PlayingPiece base class | Define piece interface |
| **Composition** | Game has Board and Players | Build complex objects |

## Board Representation (3x3 Example)

```
    Indices:                    Visual:
    
    ┌───┬───┬───┐              ┌───┬───┬───┐
    │0,0│0,1│0,2│              │ X │ O │   │
    ├───┼───┼───┤              ├───┼───┼───┤
    │1,0│1,1│1,2│              │   │ X │   │
    ├───┼───┼───┤              ├───┼───┼───┤
    │2,0│2,1│2,2│              │ O │   │ X │
    └───┴───┴───┘              └───┴───┴───┘
    
    Main Diagonal: (0,0), (1,1), (2,2)
    Anti-Diagonal: (0,2), (1,1), (2,0)
```
