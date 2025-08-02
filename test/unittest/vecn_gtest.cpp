#include <gtest/gtest.h>
#include <data_structures/grids/vec_n.hpp>
#include <array>
#include <vector>
#include <type_traits>

using namespace ccl::ds::grids;

// ------------------------- VectorN Tests -------------------------

TEST(VectorNTest, CopyConstructor) {
    constexpr size_t N = 5;
    VectorN<int, N> vec1;
    for (size_t i = 0; i < N; ++i) {
        vec1.at(i) = static_cast<int>(i + 1);
    }

    VectorN<int, N> vec2(vec1);

    for (size_t i = 0; i < N; ++i) {
        EXPECT_EQ(vec2.at(i), vec1.at(i));
    }
}

TEST(VectorNTest, CopyAssignment) {
    constexpr size_t N = 5;
    VectorN<int, N> vec1;
    for (size_t i = 0; i < N; ++i) {
        vec1.at(i) = static_cast<int>(10 * (i + 1));
    }

    VectorN<int, N> vec2;
    vec2 = vec1;

    for (size_t i = 0; i < N; ++i) {
        EXPECT_EQ(vec2.at(i), vec1.at(i));
    }
}

TEST(VectorNTest, MoveConstructor) {
    constexpr size_t N = 5;
    VectorN<int, N> vec1;
    for (size_t i = 0; i < N; ++i) {
        vec1.at(i) = i + 3;
    }

    VectorN<int, N> vec2(std::move(vec1));

    for (size_t i = 0; i < N; ++i) {
        EXPECT_EQ(vec2.at(i), i + 3);
    }
}

TEST(VectorNTest, TemplatedAssignmentConvertibleTypes) {
    constexpr size_t N = 4;
    VectorN<double, N> vec_d;
    for (size_t i = 0; i < N; ++i) {
        vec_d.at(i) = static_cast<double>(i + 0.5);
    }

    VectorN<int, N> vec_i;
    vec_i = vec_d;

    for (size_t i = 0; i < N; ++i) {
        EXPECT_EQ(vec_i.at(i), static_cast<int>(vec_d.at(i)));
    }
}

// ---------------------- DynamicVectorN Tests ----------------------

TEST(DynamicVectorNTest, ConstructorInitializesSize) {
    DynamicVectorN<int> vec(6);
    EXPECT_EQ(vec.size(), 6);
}

TEST(DynamicVectorNTest, CopyAssignmentSameSize) {
    DynamicVectorN<int> vec1(4);
    for (size_t i = 0; i < 4; ++i) {
        vec1.at(i) = static_cast<int>(i + 7);
    }

    DynamicVectorN<int> vec2(4);
    vec2 = vec1;

    for (size_t i = 0; i < 4; ++i) {
        EXPECT_EQ(vec2.at(i), vec1.at(i));
    }
}

TEST(DynamicVectorNTest, TemplatedAssignmentDifferentType) {
    DynamicVectorN<float> vec_f(3);
    for (size_t i = 0; i < 3; ++i) {
        vec_f.at(i) = static_cast<float>(i + 1.2f);
    }

    DynamicVectorN<int> vec_i(3);
    vec_i = vec_f;

    for (size_t i = 0; i < 3; ++i) {
        EXPECT_EQ(vec_i.at(i), static_cast<int>(vec_f.at(i)));
    }
}

TEST(DynamicVectorNTest, AssignmentWithResize) {
    DynamicVectorN<int> vec_large(6);
    for (size_t i = 0; i < 6; ++i) {
        vec_large.at(i) = i + 100;
    }

    DynamicVectorN<int> vec_small(2);
    vec_small = vec_large;

    ASSERT_EQ(vec_small.size(), vec_large.size());
    for (size_t i = 0; i < vec_large.size(); ++i) {
        EXPECT_EQ(vec_small.at(i), vec_large.at(i));
    }
}
