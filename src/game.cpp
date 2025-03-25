#include "game.h"
#include <algorithm>  
#include <iostream>
#include "player.h"
#include "powerup.h"

game::game(bool singlePlayer, int difficulty)
    : isSinglePlayerMode(singlePlayer),
      window(sf::VideoMode({750u, 550u}), "BomberBot"),
      winnerText(font, ""),  
      restartText(font, ""),
      menuText(font, ""),
      quitText(font, ""),
      difficultyLevel(difficulty)
{
    player1 = new HumanPlayer(1, "assets/skins/player1");
    
    if (isSinglePlayerMode) {
        bot = new IA(2, "assets/skins/player2", difficultyLevel);
        static_cast<IA*>(bot)->setContext(this, &gameGrid, player1);
        player2 = bot;
    } else {
        player2 = new HumanPlayer(2, "assets/skins/player2");
    }
    
    gameGrid.generateObstacles();

    if (!font.openFromFile("assets/fonts/LuckiestGuy-Regular.ttf")) {
        std::cerr << "Erreur : Impossible de charger la police !" << std::endl;
        exit(1);
    }

    winnerText.setCharacterSize(50);
    winnerText.setFillColor(sf::Color::White);
    winnerText.setStyle(sf::Text::Bold);

    restartText.setString("Recommencer");
    menuText.setString("Menu Principal");
    quitText.setString("Quitter");

    restartText.setCharacterSize(24);
    menuText.setCharacterSize(24);
    quitText.setCharacterSize(24);

    restartText.setFillColor(sf::Color::White);
    menuText.setFillColor(sf::Color::White);
    quitText.setFillColor(sf::Color::White);

    // Overlay semi-transparent

    overlay.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
    overlay.setFillColor(sf::Color(0, 0, 0, 150));

    // Boutons

    sf::Vector2f buttonSize(200, 50);
    restartButton.setSize(buttonSize);
    menuButton.setSize(buttonSize);
    quitButton.setSize(buttonSize);

    restartButton.setFillColor(sf::Color(0, 200, 0));  // Vert
    menuButton.setFillColor(sf::Color(0, 0, 200));     // Bleu
    quitButton.setFillColor(sf::Color(200, 0, 0));     // Rouge

    // Position des boutons

    restartButton.setPosition(sf::Vector2f(275, 250));
    menuButton.setPosition(sf::Vector2f(275, 320));
    quitButton.setPosition(sf::Vector2f(275, 390));

    // Centrage des textes sur les boutons

    restartText.setPosition(sf::Vector2f(restartButton.getPosition().x + 20, restartButton.getPosition().y + 10));
    menuText.setPosition(sf::Vector2f(menuButton.getPosition().x + 20, menuButton.getPosition().y + 10));
    quitText.setPosition(sf::Vector2f(quitButton.getPosition().x + 20, quitButton.getPosition().y + 10));

    if (!bomb::loadTexture()) {
        std::cerr << "Erreur critique : La texture de la bombe n'a pas pu être chargée au démarrage !" << std::endl;
        exit(1);
    }

}

void game::run() {
    while (window.isOpen()) {
        processEvents();
        update();
        render();
    }
}

void game::processEvents() {
    while (auto event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }

        if (gameOver) {
            handleGameOverEvents();
            return;
        }

        if (const auto* keyEvent = event->getIf<sf::Event::KeyReleased>()) {
            sf::Keyboard::Key key = keyEvent->code; 

            // Joueur 1 (Flèches)
            if (key == sf::Keyboard::Key::Left || key == sf::Keyboard::Key::Right || 
                key == sf::Keyboard::Key::Up || key == sf::Keyboard::Key::Down) {
                player1->resetMove();
            }

            // Joueur 2 (ZQSD)
            if (!isSinglePlayerMode && (key == sf::Keyboard::Key::Q || key == sf::Keyboard::Key::D || 
                key == sf::Keyboard::Key::Z || key == sf::Keyboard::Key::S)) {
                player2->resetMove();
            }
        }

        // Désactiver la pose de bombe avec Espace en solo
        if (isSinglePlayerMode && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
            placeBomb(player2->getX(), player2->getY(), player2);
}
    }

    handlePlayerMovement();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter)) {
        placeBomb(player1->getX(), player1->getY(), player1);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
        placeBomb(player2->getX(), player2->getY(), player2);
    }
}


