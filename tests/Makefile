CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++20 -I../src -I/opt/homebrew/include

LDFLAGS = -L/opt/homebrew/lib -lsfml-graphics -lsfml-window -lsfml-system

TESTS = test_player test_grid test_bomb test_powerup

all: $(TESTS)

test_player: test_player.o ../src/player.o ../src/human_player.o ../src/powerup.o ../src/grid.o
	$(CXX) $^ -o $@ $(LDFLAGS)

test_grid: test_grid.o ../src/grid.o
	$(CXX) $^ -o $@ $(LDFLAGS)

test_bomb: test_bomb.o ../src/bomb.o ../src/player.o ../src/grid.o
	$(CXX) $^ -o $@ $(LDFLAGS)

test_powerup: test_powerup.o ../src/powerup.o
	$(CXX) $^ -o $@ -L/opt/homebrew/lib -lsfml-graphics -lsfml-window -lsfml-system

test: $(TESTS)
	cd .. && ./tests/test_player && ./tests/test_grid && ./tests/test_bomb && ./tests/test_powerup

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TESTS) *.o
