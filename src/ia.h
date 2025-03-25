// IA.h
#ifndef IA_H
#define IA_H

#include "player.h"
#include "grid.h"
#include <deque>

#include <vector>
#include <chrono>  // Pour gérer le temps

struct DangerZone {
    int x, y;
    std::chrono::steady_clock::time_point expirationTime;
};

class game;

class IA : public player {
public:
    IA(int id, const std::string& skinPath, int difficulty);
    void updateAI(game &gameInstance, const grid& gameGrid, const player& opponent, int difficulty);
    void placeSmartBomb(const grid& gameGrid, game &gameInstance, const player& opponent);
    void moveRandomly(const grid& gameGrid, game &gameInstance);

    bool isSolidWall(int x, int y, const std::vector<std::pair<int, int>> &solidWalls);
    bool isInDangerZone(int x, int y);
    void moveTowardsOpponent_medium(const grid& gameGrid, const player& opponent, game &gameInstance);
    void moveTowardsOpponent_hard(const grid& gameGrid, const player& opponent, game &gameInstance);
    void update() override;
    void draw(sf::RenderWindow &window) override;
    void setContext(game* g, const grid* gr, const player* opp);
    
private:
    int slowDown=0;
    std::pair<int, int> lastBlockedMove = {0, 0}; // Mémorise la dernière direction bloquée
    int retreatSteps = 0; // Compteur pour le recul
    std::pair<int, int> retreatDirection = {0, 0}; // Direction de recul
    std::deque<std::pair<int, int>> lastMoves;  // Stocke les 4 derniers déplacements
    std::deque<std::pair<int, int>> escapeRoute; // Chemin de repli après une bombe
    std::vector<DangerZone> dangerZones; 
    int difficultyLevel;
    game* gameInstance = nullptr;
    const grid* gameGrid = nullptr;
    const player* opponent = nullptr;

};

#endif

