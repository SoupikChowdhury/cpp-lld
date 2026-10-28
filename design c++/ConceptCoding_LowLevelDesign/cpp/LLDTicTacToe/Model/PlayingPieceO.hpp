#pragma once

#include "PlayingPiece.hpp"

namespace TicTacToe {

class PlayingPieceO : public PlayingPiece {
public:
    PlayingPieceO() : PlayingPiece(PieceType::O) {}

    char getSymbol() const override {
        return 'O';
    }

    const char* getName() const override {
        return "O";
    }
};

} // namespace TicTacToe