void game::update() {
    if (gameOver) {
        return;
    }

    player1->update();
    if (isSinglePlayerMode && bot) {
        bot->update();
    } else {
        player2->update(); // Mode 1v1
    }

    for (auto &b : bombs) {
        b.update();
        if (b.isExploded()) {
            explodeBomb(b);
        }
    }

    bombs.erase(std::remove_if(bombs.begin(), bombs.end(),
        [](const bomb &b) { 
            if (!b.isActive()) {
                b.getOwner()->bombExploded();
                return true;
            }
            return false;
        }),
        bombs.end()
    );

    //std::cout << "Bombes après suppression : " << bombs.size() << std::endl;

    for (auto &exp : explosionVisuals) {
        if (exp.timer > 0) {
            exp.timer--;
        }
    }

    explosionVisuals.erase(std::remove_if(explosionVisuals.begin(), explosionVisuals.end(),
                                          [](const ExplosionEffect &exp) { return exp.timer <= 0; }),
                                          explosionVisuals.end());

    for (auto it = powerUps.begin(); it != powerUps.end();) {
        int powerUpX = it->getX();
        int powerUpY = it->getY();

        bool collected = false;

        if (player1->getX() == powerUpX && player1->getY() == powerUpY) {
            player1->collectPowerUp(it->getType());
            collected = true;
        }
        else if (player2->getX() == powerUpX && player2->getY() == powerUpY) {
            player2->collectPowerUp(it->getType());
            collected = true;
        }
        else if (isSinglePlayerMode && bot->getX() == powerUpX && bot->getY() == powerUpY) {
            bot->collectPowerUp(it->getType());
            collected = true;
        }

        if (collected) {
            it = powerUps.erase(it);
        } else {
            ++it;
        }
    }

    if (!player1->isAlive()) {
        if (isSinglePlayerMode) {
            winnerMessage = "      Game Over!";
        } else {
            winnerMessage = "Joueur 2 Vainqueur !";
        }
        gameOver = true;
    } 
    else if (!player2->isAlive() && !isSinglePlayerMode) {
        winnerMessage = "Joueur 1 Vainqueur !";
        gameOver = true;
    }
    else if (isSinglePlayerMode && !bot->isAlive()) {
        winnerMessage = " Tu as vaincu l'IA !";
        gameOver = true;
    }
    
}

void game::render() {
    window.clear();

    if (gameOver) {
        drawGameOverScreen();
        return;
    }else {
        gameGrid.draw(window);

        for (const auto &b : bombs) {
            b.draw(window);
        }

        player1->draw(window);

        if (isSinglePlayerMode  && bot) {
            bot->draw(window);
        } else {
            player2->draw(window);
        }

        for (const auto &explosion : explosionVisuals) {
            window.draw(explosion.shape);
        }

        for (const auto &powerUp : powerUps) {
            powerUp.draw(window);
        }
    }
    window.display();
}


void game::placeBomb(int x, int y, player *p) {
    if (!p->canPlaceBomb()) return;

    for (const auto &b : bombs) {
        if (b.getX() == x && b.getY() == y) return;
    }

    bomb newBomb(x, y, p);
    bombs.push_back(newBomb);
    p->increaseActiveBombs();
}


void game::addExplosionEffect(int x, int y) {
    ExplosionEffect newExplosion;
    newExplosion.shape.setSize(sf::Vector2f(50, 50));
    newExplosion.shape.setFillColor(sf::Color(255, 165, 0, 150)); // Orange pour les flammes
    newExplosion.shape.setPosition(sf::Vector2f(x * 50.0f, y * 50.0f));
    newExplosion.timer = 1000; // Durée plus courte pour correspondre à l'animation
    explosionVisuals.push_back(newExplosion);
}

