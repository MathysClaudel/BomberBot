#ifndef HUMAN_PLAYER_H
#define HUMAN_PLAYER_H

#include "player.h"

class HumanPlayer : public player {
public:
    HumanPlayer(int id, const std::string& skinPath);

    void update() override;
    void draw(sf::RenderWindow &window) override;
};

#endif
