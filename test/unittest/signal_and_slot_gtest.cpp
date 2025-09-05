#include <patterns/signals_slot/signals_slot.hpp>  // Adjust as needed
#include <gtest/gtest.h>
#include <memory>
#include <string>
#include <mutex>
#include <vector>
#include <thread>

using namespace ccl::dp::signals;

// ---------------------------------------------
// Mock-like Receiver class for test callbacks
// ---------------------------------------------
class Receiver {
public:
    void onSignalInt(int x) { receivedInt = x; }
    void onSignalString(const std::string& msg) { receivedStr = msg; }

    int receivedInt = 0;
    std::string receivedStr;
};

// ---------------------------------------------
// Test Fixture for Signal<int> related tests
// ---------------------------------------------
class IntSignalTest : public ::testing::Test {
protected:
    Signal<int> signal;
    std::shared_ptr<Receiver> receiver;

    void SetUp() override {
        receiver = std::make_shared<Receiver>();
    }

    void TearDown() override {
        receiver.reset();
    }
};

// ---------------------------------------------
// Test Fixture for Signal<std::string> tests
// ---------------------------------------------
class StringSignalTest : public ::testing::Test {
protected:
    Signal<std::string> signal;
    std::shared_ptr<Receiver> receiver;

    void SetUp() override {
        receiver = std::make_shared<Receiver>();
    }

    void TearDown() override {
        receiver.reset();
    }
};

// ---------------------------------------------
//               TEST CASES
// ---------------------------------------------

TEST_F(IntSignalTest, BasicEmit) {
    auto conn = signal.connect([&](int x) { receiver->receivedInt = x; });
    signal.emit(42);
    EXPECT_EQ(receiver->receivedInt, 42);
    EXPECT_TRUE(conn.isConnected());
}

TEST_F(IntSignalTest, ManualDisconnect) {
    int value = 0;
    auto conn = signal.connect([&](int x) { value = x; });
    conn.disconnect();
    signal.emit(99);
    EXPECT_EQ(value, 0);
    EXPECT_FALSE(conn.isConnected());
}

TEST_F(IntSignalTest, MultipleConnections) {
    int a = 0, b = 0;
    signal.connect([&](int x) { a = x; });
    signal.connect([&](int x) { b = x; });

    signal.emit(7);
    EXPECT_EQ(a, 7);
    EXPECT_EQ(b, 7);
    EXPECT_EQ(signal.getNofConnections(), 2);
}

TEST_F(IntSignalTest, LifetimeTokenAutoDisconnect) {
    int value = 0;
    {
        auto token = std::make_shared<int>(5);
        signal.connect([&](int x) { value = x; }, token);
        signal.emit(10);
        EXPECT_EQ(value, 10);
    }

    signal.emit(99);  // Token expired
    EXPECT_EQ(value, 10);
    EXPECT_EQ(signal.getNofConnections(), 0);
}

TEST_F(IntSignalTest, MakeCallbackBindsCorrectly) {
    signal.connect(make_callback(&Receiver::onSignalInt, receiver), receiver);
    signal.emit(123);
    EXPECT_EQ(receiver->receivedInt, 123);
}

TEST_F(IntSignalTest, SelfDisconnectAfterTokenExpire) {
    auto token = std::make_shared<int>(1);
    auto conn = signal.connect([](int) {}, token);
    EXPECT_TRUE(conn.isConnected());

    token.reset();  // Expire the token
    signal.emit(1);
    EXPECT_FALSE(conn.isConnected());
}

TEST_F(IntSignalTest, EmitAfterReceiverDestroyed) {
    int backup = 0;
    {
        auto rcv = std::make_shared<Receiver>();
        signal.connect(make_callback(&Receiver::onSignalInt, rcv), rcv);
        signal.connect([&](int x) { backup = x; });  // alive
        signal.emit(55);
        EXPECT_EQ(rcv->receivedInt, 55);
    }

    signal.emit(88);
    EXPECT_EQ(backup, 88);
}

TEST_F(IntSignalTest, ThreadSafetyEmit) {
    std::mutex mtx;
    int sum = 0;

    signal.connect([&](int x) {
        std::lock_guard<std::mutex> lock(mtx);
        sum += x;
    });

    std::vector<std::thread> threads;
    for (int i = 0; i < 10; ++i) {
        threads.emplace_back([&]() {
            signal.emit(1);
        });
    }

    for (auto& t : threads) t.join();
    EXPECT_EQ(sum, 10);
}

TEST_F(StringSignalTest, EmitsCorrectString) {
    signal.connect(make_callback(&Receiver::onSignalString, receiver), receiver);
    signal.emit("hello world");
    EXPECT_EQ(receiver->receivedStr, "hello world");
}
