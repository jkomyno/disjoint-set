#include "disjoint_set/DisjointSet.h"

#include <vector>  // std::vector

#include "gtest/gtest.h"

using namespace disjoint_set;

class DisjointSetTest : public ::testing::Test {
protected:
    DisjointSetTest() {
    }

    std::vector<std::size_t> test_vector = {0, 1, 2, 3, 4, 5};
};

TEST_F(DisjointSetTest, InitiallyDisjoint) {
    auto d_set = DisjointSet(test_vector);

    for (const auto& v : test_vector) {
        ASSERT_EQ(v, d_set.find(v));
    }
}

TEST_F(DisjointSetTest, SimpleUnion) {
    auto d_set = DisjointSet(test_vector);

    ASSERT_FALSE(d_set.are_connected(0, 1));
    ASSERT_FALSE(d_set.are_connected(2, 3));
    ASSERT_FALSE(d_set.are_connected(4, 5));

    d_set.unite(0, 1);
    ASSERT_TRUE(d_set.are_connected(0, 1));

    d_set.unite(2, 3);
    ASSERT_TRUE(d_set.are_connected(2, 3));

    d_set.unite(4, 5);
    ASSERT_TRUE(d_set.are_connected(4, 5));
}

TEST_F(DisjointSetTest, ComplexUnion) {
    auto d_set = DisjointSet(test_vector);

    d_set.unite(0, 1);
    d_set.unite(2, 3);
    d_set.unite(0, 2);

    ASSERT_TRUE(d_set.are_connected(0, 1));
    ASSERT_TRUE(d_set.are_connected(2, 3));
    ASSERT_TRUE(d_set.are_connected(0, 2));
    ASSERT_TRUE(d_set.are_connected(0, 3));
    ASSERT_FALSE(d_set.are_connected(4, 5));
    ASSERT_FALSE(d_set.are_connected(3, 4));
}

TEST_F(DisjointSetTest, ComplexUnionSymmetric) {
    auto d_set = DisjointSet(test_vector);

    d_set.unite(0, 1);
    d_set.unite(2, 3);
    d_set.unite(2, 0);

    ASSERT_TRUE(d_set.are_connected(0, 1));
    ASSERT_TRUE(d_set.are_connected(2, 3));
    ASSERT_TRUE(d_set.are_connected(2, 0));
    ASSERT_TRUE(d_set.are_connected(3, 0));
    ASSERT_FALSE(d_set.are_connected(5, 4));
    ASSERT_FALSE(d_set.are_connected(4, 3));
}
