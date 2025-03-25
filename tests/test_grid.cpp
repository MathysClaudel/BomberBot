
#define BOOST_TEST_MODULE GridTest
#include <boost/test/included/unit_test.hpp>
#include "../src/grid.h"

BOOST_AUTO_TEST_CASE(test_remove_breakable_obstacle) {
    grid g;
    g.generateObstacles();

    bool tested = false;

    for (int x = 0; x < 15; ++x) {
        for (int y = 0; y < 11; ++y) {
            if (g.isBreakable(x, y)) {
                BOOST_TEST(g.isObstacle(x, y));  // doit être un obstacle
                g.removeObstacle(x, y);
                BOOST_CHECK(!g.isObstacle(x, y));  // doit être vide maintenant
                tested = true;
                break;
            }
        }
        if (tested) break;
    }

    BOOST_CHECK(tested);  // s'assurer qu'au moins un cas a été testé
}

