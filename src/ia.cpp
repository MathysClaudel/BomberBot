#include "ia.h"
#include "game.h"
#include "grid.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <deque>
#include <chrono>
#include <thread>


IA::IA(int id, const std::string& skinPath, int difficulty) : player(id, skinPath), difficultyLevel(difficulty)  {
    std::srand(std::time(nullptr)); // Initialisation de la seed aléatoire
    lastMoves.clear();
}

void IA::setContext(game* g, const grid* gr, const player* opp) {
    std::cout << "[IA::setContext] called" << std::endl;
    gameInstance = g;
    gameGrid = gr;
    opponent = opp;
}

void IA::update() {
    player::update(); 
    if (!gameInstance) std::cerr << "[IA::update] gameInstance is null!\n";
    if (!gameGrid) std::cerr << "[IA::update] gameGrid is null!\n";
    if (!opponent) std::cerr << "[IA::update] opponent is null!\n";

    if (gameInstance && gameGrid && opponent) {
        updateAI(*gameInstance, *gameGrid, *opponent, difficultyLevel);
    } else {
        std::cerr << "[IA::update] contexte incomplet, skip update.\n";
    }
}

void IA::updateAI(game &gameInstance, const grid& gameGrid, const player& opponent, int difficulty) {
    if (difficulty == 1) {
        moveRandomly(gameGrid, gameInstance);
    } else if (difficulty == 2) {
        moveTowardsOpponent_medium(gameGrid, opponent, gameInstance);
    } else {
        placeSmartBomb(gameGrid, gameInstance, opponent);
        moveTowardsOpponent_hard(gameGrid, opponent, gameInstance);
    }
}

bool IA::isInDangerZone(int x, int y) {
    auto now = std::chrono::steady_clock::now();
    dangerZones.erase(std::remove_if(dangerZones.begin(), dangerZones.end(),
        [now](const DangerZone &dz) {
            return now > dz.expirationTime; // Supprimer les zones expirées
        }), 
        dangerZones.end());

    for (const auto& dz : dangerZones) {
        if (dz.x == x && dz.y == y) {
            return true; // Case dangereuse
        }
        else if ((dz.x + 1 == x && dz.y ==y) || (dz.x - 1 == x && dz.y ==y) || (dz.x == x && dz.y+1 ==y) || (dz.x== x && dz.y - 1 ==y)){
            return true;
        }
    }
    return false;
}

void IA::moveRandomly(const grid& gameGrid, game &gameInstance) {
    if (slowDown > 500) {
        canMove = true;
        
        int dx[] = {0, 0, -1, 1};
        int dy[] = {-1, 1, 0, 0};
        
        int direction;
        bool foundMove = false;

        // Cherche une direction valide
        for (int attempts = 0; attempts < 4; ++attempts) {
            direction = std::rand() % 4;
            int newX = getX() + dx[direction];
            int newY = getY() + dy[direction];

            if (!gameGrid.isObstacle(newX, newY) && !isInDangerZone(newX, newY)) {
                move(dx[direction], dy[direction], gameGrid);
            }

            // Vérifie si la case n'est pas un mur
            if (gameGrid.isObstacle(newX, newY)) {
                if (gameGrid.isBreakable(newX, newY)) {
                    std::cout << "Mur cassable détecté en (" << newX << ", " << newY << ")" << std::endl;
                    // Placer une bombe à la position actuelle de l'IA
                    gameInstance.placeBomb(getX(), getY(), this);

                    // Ajouter la position de la bombe à la liste des zones dangereuses
                    DangerZone dz;
                    dz.x = getX();
                    dz.y = getY();
                    dz.expirationTime = std::chrono::steady_clock::now() + std::chrono::seconds(3);
                    dangerZones.push_back(dz);


                    // Tenter de reculer après avoir posé la bombe
                    int dx[] = {0, 0, -1, 1}; // Gauche / Droite
                    int dy[] = {-1, 1, 0, 0}; // Haut / Bas

                    for (int i = 0; i < 4; ++i) {
                        int newX = getX() + dx[i];
                        int newY = getY() + dy[i];

                        if (!gameGrid.isObstacle(newX, newY)) {
                            move(dx[i], dy[i], gameGrid);
                            break; // Sortir dès qu'on trouve une case libre
                        }
                    }

                    return; // On ne bouge pas après avoir placé une bombe
                } else {
                    std::cout << "Mur incassable détecté en (" << newX << ", " << newY << ")" << std::endl;
                }
            }
            else {
                foundMove = true;
                break;
            }
        }

        // Si on a trouvé une direction valide, on bouge
        if (foundMove) {
            move(dx[direction], dy[direction], gameGrid);
        }

        slowDown = 0;
    } else {
        slowDown += 1;
    }
}

