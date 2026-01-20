//
// Created by Henrik Ravnborg on 2026-01-18.
//

#ifndef SCHACK_LOADMOVEATTACKPATTERNS_H
#define SCHACK_LOADMOVEATTACKPATTERNS_H
#include "iostream"
using U64 = std::uint64_t;

class loadMoveAttackPatterns {
public:
    static const U64 notAFile = 0xfefefefefefefefe;
    static const U64 notHFile = 0x7f7f7f7f7f7f7f7f;
    static U64 shiftSouth(U64 b) {return b >> 8;}
    static U64 shiftNorth(U64 b) {return b << 8;}
    static U64 shiftEast(U64 b)      {return (b & notHFile) << 1;}
    static U64 shiftNortheast(U64 b) {return (b & notHFile) << 9;}
    static U64 shiftSoutheast(U64 b) {return (b & notHFile) >> 7;}
    static U64 shiftWest(U64 b)      {return (b & notAFile) >> 1;}
    static U64 shiftSouthwest(U64 b) {return (b & notAFile) >> 9;}
    static U64 shiftNorthwest(U64 b) {return (b & notAFile) << 7;}


    static void load_knight_moves (U64* array){
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

    static void load_king_moves(U64* array){
        for (int i = 0; i < 64; i++){
            U64 attacks = 0;
            U64 start = 1ULL << i;
            U64 ray = start;
            ray = shiftNorthwest(ray);
            attacks |= ray;

            ray = start;
            ray = shiftNorth(ray);
            attacks |= ray;

            ray = start;
            ray = shiftNortheast(ray);
            attacks |= ray;

            ray = start;
            ray = shiftEast(ray);
            attacks |= ray;

            ray = start;
            ray = shiftSoutheast(ray);
            attacks |= ray;

            ray = start;
            ray = shiftSouth(ray);
            attacks |= ray;

            ray = start;
            ray = shiftSouthwest(ray);
            attacks |= ray;

            ray = start;
            ray = shiftWest(ray);
            attacks |= ray;

            array[i] = attacks;
        }
    }

    static void rook_moves(U64* array){
        for (int i = 0; i < 64; ++i) {
            U64 rook = 1ULL << i;
            array[i] = rook_slides(i);
        }
    }

    static void queen_moves(U64* array){
        for (int i = 0; i < 64; i++){
            U64 rook = rook_slides(i);
            U64 bishop = bishop_slides(i);
            U64 queen = rook | bishop;
            array[i] = queen;
        }
    }

    static U64 queen_move(int pos, U64 opposingPieces, U64 ownPieces){
        U64 rook = rook_move(pos, opposingPieces, ownPieces);
        U64 bishop = bishop_move(pos, opposingPieces, ownPieces);
        U64 queen = rook | bishop;
        return queen;
    }

    static U64 bishop_move(int pos, U64 opposingPieces, U64 ownPieces) {
        U64 attacks = 0;
        U64 start = 1ULL << pos;
        U64 ray = start;
        while ((ray = shiftNortheast(ray))){
            if (ray & ownPieces) break;
            attacks |= ray;
            if (ray & opposingPieces) break;
        }
        ray = start;
        while((ray = shiftNorthwest(ray))){
            if (ray & ownPieces) break;
            attacks |= ray;
            if (ray & opposingPieces) break;
        }
        ray = start;
        while((ray = shiftSoutheast(ray))){
            if (ray & ownPieces) break;
            attacks |= ray;
            if (ray & opposingPieces) break;
        }
        ray = start;
        while((ray = shiftSouthwest(ray))){
            if (ray & ownPieces) break;
            attacks |= ray;
            if (ray & opposingPieces) break;
        }
        return attacks;
    }

    static U64 rook_move(int pos, U64 opposingPieces, U64 ownPieces){
        U64 attacks = 0;
        U64 start = 1ULL << pos;
        U64 ray = start;
        while((ray = shiftNorth(ray))){
            if (ray & ownPieces) break;
            attacks |= ray;
            if (ray & opposingPieces) break;
        }

        ray = start;
        while((ray = shiftEast(ray))){
            if (ray & ownPieces) break;
            attacks |= ray;
            if (ray & opposingPieces) break;
        }

        ray = start;
        while((ray = shiftSouth(ray))){
            if (ray & ownPieces) break;
            attacks |= ray;
            if (ray & opposingPieces) break;
        }

        ray = start;
        while((ray = shiftWest(ray))){
            if (ray & ownPieces) break;
            attacks |= ray;
            if (ray & opposingPieces) break;
        }
        return attacks;
    }






    static U64 rook_slides(U64 board){
        U64 attacks = 0;
        U64 start = 1ULL << board;

        U64 ray = start;
        while((ray = shiftNorth(ray))){
            attacks |= ray;
        }

        ray = start;
        while((ray = shiftEast(ray))){
            attacks |= ray;
        }

        ray = start;
        while((ray = shiftSouth(ray))){
            attacks |= ray;
        }

        ray = start;
        while((ray = shiftWest(ray))){
            attacks |= ray;
        }

        return attacks;

    }

    static U64 bishop_slides(U64 board){
        U64 attacks = 0;
        U64 start = 1ULL << board;
        U64 ray = start;
        while((ray = shiftNorthwest(ray))){
            attacks |= ray;
        }

        ray = start;
        while((ray = shiftNortheast(ray))){
            attacks |= ray;
        }

        ray = start;
        while((ray = shiftSoutheast(ray))){
            attacks |= ray;
        }

        ray = start;
        while((ray = shiftSouthwest(ray))){
            attacks |= ray;
        }

        return attacks;

    }




    static void bishop_move(U64* array){

    }




};


#endif //SCHACK_LOADMOVEATTACKPATTERNS_H
