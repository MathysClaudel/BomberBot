#include "menu.h"
#include <iostream>

Menu::Menu(sf::RenderWindow& window)
    : window(window),
      backgroundTexture(),
      backgroundSprite(backgroundTexture),
      titleSprite(titleTexture),
      title(font, "BOMBERBOT", 50),
      startButton(font, "1 VS 1", 40),
      singlePlayerButton(font, "1 vs IA", 40),
      exitButton(font, "Exit Game", 40) 
{
    const std::string fontPath = "assets/fonts/LuckiestGuy-Regular.ttf"; 
    const std::string bgPath = "assets/textures/background.jpg"; 

    // Texture du fond d'écran

    if (!backgroundTexture.loadFromFile(bgPath)) {
        throw std::runtime_error("Erreur lors du chargements du fond d'écran");
    }
    backgroundTexture.setRepeated(true);
    backgroundTexture.setSmooth(true);
    
    if (!titleTexture.loadFromFile("assets/animations/Bomberbot.png")) {
        throw std::runtime_error("Erreur : Impossible de charger l'animation du titre !");
    }
    titleSprite.setTexture(titleTexture);
    titleSprite.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(frameWidth, frameHeight)));
    titleSprite.setPosition(sf::Vector2f(65.f, 30.f));
    titleSprite.setScale(sf::Vector2f(0.8f, 0.8f)); // Réduit à 50% de la taille d'origine
    
    // Chargement des fonts
    if (!font.openFromFile(fontPath)) {
        throw std::runtime_error("Erreur lors du chargement des fonts");
    }

    // Configuration du titre
    title.setFillColor(sf::Color(255, 215, 0)); 
    title.setPosition(sf::Vector2f(220, 100));
    title.setOutlineColor(sf::Color::Black);
    title.setOutlineThickness(2);

    // Initialisation des boutons
    const sf::Color buttonColor(30, 144, 255);
    const float buttonWidth = 300.f;
    const float buttonHeight = 60.f;

    // Bouton start
    startButtonBg.setSize(sf::Vector2f(buttonWidth, buttonHeight));
    startButtonBg.setFillColor(buttonColor);
    startButtonBg.setPosition(sf::Vector2f(310, 170));
    startButtonBg.setOrigin(sf::Vector2f(startButtonBg.getSize().x / 2, startButtonBg.getSize().y / 2)); // Centre l'origine

    startButton.setFont(font);
    startButton.setString("1 VS 1");
    startButton.setCharacterSize(40);
    startButton.setFillColor(sf::Color::White);
    startButton.setPosition(sf::Vector2f(
        startButtonBg.getPosition().x + buttonPadding,
        startButtonBg.getPosition().y + buttonPadding / 2
    ));

    // Bouton exit
    exitButtonBg.setSize(sf::Vector2f(buttonWidth, buttonHeight));
    exitButtonBg.setFillColor(sf::Color(220, 20, 60));
    exitButtonBg.setPosition(sf::Vector2f(470, 470));
    exitButtonBg.setOrigin(sf::Vector2f(exitButtonBg.getSize().x / 2, exitButtonBg.getSize().y / 2)); // Centre l'origine

    exitButton.setPosition(sf::Vector2f(
        exitButtonBg.getPosition().x + buttonPadding,
        exitButtonBg.getPosition().y + buttonPadding / 2
    ));

    // Configurer le sprite du background
    backgroundSprite.setTextureRect(sf::IntRect(
        sf::Vector2i(0, 0),
        sf::Vector2i(static_cast<int>(window.getSize().x), static_cast<int>(window.getSize().y))
    ));
    backgroundSprite.setPosition(sf::Vector2f(0, 0));

    // Bouton IA
    // Ajouter le bouton Mode Solo
    singlePlayerButtonBg.setSize(sf::Vector2f(buttonWidth, buttonHeight));
    singlePlayerButtonBg.setFillColor(sf::Color(34, 139, 34)); // Vert foncé
    singlePlayerButtonBg.setPosition(sf::Vector2f(300, 238));
    singlePlayerButtonBg.setOrigin(sf::Vector2f(singlePlayerButtonBg.getSize().x / 2, singlePlayerButtonBg.getSize().y / 2)); // Centre l'origine

    singlePlayerButton.setFont(font);
    singlePlayerButton.setString("1 VS IA");
    singlePlayerButton.setCharacterSize(40);
    singlePlayerButton.setFillColor(sf::Color::White);
    singlePlayerButton.setPosition(sf::Vector2f(
        singlePlayerButtonBg.getPosition().x + buttonPadding,
        singlePlayerButtonBg.getPosition().y + buttonPadding / 2
    ));

}

