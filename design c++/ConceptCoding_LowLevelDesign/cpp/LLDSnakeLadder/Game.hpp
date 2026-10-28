#pragma once

#include <vector>
#include <memory>
#include <queue>
#include <iostream>
#include <string>
#include "Board.hpp"
#include "Player.hpp"
#include "Dice.hpp"

namespace SnakeLadderGame {

class Game {
private:
    std::unique_ptr<Board> board;
    std::unique_ptr<Dice> dice;
    std::queue<std::shared_ptr<Player>> players;
    std::shared_ptr<Player> winner;
    bool gameOver;
    int turnCount;

public:
    Game(int boardSize, int numSnakes, int numLadders, int numDice = 1)
        : board(std::make_unique<Board>(boardSize, numSnakes, numLadders)),
          dice(std::make_unique<Dice>(numDice)),
          winner(nullptr),
          gameOver(false),
          turnCount(0) {}

    // Constructor with predefined snakes and ladders
    Game(int boardSize,
         const std::vector<std::pair<int, int>>& snakes,
         const std::vector<std::pair<int, int>>& ladders,
         int numDice = 1)
        : board(std::make_unique<Board>(boardSize, snakes, ladders)),
          dice(std::make_unique<Dice>(numDice)),
          winner(nullptr),
          gameOver(false),
          turnCount(0) {}

    void addPlayer(const std::string& id, const std::string& name) {
        auto player = std::make_shared<Player>(id, name);
        players.push(player);
        std::cout << "Player added: " << name << std::endl;
    }

    void addPlayer(std::shared_ptr<Player> player) {
        players.push(player);
        std::cout << "Player added: " << player->getName() << std::endl;
    }

    bool isGameOver() const { return gameOver; }
    std::shared_ptr<Player> getWinner() const { return winner; }
    int getTurnCount() const { return turnCount; }

    // Play one turn
    std::shared_ptr<Player> playTurn() {
        if (gameOver || players.empty()) {
            return nullptr;
        }

        // Get current player
        auto currentPlayer = players.front();
        players.pop();

        turnCount++;
        int currentPos = currentPlayer->getCurrentPosition();
        int diceValue = dice->rollDice();

        std::cout << "\n--- Turn " << turnCount << " ---" << std::endl;
        std::cout << currentPlayer->getName() << " (at position " << currentPos 
                  << ") rolled: " << diceValue << std::endl;

        int newPos = currentPos + diceValue;

        // Check if move is valid
        if (newPos > board->getSize()) {
            std::cout << "Cannot move! Need exact roll to reach " << board->getSize() << std::endl;
            players.push(currentPlayer);
            return currentPlayer;
        }

        // Check for snake or ladder
        int finalPos = board->getFinalPosition(newPos);
        if (finalPos != newPos) {
            Cell* cell = board->getCell(newPos);
            if (cell && cell->hasJump()) {
                Jump* jump = cell->getJump();
                if (jump->isSnake()) {
                    std::cout << "OOPS! Snake bite at " << newPos 
                              << "! Sliding down to " << finalPos << std::endl;
                } else {
                    std::cout << "YAY! Ladder at " << newPos 
                              << "! Climbing up to " << finalPos << std::endl;
                }
            }
        }

        currentPlayer->moveTo(finalPos);
        std::cout << currentPlayer->getName() << " moved to position " << finalPos << std::endl;

        // Check for win
        if (board->isWinningPosition(finalPos)) {
            winner = currentPlayer;
            gameOver = true;
            std::cout << "\n🎉 " << currentPlayer->getName() << " WINS! 🎉" << std::endl;
            return currentPlayer;
        }

        // Add player back to queue for next turn
        players.push(currentPlayer);
        return currentPlayer;
    }

    // Play entire game
    std::shared_ptr<Player> play() {
        std::cout << "\n========================================" << std::endl;
        std::cout << "    SNAKE AND LADDER GAME STARTED!" << std::endl;
        std::cout << "========================================" << std::endl;

        board->displayBoard();

        std::cout << "Number of players: " << players.size() << std::endl;
        
        while (!gameOver) {
            playTurn();
        }

        std::cout << "\n========================================" << std::endl;
        std::cout << "    GAME OVER! Total turns: " << turnCount << std::endl;
        std::cout << "    Winner: " << winner->getName() << std::endl;
        std::cout << "========================================" << std::endl;

        return winner;
    }

    // Play with max turns limit (for testing)
    std::shared_ptr<Player> playWithLimit(int maxTurns) {
        std::cout << "\n========================================" << std::endl;
        std::cout << "    SNAKE AND LADDER GAME STARTED!" << std::endl;
        std::cout << "========================================" << std::endl;

        board->displayBoard();

        int turn = 0;
        while (!gameOver && turn < maxTurns) {
            playTurn();
            turn++;
        }

        if (!gameOver) {
            std::cout << "\n--- Game ended due to turn limit (" << maxTurns << ") ---" << std::endl;
            displayPlayerPositions();
        }

        return winner;
    }

    void displayPlayerPositions() const {
        std::cout << "\n=== Current Positions ===" << std::endl;
        auto tempQueue = players;
        while (!tempQueue.empty()) {
            auto player = tempQueue.front();
            tempQueue.pop();
            std::cout << player->getName() << ": Position " << player->getCurrentPosition() << std::endl;
        }
    }

    Board* getBoard() const { return board.get(); }
    Dice* getDice() const { return dice.get(); }
};

} // namespace SnakeLadderGame
