#include "menu.h"
#include "game.h"
#include "test.h"
#include <iostream> 

int main() {
    while (true) {
        test();
        sf::RenderWindow window(sf::VideoMode({750, 550}), "BomberBot");

        Menu menu(window);
        bool isSinglePlayer = false;
        bool startGame = menu.run(isSinglePlayer);

        if (startGame) {
            game myGame(isSinglePlayer, menu.difficultyLevel);
            myGame.run();
            
            if (myGame.goToMenu) {
                std::cout << "Rechargement du menu principal..." << std::endl;
                continue;  // Relancer la boucle pour revenir au menu
            }
        }

        std::cout << "Fermeture définitive du programme." << std::endl;
        return 0;
    }

    return 0;
}


