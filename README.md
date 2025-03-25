# Projet C++ ROB4

Romain DARDE
Ayoub HADJAB
Mathys CLAUDEL

# BomberBot

BomberBot est un jeu multijoueur inspiré de Bomberman ou BombIt.
Le but étant de bloquer et faire exploser l'adversaire à l'aide de bombe sur une map qui possède des murs indestructibles pour pouvoir se protéger et des murs destructibles dans lesquels se cachent des super-pouvoirs comme par exemple le bouclier.

## Fonctionnalités

- Mode 1 vs 1 local ou joueur vs IA (3 niveaux de difficulté)
- IA intelligente (stratégie simple, moyenne, et avancée)
- Système de bonus : bombes, flammes, vitesse, bouclier
- Animations et effets visuels
- Interface SFML complète avec écran de menu et de fin

---

## Compilation

Assurez-vous d’avoir installé :

- SFML (>= 2.5)
- Boost (pour les tests unitaires)

### Compilation du jeu :

`make`
L’exécutable `./bomberbot` sera généré dans le dossier principal.

Attention, lors du make, il peut y avoir une erreur sur le chemin d'installation de SFML, pensez à verifier le chemin d'accès et modifier pour 
    CXXFLAGS = -Wall -Wextra -std=c++20 (-I/opt/homebrew/opt/sfml/include) // (path)
    LDFLAGS = (-L/opt/homebrew/opt/sfml/lib) -lsfml-graphics -lsfml-window -lsfml-system //(path)

### Compilation des tests :

```bash
cd tests
make
```

---

## Lancement

### Lancer le jeu :

```bash
./bomberbot
```

### Lancer les tests unitairement :

```bash
cd tests
./test_player
./test_grid
./test_bomb
./test_powerup
```

### Ou lancer tous les tests d’un coup :

```bash
make test
```

---

## Vérification mémoire avec Valgrind

Exécuter dans un terminal Linux (pas sous macOS) :

```bash
valgrind --leak-check=full ./bomberbot
```

---

## Arborescence

```
BomberBot/
├── src/            # Fichiers sources C++
├── assets/         # Textures et polices
├── tests/          # Fichiers de test Boost.Test
├── Makefile        # Compilation du jeu
├── README.md       # Ce fichier
└── bomberbot       # Exécutable compilé
```