void game::explodeBomb(bomb &b) {
    int bx = b.getX();
    int by = b.getY();
    explosionVisuals.clear();

    // Récupérer la portée de l'explosion du propriétaire de la bombe
    int explosionRange = b.getOwner()->getExplosionRange();

    // Vérifier si un joueur est sur la bombe au moment de l'explosion
    if (player1->getX() == bx && player1->getY() == by) {
        if (player1->hasShield()) {
            player1->setShield(false);
            std::cout << "Bouclier utilisé par le joueur 1 !" << std::endl;
        } else {
            player1->kill();
        }
    }
    if (player2->getX() == bx && player2->getY() == by) {
        if (player2->hasShield()) {
            player2->setShield(false);
            std::cout << "Bouclier utilisé par le joueur 2 !" << std::endl;
        } else {
            player2->kill();
        }
    }

    if (isSinglePlayerMode  && bot && bot->getX() == bx && bot->getY() == by) {
        if (bot->hasShield()) {
            bot->setShield(false);
            std::cout << "Bouclier utilisé par l'IA !" << std::endl;
        } else {
            bot->kill();
        }
    }    

    // Ajouter l'effet de l'explosion au centre
    addExplosionEffect(bx, by);

    // Directions : haut, bas, gauche, droite
    int directions[4][2] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};

    // Propagation de l'explosion en croix en fonction de explosionRange
    for (auto& dir : directions) {
        for (int step = 1; step <= explosionRange; step++) {
            int nx = bx + dir[0] * step;
            int ny = by + dir[1] * step;

            // Vérifier si on sort des limites de la grille
            if (nx < 0 || ny < 0 || nx >= 15 || ny >= 11) break;

            // Si on rencontre un obstacle, vérifier s'il est destructible
            if (gameGrid.isObstacle(nx, ny)) {
                if (gameGrid.isBreakable(nx, ny)) {
                    gameGrid.removeObstacle(nx, ny);
                    addExplosionEffect(nx, ny);

                    // Générer un power-up avec 33% de chance
                    if (rand() % 100 < 33) { 
                        PowerUpType type = static_cast<PowerUpType>(rand() % 4);
                        powerUps.push_back(PowerUp(nx, ny, type));
                        std::cout << "Power-up généré en (" << nx << ", " << ny << ") de type " << type << std::endl;
                    }
                }
                break; // Stopper la propagation de l'explosion dans cette direction
            }

            // Ajouter un effet d'explosion à la case actuelle
            addExplosionEffect(nx, ny);

            // Vérifier si un joueur est touché par l'explosion
            if (player1->getX() == nx && player1->getY() == ny) {
                if (player1->hasShield()) {
                    player1->setShield(false);
                    std::cout << "Bouclier utilisé par le joueur 1 !" << std::endl;
                } else {
                    player1->kill();
                }
            }
            if (player2->getX() == nx && player2->getY() == ny) {
                if (player2->hasShield()) {
                    player2->setShield(false);
                    std::cout << "Bouclier utilisé par le joueur 2 !" << std::endl;
                } else {
                    player2->kill();
                }
            }
            if (isSinglePlayerMode  && bot && bot->getX() == nx && bot->getY() == ny) {
                if (bot->hasShield()) {
                    bot->setShield(false);
                    std::cout << "Bouclier utilisé par l'IA !" << std::endl;
                } else {
                    bot->kill();
                }
            }
            
        }
    }

    // Mise à jour des bombes actives après leur explosion
    if (player1->getX() == bx && player1->getY() == by) {
        player1->bombExploded();
    }
    if (player2->getX() == bx && player2->getY() == by) {
        player2->bombExploded();
    }
}

