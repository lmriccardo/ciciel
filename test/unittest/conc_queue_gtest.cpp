#include <gtest/gtest.h>
#include <thread>
#include <vector>
#include <chrono>
#include <data_structures/queue/concurrent_queue.hpp>

using namespace ccl::ds::queue;

class ConcurrentQueueTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

// Basic push/pop
TEST_F(ConcurrentQueueTest, PushPopSingleThread) {
    ConcurrentQueue<int> q;
    EXPECT_TRUE(q.empty());
    EXPECT_EQ(q.size(), 0u);

    q.push(1);
    q.push(2);
    EXPECT_FALSE(q.empty());
    EXPECT_EQ(q.size(), 2u);

    int val = q.pop();
    EXPECT_EQ(val, 1);
    q.pop(val);
    EXPECT_EQ(val, 2);
    EXPECT_TRUE(q.empty());
}

// Try push/pop
TEST_F(ConcurrentQueueTest, TryPushTryPop) {
    ConcurrentQueue<int> q(2); // bounded
    EXPECT_TRUE(q.tryPush(10));
    EXPECT_TRUE(q.tryPush(20));
    EXPECT_FALSE(q.tryPush(30)); // full

    int val;
    EXPECT_TRUE(q.tryPop(val));
    EXPECT_EQ(val, 10);
    EXPECT_TRUE(q.tryPop(val));
    EXPECT_EQ(val, 20);
    EXPECT_FALSE(q.tryPop(val));
}

// Peek
TEST_F(ConcurrentQueueTest, PeekTest) {
    ConcurrentQueue<int> q;
    int val;
    EXPECT_FALSE(q.peek(val));

    q.push(42);
    EXPECT_TRUE(q.peek(val));
    EXPECT_EQ(val, 42);
    EXPECT_EQ(q.size(), 1u); // peek should not remove
}

// Full/empty/capacity
TEST_F(ConcurrentQueueTest, FullEmptyCapacity) {
    ConcurrentQueue<int> q(2);
    EXPECT_TRUE(q.empty());
    EXPECT_FALSE(q.full());

    q.push(1);
    EXPECT_FALSE(q.empty());
    EXPECT_FALSE(q.full());

    q.push(2);
    EXPECT_TRUE(q.full());
    EXPECT_EQ(q.capacity(), 2u);
}

// Blocking push behavior
TEST_F(ConcurrentQueueTest, BlockingPush) {
    ConcurrentQueue<int> q(1);
    q.push(1);

    std::thread t([&q]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        int val;
        q.pop(val); // unblock push
    });

    auto start = std::chrono::steady_clock::now();
    q.push(2); // should block until pop occurs
    auto end = std::chrono::steady_clock::now();

    auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    EXPECT_GE(duration_ms, 90); // blocked ~100ms
    t.join();
}

// Unlimited queue
TEST_F(ConcurrentQueueTest, UnlimitedQueue) {
    ConcurrentQueue<int> q; // unlimited
    for (int i = 0; i < 1000; ++i) {
        EXPECT_TRUE(q.tryPush(i));
    }
    EXPECT_EQ(q.size(), 1000u);
}

// Move semantics
TEST_F(ConcurrentQueueTest, PushMoveTest) {
    ConcurrentQueue<std::string> q;
    std::string s = "hello";

    q.push(std::move(s));
    EXPECT_EQ(q.size(), 1u);

    std::string val;
    q.pop(val);
    EXPECT_EQ(val, "hello");
}

// Copy constructor
TEST_F(ConcurrentQueueTest, CopyConstructor) {
    ConcurrentQueue<int> q1;
    q1.push(1);
    q1.push(2);

    ConcurrentQueue<int> q2(q1);
    EXPECT_EQ(q2.size(), 2u);
    int val;
    q2.pop(val);
    EXPECT_EQ(val, 1);
}

// Move constructor
TEST_F(ConcurrentQueueTest, MoveConstructor) {
    ConcurrentQueue<int> q1;
    q1.push(5);
    q1.push(10);

    ConcurrentQueue<int> q2(std::move(q1));
    EXPECT_EQ(q2.size(), 2u);
    int val;
    q2.pop(val);
    EXPECT_EQ(val, 5);
}

// Multithreaded producer/consumer
TEST_F(ConcurrentQueueTest, MultiThreaded) {
    constexpr int N = 1000;
    ConcurrentQueue<int> q(1000);

    std::thread producer([&]() {
        for (int i = 0; i < N; ++i) {
            q.push(i);
        }
    });

    std::thread consumer([&]() {
        int count = 0;
        int val;
        while (count < N) {
            if (q.tryPop(val)) ++count;
        }
    });

    producer.join();
    consumer.join();
    EXPECT_TRUE(q.empty());
}

// Setting capacity dynamically
TEST_F(ConcurrentQueueTest, SetCapacity) {
    ConcurrentQueue<int> q;
    q.capacity(10);
    EXPECT_EQ(q.capacity(), 10u);
    EXPECT_FALSE(q.full());

    for (int i = 0; i < 10; ++i) q.push(i);
    EXPECT_TRUE(q.full());
}

