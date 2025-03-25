
#define BOOST_TEST_MODULE BombTest
#include <boost/test/included/unit_test.hpp>
#include "../src/bomb.h"
#include "../src/player.h"

class DummyPlayer : public player {
public:
    DummyPlayer() : player(1, "assets/skins/player1") {}
    void update() override {}
    void draw(sf::RenderWindow&) override {}
};

class TestBomb : public bomb {
    public:
        TestBomb(int x, int y, player* p) : bomb(x, y, p) {
            setTimer(10);
        }
    };    

BOOST_AUTO_TEST_CASE(test_bomb_initial_state) {
    DummyPlayer p;

    // Bombe normale
    bomb b(3, 5, &p);
    BOOST_CHECK_EQUAL(b.getX(), 3);
    BOOST_CHECK_EQUAL(b.getY(), 5);
    BOOST_CHECK_EQUAL(b.getOwner(), &p);
    BOOST_CHECK(b.isActive());
    BOOST_CHECK(!b.isExploded());
}
    
BOOST_AUTO_TEST_CASE(test_bomb_explosion_state) {
    DummyPlayer p;
    TestBomb b(0, 0, &p);  // timer à 10

    for (int i = 0; i < 20; ++i) {
        b.update();
    }

    BOOST_CHECK(b.isExploded());
    BOOST_CHECK(!b.isActive());
}

