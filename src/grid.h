#ifndef GRID_H
#define GRID_H

#include <SFML/Graphics.hpp>
#include <vector>

#define EMPTY 0       // Case vide
#define SOLID 1       // Mur indestructible
#define BREAKABLE 2   // Mur destructible

class grid {
public:
    grid();
    void draw(sf::RenderWindow &window);
    
    void generateObstacles();  
    bool isObstacle(int x, int y) const;  
    void removeObstacle(int x, int y);  
    bool isBreakable(int x, int y) const; 
    const std::vector<std::pair<int, int>>& getSolidWalls() const;
    std::vector<std::pair<int, int>> solidWalls; 

private:
    static const int rows = 11;
    static const int cols = 15;
    std::vector<std::vector<int>> gridData;

    sf::Texture solidWallTexture;      // Texture pour les murs incassable
    sf::Texture breakableWallTexture;  // Texture pour les murs cassable
    sf::Sprite wallSprite;   

};

#endif
