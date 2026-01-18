//
// Created by Henrik Ravnborg on 2026-01-16.
//

#include "GUI.h"
using namespace std;
#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))

void GUI::init() {
    UpdateTileSize();
    if (!whitePawnTexture.loadFromFile("../Bilder/VitBonde.png")) {
        cerr << "Failed to load texture for white rook!" << endl;
    }
    if (!whiteKnightTexture.loadFromFile("../Bilder/VitHäst.png")) {
        cerr << "Failed to load texture for white knight!" << endl;
    }
    if (!whiteBishopTexture.loadFromFile("../Bilder/VitLöpare.png")) {
        cerr << "Failed to load texture for white rook!" << endl;
    }
    if (!whiteRookTexture.loadFromFile("../Bilder/VittTorn.png")) {
        cerr << "Failed to load texture for white knight!" << endl;
    }
    if (!whiteQueenTexture.loadFromFile("../Bilder/VitDrottning.png")) {
        cerr << "Failed to load texture for white rook!" << endl;
    }
    if (!whiteKingTexture.loadFromFile("../Bilder/VitKung.png")) {
        cerr << "Failed to load texture for white knight!" << endl;
    }
    if (!blackPawnTexture.loadFromFile("../Bilder/SvartBonde.png")) {
        cerr << "Failed to load texture for white rook!" << endl;
    }
    if (!blackKnightTexture.loadFromFile("../Bilder/SvartHäst.png")) {
        cerr << "Failed to load texture for white knight!" << endl;
    }
    if (!blackBishopTexture.loadFromFile("../Bilder/SvartLöpare.png")) {
        cerr << "Failed to load texture for white rook!" << endl;
    }
    if (!blackRookTexture.loadFromFile("../Bilder/SvartTorn.png")) {
        cerr << "Failed to load texture for white knight!" << endl;
    }
    if (!blackQueenTexture.loadFromFile("../Bilder/SvartDrottning.png")) {
        cerr << "Failed to load texture for white knight!" << endl;
    }
    if (!blackKingTexture.loadFromFile("../Bilder/SvartKung.png")) {
        cerr << "Failed to load texture for white knight!" << endl;
    }

    int it = 0;
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            sf::RectangleShape rect({Size, Size});
            rect.setPosition({Size * col, Size * row});
            bool black = (col + row) % 2 == 0;
            sf::Color blacky(105, 105, 105);
            rect.setFillColor(black ? blacky : sf::Color::White);
            if (row < 2 || row > 5){
                Piece piece(whiteRookTexture);
                piece.color = black ? Color::Black : Color::White;

                if (it == 0 || it == 7)
                    piece.sprite.setTexture(blackRookTexture);
                else if (it == 1 || it == 6)
                    piece.sprite.setTexture(blackKnightTexture);
                else if (it == 2 || it == 5)
                    piece.sprite.setTexture(blackBishopTexture);
                else if (it == 3)
                    piece.sprite.setTexture(blackQueenTexture);
                else if (it == 4)
                    piece.sprite.setTexture(blackKingTexture);
                else if (it >= 8 && it <= 15)
                    piece.sprite.setTexture(blackPawnTexture);

                else if (it >= 48 && it <= 55)
                    piece.sprite.setTexture(whitePawnTexture);
                else if (it == 56 || it == 63)
                    piece.sprite.setTexture(whiteRookTexture);
                else if (it == 57 || it == 62)
                    piece.sprite.setTexture(whiteKnightTexture);
                else if (it == 58 || it == 61)
                    piece.sprite.setTexture(whiteBishopTexture);
                else if (it == 59)
                    piece.sprite.setTexture(whiteQueenTexture);
                else if (it == 60)
                    piece.sprite.setTexture(whiteKingTexture);

                piece.sprite.setPosition({Size * col, Size * row});
                piece.sprite.scale({0.4f, 0.4f});
                piece.col = col;
                piece.row = row;
                Pieces.emplace_back(piece);
            }
            it++;
            board.emplace_back(rect);
        }
    }
    update();
}

void GUI::run() {
    window.setActive(false);
    std::thread thread([this]() { draw(&window); });

    while (window.isOpen()) {
        std::optional<sf::Event> ev;
        {
            ev = window.pollEvent();
        }
        if (!ev) continue;

        if (ev->is<sf::Event::Closed>()) {
            window.close();
        } else if (const auto* resized = ev->getIf<sf::Event::Resized>()) {
            {
                update();
            }
            window.setView(sf::View(sf::FloatRect({0,0}, {static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y)})));
        }
        else if (const auto* mousePress = ev->getIf<sf::Event::MouseButtonPressed>()){
            handleMousePress(mousePress);
        }
        else if (const auto* mouseMove = ev->getIf<sf::Event::MouseMoved>()){
            handleMouseMove(mouseMove);
        }

    }
    thread.join();
}



void GUI::handleMouseMove(sf::Event::MouseMoved const * data){
    if (pieceSelected){
        selectedPiece->sprite.setPosition({static_cast<float>(data->position.x) - moveData.deltax, static_cast<float>(data->position.y - moveData.deltay)});
    }
}

