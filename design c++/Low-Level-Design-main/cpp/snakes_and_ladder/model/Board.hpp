#ifndef BOARD_HPP
#define BOARD_HPP

#include <vector>
#include <iostream>
#include <iomanip>
#include "Cell.hpp"
#include "Obstacle.hpp"

class Board {
private:
    int size;
    std::vector<Cell*> cells;  // Flattened 1D array for positions 1 to size*size

public:
    explicit Board(int dimensions) : size(dimensions * dimensions) {
        // Create cells for positions 1 to size
        cells.resize(size + 1);  // Index 0 unused
        for (int i = 1; i <= size; i++) {
            cells[i] = new Cell(i);
        }
    }

    ~Board() {
        for (int i = 1; i <= size; i++) {
            delete cells[i];
        }
    }

    int getSize() const { return size; }

    Cell* getCell(int position) {
        if (position >= 1 && position <= size) {
            return cells[position];
        }
        return nullptr;
    }

    void placeObstacle(Obstacle* obstacle) {
        int start = obstacle->getStart();
        if (start >= 1 && start <= size) {
            cells[start]->setObstacle(obstacle);
        }
    }

    void display() {
        int dim = static_cast<int>(std::sqrt(size));
        
        std::cout << "\n=== Board Layout ===\n";
        
        for (int row = dim - 1; row >= 0; row--) {
            bool leftToRight = (row % 2 == 0);
            
            for (int col = 0; col < dim; col++) {
                int position;
                if (leftToRight) {
                    position = row * dim + col + 1;
                } else {
                    position = row * dim + (dim - col);
                }
                
                Cell* cell = cells[position];
                char marker = ' ';
                if (cell->hasObstacle()) {
                    marker = (cell->getObstacle()->getType() == ObstacleType::SNAKE) ? 'S' : 'L';
                }
                
                std::cout << std::setw(3) << position << marker << " ";
            }
            std::cout << std::endl;
        }
        std::cout << "S = Snake, L = Ladder\n";
    }
};

#endif // BOARD_HPP
