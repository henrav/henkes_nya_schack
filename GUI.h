//
// Created by Henrik Ravnborg on 2026-01-16.
//

#ifndef SCHACK_GUI_H
#define SCHACK_GUI_H


#include <vector>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>
#include "Enums.h"
#include "WorkerThread.h"

struct Piece {
    chessPiece type;
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
    float startXCoord = 0;
    float startYCoord = 0;
};

struct highlight{
    sf::Color originalColor;
    int index = 0;
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

    /**
     * pointer to piece in Pieces list;
     */
    Piece *selectedPiece = nullptr;
    vector<highlight> highlightHistory = {};
    bool playersTurn;
    Board bitBoard;
    atomic<bool> jobDone;
    WorkerThread worker = WorkerThread();
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

    void printType(chessPiece type);
    void send_to_board();
    void set_boards_to_green(int x, int y);
    void reset_green_boards();
    void updateGUIFromBoard( Board b);






    };



#endif //SCHACK_GUI_H
