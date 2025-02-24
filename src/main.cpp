#include "../headers/Game.hpp"
#include <iostream>
int main() {
    try {
        Game game;
        game.play();
    } catch (std::exception &e) {
        std::cerr << "Error initializing game: " << e.what() << std::endl;
    }
    return 0;
}