/**
 *
 * @param posx
 * @param posy
 * @return returns index of potential board if found, else returns -1
 */
int GUI::findBoard(int posx, int posy){
    sf::Vector2<float> offset = getOffsetPos();
    for (int i = 0; i < board.size(); i++){
        sf::Vector2f pos = board[i].getPosition();
        int boardx = (pos.x - offset.x) / Size;
        int boardY = (pos.y - offset.y) / Size;
        if (posx == boardx && posy == boardY){
            cout << " boardx: "<<  boardx<< " boardY: "  << boardY <<  endl;
            cout<< "found" << endl;
            return i;
        }
    }
    return -1;
}

sf::Vector2<float> GUI::getOffsetPos(){
    float width = (8.f*Size);
    float offsetx = (window.getSize().x - width) * 0.5f;
    float offsety = (window.getSize().y - width) * 0.5f;
    return {sf::Vector2<float>(offsetx, offsety)};
}

void GUI::handlePieceDrop(sf::Event::MouseButtonPressed const * data){
    float mousex = data->position.x;
    float mousey = data->position.y;

    cout<< "green1" << endl;
    sf::Vector2<float> pos = getOffsetPos();

    int posx = static_cast<int> (mousex - pos.x) / Size;
    int posy = static_cast<int>(mousey -pos.y) / Size;
    cout << " posx: "<<  posx<< " posy: "  << posy <<  endl;
    int boardPos = findBoard(posx, posy);

    if (boardPos != -1){
        selectedPiece->sprite.setPosition({board[boardPos].getPosition().x, board[boardPos].getPosition().y });
        cout <<   moveData.startX  << moveData.startY << endl;
        if (moveData.startX == posx && moveData.startY == posy){
            cout << "same pos" << endl;
        }else{
            selectedPiece->row = posy;
            selectedPiece->col = posx;
        }
        selectedPiece = nullptr;
        pieceSelected = false;
        /*
        Highlight.push_back({Board[boardPos].getFillColor(), &Board[boardPos]});
        Board[boardPos].setFillColor(sf::Color::Green);
        cout<< "green2" << endl;
        */
    }
}

void GUI::handlePiecePickup(sf::Event::MouseButtonPressed const * data){
    float mousex = data->position.x;
    float mousey = data->position.y;
    for (auto &chessPiece : Pieces){
        if (chessPiece.sprite.getGlobalBounds().contains({mousex, mousey})){
            moveData = {&chessPiece,
                        static_cast<int>(mousex - chessPiece.sprite.getGlobalBounds().position.x),
                        static_cast<int>(mousey - chessPiece.sprite.getGlobalBounds().position.y),
                        chessPiece.col,
                        chessPiece.row};
            pieceSelected = true;
            selectedPiece = &chessPiece;
        }
    }
}


void GUI::handleMousePress(sf::Event::MouseButtonPressed const * data){
    if (pieceSelected){
        handlePieceDrop(data);
    }else{
        handlePiecePickup(data);
    }
}

void GUI::update() {
    UpdateTileSize();
    sf::Vector2<float> offset = getOffsetPos();
    int it = 0;
    int pieceIt = 0;
    for (int i = 0; i < 8; i++){
        for (int a = 0; a < 8; a++){
            auto& rect = board[it++];
            rect.setSize({Size, Size});
            rect.setPosition({offset.x + a * Size, offset.y + i * Size});
        }
    }
    for (auto& piece : Pieces) {
        piece.sprite.setPosition({offset.x + piece.col * Size, offset.y + piece.row * Size });

        auto ts = piece.sprite.getTexture().getSize();
        if (ts.x && ts.y) {
            piece.sprite.setScale({Size / (float) ts.x, Size / (float) ts.y});
        }
    }

}

void GUI::draw(sf::RenderWindow* Window) {
    Window->setActive(true);
    Window->setFramerateLimit(60);

    while (Window->isOpen())
    {
        {
            sf::Color color(128,128,128);
            Window->clear(color);
            {
                for (const  auto& rect : board)
                    Window->draw(rect);
                for (const auto& rect : Pieces){
                    Window->draw(rect.sprite);
                }
            }
            Window->display();

        }
    }
}


GUI::GUI(sf::RenderWindow& Window) : window(Window) {
    init();
    run();
}

inline float GUI::UpdateTileSize() {
    float w = (float)window.getSize().x;
    float h = (float)window.getSize().y;
    if (h <= w){
        width_max = true;
        Size =  h / 8.0f;
    }else{
        width_max = false;
        Size = w / 8.0f;
    }
}

void updateGUI(Board& b){
    for (int p = 0; p < 12; ++p) {
        U64 bb = b.fullBoard[p];
        while(bb){
            int sq = __builtin_ctzll(bb);
            bb &= bb - 1;

            int x = sq % 8;
            int y = sq / 8;
        }
    }
}

