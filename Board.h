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

struct moveHistoryData{
    U64 from = -1;
    U64 to = -1;
    chessPiece fromBoard = pieceCount;
    chessPiece toBoard = pieceCount;
};


class Board {
    bool playersTurn;
    vector<moveHistoryData> history = {};

public:
    Board(){
        init();
        loadMoveAttackPatterns::init_tables();
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

    void on_player_move(MoveRequest data){

        U64 from = convert_coords_to_U64(data.fromX, data.fromY);
        U64 to = convert_coords_to_U64(data.toX, data.toY);
        //cout <<"\n" << endl;
        int type = (int)data.type;
       // cout << "from Board" << endl;
        //std::cout << "Y: " << data.fromY  << " X: " << data.fromX << endl;
        //cout << "to Board" << endl;
        //std::cout << "Y: " << data.toY  << " X: " << data.toX << endl;
        if (!(fullBoard[type] & from)) {
            return;
        }
        moveHistoryData h;
        h.from = fullBoard[type];
        for (int i = 0; i < pieceCount; i++) {
            if (fullBoard[i] & to){
                h.to = fullBoard[i];
                h.toBoard = static_cast<chessPiece> (i);
                fullBoard[i] &= ~to;
            }
        }
        h.fromBoard = static_cast<chessPiece> (type);
        history.push_back(h);
        fullBoard[type] &= ~from;
        fullBoard[type] |= to;
    }

    void pop_back_history(){
        if (!history.empty()){
            auto h = history.back();
            fullBoard[h.fromBoard] = h.from;
            if (h.toBoard != pieceCount){
                fullBoard[h.toBoard] = h.to;
            }
            history.pop_back();
        }
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

    bool isLegal(MoveRequest data){
        U64 from = convert_coords_to_U64(data.fromX, data.fromY);
        U64 to = convert_coords_to_U64(data.toX, data.toY);
        //cout << "from y: " << data.fromY << "to y: " << data.toY << "from x: "<< data.fromX << "to x" << data.toX << endl;
        int pos  = data.fromX + data.fromY * 8;
        if (!(fullBoard[data.type] & from)) return false;
        U64 ownPieces = (data.type <= whiteKing) ? whitePieces() : blackPieces();
        U64 opposingPieces = (data.type <= whiteKing) ? blackPieces() : whitePieces();
        if (to & ownPieces) return false;

        switch (data.type) {
            case whiteKing:
            case blackKing: {
                U64 moves = loadMoveAttackPatterns::loaded_king_moves[pos] & ~ownPieces;
                return (moves & to) != 0;
            }

            case whiteKnight:
            case blackKnight: {
                U64 moves = loadMoveAttackPatterns::loaded_knight_moves[pos] & ~ownPieces;
                return (moves & to) != 0;
            }

            case whiteBishop:
            case blackBishop: {
                U64 moves = loadMoveAttackPatterns::bishop_move(pos, opposingPieces, ownPieces);
                return (moves & to) != 0;
            }

            case whiteRook:
            case blackRook: {
                U64 moves = loadMoveAttackPatterns::rook_move(pos, opposingPieces, ownPieces);
                return (moves & to) != 0;
            }

            case whiteQueen:
            case blackQueen: {
                U64 moves = loadMoveAttackPatterns::queen_move(pos, opposingPieces, ownPieces);
                return (moves & to) != 0;
            }
            case whitePawn: {
                U64 attacks = loadMoveAttackPatterns::loaded_pawn_moves[0][pos] & ~ownPieces;
                if ((attacks & to) && (attacks & opposingPieces)) return true;
                U64 empty = ~(ownPieces | opposingPieces);
                U64 move = from << 8;
                if ((move == to) && (move & empty)) return true;

                if (data.fromY == 1){
                    U64 to2 = from << 16;
                    U64 between = from << 8;
                    if ((to2 == to) && (between & empty) && (to2 & empty)) return true;
                }
                return false;
            }
            case blackPawn:{
                U64 attacks = loadMoveAttackPatterns::loaded_pawn_moves[1][pos] & ~ownPieces;
                if ((attacks & to) && (attacks & opposingPieces)) return true;
                U64 empty = ~(ownPieces | opposingPieces);
                U64 move = from >> 8;
                if ((move == to) && (move & empty)) return true;

                if (data.fromY == 6){
                    U64 to2 = from >> 16;
                    U64 between = from >> 8;
                    if ((to2 == to) && (between & empty) && (to2 & empty)) return true;
                }
                return false;
            }
            default:
                return false;
        }
    }


    vector<pair<int, int>> getpositions(int x, int y, chessPiece piece){
        U64 from = convert_coords_to_U64(x, y);
        int pos  = x + (y * 8);
        if (!(fullBoard[piece] & from)) return {};

        U64 ownPieces = (piece <= whiteKing) ? whitePieces() : blackPieces();
        U64 opposingPieces = (piece <= whiteKing) ? blackPieces() : whitePieces();
        switch ((int)piece) {
            case whiteKing:
            case blackKing: {
                return convert_coords_to_pair(loadMoveAttackPatterns::loaded_king_moves[pos] & ~ownPieces);
            }

            case whiteKnight:
            case blackKnight: {
                return convert_coords_to_pair(loadMoveAttackPatterns::loaded_knight_moves[pos] & ~ownPieces);
            }

            case whiteBishop:
            case blackBishop: {
                return convert_coords_to_pair(loadMoveAttackPatterns::bishop_move(pos, opposingPieces, ownPieces));
            }

            case whiteRook:
            case blackRook: {
                return convert_coords_to_pair(loadMoveAttackPatterns::rook_move(pos, opposingPieces, ownPieces));

            }

            case whiteQueen:
            case blackQueen: {
                return convert_coords_to_pair(loadMoveAttackPatterns::queen_move(pos, opposingPieces, ownPieces));
            }
            case whitePawn: {
                cout << "x: " << x << "y: " << y << endl;
                U64 moves = 0;
                U64 attacks = loadMoveAttackPatterns::loaded_pawn_moves[0][pos] & ~ownPieces;
                moves |= attacks & opposingPieces;
                U64 empty = ~(ownPieces | opposingPieces);
                U64 one = from << 8;
                moves |= one & empty;
                if (y == 1) {
                    U64 two = from << 16;
                    U64 between = from << 8;
                    if ((between & empty) && (two & empty))
                        moves |= two;
                }
                return convert_coords_to_pair(moves);

            }
            case blackPawn:{

                U64 moves = 0;
                U64 attacks = loadMoveAttackPatterns::loaded_pawn_moves[1][pos] & ~ownPieces;
                moves |= attacks & opposingPieces;
                U64 empty = ~(ownPieces | opposingPieces);
                U64 one = from >> 8;
                moves |= one & empty;

                if (y == 6) {
                    U64 two = from >> 16;
                    U64 between = from >> 8;
                    if ((between & empty) && (two & empty))
                        moves |= two;
                }

                return convert_coords_to_pair(moves);
            }
            default:
                return {};
        }
    }
    std::vector<std::pair<int,int>> convert_coords_to_pair(U64 moves) {
        std::vector<std::pair<int,int>> coords;
        coords.reserve(32);

        for (int i = 0; i < 64; i++) {
            U64 pos = 1ULL << i;
            if (pos & moves){
                coords.emplace_back( i % 8, i / 8 );
            }
        }
        return coords;
    }


};


#endif //SCHACK_BOARD_H
