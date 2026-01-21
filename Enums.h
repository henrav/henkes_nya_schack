//
// Created by Henrik Ravnborg on 2026-01-21.
//

#ifndef SCHACK_ENUMS_H
#define SCHACK_ENUMS_H
#include <iostream>
#include <SFML/Graphics.hpp>

using namespace std;

using U64 = uint64_t;

enum chessPiece {
    whitePawn,
    whiteKnight,
    whiteBishop,
    whiteRook,
    whiteQueen,
    whiteKing,
    blackPawn,
    blackKnight,
    blackBishop,
    blackRook,
    blackQueen,
    blackKing,
    pieceCount
};

enum class Color { White, Black };

struct MoveRequest{
    int fromX, fromY;
    int toX, toY;
    chessPiece type;
};



#endif //SCHACK_ENUMS_H
