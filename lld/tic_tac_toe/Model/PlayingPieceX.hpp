#pragma once

#include "PlayingPiece.hpp"

namespace TicTacToe {

class PlayingPieceX : public PlayingPiece {
public:
    PlayingPieceX() : PlayingPiece(PieceType::X) {}

    char getSymbol() const override {
        return 'X';
    }

    const char* getName() const override {
        return "X";
    }
};

} // namespace TicTacToe
