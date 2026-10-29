/*
 * LLD Tic Tac Toe Game
 * 
 * This implementation demonstrates a complete Tic Tac Toe game with:
 * - Configurable board size (default 3x3)
 * - Two players with X and O pieces
 * - Win detection for rows, columns, and diagonals
 * - Draw detection
 * 
 * Design Patterns Used:
 * - Factory: PlayingPiece creation
 * - Strategy: Win checking algorithm
 * - State: Game state management
 */

#include <iostream>
#include <memory>
#include <vector>
#include "TicTacToeGame.hpp"

using namespace TicTacToe;

void playGame1_WinByRow() {
    std::cout << "\n╔════════════════════════════════════════╗" << std::endl;
    std::cout << "║    GAME 1: Player 1 Wins (Row)         ║" << std::endl;
    std::cout << "╚════════════════════════════════════════╝" << std::endl;

    TicTacToeGame game(3);
    game.initializePlayers("Alice", "Bob");

    // Sequence of moves
    std::vector<std::pair<int, int>> moves = {
        {0, 0},  // Alice X
        {1, 0},  // Bob O
        {0, 1},  // Alice X
        {1, 1},  // Bob O
        {0, 2}   // Alice X - Wins (top row)
    };

    game.playWithMoves(moves);
}

void playGame2_WinByColumn() {
    std::cout << "\n╔════════════════════════════════════════╗" << std::endl;
    std::cout << "║    GAME 2: Player 2 Wins (Column)      ║" << std::endl;
    std::cout << "╚════════════════════════════════════════╝" << std::endl;

    TicTacToeGame game(3);
    game.initializePlayers("Charlie", "Diana");

    std::vector<std::pair<int, int>> moves = {
        {0, 0},  // Charlie X
        {0, 2},  // Diana O
        {1, 0},  // Charlie X
        {1, 2},  // Diana O
        {0, 1},  // Charlie X
        {2, 2}   // Diana O - Wins (right column)
    };

    game.playWithMoves(moves);
}

void playGame3_WinByDiagonal() {
    std::cout << "\n╔════════════════════════════════════════╗" << std::endl;
    std::cout << "║   GAME 3: Win by Diagonal              ║" << std::endl;
    std::cout << "╚════════════════════════════════════════╝" << std::endl;

    TicTacToeGame game(3);
    game.initializePlayers("Eve", "Frank");

    std::vector<std::pair<int, int>> moves = {
        {0, 0},  // Eve X
        {0, 1},  // Frank O
        {1, 1},  // Eve X
        {0, 2},  // Frank O
        {2, 2}   // Eve X - Wins (main diagonal)
    };

    game.playWithMoves(moves);
}

void playGame4_Draw() {
    std::cout << "\n╔════════════════════════════════════════╗" << std::endl;
    std::cout << "║   GAME 4: Draw Game                    ║" << std::endl;
    std::cout << "╚════════════════════════════════════════╝" << std::endl;

    TicTacToeGame game(3);
    game.initializePlayers("George", "Hannah");

    // A draw sequence
    std::vector<std::pair<int, int>> moves = {
        {1, 1},  // George X (center)
        {0, 0},  // Hannah O
        {0, 1},  // George X
        {2, 1},  // Hannah O
        {0, 2},  // George X
        {0, 0},  // Hannah O - Invalid (already occupied)
        {1, 0},  // Hannah O
        {1, 2},  // George X
        {2, 0},  // Hannah O
        {2, 2}   // George X - Draw!
    };

    game.playWithMoves(moves);
}

void playGame5_LargerBoard() {
    std::cout << "\n╔════════════════════════════════════════╗" << std::endl;
    std::cout << "║   GAME 5: 4x4 Board                    ║" << std::endl;
    std::cout << "╚════════════════════════════════════════╝" << std::endl;

    TicTacToeGame game(4);  // 4x4 board
    game.initializePlayers("Ivan", "Julia");

    std::vector<std::pair<int, int>> moves = {
        {0, 0},  // Ivan X
        {1, 0},  // Julia O
        {0, 1},  // Ivan X
        {1, 1},  // Julia O
        {0, 2},  // Ivan X
        {1, 2},  // Julia O
        {0, 3}   // Ivan X - Wins (top row of 4)
    };

    game.playWithMoves(moves);
}

void playGame6_AntiDiagonalWin() {
    std::cout << "\n╔════════════════════════════════════════╗" << std::endl;
    std::cout << "║   GAME 6: Anti-Diagonal Win            ║" << std::endl;
    std::cout << "╚════════════════════════════════════════╝" << std::endl;

    TicTacToeGame game(3);
    game.initializePlayers("Kevin", "Laura");

    std::vector<std::pair<int, int>> moves = {
        {0, 2},  // Kevin X
        {0, 0},  // Laura O
        {1, 1},  // Kevin X
        {0, 1},  // Laura O
        {2, 0}   // Kevin X - Wins (anti-diagonal)
    };

    game.playWithMoves(moves);
}

void demonstrateInvalidMoves() {
    std::cout << "\n╔════════════════════════════════════════╗" << std::endl;
    std::cout << "║   DEMO: Invalid Move Handling          ║" << std::endl;
    std::cout << "╚════════════════════════════════════════╝" << std::endl;

    TicTacToeGame game(3);
    game.initializePlayers("Mike", "Nancy");

    game.getBoard()->displayBoard();

    // Valid move
    std::cout << "--- Valid move ---" << std::endl;
    game.makeMove(1, 1);

    // Invalid: Same position
    std::cout << "\n--- Invalid: Same position ---" << std::endl;
    game.makeMove(1, 1);

    // Invalid: Out of bounds
    std::cout << "\n--- Invalid: Out of bounds ---" << std::endl;
    game.makeMove(5, 5);

    // Valid move
    std::cout << "\n--- Valid move ---" << std::endl;
    game.makeMove(0, 0);
}

int main() {
    try {
        std::cout << "╔════════════════════════════════════════╗" << std::endl;
        std::cout << "║                                        ║" << std::endl;
        std::cout << "║          ❌ TIC TAC TOE ⭕            ║" << std::endl;
        std::cout << "║                                        ║" << std::endl;
        std::cout << "╚════════════════════════════════════════╝" << std::endl;

        // Run all demo games
        playGame1_WinByRow();
        playGame2_WinByColumn();
        playGame3_WinByDiagonal();
        playGame6_AntiDiagonalWin();
        playGame4_Draw();
        playGame5_LargerBoard();
        demonstrateInvalidMoves();

        std::cout << "\n╔════════════════════════════════════════╗" << std::endl;
        std::cout << "║    ALL DEMONSTRATIONS COMPLETED!       ║" << std::endl;
        std::cout << "╚════════════════════════════════════════╝" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
