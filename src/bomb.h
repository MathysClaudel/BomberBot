#ifndef BOMB_H
#define BOMB_H

#include <SFML/Graphics.hpp>
#include "player.h"  

class bomb {
public:
    bomb(int x, int y, player* owner);
    void update();
    void draw(sf::RenderWindow &window) const;
    bool isExploded() const;
    int getX() const;
    int getY() const;
    bool isActive() const;  
    player* getOwner() const { return owner; }
    static bool loadTexture();
    void setTimer(int t);


private:
    int x, y;  
    int timer;  // Compteur pour gérer l'explosion après 3 secondes
    bool exploded;
    bool explosionVisible;
    int explosionTimer;  // Timer pour garder l'effet d'explosion visible
    static sf::Texture texture;  // Texture partagée entre toutes les bombes
    sf::Sprite sprite;  // Sprite de la bombe
    sf::RectangleShape explosionEffect[9];  // Effet de l'explosion
    static bool textureLoaded;
    void triggerExplosion();  // Déclencher l'effet d'explosion
    player* owner;
    int explosionRange;  // Portée de l'explosion
};

#endif
