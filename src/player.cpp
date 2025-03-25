#include "player.h"
#include <iostream>
#include "grid.h"

std::map<PowerUpType, int> powerUpCount;

extern grid gameGrid;

player::player(int id, const std::string& skinPath) 
    : id(id), sprite(texture) {
        
    if (!texture.loadFromFile(skinPath + "/sprite_sheet.png")) {
        std::cerr << "Erreur : Impossible de charger la spritesheet du joueur : " 
                  << skinPath + "/sprite_sheet.png" << std::endl;
    }

    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(64, 64))); 
    sprite.setOrigin(sf::Vector2f(64 / 2.0f, 64 / 2.0f));
    sprite.setScale(sf::Vector2f(0.9f, 0.9f));  

    if (id == 1) { 
        x = 1;
        y = 1;
    } else { 
        x = 13;
        y = 9;
    }
  
    sprite.setPosition(sf::Vector2f(x * 50.0f + 25.0f, y * 50.0f + 25.0f));
}

void player::move(int dx, int dy, const grid &gameGrid) {
    if (moveCooldown > 0 || !canMove) return;

    int newX = x + dx;
    int newY = y + dy;

    if (gameGrid.isObstacle(newX, newY)) return;

    x = newX;
    y = newY;
    sprite.setPosition(sf::Vector2f(x * 50.0f + 25.0f, y * 50.0f + 25.0f));

    if (dx == 1) directionY = 128;  // Droite
    if (dx == -1) directionY = 64;  // Gauche
    if (dy == 1) directionY = 0;    // Bas
    if (dy == -1) directionY = 192; // Haut

    moveCooldown = 10;
    canMove = false;
}

void player::draw(sf::RenderWindow &window) {
    window.draw(sprite);
}

void player::update() {
    if (moveCooldown > 0) {
        moveCooldown--;
        // Animation uniquement pendant le déplacement
        elapsedTime += 0.016f; 
        if (elapsedTime >= frameTime) {
            elapsedTime = 0;
            frameIndex = (frameIndex + 1) % 4;
            sprite.setTextureRect(sf::IntRect(sf::Vector2i(frameIndex * 64, directionY), sf::Vector2i(64, 64)));
        }
    } else {
        // Réinitialise l'animation quand le joueur est immobile
        frameIndex = 0;
        sprite.setTextureRect(sf::IntRect(sf::Vector2i(0, directionY), sf::Vector2i(64, 64)));
    }
}

void player::resetMove() {
    canMove = true;
}

int player::getX() const {
    return x;
}

int player::getY() const {
    return y;
}

void player::setPosition(int newX, int newY) {
    x = newX;
    y = newY;
    shape.setPosition(sf::Vector2f(x * 50.0f + 5.0f, y * 50.0f + 5.0f));  
}

void player::kill() {
    x = -1;
    y = -1;
    sprite.setPosition(sf::Vector2f(-100, -100)); // Cache le sprite hors écran lorsque le joueur meurt
}

bool player::isAlive() const {
    return x >= 0 && y >= 0;  // Le joueur est vivant s'il est encore sur la grille
}

bool player::canPlaceBomb() const {
    return activeBombs < maxBombs;
}

void player::increaseBombLimit() {
    maxBombs++;
}

void player::bombExploded() {
    if (activeBombs > 0) activeBombs--;
}

void player::collectPowerUp(PowerUpType type) {
    switch (type) {
        case BOMB_UP:
            maxBombs++;  
            break;
        case FLAME_UP:
            explosionRange++;  
            break;
        case SPEED_UP:
            speed += 1.0f;  
            break;
        case SHIELD:
            setShield(true);
            std::cout << "Bouclier activé pour le joueur !" << std::endl;
            break;
    }
}

void player::increaseActiveBombs() {
    activeBombs++;
}

sf::FloatRect player::getBounds() const {
    return shape.getGlobalBounds();
}

bool player::hasShield() const {
    return shieldActive; 
}

void player::setShield(bool state) {
    shieldActive = state; 
}

void player::reset() {
    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(64, 64))); 
    sprite.setOrigin(sf::Vector2f(64 / 2.0f, 64 / 2.0f));
    sprite.setScale(sf::Vector2f(0.9f, 0.9f));

    powerUpCount = { {BOMB_UP, 0}, {FLAME_UP, 0}, {SPEED_UP, 0}, {SHIELD, 0} };
    
    speed = DEFAULT_SPEED;          // Réinitialisation de la vitesse
    maxBombs = DEFAULT_BOMBS;       // Réinitialisation du nombre max de bombes
    explosionRange = DEFAULT_RANGE; // Réinitialisation de la portée d'explosion
    activeBombs = 0;

    shieldActive = false;  // Au lieu de hasShieldActive = false;

    // Remettre les joueur à leur position de départ
    if (id == 1) { 
        x = 1;
        y = 1;
    } else { 
        x = 13;
        y = 9;
    }
    sprite.setPosition(sf::Vector2f(x * 50.0f + 25.0f, y * 50.0f + 25.0f));
}