void Menu::update() {
    // Animation du fond

    backgroundOffset.x += 0.02f;
    backgroundOffset.y += 0.01f;

    if (backgroundOffset.x > backgroundTexture.getSize().x)
        backgroundOffset.x -= backgroundTexture.getSize().x;
    if (backgroundOffset.y > backgroundTexture.getSize().y)
        backgroundOffset.y -= backgroundTexture.getSize().y;

    backgroundSprite.setTextureRect(sf::IntRect(
        sf::Vector2i(static_cast<int>(backgroundOffset.x), static_cast<int>(backgroundOffset.y)),
        sf::Vector2i(static_cast<int>(window.getSize().x), static_cast<int>(window.getSize().y))
    ));

    if (animationClock.getElapsedTime().asMilliseconds() > 100) {
        currentFrame = (currentFrame + 1) % (totalFrames * totalRows); // Change la frame
    
        int currentRow = currentFrame / totalFrames;  // Trouve la ligne actuelle
        int currentColumn = currentFrame % totalFrames;  // Trouve la colonne actuelle
    
        titleSprite.setTextureRect(sf::IntRect(
            sf::Vector2i(currentColumn * frameWidth, currentRow * frameHeight), 
            sf::Vector2i(frameWidth, frameHeight)
        ));
        
        animationClock.restart(); // Remet le timer à zéro
    }
    
    // Gestion des effets de survol

    isStartButtonHovered = isMouseOver(startButtonBg);
    isSinglePlayerButtonHovered = isMouseOver(singlePlayerButtonBg);
    isExitButtonHovered = isMouseOver(exitButtonBg);

    // Appliquer les effets visuels

    if (isStartButtonHovered) {
        startButtonBg.setFillColor(sf::Color(50, 164, 255)); // Couleur plus claire
        startButtonBg.setScale(sf::Vector2f(1.05f, 1.05f));
    } else {
        startButtonBg.setFillColor(sf::Color(30, 144, 255)); // Couleur par défaut
        startButtonBg.setScale(sf::Vector2f(1.0f, 1.0f));
    }
    startButtonBg.setPosition(sf::Vector2f(370, 200)); // Réinitialise la position

    if (isSinglePlayerButtonHovered) {
        singlePlayerButtonBg.setFillColor(sf::Color(50, 189, 50)); // Couleur plus claire
        singlePlayerButtonBg.setScale(sf::Vector2f(1.05f, 1.05f));
    } else {
        singlePlayerButtonBg.setFillColor(sf::Color(34, 139, 34)); // Couleur par défaut
        singlePlayerButtonBg.setScale(sf::Vector2f(1.0f, 1.0f));
    }
    singlePlayerButtonBg.setPosition(sf::Vector2f(370, 270)); // Réinitialise la position

    if (isExitButtonHovered) {
        exitButtonBg.setFillColor(sf::Color(240, 50, 50)); // Couleur plus claire
        exitButtonBg.setScale(sf::Vector2f(1.05f, 1.05f));
    } else {
        exitButtonBg.setFillColor(sf::Color(220, 20, 60)); // Couleur par défaut
        exitButtonBg.setScale(sf::Vector2f(1.0f, 1.0f));
    }
    exitButtonBg.setPosition(sf::Vector2f(580, 500)); // Réinitialise la position
}

