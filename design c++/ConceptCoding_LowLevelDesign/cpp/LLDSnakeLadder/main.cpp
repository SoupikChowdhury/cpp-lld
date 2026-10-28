/*
 * LLD Snake and Ladder Game
 * 
 * This implementation demonstrates a complete Snake and Ladder game with:
 * - Configurable board size
 * - Random or predefined snakes and ladders
 * - Multiple players
 * - Dice rolling mechanism
 * 
 * Design Patterns Used:
 * - Composition: Board contains Cells, Cells contain Jumps
 * - Factory: Can be extended for different board configurations
 * - Strategy: Can be extended for different dice strategies
 */

#include <iostream>
#include <memory>
#include <vector>
#include "Game.hpp"

using namespace SnakeLadderGame;

void playRandomGame() {
    std::cout << "\n╔════════════════════════════════════════╗" << std::endl;
    std::cout << "║     RANDOM SNAKE AND LADDER GAME       ║" << std::endl;
    std::cout << "╚════════════════════════════════════════╝" << std::endl;

    // Create game with random snakes and ladders
    // Board size: 100, 5 snakes, 5 ladders, 1 dice
    Game game(100, 5, 5, 1);

    // Add players
    game.addPlayer("P1", "Alice");
    game.addPlayer("P2", "Bob");
    game.addPlayer("P3", "Charlie");

    // Play the game (with turn limit to prevent infinite games)
    game.playWithLimit(200);
}

void playPredefinedGame() {
    std::cout << "\n╔════════════════════════════════════════╗" << std::endl;
    std::cout << "║   PREDEFINED SNAKE AND LADDER GAME     ║" << std::endl;
    std::cout << "╚════════════════════════════════════════╝" << std::endl;

    // Define snakes (start, end) - start > end
    std::vector<std::pair<int, int>> snakes = {
        {99, 54},   // Snake from 99 to 54
        {70, 55},   // Snake from 70 to 55
        {52, 42},   // Snake from 52 to 42
        {25, 2},    // Snake from 25 to 2
        {95, 72}    // Snake from 95 to 72
    };

    // Define ladders (start, end) - start < end
    std::vector<std::pair<int, int>> ladders = {
        {6, 25},    // Ladder from 6 to 25
        {11, 40},   // Ladder from 11 to 40
        {60, 85},   // Ladder from 60 to 85
        {46, 90},   // Ladder from 46 to 90
        {17, 69}    // Ladder from 17 to 69
    };

    // Create game with predefined snakes and ladders
    Game game(100, snakes, ladders, 1);

    // Add players
    game.addPlayer("P1", "Player 1");
    game.addPlayer("P2", "Player 2");

    // Play the game
    game.playWithLimit(150);
}

void playSmallBoardGame() {
    std::cout << "\n╔════════════════════════════════════════╗" << std::endl;
    std::cout << "║      SMALL BOARD DEMO (Size: 30)       ║" << std::endl;
    std::cout << "╚════════════════════════════════════════╝" << std::endl;

    // Snakes and ladders for small board
    std::vector<std::pair<int, int>> snakes = {
        {27, 5},
        {21, 9},
        {17, 4}
    };

    std::vector<std::pair<int, int>> ladders = {
        {3, 22},
        {8, 26},
        {11, 28}
    };

    Game game(30, snakes, ladders, 1);

    game.addPlayer("P1", "Quick");
    game.addPlayer("P2", "Slow");

    // Play full game on small board
    game.play();
}

void demonstrateTwoDice() {
    std::cout << "\n╔════════════════════════════════════════╗" << std::endl;
    std::cout << "║       TWO DICE GAME DEMO               ║" << std::endl;
    std::cout << "╚════════════════════════════════════════╝" << std::endl;

    // Create game with 2 dice (range 2-12 per roll)
    Game game(100, 4, 4, 2);

    game.addPlayer("P1", "DiceRoller1");
    game.addPlayer("P2", "DiceRoller2");

    // Play with limit
    game.playWithLimit(50);
}

int main() {
    try {
        std::cout << "╔════════════════════════════════════════╗" << std::endl;
        std::cout << "║                                        ║" << std::endl;
        std::cout << "║     🐍 SNAKE AND LADDER GAME 🪜       ║" << std::endl;
        std::cout << "║                                        ║" << std::endl;
        std::cout << "╚════════════════════════════════════════╝" << std::endl;

        // Run different game modes
        playSmallBoardGame();
        
        std::cout << "\n\n";
        playPredefinedGame();
        
        std::cout << "\n\n";
        demonstrateTwoDice();

        std::cout << "\n╔════════════════════════════════════════╗" << std::endl;
        std::cout << "║     ALL DEMONSTRATIONS COMPLETED!      ║" << std::endl;
        std::cout << "╚════════════════════════════════════════╝" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