void game::handlePlayerMovement() {
    if (player1->isAlive()) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
            int newX = player1->getX() - 1;
            int newY = player1->getY();
            if (!isBombAt(newX, newY)) {
                player1->move(-1, 0, gameGrid);
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
            int newX = player1->getX() + 1;
            int newY = player1->getY();
            if (!isBombAt(newX, newY)) {
                player1->move(1, 0, gameGrid);
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
            int newX = player1->getX();
            int newY = player1->getY() - 1;
            if (!isBombAt(newX, newY)) {
                player1->move(0, -1, gameGrid);
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
            int newX = player1->getX();
            int newY = player1->getY() + 1;
            if (!isBombAt(newX, newY)) {
                player1->move(0, 1, gameGrid);
            }
        }
    }

    if (!isSinglePlayerMode && player2->isAlive()) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) {
            int newX = player2->getX() - 1;
            int newY = player2->getY();
            if (!isBombAt(newX, newY)) {
                player2->move(-1, 0, gameGrid);
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
            int newX = player2->getX() + 1;
            int newY = player2->getY();
            if (!isBombAt(newX, newY)) {
                player2->move(1, 0, gameGrid);
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z)) {
            int newX = player2->getX();
            int newY = player2->getY() - 1;
            if (!isBombAt(newX, newY)) {
                player2->move(0, -1, gameGrid);
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
            int newX = player2->getX();
            int newY = player2->getY() + 1;
            if (!isBombAt(newX, newY)) {
                player2->move(0, 1, gameGrid);
            }
        }
    }
}

bool game::isBombAt(int x, int y) {
    for (const auto& b : bombs) {
        if (b.getX() == x && b.getY() == y && b.isActive()) {
            return true;
        }
    }
    return false;
}

void game::restartGame() {
    gameGrid.generateObstacles();

    player1->reset();
    player2->reset();
    if (bot) bot->reset();
    bombs.clear();
    explosionVisuals.clear();
    powerUps.clear();

    gameOver = false;
    winnerMessage = "";
}

void game::drawGameOverScreen() {
    window.clear();
    
    // Dessiner le fond du jeu pour l'effet overlay
    gameGrid.draw(window);

    for (const auto &b : bombs) {
        b.draw(window);
    }

    player1->draw(window);

    if (!isSinglePlayerMode) {
        player2->draw(window);
    }

    // Effet hover sur restartButton
    if (isRestartButtonHovered) {
        restartButton.setFillColor(sf::Color(50, 255, 50));  // Vert plus clair
        restartButton.setScale(sf::Vector2f(1.05f, 1.05f));  // Grossit légèrement
    } else {
        restartButton.setFillColor(sf::Color(0, 200, 0));    // Vert normal
        restartButton.setScale(sf::Vector2f(1.0f, 1.0f));
    }

    // Effet hover sur menuButton
    if (isMenuButtonHovered) {
        menuButton.setFillColor(sf::Color(50, 50, 255));  // Bleu plus clair
        menuButton.setScale(sf::Vector2f(1.05f, 1.05f));
    } else {
        menuButton.setFillColor(sf::Color(0, 0, 200));    // Bleu normal
        menuButton.setScale(sf::Vector2f(1.0f, 1.0f));
    }

    // Effet hover sur quitButton
    if (isQuitButtonHovered) {
        quitButton.setFillColor(sf::Color(255, 50, 50));  // Rouge plus clair
        quitButton.setScale(sf::Vector2f(1.05f, 1.05f));
    } else {
        quitButton.setFillColor(sf::Color(200, 0, 0));    // Rouge normal
        quitButton.setScale(sf::Vector2f(1.0f, 1.0f));
    }

    // Dessiner l'overlay semi-transparent
    window.draw(overlay);

    // Afficher le texte du gagnant
    winnerText.setString(winnerMessage);
    winnerText.setFillColor(sf::Color(255, 215, 0));
    winnerText.setPosition(sf::Vector2f(150, 150));
    window.draw(winnerText);

    // Dessiner les boutons
    window.draw(restartButton);
    window.draw(menuButton);
    window.draw(quitButton);

    // Dessiner les textes des boutons
    window.draw(restartText);
    window.draw(menuText);
    window.draw(quitText);

    window.display();
}

void game::handleGameOverEvents() {
    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

            isRestartButtonHovered = restartButton.getGlobalBounds().contains(mousePos);
            isMenuButtonHovered = menuButton.getGlobalBounds().contains(mousePos);
            isQuitButtonHovered = quitButton.getGlobalBounds().contains(mousePos);

            if (event->is<sf::Event::MouseButtonPressed>()) {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                if (restartButton.getGlobalBounds().contains(mousePos)) {
                    restartGame();
                    return;
                } 
                else if (menuButton.getGlobalBounds().contains(mousePos)) {
                    std::cout << "Retour au menu..." << std::endl;
                    goToMenu = true;
                    window.close();
                    return;
                }                
                else if (quitButton.getGlobalBounds().contains(mousePos)) {
                    window.close();
                    std::exit(0);
                }
            }
        }

        drawGameOverScreen();
    }
}

game::~game() {
    delete player1;
    // Éviter de supprimer deux fois le même pointeur
    if (isSinglePlayerMode) {
        delete bot;  // player2 == bot, donc on ne touche pas player2
    } else {
        delete player2;
    }
}
