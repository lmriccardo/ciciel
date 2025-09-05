#include <gtest/gtest.h>
#include <thread>
#include <chrono>
#include <vector>
#include <string>
#include <future>
#include <data_structures/buffers/concurrent_ring_buffer.hpp>

using namespace ccl::ds::buffers;

TEST(ConcurrentRingBufferTest, CapacityConstructorAndBasicProps)
{
    ConcurrentRingBuffer<int> buffer(3);
    EXPECT_TRUE(buffer.empty());
    EXPECT_EQ(buffer.size(), 0);
    EXPECT_EQ(buffer.capacity(), 3);
}

TEST(ConcurrentRingBufferTest, VectorConstructorCopy)
{
    std::vector<int> v = {1, 2, 3};
    ConcurrentRingBuffer<int> buffer(v);
    EXPECT_EQ(buffer.size(), 3);
    EXPECT_EQ(buffer.capacity(), 3);
    EXPECT_EQ(buffer.front(), 1);
    EXPECT_EQ(buffer.back(), 3);
}

TEST(ConcurrentRingBufferTest, VectorConstructorMove)
{
    std::vector<int> v = {4, 5, 6};
    ConcurrentRingBuffer<int> buffer(std::move(v));
    EXPECT_EQ(buffer.size(), 3);
    EXPECT_EQ(buffer.capacity(), 3);
    EXPECT_EQ(buffer.front(), 4);
    EXPECT_EQ(buffer.back(), 6);
}

TEST(ConcurrentRingBufferTest, PutAndAccess)
{
    ConcurrentRingBuffer<int> buffer(3);
    buffer.put(10);
    buffer.put(20);
    buffer.put(30);

    EXPECT_EQ(buffer.size(), 3);
    EXPECT_EQ(buffer[0], 10);
    EXPECT_EQ(buffer[1], 20);
    EXPECT_EQ(buffer[2], 30);
}

TEST(ConcurrentRingBufferTest, WraparoundOverCapacity)
{
    ConcurrentRingBuffer<int> buffer(3);
    buffer.put(1);
    buffer.put(2);
    buffer.put(3);
    buffer.put(4); // Overwrites oldest (1)

    EXPECT_EQ(buffer.size(), 3);
    EXPECT_EQ(buffer[0], 2);
    EXPECT_EQ(buffer[1], 3);
    EXPECT_EQ(buffer[2], 4);
}

TEST(ConcurrentRingBufferTest, FrontBackMethods)
{
    ConcurrentRingBuffer<std::string> buffer(3);
    buffer.put("first");
    buffer.put("second");

    EXPECT_EQ(buffer.front(), "first");
    EXPECT_EQ(buffer.back(), "second");

    buffer.put("third");
    buffer.put("fourth");

    EXPECT_EQ(buffer.front(), "second");
    EXPECT_EQ(buffer.back(), "fourth");
}

TEST(ConcurrentRingBufferTest, AtBoundsCheck)
{
    ConcurrentRingBuffer<int> buffer(2);
    buffer.put(1);
    buffer.put(2);

    EXPECT_NO_THROW(buffer.at(1));
    EXPECT_THROW(buffer.at(2), std::out_of_range);
}

TEST(ConcurrentRingBufferTest, TryAt)
{
    ConcurrentRingBuffer<int> buffer(2);
    buffer.put(42);
    int out = 0;

    EXPECT_TRUE(buffer.tryAt(out, 0));
    EXPECT_EQ(out, 42);
    EXPECT_FALSE(buffer.tryAt(out, 2)); // out-of-bounds
}

TEST(ConcurrentRingBufferTest, TryPopFrontAndBack)
{
    ConcurrentRingBuffer<int> buffer(2);
    int out = 0;

    EXPECT_FALSE(buffer.tryPopFront(out));
    buffer.put(5);
    buffer.put(10);

    EXPECT_TRUE(buffer.tryPopFront(out));
    EXPECT_EQ(out, 5);

    EXPECT_TRUE(buffer.tryPopBack(out));
    EXPECT_EQ(out, 10);
}

TEST(ConcurrentRingBufferTest, PopFrontBlocksUntilItemAvailable)
{
    ConcurrentRingBuffer<int> buffer(1);

    // Launch async popFront that should block until an element is added
    auto future = std::async(std::launch::async, [&buffer] {
        return buffer.popFront();
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    EXPECT_NE(future.wait_for(std::chrono::milliseconds(0)), std::future_status::ready);

    buffer.put(99);
    EXPECT_EQ(future.get(), 99);
}

TEST(ConcurrentRingBufferTest, PopBackBlocksUntilItemAvailable)
{
    ConcurrentRingBuffer<int> buffer(1);

    // Launch async popBack that should block until an element is added
    auto future = std::async(std::launch::async, [&buffer] {
        return buffer.popBack();
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    EXPECT_NE(future.wait_for(std::chrono::milliseconds(0)), std::future_status::ready);

    buffer.put(77);
    EXPECT_EQ(future.get(), 77);
}

TEST(ConcurrentRingBufferTest, ConcurrentPutAndPop)
{
    ConcurrentRingBuffer<int> buffer(10);

    // Producer thread adds elements
    std::thread producer([&buffer]() {
        for (int i = 0; i < 10; ++i) {
            buffer.put(i);
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    });

    // Consumer thread pops elements
    std::thread consumer([&buffer]() {
        for (int i = 0; i < 10; ++i) {
            int val = buffer.popFront();
            EXPECT_GE(val, 0);
            EXPECT_LT(val, 10);
        }
    });

    producer.join();
    consumer.join();

    EXPECT_TRUE(buffer.empty());
}

TEST(ConcurrentRingBufferTest, OperatorBracketAccessConstAndNonConst)
{
    ConcurrentRingBuffer<int> buffer(3);
    buffer.put(5);
    buffer.put(10);

    const ConcurrentRingBuffer<int>& cbuffer = buffer;
    EXPECT_EQ(cbuffer[0], 5);
    EXPECT_EQ(buffer[1], 10);

    buffer[1] = 15;
    EXPECT_EQ(cbuffer[1], 15);
}

