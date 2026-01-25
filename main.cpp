#include <iostream>
#include <SFML/Window.hpp>
#include "Board.h"
#include <SFML/Graphics.hpp>
#include "GUI.h"





int main() {
    sf::RenderWindow window = sf::RenderWindow(sf::VideoMode({800,600}), "tjena");
    GUI gui(window);
    gui.run();
    return 0;
}




