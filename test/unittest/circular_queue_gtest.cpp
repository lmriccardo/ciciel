#include <gtest/gtest.h>
#include <data_structures/queue/circular_queue.hpp>
#include <data_structures/base/enum.hpp>

using namespace ccl::ds::queue;
using namespace ccl::ds;

class CircularQueueTest : public ::testing::Test {
protected:
    CircularQueue<int> queue;

    void SetUp() override {
        queue = CircularQueue<int>(3, LossPolicy::ERROR);
    }
};

TEST_F(CircularQueueTest, InitializeEmptyQueue) {
    EXPECT_TRUE(queue.empty());
    EXPECT_EQ(queue.size(), 0);
    EXPECT_EQ(queue.capacity(), 3);
    EXPECT_FALSE(queue.full());
    EXPECT_EQ(queue.getLossPolicy(), LossPolicy::ERROR);
}

TEST_F(CircularQueueTest, PushAndPopSingleElement) {
    queue.push(42);
    EXPECT_EQ(queue.size(), 1);
    EXPECT_FALSE(queue.empty());

    int value;
    queue.pop(value);
    EXPECT_EQ(value, 42);
    EXPECT_TRUE(queue.empty());
}

TEST_F(CircularQueueTest, PushToFullQueueThrowsWithErrorPolicy) {
    queue.push(1);
    queue.push(2);
    queue.push(3);
    EXPECT_TRUE(queue.full());

    EXPECT_THROW(queue.push(4), std::runtime_error);
}

TEST_F(CircularQueueTest, TryPushFailsWhenFullAndErrorPolicy) {
    queue.push(1);
    queue.push(2);
    queue.push(3);
    EXPECT_FALSE(queue.tryPush(99));
}

TEST(CircularQueueOverwriteTest, OverwriteOldestWhenFull) {
    CircularQueue<int> q(3, LossPolicy::OVERWRITE_OLDEST);
    q.push(10);
    q.push(20);
    q.push(30);
    q.push(40); // Should overwrite 10

    int val;
    q.pop(val);
    EXPECT_EQ(val, 20);
    q.pop(val);
    EXPECT_EQ(val, 30);
    q.pop(val);
    EXPECT_EQ(val, 40);
}

TEST_F(CircularQueueTest, TryPopReturnsFalseWhenEmpty) {
    CircularQueue<int> q(2);
    int val = -1;
    EXPECT_FALSE(q.tryPop(val));
}

TEST_F(CircularQueueTest, PeekReturnsFrontWithoutPopping) {
    CircularQueue<int> q(2);
    q.push(77);

    int val = -1;
    EXPECT_TRUE(q.peek(val));
    EXPECT_EQ(val, 77);

    q.pop(val);
    EXPECT_EQ(val, 77);
    EXPECT_TRUE(q.empty());
}

TEST_F(CircularQueueTest, PopThrowsWhenEmpty) {
    CircularQueue<int> q(2);
    EXPECT_THROW(q.pop(), std::out_of_range);
}

TEST_F(CircularQueueTest, TryPushAndTryPop) {
    CircularQueue<int> q(2);
    EXPECT_TRUE(q.tryPush(100));
    EXPECT_TRUE(q.tryPush(200));

    int val;
    EXPECT_TRUE(q.tryPop(val));
    EXPECT_EQ(val, 100);

    EXPECT_TRUE(q.tryPop(val));
    EXPECT_EQ(val, 200);
    EXPECT_FALSE(q.tryPop(val));
}

TEST_F(CircularQueueTest, MoveConstructorPreservesState) {
    CircularQueue<int> original(2, LossPolicy::ERROR);
    original.push(1);
    original.push(2);

    CircularQueue<int> moved(std::move(original));
    int val;
    moved.pop(val);
    EXPECT_EQ(val, 1);
}

TEST_F(CircularQueueTest, CopyConstructorCopiesElements) {
    CircularQueue<int> original(2, LossPolicy::ERROR);
    original.push(5);
    original.push(6);

    CircularQueue<int> copy(original);
    int val;
    copy.pop(val);
    EXPECT_EQ(val, 5);
    copy.pop(val);
    EXPECT_EQ(val, 6);
}
