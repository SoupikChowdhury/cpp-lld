// Snakes and Ladder Game - Main Entry Point
// Demonstrates Factory, Template Method, and Composition patterns

#include <iostream>
#include "model/Player.hpp"
#include "service/GameService.hpp"

int main() {
    std::cout << "=== Snakes and Ladder Game Demo ===" << std::endl;
    std::cout << std::endl;

    // Create game with 10x10 board (100 cells) and 1 dice
    GameService game(10, 1);

    // Add snakes (head -> tail, going DOWN)
    std::cout << "--- Setting up Snakes ---" << std::endl;
    game.addSnake(99, 54);
    game.addSnake(70, 55);
    game.addSnake(52, 42);
    game.addSnake(25, 2);
    game.addSnake(95, 72);

    // Add ladders (bottom -> top, going UP)
    std::cout << "\n--- Setting up Ladders ---" << std::endl;
    game.addLadder(6, 25);
    game.addLadder(11, 40);
    game.addLadder(60, 85);
    game.addLadder(46, 90);
    game.addLadder(17, 69);

    // Display the board
    game.displayBoard();

    // Add players
    std::cout << "\n--- Adding Players ---" << std::endl;
    Player player1("Alice");
    Player player2("Bob");
    Player player3("Charlie");

    game.addPlayer(&player1);
    game.addPlayer(&player2);
    game.addPlayer(&player3);

    std::cout << "Players: Alice, Bob, Charlie" << std::endl;

    // Start the game
    game.startGame();

    std::cout << "\n=== Game Over ===" << std::endl;

    return 0;
}
