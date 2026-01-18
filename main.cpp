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
    std::thread renderThread(render);   // start board thread first
    GUI gui(window);

    return 0;
}




