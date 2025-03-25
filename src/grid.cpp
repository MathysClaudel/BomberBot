#include "grid.h"
#include <cstdlib>
#include <ctime>  
#include <iostream> 

grid::grid() : wallSprite(solidWallTexture) {
    gridData = std::vector<std::vector<int>>(rows, std::vector<int>(cols, EMPTY));
    generateObstacles();  // Générer les obstacles au démarrage

    if (!solidWallTexture.loadFromFile("assets/textures/mur.png")) {
        std::cerr << "Erreur : Impossible de charger la texture du mur indestructible !" << std::endl;
        exit(1);
    }
    
    if (!breakableWallTexture.loadFromFile("assets/textures/mur_destructible.png")) {
        std::cerr << "Erreur : Impossible de charger la texture du mur destructible !" << std::endl;
        exit(1);
    }

    wallSprite.setTexture(solidWallTexture);
}

void grid::generateObstacles() {
    std::srand(std::time(nullptr));

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if ((i == 1 && j == 1) || (i == 1 && j == 2) || (i == 2 && j == 1) || 
                (i == rows - 2 && j == cols - 2) || (i == rows - 2 && j == cols - 3) || (i == rows - 3 && j == cols - 2)) {
                gridData[i][j] = EMPTY;  // ✅ Assure un espace autour des joueurs
                continue;
            }

            if (rand() % 100 < 30) {
                gridData[i][j] = BREAKABLE;
            } else {
                gridData[i][j] = EMPTY;
            }
        }
    }

    // Ajouter les murs solides (gris) autour de la grille et en alternance
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (i == 0 || j == 0 || i == rows - 1 || j == cols - 1 || (i % 2 == 0 && j % 2 == 0)) {
                gridData[i][j] = SOLID;
                solidWalls.push_back({i, j});
            }
        }
    }
}

void grid::draw(sf::RenderWindow &window) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            sf::Sprite sprite((gridData[i][j] == SOLID) ? solidWallTexture : breakableWallTexture);
            sprite.setPosition(sf::Vector2f(j * 50.0f, i * 50.0f));
            const sf::Texture* texture = (gridData[i][j] == SOLID) ? &solidWallTexture : &breakableWallTexture;
            sprite.setScale(sf::Vector2f(50.0f / texture->getSize().x, 
                                                 50.0f / texture->getSize().y));

            if (gridData[i][j] == SOLID || gridData[i][j] == BREAKABLE) {
                window.draw(sprite);
            } else {
                sf::RectangleShape cell(sf::Vector2f(50, 50));
                cell.setPosition(sf::Vector2f(j * 50.0f, i * 50.0f));
                cell.setFillColor(sf::Color(200, 200, 200));
                cell.setOutlineThickness(1);
                cell.setOutlineColor(sf::Color::Black);
                window.draw(cell);
            }
        }
    }
}


bool grid::isObstacle(int x, int y) const {
    return gridData[y][x] == SOLID || gridData[y][x] == BREAKABLE;
}

void grid::removeObstacle(int x, int y) {
    if (x < 0 || y < 0 || x >= cols || y >= rows) return;
    if (gridData[y][x] == BREAKABLE) {
        gridData[y][x] = EMPTY;
    }
}

bool grid::isBreakable(int x, int y) const {
    if (x < 0 || y < 0 || x >= cols || y >= rows) return false;
    return gridData[y][x] == BREAKABLE;
}

const std::vector<std::pair<int, int>>& grid::getSolidWalls() const {
    return solidWalls;
}
