#include "bomb.h"
#include <iostream>  

sf::Texture bomb::texture; 
bool bomb::textureLoaded = false;

bomb::bomb(int x, int y, player* owner) 
    : x(x), y(y), timer(2000), exploded(false), explosionVisible(false),
      explosionTimer(30), sprite(texture), owner(owner), explosionRange(owner->getExplosionRange()) {  

    // Configurer le sprite
    sprite.setTexture(texture);
    sprite.setOrigin(sf::Vector2f(texture.getSize().x / 2.0f, texture.getSize().y / 2.0f));  
    sprite.setScale(sf::Vector2f(0.7f, 0.7f));  // Taille ajustée
    sprite.setPosition(sf::Vector2f(x * 50.0f + 25.0f, y * 50.0f + 25.0f)); // Centre de la case

    // Effets d'explosion
    for (int i = 0; i < 9; i++) {
        explosionEffect[i].setSize(sf::Vector2f(50, 50));
        explosionEffect[i].setFillColor(sf::Color(255, 255, 0, 150)); // Jaune semi-transparent
    }
}

void bomb::update() {
    if (timer > 0) {
        timer--;  // Décrémenter le timer à chaque frame
    } else if (!exploded) {
        exploded = true;
        explosionVisible = true;
        triggerExplosion(); // Effet de l'explosion 3x3
    }
    
    if (exploded && explosionTimer > 0) {
        explosionTimer--;
    } else if (exploded) {
        explosionVisible = false;  // Disparition de l'explosion après 30 frames
    }
}

void bomb::draw(sf::RenderWindow &window) const {
    if (!exploded) {
        //std::cout << "Dessin de la bombe à (" << sprite.getPosition().x << ", " << sprite.getPosition().y << ")\n";
        window.draw(sprite);
    }
    
    // Afficher tous les effets d'explosion
    if (explosionVisible) {
        for (int i = 0; i < 9; i++) {
            window.draw(explosionEffect[i]);
        }
    }
}

bool bomb::isExploded() const {
    return exploded;
}

bool bomb::isActive() const {
    return !exploded;  // La bombe est activé tant qu'elle n'a pas explosé
}

int bomb::getX() const {
    return x;
}

int bomb::getY() const {
    return y;
}

void bomb::triggerExplosion() {
    exploded = true;
    explosionVisible = true;

    // Directions : haut, bas, gauche, droite
    int directions[4][2] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};
    int index = 0;

    // Centre
    explosionEffect[index++].setPosition(sf::Vector2f(x * 50.0f, y * 50.0f));

    // Explosions en croix
    for (auto &dir : directions) {
        int dx = dir[0];
        int dy = dir[1];
        
        for (int step = 1; step <= explosionRange; step++) {
            int newX = x + dx * step;
            int newY = y + dy * step;
            
            if (index < 9) {  // Évite le débordement du tableau
                explosionEffect[index++].setPosition(sf::Vector2f(newX * 50.0f, newY * 50.0f));
            }
        }
    }

    // Cacher les effets inutiles
    for (; index < 9; index++) {
        explosionEffect[index].setPosition(sf::Vector2f(-100, -100));
    }
}

bool bomb::loadTexture() {
    if (!texture.loadFromFile("assets/textures/bomb.png")) {
        std::cerr << "Erreur : Impossible de charger l'image de la bombe !" << std::endl;
        return false;
    }
    std::cout << "Texture de la bombe chargée avec succès !" << std::endl;
    textureLoaded = true;
    return true;
}

void bomb::setTimer(int t) {
    timer = t;
}
