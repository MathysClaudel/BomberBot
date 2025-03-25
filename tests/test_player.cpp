
#define BOOST_TEST_MODULE PlayerTest
#include <boost/test/included/unit_test.hpp>
#include "../src/human_player.h"
#include "../src/player.h"
#include "../src/grid.h"
#include "../src/powerup.h"

// DummyGrid sans obstacle pour tester les mouvements
class DummyGrid : public grid {
public:
bool isObstacle([[maybe_unused]] int x, [[maybe_unused]] int y) const { return false; }
};

BOOST_AUTO_TEST_CASE(test_player_movement) {
    HumanPlayer p(1, "assets/skins/player1");
    DummyGrid g;
    p.resetMove();
    int startX = p.getX();
    int startY = p.getY();
    p.move(1, 0, g);
    BOOST_CHECK(p.getX() == startX + 1);
    BOOST_CHECK(p.getY() == startY);
}

BOOST_AUTO_TEST_CASE(test_powerup_collection) {
    HumanPlayer p(1, "assets/skins/player1");
    BOOST_CHECK(!p.hasShield());
    p.collectPowerUp(SHIELD);
    BOOST_CHECK(p.hasShield());
}
