#pragma once

#include <vector>
#include <memory>
#include <iostream>
#include <random>
#include <chrono>
#include <set>
#include "Cell.hpp"
#include "Jump.hpp"

namespace SnakeLadderGame {

class Board {
private:
    int size;
    std::vector<std::unique_ptr<Cell>> cells;
    int numSnakes;
    int numLadders;

public:
    Board(int size, int snakes, int ladders)
        : size(size), numSnakes(snakes), numLadders(ladders) {
        
        // Initialize all cells (1 to size)
        // Index 0 is unused, cells go from 1 to size
        cells.resize(size + 1);
        for (int i = 0; i <= size; ++i) {
            cells[i] = std::make_unique<Cell>(i);
        }

        // Add snakes and ladders
        initializeSnakesAndLadders();
    }

    void initializeSnakesAndLadders() {
        std::mt19937 rng(static_cast<unsigned>(
            std::chrono::system_clock::now().time_since_epoch().count()
        ));

        std::set<int> usedPositions;
        usedPositions.insert(0);  // Don't use position 0
        usedPositions.insert(1);  // Don't start with jump at 1
        usedPositions.insert(size);  // Don't put anything at final position

        // Add Ladders (start < end)
        int laddersAdded = 0;
        while (laddersAdded < numLadders) {
            std::uniform_int_distribution<int> startDist(2, size - 2);
            int start = startDist(rng);
            
            if (usedPositions.count(start)) continue;

            std::uniform_int_distribution<int> endDist(start + 1, size - 1);
            int end = endDist(rng);
            
            if (usedPositions.count(end)) continue;

            usedPositions.insert(start);
            usedPositions.insert(end);
            cells[start]->setJump(std::make_unique<Jump>(start, end));
            laddersAdded++;

            std::cout << "Added LADDER: " << start << " -> " << end << std::endl;
        }

        // Add Snakes (start > end)
        int snakesAdded = 0;
        while (snakesAdded < numSnakes) {
            std::uniform_int_distribution<int> startDist(2, size - 1);
            int start = startDist(rng);
            
            if (usedPositions.count(start)) continue;

            std::uniform_int_distribution<int> endDist(1, start - 1);
            int end = endDist(rng);
            
            if (usedPositions.count(end)) continue;

            usedPositions.insert(start);
            cells[start]->setJump(std::make_unique<Jump>(start, end));
            snakesAdded++;

            std::cout << "Added SNAKE: " << start << " -> " << end << std::endl;
        }
    }

    // Constructor with predefined snakes and ladders
    Board(int size, 
          const std::vector<std::pair<int, int>>& snakes,
          const std::vector<std::pair<int, int>>& ladders)
        : size(size), numSnakes(static_cast<int>(snakes.size())), 
          numLadders(static_cast<int>(ladders.size())) {
        
        // Initialize all cells
        cells.resize(size + 1);
        for (int i = 0; i <= size; ++i) {
            cells[i] = std::make_unique<Cell>(i);
        }

        // Add snakes
        for (const auto& [start, end] : snakes) {
            if (start > 1 && start < size && end >= 1 && end < start) {
                cells[start]->setJump(std::make_unique<Jump>(start, end));
                std::cout << "Added SNAKE: " << start << " -> " << end << std::endl;
            }
        }

        // Add ladders
        for (const auto& [start, end] : ladders) {
            if (start > 1 && start < size && end > start && end <= size) {
                cells[start]->setJump(std::make_unique<Jump>(start, end));
                std::cout << "Added LADDER: " << start << " -> " << end << std::endl;
            }
        }
    }

    int getSize() const { return size; }
    int getNumSnakes() const { return numSnakes; }
    int getNumLadders() const { return numLadders; }

    Cell* getCell(int position) const {
        if (position >= 0 && position <= size) {
            return cells[position].get();
        }
        return nullptr;
    }

    // Get final position after applying any jumps
    int getFinalPosition(int position) const {
        if (position > size || position < 0) {
            return position;
        }
        Cell* cell = getCell(position);
        return cell ? cell->getFinalPosition() : position;
    }

    bool isWinningPosition(int position) const {
        return position == size;
    }

    void displayBoard() const {
        std::cout << "\n=== BOARD (Size: " << size << ") ===" << std::endl;
        std::cout << "Snakes: " << numSnakes << ", Ladders: " << numLadders << std::endl;
        
        for (int i = 1; i <= size; ++i) {
            Cell* cell = getCell(i);
            if (cell && cell->hasJump()) {
                Jump* jump = cell->getJump();
                std::cout << "[" << i << "] " << jump->getTypeName() 
                          << " -> " << jump->getEndPosition() << std::endl;
            }
        }
        std::cout << "=========================\n" << std::endl;
    }
};

} // namespace SnakeLadderGame
