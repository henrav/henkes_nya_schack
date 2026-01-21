#include <iostream>
#include <SFML/Window.hpp>
#include "Board.h"
#include <SFML/Graphics.hpp>
#include "GUI.h"


void render(){
    Board board = Board();
}



int main() {
    sf::RenderWindow window = sf::RenderWindow(sf::VideoMode({800,600}), "tjena");
    std::thread renderThread(render);
    GUI gui(window);
    gui.run();
    return 0;
}




