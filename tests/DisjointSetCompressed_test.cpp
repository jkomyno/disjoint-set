#include <vector>  // std::vector

#include "gtest/gtest.h"
#include "disjoint_set/DisjointSetCompressed.h"

using namespace disjoint_set;

class DisjointSetCompressedTest : public ::testing::Test {
protected:
    DisjointSetCompressedTest() {
    }

    std::vector<std::size_t> test_vector = {0, 1, 2, 3, 4, 5};
};

TEST_F(DisjointSetCompressedTest, InitiallyDisjoint) {
    auto d_set = DisjointSetCompressed(test_vector);

    for (const auto& v : test_vector) {
        ASSERT_EQ(v, d_set.find(v));
    }
}

TEST_F(DisjointSetCompressedTest, SimpleUnion) {
    auto d_set = DisjointSetCompressed(test_vector);

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

TEST_F(DisjointSetCompressedTest, ComplexUnion) {
    auto d_set = DisjointSetCompressed(test_vector);

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

TEST_F(DisjointSetCompressedTest, ComplexUnionSymmetric) {
    auto d_set = DisjointSetCompressed(test_vector);

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
