#include "human_player.h"
#include <SFML/Graphics.hpp>

HumanPlayer::HumanPlayer(int id, const std::string& skinPath)
    : player(id, skinPath) {}

void HumanPlayer::update() {
    if (moveCooldown > 0) {
        moveCooldown--;
        elapsedTime += 0.016f;
        if (elapsedTime >= frameTime) {
            elapsedTime = 0;
            frameIndex = (frameIndex + 1) % 4;
            sprite.setTextureRect(sf::IntRect(sf::Vector2i(frameIndex * 64, directionY), sf::Vector2i(64, 64)));
        }
    } else {
        frameIndex = 0;
        sprite.setTextureRect(sf::IntRect(sf::Vector2i(0, directionY), sf::Vector2i(64, 64)));
    }
}

void HumanPlayer::draw(sf::RenderWindow &window) {
    window.draw(sprite);
}
