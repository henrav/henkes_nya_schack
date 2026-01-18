//
// Created by Henrik Ravnborg on 2026-01-15.
//

#ifndef SCHACK_BOARD_H
#define SCHACK_BOARD_H
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "loadMoveAttackPatterns.h"

using U64 = std::uint64_t;

enum Pieces {
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




class Board {
public:
    Board(){
        init();
        loadMoveAttackPatterns::knight_move_attack(knight_attacks);
        for (int i = 0; i < 64; i++){
            printBoard(knight_attacks[i]);
            std::cout << "\n";

        }
        printBoard(fullBoard[whiteKnight]);
    };

    U64 knight_attacks[63];
    U64 rook_attacks[63];
    U64 bishop_attacks[63];
    U64 pawn_attacks[63];
    U64 king_attacks[63];
    U64 queen_attacks[63];

    U64 fullBoard[pieceCount]{};

    U64 whitePieces() {
        return fullBoard[whitePawn] | fullBoard[whiteKnight] | fullBoard[whiteBishop] | fullBoard[whiteRook] |
               fullBoard[whiteQueen] | fullBoard[whiteKing];
    }

    U64 blackPieces() {
        return fullBoard[blackPawn] | fullBoard[blackKnight] | fullBoard[blackBishop] | fullBoard[blackRook] |
               fullBoard[blackQueen] | fullBoard[blackKing];
    }

    void init() {
        fullBoard[whitePawn] = 0x000000000000FF00;
        fullBoard[whiteKnight] = 0x0000000000000042;
        fullBoard[whiteBishop] = 0x0000000000000024;
        fullBoard[whiteRook] = 0x0000000000000081;
        fullBoard[whiteQueen] = 0x0000000000000008;
        fullBoard[whiteKing] = 0x0000000000000010;

        fullBoard[blackPawn] = 0x00FF000000000000;
        fullBoard[blackKnight] = 0x4200000000000000;
        fullBoard[blackBishop] = 0x2400000000000000;
        fullBoard[blackRook] = 0x8100000000000000;
        fullBoard[blackQueen] = 0x0800000000000000;
        fullBoard[blackKing] = 0x1000000000000000;
    };

    static void printBoard(U64 board){
        for (int i = 63; i >= 0; i--) {

            if (board & (1ULL << i))
                std::cout << " x ";
            else
                std::cout << " . ";

            if (i % 8 == 0)
                std::cout << "\n";
        }
    }

};


#endif //SCHACK_BOARD_H
