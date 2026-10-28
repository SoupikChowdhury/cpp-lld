#pragma once

namespace TicTacToe {

enum class PieceType {
    X,
    O,
    EMPTY
};

inline char pieceToChar(PieceType piece) {
    switch (piece) {
        case PieceType::X: return 'X';
        case PieceType::O: return 'O';
        case PieceType::EMPTY: return ' ';
        default: return '?';
    }
}

inline const char* pieceToString(PieceType piece) {
    switch (piece) {
        case PieceType::X: return "X";
        case PieceType::O: return "O";
        case PieceType::EMPTY: return "EMPTY";
        default: return "UNKNOWN";
    }
}

} // namespace TicTacToe
