#pragma once

#include "PieceType.hpp"

namespace TicTacToe {

class PlayingPiece {
protected:
    PieceType pieceType;

public:
    explicit PlayingPiece(PieceType type) : pieceType(type) {}
    virtual ~PlayingPiece() = default;

    PieceType getPieceType() const { return pieceType; }
    
    virtual char getSymbol() const {
        return pieceToChar(pieceType);
    }

    virtual const char* getName() const {
        return pieceToString(pieceType);
    }
};

} // namespace TicTacToe
