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
            //up-left = -17 positions
            if (y >= 2 && x >= 1) attacks |= knight >> 17;
            //up-right == -15
            if (y >= 2 && x <= 6) attacks |= knight >> 15;
            //left-up == -10
            if (y >= 1 && x >= 2) attacks |= knight >> 10;
            //left-down == 6
            if (y <=6 && x >= 2) attacks |= knight << 6;
            //down-left == 15
            if (y <= 5 && x >= 1) attacks |= knight << 15;
            //down-right == 17
            if (y <= 5 && x <= 6)  attacks |= knight << 17;
            //right-up == -6
            if (y >= 1 && x <= 5) attacks |= knight >> 6;
            //right-down ==10
            if (y <= 6 && x <= 5) attacks |= knight << 10;
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

    static void rook_slide_right(U64* pos){
        for (int i = 0; i <64; i++){
            U64 rook = 1ULL << i;
            bool thing = true;
            int startx = i %8;
            int starty = i /8;
            int pos = i;
            while (pos % 8 != 0){

            }


        }
    }

    static void bishop_move(U64* array){

    }




};


#endif //SCHACK_LOADMOVEATTACKPATTERNS_H
