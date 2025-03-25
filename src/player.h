#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include "grid.h"
#include "powerup.h"
#include <map>

extern std::map<PowerUpType, int> powerUpCount;

class player {
public:
    player(int id, const std::string& skinPath);
    virtual ~player() = default; 
    void move(int dx, int dy, const grid &gameGrid);
    virtual void draw(sf::RenderWindow &window) = 0;
    virtual void update() = 0;
    void resetMove();
    void kill();  // Gérer la mort du joueur
    bool isAlive() const;  // Vérifier si le joueur est encore en vie
    int getX() const; // Avoir les coordonnées du joueur de la grille
    int getY() const; 
    void setPosition(int newX, int newY);
    bool canPlaceBomb() const;  // Vérifier si le joueur peut poser une bombe
    void bombExploded();  // Décrémente les bombes posées après explosion
    void collectPowerUp(PowerUpType type);  // Récupère un power-up
    sf::FloatRect getBounds() const;  // Permet de détecter les collisions avec les power-ups
    void increaseActiveBombs();  // Ajoute une bombe active (évite d’accéder directement à activeBombs)
    void increaseBombLimit();  // Augmente le nombre de bombes posables
    
    bool hasShield() const;  // Vérifier si le joueur a un bouclier
    void setShield(bool state);  // Activer/désactiver le bouclier
    int getExplosionRange() const { return explosionRange; }

    void reset();
    // reset des pouvoirs
    static constexpr int DEFAULT_BOMBS = 1;
    static constexpr int DEFAULT_RANGE = 1;
    static constexpr float DEFAULT_SPEED = 1.0f;
    
private:
    [[maybe_unused]] int id;
    int x, y;
    int maxBombs = 1;  // Par défaut 1 bombe à la fois
    int activeBombs = 0;  // Nombre de bombes posées
    sf::RectangleShape shape;
    int explosionRange = 1;  // Portée d’explosion normale
    float speed = 1.0f;      // Vitesse normale
    bool shieldActive = false;

protected: // Accessibilité pour Human_Player et l'IA
    int moveCooldown = 0;
    bool canMove = true;

        // Gestion du sprite et de l'animation

        sf::Texture texture;
        sf::Sprite sprite;
        int frameIndex = 0;
        int directionY = 0;
        float elapsedTime = 0.0f;
        float frameTime = 0.01f;
    

};

#endif
