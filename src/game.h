#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "grid.h"
#include "player.h"
#include "ia.h"
#include "bomb.h"
#include <list>
#include "powerup.h"  // ✅ Assure que PowerUp est bien pris en compte
#include "human_player.h"

// Ajouter une structure pour gérer la durée des explosions
struct ExplosionEffect {
    sf::RectangleShape shape;
    int timer;
};

class game {
public:
    game(bool singlePlayer, int difficultyLevel); 
    ~game();  // destructeur
    void placeBomb(int x, int y, player *p);
    void run();
    const std::vector<bomb>& getBombs() const { return bombs; }
    bool goToMenu = false;
    

private:
    void processEvents();
    void update();
    void render();

    void explodeBomb(bomb &b);
    bool isBombAt(int x, int y);  // Vérification de l'emplacement de la bombe
    void addExplosionEffect(int x, int y);
    void handlePlayerMovement();

    // IA
    bool isSinglePlayerMode = true; // Mode solo activé
    player* bot; // IA en tant que deuxième joueur

    std::string winnerMessage;
    bool gameOver = false;
    void restartGame();
    
    sf::RenderWindow window;
    grid gameGrid;

    player* player1;
    player* player2;

    std::vector<bomb> bombs;
    std::vector<ExplosionEffect> explosionVisuals; // Remplacer l'ancien vecteur
    std::vector<std::pair<int, int>> explosionTimers; // Ajouter un vecteur pour suivre le temps des explosions

    void drawExplosions(); // Ajouter une méthode pour dessiner les explosions
    std::list<PowerUp> powerUps;  // Liste des pouvoirs sur la grille

    // Éléments graphiques pour l'écran de fin de jeu

    sf::Font font;
    sf::Text winnerText;
    sf::Text restartText;
    sf::Text menuText;
    sf::Text quitText;

    sf::RectangleShape overlay;
    sf::RectangleShape restartButton, menuButton, quitButton;

    bool isRestartButtonHovered = false;
    bool isMenuButtonHovered = false;
    bool isQuitButtonHovered = false;

    // Méthodes pour dessiner et gérer le menu de fin
    
    void drawGameOverScreen();
    void handleGameOverEvents();

    int difficultyLevel;
};

#endif

