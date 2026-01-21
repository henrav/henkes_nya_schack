//
// Created by Henrik Ravnborg on 2026-01-15.
//

#ifndef SCHACK_BOARD_H
#define SCHACK_BOARD_H
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "loadMoveAttackPatterns.h"
#include "Enums.h"


class Board {
    bool playersTurn;
public:
    Board(){
        init();
        loadMoveAttackPatterns::init_tables();
        printBoard(fullBoard[whitePawn]);
        for (int i = 0; i < 64; i++){
             printBoard(loadMoveAttackPatterns::loaded_pawn_moves[0][i])       ;

        }
    };

    U64 fullBoard[pieceCount];

    U64 whitePieces() {
        return fullBoard[whitePawn] | fullBoard[whiteKnight] | fullBoard[whiteBishop] | fullBoard[whiteRook] |
               fullBoard[whiteQueen] | fullBoard[whiteKing];
    }

    U64 blackPieces() {
        return fullBoard[blackPawn] | fullBoard[blackKnight] | fullBoard[blackBishop] | fullBoard[blackRook] |
               fullBoard[blackQueen] | fullBoard[blackKing];
    }

    void on_player_move(int toX, int toY, int fromX, int fromY, chessPiece piece){
        U64 from = convert_coords_to_U64(fromX, fromY);
        U64 to = convert_coords_to_U64(toX, toY);
        for (int i = 0; i < pieceCount; i++){
            if (from & fullBoard[i]){
                fullBoard[i] &= ~from;
                fullBoard[i] |= to;
                printBoard(fullBoard[i]);
                return;
            }
        }
        cout <<"ingen där " << endl;
    }





    U64 convert_coords_to_U64(int xpos, int ypos){
       return 1ULL << xpos + (ypos * 8);
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
    static void printBoard(U64 board) {
        for (int rank = 7; rank >= 0; rank--) {
            for (int file = 0; file < 8; file++) {
                int sq = rank * 8 + file;
                std::cout << ((board & (1ULL << sq)) ? " x " : " . ");
            }
            std::cout << "\n";
        }
        std::cout << "\n";

    }

};


#endif //SCHACK_BOARD_H
