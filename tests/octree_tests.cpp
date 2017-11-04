//
// Created by Adrian Krupa on 11/2/17.
//

#include <gtest/gtest.h>
#include <glm/glm.hpp>
#include "octree_cpp.h"

using namespace AKOctree;

struct Point {
    glm::dvec3 position;
    double mass;
};

class OctreeTests : public ::testing::Test {

protected:

    void SetUp() override {
        ::testing::Test::SetUp();
    }

    void TearDown() override {
        ::testing::Test::TearDown();
    }
};

TEST_F (OctreeTests, OctreeDefaultConstructor) {
    auto o = new Octree<OctreeTests, OctreeTests, double>(4);
    ASSERT_NE(o, nullptr);
}

TEST_F (OctreeTests, OctreMaxItemsPerCellGetter) {
    auto o = new Octree<Point, Point, double>(4);
    ASSERT_EQ(4, o->getMaxItemsPerCell());
}

int main(int ac, char* av[])
{
    testing::InitGoogleTest(&ac, av);
    return RUN_ALL_TESTS();
}