CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++20 -I/opt/homebrew/opt/sfml/include
LDFLAGS = -L/opt/homebrew/opt/sfml/lib -lsfml-graphics -lsfml-window -lsfml-system

SRC = src/main.cpp src/game.cpp src/grid.cpp src/player.cpp src/bomb.cpp src/powerup.cpp src/menu.cpp src/ia.cpp src/human_player.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = bomberbot

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
