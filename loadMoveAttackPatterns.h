//
// Created by Henrik Ravnborg on 2026-01-18.
//

#ifndef SCHACK_LOADMOVEATTACKPATTERNS_H
#define SCHACK_LOADMOVEATTACKPATTERNS_H
#include "iostream"
using U64 = std::uint64_t;

class loadMoveAttackPatterns {
public:

    static void knight_move_attack (U64* array){
        for (int i = 0; i < 64; i++){
            U64 knight = 1ULL << i;
            U64 attacks = 0;
            int x = i % 8;
            int y = i / 8;
            //up-left
            if (y <= 5 && x >= 1) attacks |= knight << 15;
            //up-right
            if (y <= 5 && x <= 6) attacks |= knight << 17;
            //left-up
            if (y <= 6  && x >= 2) attacks |= knight << 6;
            //left-down
            if (y >= 1 && x >= 2) attacks |= knight >> 10;
            //down-left
            if (y >= 2 && x >= 1) attacks |= knight >> 17;
            //down-right
            if (y >= 2  && x <= 6)  attacks |= knight >> 15;
            //right-up
            if (y <= 6  && x <= 5) attacks |= knight << 10;
            //right-down
            if (y >= 1 && x <= 5) attacks |= knight >> 6;
            array[i] = attacks;
        }
    }

    static void king_move_attack(U64* array){
        for (int i = 0; i < 64; i++){
            U64 king = 1ULL << i;
            U64 attacks = 0;
            int x = i % 8;
            int y = i / 8;
        }
    }

    static void rook_moves(U64* array){
        for (int i = 0; i < 64; ++i) {
            U64 rook = 1ULL << i;
            array[i] = rook_slides(rook);
        }
    }

    static U64 rook_slides(U64 board){
        int ls = __builtin_ctzll(board);
        U64 rook = 1ULL << ls;

        int x = ls % 8;
        int y = ls / 8;

        U64 moves = 0;

        // left
        for (int nx = x - 1; nx >= 0; nx--)
            moves |= 1ULL << (y * 8 + nx);
        //right
        for (int nx = x + 1; nx < 8; nx++)
            moves |= 1ULL << (y * 8 + nx);
        //up
        for (int ny = y + 1; ny < 8; ++ny)
            moves |= 1ULL << (ny * 8 + x);
        //down
        for (int ny = y - 1; ny >= 0; --ny)
            moves |= 1ULL << (ny * 8 + x);


        return moves;
    }




    static void bishop_move(U64* array){

    }




};


#endif //SCHACK_LOADMOVEATTACKPATTERNS_H