void Menu::processEvents(bool &startGame, bool &isSinglePlayer) {
    while (auto eventOpt = window.pollEvent()) {  
        const sf::Event& event = *eventOpt;  

        if (event.is<sf::Event::Closed>()) {  
            window.close();
            return;
        }

        if (showDifficultySelection) {
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        
            if (event.is<sf::Event::MouseButtonPressed>()) {
                if (mousePos.x >= 100  && mousePos.x <= 250) {
                    difficultyLevel = 1; // Facile
                    showDifficultySelection = false;
                    hideMainMenu = false;
                    startGame = true;
                    isSinglePlayer = true;
                } else if (mousePos.x >= 300 && mousePos.x <= 450) {
                    difficultyLevel = 2; // Moyen
                    showDifficultySelection = false;
                    hideMainMenu = false;
                    startGame = true;
                    isSinglePlayer = true;
                } else if (mousePos.x >= 500 && mousePos.x <= 650) {
                    difficultyLevel = 3; // Difficile
                    showDifficultySelection = false;
                    hideMainMenu = false;
                    startGame = true;
                    isSinglePlayer = true;
                }
            }
            return;
        }
        

        if (event.is<sf::Event::MouseButtonPressed>()) {
            if (const auto* mouseEvent = event.getIf<sf::Event::MouseButtonPressed>()) {
                if (mouseEvent->button == sf::Mouse::Button::Left) {
                    if (isMouseOver(startButtonBg)) {
                        startGame = true;
                        isSinglePlayer = false;  
                    } 
                    else if (isMouseOver(singlePlayerButtonBg)) {
                        showDifficultySelection = true;
                        hideMainMenu = true;
                    }
                    else if (isMouseOver(exitButtonBg)) {
                        window.close();
                        std::exit(0);
                    }
                }
            }
        }
    }
}

bool Menu::isMouseOver(const sf::RectangleShape& shape) {
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    return shape.getGlobalBounds().contains(mousePos);
}

void Menu::render() {
    window.clear(sf::Color(30, 30, 30));
    window.draw(backgroundSprite);
    sf::RectangleShape overlay(sf::Vector2f(window.getSize()));
    overlay.setFillColor(sf::Color(0, 0, 0, 100));
    window.draw(overlay);
    // window.draw(title);
    window.draw(titleSprite);


    // Lorsque l'on clique sur le mode de jeux IA hideMainMenu = true et on affiche les niveaux de l'IA
    if (!hideMainMenu) {
        window.draw(startButtonBg);
        window.draw(startButton);
        window.draw(exitButtonBg);
        window.draw(exitButton);
        window.draw(singlePlayerButtonBg);
        window.draw(singlePlayerButton);
    }
    
    if (showDifficultySelection) {
        // Taille des boutons
        sf::RectangleShape easyButtonBg(sf::Vector2f(150, 60));
        sf::RectangleShape mediumButtonBg(sf::Vector2f(150, 60));
        sf::RectangleShape hardButtonBg(sf::Vector2f(150, 60));
        
        // Couleur des boutons
        easyButtonBg.setFillColor(sf::Color(50, 200, 50));  // Vert
        mediumButtonBg.setFillColor(sf::Color(200, 200, 50)); // Jaune
        hardButtonBg.setFillColor(sf::Color(200, 50, 50));  // Rouge
    
        // Position des boutons
        easyButtonBg.setPosition(sf::Vector2f(100, 300));
        mediumButtonBg.setPosition(sf::Vector2f(300, 300));
        hardButtonBg.setPosition(sf::Vector2f(500, 300));
        
        window.draw(easyButtonBg);
        window.draw(mediumButtonBg);
        window.draw(hardButtonBg);
    
        // Texte des boutons de niveau de l'IA
        sf::Text easyText(font, "Facile", 30);
        sf::Text mediumText(font, "Moyen", 30);
        sf::Text hardText(font, "Difficile", 30);        
    
        easyText.setFillColor(sf::Color::White);
        mediumText.setFillColor(sf::Color::White);
        hardText.setFillColor(sf::Color::White);
        
        // Position du texte sur les boutons
        easyText.setPosition(sf::Vector2f(130, 310));
        mediumText.setPosition(sf::Vector2f(330, 310));
        hardText.setPosition(sf::Vector2f(525, 310));        
    
        window.draw(easyText);
        window.draw(mediumText);
        window.draw(hardText);
    }    

    window.display();
}

bool Menu::run(bool &isSinglePlayer) {
    bool startGame = false;

    while (window.isOpen()) {
        processEvents(startGame, isSinglePlayer);
        if (startGame) {
            window.close();  
            return true;     
        }
        update();
        render();
    }

    return false;
}