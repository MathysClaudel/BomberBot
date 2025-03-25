#include "powerup.h"
#include <iostream>

sf::Texture PowerUp::bombUpTexture;
sf::Texture PowerUp::flameUpTexture;
sf::Texture PowerUp::speedUpTexture;
sf::Texture PowerUp::shieldTexture;
bool PowerUp::texturesLoaded = false;

PowerUp::StaticInitializer PowerUp::staticInitializer;

PowerUp::StaticInitializer::StaticInitializer() {
    if (!PowerUp::texturesLoaded) {
        // Load textures
        if (!bombUpTexture.loadFromFile("assets/powerups/bomb_up.png"))
            std::cerr << "Error loading bomb_up.png" << std::endl;
        if (!flameUpTexture.loadFromFile("assets/powerups/flame_up.png"))
            std::cerr << "Error loading flame_up.png" << std::endl;
        if (!speedUpTexture.loadFromFile("assets/powerups/speed_up.png"))
            std::cerr << "Error loading speed_up.png" << std::endl;
        if (!shieldTexture.loadFromFile("assets/powerups/shield.png"))
            std::cerr << "Error loading shield.png" << std::endl;

        texturesLoaded = true;
    }
}

const sf::Texture& PowerUp::getTexture(PowerUpType type) {
    switch (type) {
        case BOMB_UP: return bombUpTexture;
        case FLAME_UP: return flameUpTexture;
        case SPEED_UP: return speedUpTexture;
        case SHIELD: return shieldTexture;
        default: throw std::invalid_argument("Invalid PowerUpType");
    }
}

PowerUp::PowerUp(int x, int y, PowerUpType type) 
    : x(x), y(y), type(type), sprite(getTexture(type)) {

    sf::Vector2u texSize = sprite.getTexture().getSize(); 
    
    sprite.setOrigin(sf::Vector2f(texSize.x / 2.0f, texSize.y / 2.0f));
     sprite.setScale(sf::Vector2f(0.1f, 0.1f));

    sprite.setPosition(sf::Vector2f(x * 50.0f + 25.0f, y * 50.0f + 25.0f));
}

void PowerUp::draw(sf::RenderWindow &window) const {
    window.draw(sprite);
}

sf::FloatRect PowerUp::getBounds() const {
    return sprite.getGlobalBounds();
}