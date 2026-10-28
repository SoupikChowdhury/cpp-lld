#ifndef GAME_SERVICE_HPP
#define GAME_SERVICE_HPP

#include <iostream>
#include <queue>
#include <vector>
#include "../model/Board.hpp"
#include "../model/Dice.hpp"
#include "../model/Player.hpp"
#include "../factory/ObstacleFactory.hpp"

class GameService {
private:
    Board* board;
    Dice* dice;
    std::queue<Player*> players;
    std::vector<Obstacle*> obstacles;  // For cleanup

    int calculateNewPosition(int current, int roll) {
        int newPos = current + roll;
        if (newPos > board->getSize()) {
            return current;  // Can't move beyond the board
        }
        return newPos;
    }

    void playTurn(Player* player) {
        int diceRoll = dice->roll();
        int currentPos = player->getPosition();
        int newPos = calculateNewPosition(currentPos, diceRoll);

        std::cout << player->getName() << " rolled " << diceRoll 
                  << " and moved from " << currentPos << " to " << newPos;

        Cell* cell = board->getCell(newPos);
        if (cell && cell->hasObstacle()) {
            Obstacle* obs = cell->getObstacle();
            int finalPos = obs->getDestination();
            
            if (obs->getType() == ObstacleType::SNAKE) {
                std::cout << " -> SNAKE! Sliding down to " << finalPos;
            } else {
                std::cout << " -> LADDER! Climbing up to " << finalPos;
            }
            newPos = finalPos;
        }

        player->setPosition(newPos);
        std::cout << std::endl;
    }

public:
    GameService(int boardDimension, int numDice = 1) {
        board = new Board(boardDimension);
        dice = new Dice(numDice);
    }

    ~GameService() {
        delete board;
        delete dice;
        for (auto* obs : obstacles) {
            delete obs;
        }
    }

    void addPlayer(Player* player) {
        player->setPosition(0);  // Start before the board
        players.push(player);
    }

    void addSnake(int head, int tail) {
        auto* snake = ObstacleFactory::create(ObstacleType::SNAKE, head, tail);
        obstacles.push_back(snake);
        board->placeObstacle(snake);
        std::cout << "Added Snake: " << head << " -> " << tail << std::endl;
    }

    void addLadder(int bottom, int top) {
        auto* ladder = ObstacleFactory::create(ObstacleType::LADDER, bottom, top);
        obstacles.push_back(ladder);
        board->placeObstacle(ladder);
        std::cout << "Added Ladder: " << bottom << " -> " << top << std::endl;
    }

    void displayBoard() {
        board->display();
    }

    void startGame() {
        std::cout << "\n=== Game Started! ===" << std::endl;
        std::cout << "First to reach " << board->getSize() << " wins!\n" << std::endl;

        while (!players.empty()) {
            Player* currentPlayer = players.front();
            players.pop();

            playTurn(currentPlayer);

            if (currentPlayer->getPosition() == board->getSize()) {
                std::cout << "\n🎉 " << currentPlayer->getName() 
                          << " WINS! 🎉" << std::endl;
                return;
            }

            players.push(currentPlayer);  // Add back to queue for next turn
        }
    }
};

#endif // GAME_SERVICE_HPP
