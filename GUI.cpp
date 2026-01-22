//
// Created by Henrik Ravnborg on 2026-01-16.
//

#include "GUI.h"
using namespace std;
#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))

void GUI::init() {
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
            sf::Color grey(105, 105, 105);
            rect.setFillColor(black ? grey : sf::Color::White);
            it++;
            board.emplace_back(rect);
        }
    }
    updateGUIFromBoard(worker.get_board());

}

void GUI::updateGUIFromBoard( Board b) {
    Pieces.clear();
    UpdateTileSize();

    for (int p = 0; p < pieceCount; ++p) {
        U64 bb = b.fullBoard[p];
        while (bb) {
            int sq = __builtin_ctzll(bb);
            bb &= bb - 1;

            int x = sq % 8;
            int y = 7 - (sq / 8);


            Piece piece(whitePawnTexture);
            piece.type = static_cast<chessPiece>(p);

            switch (piece.type) {
                case whitePawn: {
                    piece.sprite.setTexture(whitePawnTexture);
                    break;
                }
                case whiteKnight: {
                    piece.sprite.setTexture(whiteKnightTexture);
                    break;
                }
                case whiteBishop: {
                    piece.sprite.setTexture(whiteBishopTexture);
                    break;
                }
                case whiteRook: {
                    piece.sprite.setTexture(whiteRookTexture);
                    break;
                }
                case whiteQueen: {
                    piece.sprite.setTexture(whiteQueenTexture);
                    break;
                }
                case whiteKing: {
                    piece.sprite.setTexture(whiteKingTexture);
                    break;
                }
                case blackPawn: {
                    piece.sprite.setTexture(blackPawnTexture);
                    break;
                }
                case blackKnight: {
                    piece.sprite.setTexture(blackKnightTexture);
                    break;
                }
                case blackBishop: {
                    piece.sprite.setTexture(blackBishopTexture);
                    break;
                }
                case blackRook: {
                    piece.sprite.setTexture(blackRookTexture);
                    break;
                }
                case blackQueen: {
                    piece.sprite.setTexture(blackQueenTexture);
                    break;
                }
                case blackKing: {
                    piece.sprite.setTexture(blackKingTexture);
                    break;
                }
                default: break;
            }
            piece.sprite.setPosition({Size * x, Size * y});
            auto pos = getOffsetPos();
            piece.sprite.setScale({2.4f, 0.4f});
            piece.col = x;
            piece.row = y;
            Pieces.emplace_back(piece);

        }
    }
    update();
}

void GUI::run() {
    window.setActive(false);
    std::thread thread([this]() { draw(&window); });
    worker.inti();
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
        else if (const auto* keyPress = ev->getIf<sf::Event::KeyPressed>()){
            if (keyPress->scancode == sf::Keyboard::Scan::Z){
                worker.undo_move();
                updateGUIFromBoard(worker.get_board());

            }
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
            return i;
        }
    }
    return -1;
}

sf::Vector2<float> GUI::getOffsetPos(){
    UpdateTileSize();
    float width = (8.f*Size);
    float offsetx = (window.getSize().x - width) * 0.5f;
    float offsety = (window.getSize().y - width) * 0.5f;
    return {sf::Vector2<float>(offsetx, offsety)};
}