void IA::moveTowardsOpponent_medium(const grid& gameGrid, const player &opponent, game &gameInstance) {
    if (slowDown > 750) {
        canMove = true; // permet à l'IA de se déplacer de nouveau
        
        // Récupérer la position du joueur à traquer (on suppose que c'est le joueur 1)
        int targetX = opponent.getX();
        int targetY = opponent.getY();

        // Calculer la meilleure direction vers le joueur
        int bestDirection = -1;
        int minDistance = std::abs(getX() - targetX) + std::abs(getY() - targetY);

        int dx[] = {0, 0, -1, 1}; // Gauche / Droite
        int dy[] = {-1, 1, 0, 0}; // Haut / Bas

        for (int i = 0; i < 4; ++i) {
            int newX = getX() + dx[i];
            int newY = getY() + dy[i];

            if (!gameGrid.isObstacle(newX, newY) && !isInDangerZone(newX, newY)) {
                int newDistance = std::abs(newX - targetX) + std::abs(newY - targetY);
                if (newDistance < minDistance) {
                    minDistance = newDistance;
                    bestDirection = i;
                }
            }
        }

        // Si une direction est trouvée, bouger vers elle
        if (bestDirection != -1) {
            move(dx[bestDirection], dy[bestDirection], gameGrid);
            slowDown = 0; // Pour faire repartir le compteur 
            return;
        }

        for (int i = 0; i < 4; ++i) {

            int newX = getX() + dx[i];
            int newY = getY() + dy[i];

            std::cout << "Vérification de la case (" << newX << ", " << newY << ")..." << std::endl;

            // Vérifier si l'IA est bloquée par un mur cassable ou est arrivé à l'endroit du joueur
            if ((gameGrid.isObstacle(newX, newY) && gameGrid.isBreakable(newX, newY))||(getX()==targetX && getY()==targetY)) {
                //std::cout << "Mur cassable détecté en (" << newX << ", " << newY << "), pose d'une bombe !" << std::endl;
                //std::cout << "Pos ia (" << getX() << ", " << getY() << "), pose d'une bombe !" << std::endl;


                // Étape 1 : Trouver une case sûre pour se déplacer après avoir posé la bombe
                int safeX = -1;
                int safeY = -1; // Coordonnées de la case sûre

                int safeX1 = -1;
                int safeY1 = -1;

                for (int j = 0; j < 4; ++j) {
                    int potentialX = getX() + dx[j];
                    int potentialY = getY() + dy[j];

                    std::cout << "Vérification de la case sûre (" << potentialX << ", " << potentialY << ")..." << std::endl;

                    if ((!gameGrid.isObstacle(potentialX, potentialY)) || (getX()==targetX && getY()==targetY)) {
                        if (!isInDangerZone(potentialX, potentialY)) {
                            safeX = potentialX;
                            safeY = potentialY;
                            for (int k=0; k<4; k++){
                                int potentialX1 = safeX + dx[k];
                                int potentialY1 = safeY + dy[k];
                                if ((!gameGrid.isObstacle(potentialX1, potentialY1))){
                                    if (!isInDangerZone(potentialX1, potentialY1)){
                                        if(getX()==potentialX1 && getY()==potentialY1){
                                            std::cout << "meme case(" << potentialX1 << ", " << potentialY1 << ") !" << std::endl;
                                            continue;
                                        }
                                        safeX1 = potentialX1;
                                        safeY1 = potentialY1;
                                        std::cout << "Case sûre de sure trouvée en (" << safeX1 << ", " << safeY1 << ") !" << std::endl;
                                        break;
                                }
                                }
                            }
                            std::cout << "Case sûre trouvée en (" << safeX << ", " << safeY << ") !" << std::endl;
                            break; // On sort de la boucle
                        } else {
                            std::cout << "Case (" << potentialX << ", " << potentialY << ") est dans une zone dangereuse." << std::endl;
                        }
                    } else {
                        std::cout << "Case (" << potentialX << ", " << potentialY << ") est un obstacle." << std::endl;
                    }
                }

                // Étape 2 : Poser la bombe
                if (safeX1 != -1 && safeY1 != -1) {

                    std::cout << "Pose de la bombe en (" << getX() << ", " << getY() << ")..." << std::endl;
                    gameInstance.placeBomb(getX(), getY(), this);

                    // Étape 3 : Ajouter la position de la bombe à la liste des zones dangereuses
                    DangerZone dz;
                    dz.x = getX();
                    dz.y = getY();
                    dz.expirationTime = std::chrono::steady_clock::now() + std::chrono::seconds(4);
                    dangerZones.push_back(dz);
                    std::cout << "Bombe ajoutée aux zones dangereuses. Expiration à : "
                                << std::chrono::duration_cast<std::chrono::seconds>(dz.expirationTime.time_since_epoch()).count()
                                << "s" << std::endl;

                    // Étape 4 : Se déplacer vers la case sûre (si une case sûre a été trouvée)

                    std::cout << "Déplacement vers la case sûre en (" << safeX << ", " << safeY << ")..." << std::endl;
                    move(safeX - getX(), safeY - getY(), gameGrid); // Déplacer l'IA
                    std::cout << "Déplacement vers la case sûre en (" << safeX1 << ", " << safeY1 << ")..." << std::endl;
                    move(safeX1 - getX(), safeY1 - getY(), gameGrid); // Déplacer l'IA

                    // Étape 5 : Réinitialiser le délai et quitter la fonction
                    slowDown = 0;

                } else {
                    std::cout << "Aucune case sûre trouvée on ne pose pas la bombe !" << std::endl;
                    break;
                }

                return;
            } else {
                std::cout << "Case (" << newX << ", " << newY << ") n'est pas un mur cassable." << std::endl;
            }
        }

        // Si aucune direction n'est trouvée, l'IA garde son mouvement aléatoire
        int randomDirection;

        for (int attempts = 0; attempts < 4; ++attempts) {
            randomDirection = std::rand() % 4;
            int newX = getX() + dx[randomDirection];
            int newY = getY() + dy[randomDirection];

            if (!gameGrid.isObstacle(newX, newY) && !isInDangerZone(newX, newY)) {
                move(dx[randomDirection], dy[randomDirection], gameGrid);
                break;
            }
        }

        slowDown = 0;

    } else {
        slowDown += 1;
    }
}

