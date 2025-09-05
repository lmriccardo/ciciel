#include <data_structures/queue/concurrent_circular_queue.hpp>
#include <data_structures/base/enum.hpp>
#include <gtest/gtest.h>
#include <thread>
#include <vector>
#include <chrono>

using namespace ccl::ds;
using namespace ccl::ds::queue;

class ConcurrentCircQueueTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

// Basic push/pop and size tests
TEST_F(ConcurrentCircQueueTest, PushPopSingleThread) {
    ConcurrentCircQueue<int> q(5);

    EXPECT_TRUE(q.empty());
    EXPECT_EQ(q.size(), 0u);
    EXPECT_EQ(q.capacity(), 5u);
    EXPECT_FALSE(q.full());

    q.push(1);
    q.push(2);
    EXPECT_EQ(q.size(), 2u);
    EXPECT_FALSE(q.empty());
    EXPECT_FALSE(q.full());

    int val = q.pop();
    EXPECT_EQ(val, 1);
    EXPECT_EQ(q.size(), 1u);

    q.pop(val);
    EXPECT_EQ(val, 2);
    EXPECT_TRUE(q.empty());
}

// Test tryPush and tryPop
TEST_F(ConcurrentCircQueueTest, TryPushTryPop) {
    ConcurrentCircQueue<int> q(2, LossPolicy::ERROR);

    EXPECT_TRUE(q.tryPush(10));
    EXPECT_TRUE(q.tryPush(20));
    EXPECT_FALSE(q.tryPush(30));

    int val;
    EXPECT_TRUE(q.tryPop(val));
    EXPECT_EQ(val, 10);
    EXPECT_TRUE(q.tryPop(val));
    EXPECT_EQ(val, 20);
    EXPECT_FALSE(q.tryPop(val)); // Should fail, queue empty
}

// Test peek
TEST_F(ConcurrentCircQueueTest, PeekTest) {
    ConcurrentCircQueue<int> q(3);
    int val;

    EXPECT_FALSE(q.peek(val)); // empty queue
    q.push(42);
    EXPECT_TRUE(q.peek(val));
    EXPECT_EQ(val, 42);
    EXPECT_EQ(q.size(), 1u); // peek should not remove
}

// Test full() behavior
TEST_F(ConcurrentCircQueueTest, FullEmptyTest) {
    ConcurrentCircQueue<int> q(2);
    EXPECT_TRUE(q.empty());
    EXPECT_FALSE(q.full());

    q.push(1);
    EXPECT_FALSE(q.empty());
    EXPECT_FALSE(q.full());

    q.push(2);
    EXPECT_TRUE(q.full());
}

// Test LossPolicy::ERROR
TEST_F(ConcurrentCircQueueTest, PushErrorPolicyThrows) {
    ConcurrentCircQueue<int> q(1, LossPolicy::ERROR);
    q.push(1);

    EXPECT_THROW(q.push(2), std::runtime_error);
}

// Test LossPolicy::BLOCK (basic thread blocking)
TEST_F(ConcurrentCircQueueTest, PushBlockPolicy) {
    ConcurrentCircQueue<int> q(1, LossPolicy::BLOCK);
    q.push(1);

    std::thread t([&]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        int val;
        q.pop(val); // This should unblock push
    });

    auto start = std::chrono::steady_clock::now();
    q.push(2); // Should block until pop occurs
    auto end = std::chrono::steady_clock::now();

    auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    EXPECT_GE(duration_ms, 90); // Should have blocked ~100ms

    t.join();
}

// Multithreaded push/pop test
TEST_F(ConcurrentCircQueueTest, MultithreadedPushPop) {
    constexpr int N = 1000;
    ConcurrentCircQueue<int> q(1000);

    std::thread producer([&]() {
        for (int i = 0; i < N; ++i) {
            q.push(i);
        }
    });

    std::thread consumer([&]() {
        int count = 0;
        int val;
        while (count < N) {
            if (q.tryPop(val)) {
                ++count;
            }
        }
    });

    producer.join();
    consumer.join();
    EXPECT_TRUE(q.empty());
}

// Move semantics
TEST_F(ConcurrentCircQueueTest, PushMoveTest) {
    ConcurrentCircQueue<std::string> q(2);
    std::string s = "hello";

    q.push(std::move(s));
    EXPECT_EQ(q.size(), 1u);
    std::string val;
    q.pop(val);
    EXPECT_EQ(val, "hello");
}

