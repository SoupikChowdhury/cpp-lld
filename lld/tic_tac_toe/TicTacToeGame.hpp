#pragma once

#include <memory>
#include <deque>
#include <iostream>
#include <utility>
#include <algorithm>
#include "Model/Board.hpp"
#include "Model/Player.hpp"
#include "Model/PlayingPieceX.hpp"
#include "Model/PlayingPieceO.hpp"

namespace TicTacToe {

class TicTacToeGame {
private:
    std::unique_ptr<Board> board;
    std::deque<std::unique_ptr<Player>> players;
    bool gameOver;
    Player* winner;
    int moveCount;

    // Check if all pieces in a line are the same (and not empty)
    bool isWinningLine(const std::vector<PieceType>& line) const {
        if (line.empty()) return false;
        
        PieceType first = line[0];
        if (first == PieceType::EMPTY) return false;
        
        return std::all_of(line.begin(), line.end(), 
            [first](PieceType p) { return p == first; });
    }

    // Check if the game has a winner
    bool checkWinner(int row, int col, PieceType piece) {
        int size = board->getSize();

        // Check row
        if (isWinningLine(board->getRow(row))) {
            return true;
        }

        // Check column
        if (isWinningLine(board->getColumn(col))) {
            return true;
        }

        // Check main diagonal (if on it)
        if (row == col) {
            if (isWinningLine(board->getMainDiagonal())) {
                return true;
            }
        }

        // Check anti-diagonal (if on it)
        if (row + col == size - 1) {
            if (isWinningLine(board->getAntiDiagonal())) {
                return true;
            }
        }

        return false;
    }

public:
    TicTacToeGame(int boardSize = 3) 
        : board(std::make_unique<Board>(boardSize)),
          gameOver(false),
          winner(nullptr),
          moveCount(0) {}

    void initializePlayers(const std::string& player1Name, 
                           const std::string& player2Name) {
        // Player 1 gets X
        auto player1 = std::make_unique<Player>(
            player1Name, 
            std::make_unique<PlayingPieceX>()
        );
        
        // Player 2 gets O
        auto player2 = std::make_unique<Player>(
            player2Name, 
            std::make_unique<PlayingPieceO>()
        );

        players.push_back(std::move(player1));
        players.push_back(std::move(player2));

        std::cout << "Game initialized!" << std::endl;
        std::cout << player1Name << " plays with X" << std::endl;
        std::cout << player2Name << " plays with O" << std::endl;
    }

    void addPlayer(std::unique_ptr<Player> player) {
        players.push_back(std::move(player));
    }

    bool makeMove(int row, int col) {
        if (gameOver) {
            std::cout << "Game is already over!" << std::endl;
            return false;
        }

        if (players.empty()) {
            std::cout << "No players in the game!" << std::endl;
            return false;
        }

        // Get current player
        auto& currentPlayer = players.front();
        PieceType piece = currentPlayer->getPieceType();

        std::cout << currentPlayer->getName() << "'s turn (" 
                  << currentPlayer->getSymbol() << ")" << std::endl;

        // Try to place the piece
        if (!board->placePiece(row, col, piece)) {
            return false;
        }

        moveCount++;
        std::cout << "Placed " << currentPlayer->getSymbol() 
                  << " at (" << row << ", " << col << ")" << std::endl;

        // Display board
        board->displayBoard();

        // Check for winner
        if (checkWinner(row, col, piece)) {
            winner = currentPlayer.get();
            gameOver = true;
            std::cout << "\n🎉 " << currentPlayer->getName() << " WINS! 🎉\n" << std::endl;
            return true;
        }

        // Check for draw
        if (board->isFull()) {
            gameOver = true;
            std::cout << "\n🤝 It's a DRAW! 🤝\n" << std::endl;
            return true;
        }

        // Switch to next player (rotate deque)
        players.push_back(std::move(players.front()));
        players.pop_front();

        return true;
    }

    // Play a complete game with simulated input
    Player* playWithMoves(const std::vector<std::pair<int, int>>& moves) {
        std::cout << "\n╔════════════════════════════════════════╗" << std::endl;
        std::cout << "║        TIC TAC TOE GAME                ║" << std::endl;
        std::cout << "╚════════════════════════════════════════╝\n" << std::endl;

        board->displayBoard();

        for (const auto& [row, col] : moves) {
            if (gameOver) break;
            makeMove(row, col);
        }

        displayResult();
        return winner;
    }

    void displayResult() const {
        std::cout << "═══════════════════════════════════════" << std::endl;
        std::cout << "  Game ended in " << moveCount << " moves" << std::endl;
        if (winner) {
            std::cout << "  Winner: " << winner->getName() << std::endl;
        } else if (gameOver) {
            std::cout << "  Result: Draw" << std::endl;
        }
        std::cout << "═══════════════════════════════════════\n" << std::endl;
    }

    void reset() {
        board->reset();
        gameOver = false;
        winner = nullptr;
        moveCount = 0;
    }

    // Getters
    bool isGameOver() const { return gameOver; }
    bool isDraw() const { return gameOver && winner == nullptr && board->isFull(); }
    Player* getWinner() const { return winner; }
    int getMoveCount() const { return moveCount; }
    Board* getBoard() const { return board.get(); }

    Player* getCurrentPlayer() const {
        if (players.empty()) return nullptr;
        return players.front().get();
    }

    void displayCurrentState() const {
        board->displayBoard();
        if (!gameOver && !players.empty()) {
            std::cout << "Current turn: " << players.front()->getName() 
                      << " (" << players.front()->getSymbol() << ")" << std::endl;
        }
    }
};

} // namespace TicTacToe
