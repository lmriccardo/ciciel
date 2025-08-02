#include <gtest/gtest.h>
#include <stdexcept>
#include <data_structures/grids/array2d.hpp>
#include <data_structures/base/enum.hpp>

using namespace ccl::ds::grids;
using namespace ccl::ds;

class Array2DTest : public ::testing::Test {
protected:
    static constexpr size_t Rows = 2;
    static constexpr size_t Cols = 3;
    using TestArray2D = Array2D<int, Rows, Cols>;
};

TEST_F(Array2DTest, DefaultConstructor) {
    TestArray2D arr;
    EXPECT_EQ(arr.size(), Rows * Cols);
    EXPECT_EQ(arr.size(static_cast<size_t>(Selector2D::ROWS)), Rows);
    EXPECT_EQ(arr.size(static_cast<size_t>(Selector2D::COLUMNS)), Cols);
}

TEST_F(Array2DTest, ConstructorFromVector) {
    std::vector<int> data = {1, 2, 3, 4, 5, 6};
    TestArray2D arr(data);
    EXPECT_EQ(arr.at(0, 0), 1);
    EXPECT_EQ(arr.at(1, 2), 6);
}

TEST_F(Array2DTest, ConstructorFromArray) {
    std::array<int, Rows * Cols> data = {10, 20, 30, 40, 50, 60};
    TestArray2D arr(data);
    EXPECT_EQ(arr.at(0, 1), 20);
    EXPECT_EQ(arr.at(1, 2), 60);
}

TEST_F(Array2DTest, ConstructorFromVectorThrowsOnWrongSize) {
    std::vector<int> tooShort = {1, 2, 3};
    std::vector<int> tooLong = {1, 2, 3, 4, 5, 6, 7};

    EXPECT_THROW(TestArray2D arr(tooShort), std::runtime_error);
    EXPECT_THROW(TestArray2D arr(tooLong), std::runtime_error);
}

TEST_F(Array2DTest, CopyConstructor) {
    TestArray2D original;
    original.set(42, 1, 1);
    TestArray2D copy(original);
    EXPECT_EQ(copy.at(1, 1), 42);
}

TEST_F(Array2DTest, MoveConstructor) {
    TestArray2D original;
    original.set(99, 0, 2);
    TestArray2D moved(std::move(original));
    EXPECT_EQ(moved.at(0, 2), 99);
}

TEST_F(Array2DTest, CopyAssignment) {
    TestArray2D a;
    a.set(7, 1, 0);
    TestArray2D b;
    b = a;
    EXPECT_EQ(b.at(1, 0), 7);
}

TEST_F(Array2DTest, MoveAssignment) {
    TestArray2D a;
    a.set(8, 0, 1);
    TestArray2D b;
    b = std::move(a);
    EXPECT_EQ(b.at(0, 1), 8);
}

TEST_F(Array2DTest, SizeThrowsOnInvalidDimension) {
    TestArray2D arr;
    EXPECT_THROW(arr.size(3), std::invalid_argument);
}

TEST_F(Array2DTest, DeepTransposeWorksCorrectly) {
    TestArray2D original;
    original.set(1, 0, 0);
    original.set(2, 0, 1);
    original.set(3, 0, 2);
    original.set(4, 1, 0);
    original.set(5, 1, 1);
    original.set(6, 1, 2);

    auto transposed = original.deepTranspose(); // Should be Array2D<int, 3, 2>

    EXPECT_EQ(transposed.size(), 6u);
    EXPECT_EQ(transposed.at(0, 0), 1);
    EXPECT_EQ(transposed.at(1, 0), 2);
    EXPECT_EQ(transposed.at(2, 0), 3);
    EXPECT_EQ(transposed.at(0, 1), 4);
    EXPECT_EQ(transposed.at(1, 1), 5);
    EXPECT_EQ(transposed.at(2, 1), 6);
}
