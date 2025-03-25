#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp>

class Menu {
public:
    Menu(sf::RenderWindow& window);
    bool run(bool &isSinglePlayer);
    int difficultyLevel = 1; // 1 = Facile, 2 = Moyen, 3 = Difficile
    bool showDifficultySelection = false; // Indique si on est sur l'écran de sélection

private:
    bool isStartButtonHovered = false;
    bool isSinglePlayerButtonHovered = false;
    bool isExitButtonHovered = false;
    void processEvents(bool &startGame, bool &isSinglePlayer);
    void update();
    void render();
    bool isMouseOver(const sf::Text& text);
    bool isMouseOver(const sf::RectangleShape& shape);
    bool isSinglePlayer = false;

    sf::RenderWindow& window;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

    float backgroundOffsetX = 0.0f;
    float backgroundOffsetY = 0.0f;

    sf::Texture titleTexture;
    sf::Sprite titleSprite;
    sf::Clock animationClock;
    int currentFrame = 0;
    const int frameWidth = 800; // Largeur d'une frame
    const int frameHeight = 203; // Hauteur d'une frame
    const int totalFrames = 5;   // Nombre total de frames en ligne
    const int totalRows = 5;     // Nombre total de lignes
    
    sf::Font font;
    sf::Text title;
    sf::Text startButton;
    sf::Text singlePlayerButton;
    sf::Text exitButton;

    sf::RectangleShape startButtonBg;
    sf::RectangleShape singlePlayerButtonBg;
    sf::RectangleShape exitButtonBg;
    const float buttonPadding = 10.0f;
    sf::Clock titleAnimationClock;
    bool titleGrowing = true;   
    sf::Vector2f backgroundOffset = {0.f, 0.f};

    // Cacher le menu et afficher les boutons de niveau de l'IA
    bool hideMainMenu = false;


};

#endif