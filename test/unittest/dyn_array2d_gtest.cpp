#include <gtest/gtest.h>
#include <data_structures/grids/dynamic_array2d.hpp>
#include <data_structures/base/enum.hpp>
#include <stdexcept>

using namespace ccl::ds::grids;
using namespace ccl::ds;

using TestArray = DynamicArray2D<int>;

class DynamicArray2DTest : public ::testing::Test {
protected:
    static constexpr size_t ROWS = 2;
    static constexpr size_t COLS = 3;
    TestArray array{ROWS, COLS};
};

TEST_F(DynamicArray2DTest, ConstructorInitializesCorrectSize) {
    EXPECT_EQ(array.size(), ROWS * COLS);
    EXPECT_EQ(array.size(static_cast<size_t>(Selector2D::ROWS)), ROWS);
    EXPECT_EQ(array.size(static_cast<size_t>(Selector2D::COLUMNS)), COLS);
}

TEST_F(DynamicArray2DTest, SquareConstructorWorks) {
    TestArray squareArray(4);
    EXPECT_EQ(squareArray.size(), 16);
}

TEST(DynamicArray2DErrorTest, InvalidSizeSelectorThrows) {
    TestArray arr(2, 2);
    EXPECT_THROW(arr.size(999), std::invalid_argument);
}

TEST(DynamicArray2DStandaloneTest, ConstructorFromVectorCorrectSize) {
    std::vector<int> data = {1, 2, 3, 4, 5, 6};
    TestArray arr(data, 2, 3);

    EXPECT_EQ(arr.size(), 6);
}

TEST(DynamicArray2DStandaloneTest, ConstructorFromVectorWrongSizeThrows) {
    std::vector<int> data = {1, 2, 3, 4, 5}; // Should be 6
    EXPECT_THROW(TestArray arr(data, 2, 3), std::invalid_argument);
}

TEST_F(DynamicArray2DTest, CopyConstructorPreservesValues) {
    array.set(42, 0, 1);
    TestArray copied(array);
    EXPECT_EQ(copied.at(0, 1), 42);
}

TEST_F(DynamicArray2DTest, MoveConstructorTransfersOwnership) {
    array.set(99, 1, 2);
    TestArray moved(std::move(array));
    EXPECT_EQ(moved.at(1, 2), 99);
}

TEST_F(DynamicArray2DTest, CopyAssignmentPreservesValues) {
    array.set(7, 0, 0);
    TestArray other(2, 3);
    other = array;
    EXPECT_EQ(other.at(0, 0), 7);
}

TEST_F(DynamicArray2DTest, MoveAssignmentTransfersOwnership) {
    array.set(123, 1, 1);
    TestArray other(2, 3);
    other = std::move(array);
    EXPECT_EQ(other.at(1, 1), 123);
}

TEST_F(DynamicArray2DTest, TransposeSwapsDimensions) {
    size_t before_rows = array.size(static_cast<size_t>(Selector2D::ROWS));
    size_t before_cols = array.size(static_cast<size_t>(Selector2D::COLUMNS));
    array.transpose();

    EXPECT_EQ(array.size(static_cast<size_t>(Selector2D::ROWS)), before_cols);
    EXPECT_EQ(array.size(static_cast<size_t>(Selector2D::COLUMNS)), before_rows);
}

TEST_F(DynamicArray2DTest, ExtendRowsIncreasesRowCount) {
    array.extend(Selector2D::ROWS, 2);
    EXPECT_EQ(array.size(static_cast<size_t>(Selector2D::ROWS)), ROWS + 2);
}

TEST_F(DynamicArray2DTest, ExtendColumnsIncreasesColumnCount) {
    array.set(1, 0, 0);
    array.extend(Selector2D::COLUMNS, 2);
    EXPECT_EQ(array.size(static_cast<size_t>(Selector2D::COLUMNS)), COLS + 2);
    EXPECT_EQ(array.at(0, 0), 1); // Ensure old data preserved
}

TEST_F(DynamicArray2DTest, ExtendBothDimensionsIncreasesSize) {
    array.extend(1, 2);
    EXPECT_EQ(array.size(static_cast<size_t>(Selector2D::ROWS)), ROWS + 1);
    EXPECT_EQ(array.size(static_cast<size_t>(Selector2D::COLUMNS)), COLS + 2);
}

TEST_F(DynamicArray2DTest, ExtendSymmetricallyIncreasesBoth) {
    array.extend(2);
    EXPECT_EQ(array.size(static_cast<size_t>(Selector2D::ROWS)), ROWS + 2);
    EXPECT_EQ(array.size(static_cast<size_t>(Selector2D::COLUMNS)), COLS + 2);
}
