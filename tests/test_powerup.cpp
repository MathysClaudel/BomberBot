
#define BOOST_TEST_MODULE PowerUpTest
#include <boost/test/included/unit_test.hpp>
#include "../src/powerup.h"

BOOST_AUTO_TEST_CASE(test_powerup_creation) {
    PowerUp p(5, 7, SPEED_UP);
    BOOST_CHECK_EQUAL(p.getX(), 5);
    BOOST_CHECK_EQUAL(p.getY(), 7);
    BOOST_CHECK_EQUAL(p.getType(), SPEED_UP);
}

BOOST_AUTO_TEST_CASE(test_powerup_bounds_dummy) {
    PowerUp p(2, 3, FLAME_UP);

    try {
        (void)p.getBounds();
        BOOST_CHECK(true);
    } catch (...) {
        BOOST_FAIL("getBounds() a levé une exception");
    }
}

