#include <gtest/gtest.h>
#include <data_structures/buffers/ring_buffer.hpp>
#include <string>
#include <vector>

using namespace ccl::ds::buffers;

TEST(RingBufferTest, DefaultConstructor)
{
    RingBuffer<int> buffer;
    EXPECT_TRUE(buffer.empty());
    EXPECT_EQ(buffer.size(), 0);
    EXPECT_EQ(buffer.capacity(), 0);
}

TEST(RingBufferTest, CapacityConstructor)
{
    RingBuffer<int> buffer(5);
    EXPECT_TRUE(buffer.empty());
    EXPECT_EQ(buffer.size(), 0);
    EXPECT_EQ(buffer.capacity(), 5);
}

TEST(RingBufferTest, VectorConstructorCopy)
{
    std::vector<int> v = {1, 2, 3};
    RingBuffer<int> buffer(v);
    EXPECT_EQ(buffer.size(), 3);
    EXPECT_EQ(buffer.capacity(), 3);
    EXPECT_EQ(buffer.front(), 1);
    EXPECT_EQ(buffer.back(), 3);
}

TEST(RingBufferTest, VectorConstructorMove)
{
    std::vector<int> v = {4, 5, 6};
    RingBuffer<int> buffer(std::move(v));
    EXPECT_EQ(buffer.size(), 3);
    EXPECT_EQ(buffer.capacity(), 3);
    EXPECT_EQ(buffer.front(), 4);
    EXPECT_EQ(buffer.back(), 6);
}

TEST(RingBufferTest, PutAndAccess)
{
    RingBuffer<int> buffer(3);
    buffer.put(10);
    buffer.put(20);
    buffer.put(30);

    EXPECT_EQ(buffer.size(), 3);
    EXPECT_EQ(buffer[0], 10);
    EXPECT_EQ(buffer[1], 20);
    EXPECT_EQ(buffer[2], 30);
}

TEST(RingBufferTest, WraparoundOverCapacity)
{
    RingBuffer<int> buffer(3);
    buffer.put(1);
    buffer.put(2);
    buffer.put(3);
    buffer.put(4); // Overwrites 1

    EXPECT_EQ(buffer.size(), 3);
    EXPECT_EQ(buffer[0], 2);
    EXPECT_EQ(buffer[1], 3);
    EXPECT_EQ(buffer[2], 4);
}

TEST(RingBufferTest, FrontBackMethods)
{
    RingBuffer<std::string> buffer(3);
    buffer.put("first");
    buffer.put("second");

    EXPECT_EQ(buffer.front(), "first");
    EXPECT_EQ(buffer.back(), "second");

    buffer.put("third");
    buffer.put("fourth");

    EXPECT_EQ(buffer.front(), "second");
    EXPECT_EQ(buffer.back(), "fourth");
}

TEST(RingBufferTest, AtBoundsCheck)
{
    RingBuffer<int> buffer(2);
    buffer.put(1);
    buffer.put(2);

    EXPECT_NO_THROW(buffer.at(1));
    EXPECT_THROW(buffer.at(2), std::out_of_range);
}

TEST(RingBufferTest, TryAt)
{
    RingBuffer<int> buffer(2);
    buffer.put(42);
    int out = 0;

    EXPECT_TRUE(buffer.tryAt(out, 0));
    EXPECT_EQ(out, 42);
    EXPECT_FALSE(buffer.tryAt(out, 2)); // out-of-bounds
}

TEST(RingBufferTest, PopFrontAndBack)
{
    RingBuffer<int> buffer(3);
    buffer.put(1);
    buffer.put(2);
    buffer.put(3);

    EXPECT_EQ(buffer.popFront(), 1);
    EXPECT_EQ(buffer.popBack(), 3);
    EXPECT_EQ(buffer.size(), 1);
    EXPECT_EQ(buffer.front(), 2);
}

TEST(RingBufferTest, TryPopFrontAndBack)
{
    RingBuffer<int> buffer(2);
    int out = 0;

    EXPECT_FALSE(buffer.tryPopFront(out));
    buffer.put(5);
    buffer.put(10);

    EXPECT_TRUE(buffer.tryPopFront(out));
    EXPECT_EQ(out, 5);

    EXPECT_TRUE(buffer.tryPopBack(out));
    EXPECT_EQ(out, 10);
}

TEST(RingBufferTest, ClearBuffer)
{
    RingBuffer<int> buffer(3);
    buffer.put(1);
    buffer.put(2);

    buffer.clear();

    EXPECT_TRUE(buffer.empty());
    EXPECT_EQ(buffer.size(), 0);
    EXPECT_EQ(buffer.capacity(), 3);
    EXPECT_THROW(buffer.at(0), std::out_of_range);
}
