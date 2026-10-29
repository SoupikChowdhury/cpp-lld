#pragma once

#include <vector>
#include <iostream>
#include <iomanip>
#include "PieceType.hpp"
#include "PlayingPiece.hpp"

namespace TicTacToe {

class Board {
private:
    int size;
    std::vector<std::vector<PieceType>> grid;
    int filledCells;

public:
    explicit Board(int size = 3) : size(size), filledCells(0) {
        // Initialize grid with EMPTY pieces
        grid.resize(size, std::vector<PieceType>(size, PieceType::EMPTY));
    }

    int getSize() const { return size; }

    bool isCellEmpty(int row, int col) const {
        if (row < 0 || row >= size || col < 0 || col >= size) {
            return false;
        }
        return grid[row][col] == PieceType::EMPTY;
    }

    bool placePiece(int row, int col, PieceType piece) {
        if (row < 0 || row >= size || col < 0 || col >= size) {
            std::cout << "Invalid position! Row and column must be between 0 and " 
                      << (size - 1) << std::endl;
            return false;
        }

        if (!isCellEmpty(row, col)) {
            std::cout << "Cell (" << row << ", " << col << ") is already occupied!" << std::endl;
            return false;
        }

        grid[row][col] = piece;
        filledCells++;
        return true;
    }

    PieceType getPiece(int row, int col) const {
        if (row < 0 || row >= size || col < 0 || col >= size) {
            return PieceType::EMPTY;
        }
        return grid[row][col];
    }

    bool isFull() const {
        return filledCells >= size * size;
    }

    int getFilledCells() const {
        return filledCells;
    }

    int getEmptyCells() const {
        return (size * size) - filledCells;
    }

    void displayBoard() const {
        std::cout << "\n";
        
        // Column numbers
        std::cout << "    ";
        for (int col = 0; col < size; ++col) {
            std::cout << " " << col << "  ";
        }
        std::cout << "\n";

        // Top border
        std::cout << "   ┌";
        for (int col = 0; col < size; ++col) {
            std::cout << "───";
            if (col < size - 1) std::cout << "┬";
        }
        std::cout << "┐\n";

        for (int row = 0; row < size; ++row) {
            // Row content
            std::cout << " " << row << " │";
            for (int col = 0; col < size; ++col) {
                std::cout << " " << pieceToChar(grid[row][col]) << " │";
            }
            std::cout << "\n";

            // Row border
            if (row < size - 1) {
                std::cout << "   ├";
                for (int col = 0; col < size; ++col) {
                    std::cout << "───";
                    if (col < size - 1) std::cout << "┼";
                }
                std::cout << "┤\n";
            }
        }

        // Bottom border
        std::cout << "   └";
        for (int col = 0; col < size; ++col) {
            std::cout << "───";
            if (col < size - 1) std::cout << "┴";
        }
        std::cout << "┘\n\n";
    }

    void reset() {
        for (int row = 0; row < size; ++row) {
            for (int col = 0; col < size; ++col) {
                grid[row][col] = PieceType::EMPTY;
            }
        }
        filledCells = 0;
    }

    // Get row values
    std::vector<PieceType> getRow(int row) const {
        if (row < 0 || row >= size) return {};
        return grid[row];
    }

    // Get column values
    std::vector<PieceType> getColumn(int col) const {
        if (col < 0 || col >= size) return {};
        std::vector<PieceType> column;
        for (int row = 0; row < size; ++row) {
            column.push_back(grid[row][col]);
        }
        return column;
    }

    // Get main diagonal (top-left to bottom-right)
    std::vector<PieceType> getMainDiagonal() const {
        std::vector<PieceType> diagonal;
        for (int i = 0; i < size; ++i) {
            diagonal.push_back(grid[i][i]);
        }
        return diagonal;
    }

    // Get anti-diagonal (top-right to bottom-left)
    std::vector<PieceType> getAntiDiagonal() const {
        std::vector<PieceType> diagonal;
        for (int i = 0; i < size; ++i) {
            diagonal.push_back(grid[i][size - 1 - i]);
        }
        return diagonal;
    }
};

} // namespace TicTacToe
