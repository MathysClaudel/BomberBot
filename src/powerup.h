#ifndef POWERUP_H
#define POWERUP_H

#include <SFML/Graphics.hpp>
#include <stdexcept>

enum PowerUpType { BOMB_UP, FLAME_UP, SPEED_UP, SHIELD };

class PowerUp {
public:
    PowerUp(int x, int y, PowerUpType type);
    
    void draw(sf::RenderWindow &window) const;
    sf::FloatRect getBounds() const;

    int getX() const { return x; }
    int getY() const { return y; }
    PowerUpType getType() const { return type; }

private:
    int x, y;
    PowerUpType type;
    sf::Sprite sprite;

    static sf::Texture bombUpTexture;
    static sf::Texture flameUpTexture;
    static sf::Texture speedUpTexture;
    static sf::Texture shieldTexture;
    static bool texturesLoaded;

    struct StaticInitializer {
        StaticInitializer();
    };
    static StaticInitializer staticInitializer;

    static const sf::Texture& getTexture(PowerUpType type);
};

#endif