void GUI::handlePieceDrop(sf::Event::MouseButtonPressed const * data){
    float mousex = data->position.x;
    float mousey = data->position.y;

    //get x and y offset, if window isnt perfect square, then theres gonna be some offset on either the x-axis or the y-axis
    // need to know that offset to calc coords
    sf::Vector2<float> pos = getOffsetPos();

    //calc coords
    int posx = static_cast<int> (mousex - pos.x) / Size;
    int posy = static_cast<int>(mousey -pos.y) / Size;

    //find index of board at coords, returns index if found, -1 if not found
    int boardPos = findBoard(posx, posy);

    if (boardPos != -1){
        // if same pos, then do nothing
        if (moveData.startX == posx && moveData.startY == posy){
            cout << "same pos" << endl;
            selectedPiece->sprite.setPosition({moveData.startXCoord, moveData.startYCoord});
            selectedPiece = nullptr;
            pieceSelected = false;
            reset_green_boards();
            return;
        }
        //create move request that we will send to engine
        // y - 7 needed for when converting coords to bitcoords
        MoveRequest moveRequest = {
                selectedPiece->col,
                7 - selectedPiece->row,
                posx,
                7-posy,
                selectedPiece->type
        };
        //if legal move
        if (worker.isLegalMove(moveRequest)) {
            //update sprite
            selectedPiece->sprite.setPosition({board[boardPos].getPosition().x, board[boardPos].getPosition().y });
            // send move to engine
            worker.submitJob(moveRequest);
            selectedPiece->row = posy;
            selectedPiece->col = posx;
            printType(selectedPiece->type);
        }
        //
        else{
           cout << "not a legal move" << endl;
           selectedPiece->sprite.setPosition({moveData.startXCoord, moveData.startYCoord});
        }
        selectedPiece = nullptr;
        pieceSelected = false;
        reset_green_boards();
    }
}

void GUI::send_to_board(){
}

void GUI::printType(chessPiece type) {
    switch (type) {
        case whitePawn:   std::cout << "white pawn\n"; break;
        case whiteKnight: std::cout << "white knight\n"; break;
        case whiteBishop: std::cout << "white bishop\n"; break;
        case whiteRook:   std::cout << "white rook\n"; break;
        case whiteQueen:  std::cout << "white queen\n"; break;
        case whiteKing:   std::cout << "white king\n"; break;

        case blackPawn:   std::cout << "black pawn\n"; break;
        case blackKnight: std::cout << "black knight\n"; break;
        case blackBishop: std::cout << "black bishop\n"; break;
        case blackRook:   std::cout << "black rook\n"; break;
        case blackQueen:  std::cout << "black queen\n"; break;
        case blackKing:   std::cout << "black king\n"; break;
        default:          std::cout << "unknown piece\n"; break;
    }
}


void GUI::handlePiecePickup(sf::Event::MouseButtonPressed const * data){
    float mousex = data->position.x;
    float mousey = data->position.y;

    //get x and y offset, if window isnt perfect square, then theres gonna be some offset on either the x-axis or the y-axis
    // need to know that offset to calc coords
    sf::Vector2<float> pos = getOffsetPos();

    //calc coords
    int posx = static_cast<int> (mousex - pos.x) / Size;
    int posy = static_cast<int>(mousey -pos.y) / Size;
    cout << "mouse x: " << mousex << " mouse y: " << mousey << endl;
    cout << "posx: " << posx << " posy: " << posy << endl;
    for (auto &chessPiece : Pieces){
        if ((chessPiece.col == posx) && (chessPiece.row == posy)){
            moveData = {&chessPiece,
                        static_cast<int>(mousex - chessPiece.sprite.getGlobalBounds().position.x),
                        static_cast<int>(mousey - chessPiece.sprite.getGlobalBounds().position.y),
                        chessPiece.col,
                        chessPiece.row,
                        chessPiece.sprite.getGlobalBounds().position.x,
                        chessPiece.sprite.getGlobalBounds().position.y};
            pieceSelected = true;
            selectedPiece = &chessPiece;
            vector<pair<int, int>> coords = worker.getPositions(chessPiece.col, 7 - chessPiece.row, chessPiece.type);
            for (auto pair : coords){
                set_boards_to_green(pair.first, 7 - pair.second);
            }
        }
    }
}

void GUI::reset_green_boards(){
    for (auto h : highlightHistory){
        board[h.index].setFillColor(h.originalColor);
    }
    highlightHistory.clear();
}

void GUI::set_boards_to_green(int x, int y){
    int index = findBoard(x, y);
    if (index != -1){
        highlight b = {
                board[index].getFillColor(),
                index
        };
        highlightHistory.emplace_back(b);
        board[index].setFillColor(sf::Color::Green);
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
                if (worker.isDone()){
                    updateGUIFromBoard(worker.get_board());
                    worker.set_isDone(false);
                }
            }
            Window->display();
        }
    }
}


GUI::GUI(sf::RenderWindow& Window) : window(Window) {
    init();
    bitBoard = Board();
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

