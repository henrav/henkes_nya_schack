//
// Created by Henrik Ravnborg on 2026-01-16.
//

#ifndef SCHACK_GUI_H
#define SCHACK_GUI_H


#include <vector>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>
#include "Board.h"

using namespace std;
using U64 = std::uint64_t;

enum class PieceType { Pawn, Knight, Bishop, Rook, Queen, King };
enum class Color { White, Black };

struct Piece {
    PieceType type;
    Color color;
    int row{};
    int col{};
    sf::Sprite sprite;
    Piece(sf::Texture texture) : sprite(texture) {};
};

struct PieceMoveData{
    Piece* piece = nullptr;
    int deltax = 0;
    int deltay = 0;
    int startX = 0;
    int startY = 0;
};

struct HighlightHistory{
    sf::Color originalColor;
    sf::RectangleShape* board;
};



class GUI {
private:
    std::vector<sf::RectangleShape> board;
    std::vector<Piece> Pieces;
    float Size;
    bool width_max;
    sf::Texture whitePawnTexture, whiteKnightTexture, whiteBishopTexture, whiteRookTexture, whiteQueenTexture, whiteKingTexture;
    sf::Texture blackPawnTexture, blackKnightTexture, blackBishopTexture, blackRookTexture, blackQueenTexture, blackKingTexture;
    sf::RenderWindow &window;
    bool pieceSelected = false;
    PieceMoveData moveData;
    Piece *selectedPiece = nullptr;
    vector<HighlightHistory> Highlight;
public:
    GUI(sf::RenderWindow &window);

    void init();

    void run();

    void draw(sf::RenderWindow *pWindow);

    inline float UpdateTileSize();

    void update();

    void handleMousePress(sf::Event::MouseButtonPressed const *data);

    void handleMouseMove(sf::Event::MouseMoved const *data);

    void handleMouseRelease(const sf::Event::MouseButtonReleased *data);

    int findBoard(int posx, int posy);

    void handlePieceDrop(sf::Event::MouseButtonPressed const *data);

    void handlePiecePickup(sf::Event::MouseButtonPressed const *data);
    sf::Vector2<float> getOffsetPos();

    void updateGUI(Board& board);


    };


#endif //SCHACK_GUI_H
