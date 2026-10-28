#pragma once

#include <string>
#include <memory>
#include "PlayingPiece.hpp"

namespace TicTacToe {

class Player {
private:
    std::string name;
    std::unique_ptr<PlayingPiece> playingPiece;

public:
    Player(const std::string& name, std::unique_ptr<PlayingPiece> piece)
        : name(name), playingPiece(std::move(piece)) {}

    // Move constructor
    Player(Player&& other) noexcept
        : name(std::move(other.name)), 
          playingPiece(std::move(other.playingPiece)) {}

    // Move assignment
    Player& operator=(Player&& other) noexcept {
        if (this != &other) {
            name = std::move(other.name);
            playingPiece = std::move(other.playingPiece);
        }
        return *this;
    }

    // Delete copy operations
    Player(const Player&) = delete;
    Player& operator=(const Player&) = delete;

    const std::string& getName() const { return name; }
    
    PlayingPiece* getPlayingPiece() const { 
        return playingPiece.get(); 
    }

    PieceType getPieceType() const {
        return playingPiece ? playingPiece->getPieceType() : PieceType::EMPTY;
    }

    char getSymbol() const {
        return playingPiece ? playingPiece->getSymbol() : ' ';
    }
};

} // namespace TicTacToe