void IA::moveTowardsOpponent_hard(const grid& gameGrid, const player &opponent, game &gameInstance) {
    if (slowDown > 400) {
        canMove = true;
        
        // Récupérer la position du joueur à traquer (on suppose que c'est le joueur 1)
        int targetX = opponent.getX();
        int targetY = opponent.getY();

        // Calculer la meilleure direction vers le joueur
        int bestDirection = -1;
        int minDistance = std::abs(getX() - targetX) + std::abs(getY() - targetY);

        int dx[] = {0, 0, -1, 1}; // Gauche / Droite
        int dy[] = {-1, 1, 0, 0}; // Haut / Bas

        for (int i = 0; i < 4; ++i) {
            int newX = getX() + dx[i];
            int newY = getY() + dy[i];

            if (!gameGrid.isObstacle(newX, newY) && !isInDangerZone(newX, newY)) {
                int newDistance = std::abs(newX - targetX) + std::abs(newY - targetY);
                if (newDistance < minDistance) {
                    minDistance = newDistance;
                    bestDirection = i;
                }
            }
        }

        // Si une direction est trouvée, bouger vers elle
        if (bestDirection != -1) {
            move(dx[bestDirection], dy[bestDirection], gameGrid);
            slowDown = 0;
            return;
        }

        // Vérifier si l'IA est bloquée par un mur cassable
    for (int i = 0; i < 4; ++i) {
    int newX = getX() + dx[i];
    int newY = getY() + dy[i];

    std::cout << "Vérification de la case (" << newX << ", " << newY << ")..." << std::endl;

    // Vérifier si l'IA est bloquée par un mur cassable
    if (gameGrid.isObstacle(newX, newY) && gameGrid.isBreakable(newX, newY)) {
        std::cout << "Mur cassable détecté en (" << newX << ", " << newY << "), pose d'une bombe !" << std::endl;

        // Étape 1 : Trouver une case sûre pour se déplacer après avoir posé la bombe
        int safeX = -1, safeY = -1; // Coordonnées de la case sûre
        for (int j = 0; j < 4; ++j) {
            int potentialX = getX() + dx[j];
            int potentialY = getY() + dy[j];

            std::cout << "Vérification de la case sûre (" << potentialX << ", " << potentialY << ")..." << std::endl;

            if (!gameGrid.isObstacle(potentialX, potentialY)) {
                if (!isInDangerZone(potentialX, potentialY)) {
                    safeX = potentialX;
                    safeY = potentialY;
                    std::cout << "Case sûre trouvée en (" << safeX << ", " << safeY << ") !" << std::endl;
                    break; // On a trouvé une case sûre, on sort de la boucle
                } else {
                    std::cout << "Case (" << potentialX << ", " << potentialY << ") est dans une zone dangereuse." << std::endl;
                }
            } else {
                std::cout << "Case (" << potentialX << ", " << potentialY << ") est un obstacle." << std::endl;
            }
        }

        // Étape 2 : Poser la bombe
        std::cout << "Pose de la bombe en (" << getX() << ", " << getY() << ")..." << std::endl;
        gameInstance.placeBomb(getX(), getY(), this);

        // Étape 3 : Ajouter la position de la bombe à la liste des zones dangereuses
        DangerZone dz;
        dz.x = getX();
        dz.y = getY();
        dz.expirationTime = std::chrono::steady_clock::now() + std::chrono::seconds(4);
        dangerZones.push_back(dz);
        std::cout << "Bombe ajoutée aux zones dangereuses. Expiration à : "
                  << std::chrono::duration_cast<std::chrono::seconds>(dz.expirationTime.time_since_epoch()).count()
                  << "s" << std::endl;

        // Étape 4 : Se déplacer vers la case sûre (si une case sûre a été trouvée)
        if (safeX != -1 && safeY != -1) {
            std::cout << "Déplacement vers la case sûre en (" << safeX << ", " << safeY << ")..." << std::endl;
            move(safeX - getX(), safeY - getY(), gameGrid); // Déplacer l'IA
        } else {
            std::cout << "Aucune case sûre trouvée pour se déplacer après la pose de la bombe !" << std::endl;
        }

        // Étape 5 : Réinitialiser le délai et quitter la fonction
        slowDown = 0;
        return;
    } else {
        std::cout << "Case (" << newX << ", " << newY << ") n'est pas un mur cassable." << std::endl;
    }
}
        // Si aucune direction n'est trouvée, l'IA garde son mouvement aléatoire
        int randomDirection;
        for (int attempts = 0; attempts < 4; ++attempts) {
            randomDirection = std::rand() % 4;
            int newX = getX() + dx[randomDirection];
            int newY = getY() + dy[randomDirection];

            if (!gameGrid.isObstacle(newX, newY) && !isInDangerZone(newX, newY)) {
                move(dx[randomDirection], dy[randomDirection], gameGrid);
                break;
            }
        }

        slowDown = 0;
    } else {
        slowDown += 1;
    }
}

void IA::placeSmartBomb([[maybe_unused]] const grid& gameGrid, game &gameInstance, const player& opponent) {
    if (std::abs(getX() - opponent.getX()) + std::abs(getY() - opponent.getY()) <= 2) {
        if (canPlaceBomb()) {
            gameInstance.placeBomb(getX(), getY(), this);
        }
    }
}

void IA::draw(sf::RenderWindow &window) {
    window.draw(sprite);  // sprite est `protected` donc accessible
}