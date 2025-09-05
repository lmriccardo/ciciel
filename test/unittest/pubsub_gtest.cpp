#include <patterns/pub_sub/pub_sub_broker.hpp>
#include <patterns/pub_sub/pub_sub_event.hpp>
#include <patterns/pub_sub/publisher.hpp>
#include <patterns/pub_sub/subscriber.hpp>
#include <gtest/gtest.h>
#include <thread>
#include <vector>
#include <chrono>

using namespace ccl::dp::pub_sub;

// -----------------------------
// Mock Events
// -----------------------------
class TestEvent : public PubSubEvent {
public:
    TestEvent(std::string n) : m_name(std::move(n)) {}
    std::string name() const override { return m_name; }
private:
    std::string m_name;
};

class AnotherEvent : public PubSubEvent {
public:
    AnotherEvent(std::string n) : m_name(std::move(n)) {}
    std::string name() const override { return m_name; }
private:
    std::string m_name;
};

// -----------------------------
// Mock Subscribers
// -----------------------------
class MockSubscriber : public Subscriber {
public:
    std::vector<std::string> received_events;

    void notify(event_ptr& event) override {
        received_events.push_back(event->name());
    }
};

class MockAsyncSubscriber : public AsyncSubscriber {
public:
    std::vector<std::string> received_events;

    void consume(event_ptr event) override {
        received_events.push_back(event->name());
    }
};

// -----------------------------
// Test Fixture
// -----------------------------
class PubSubFixture : public ::testing::Test {
protected:
    void SetUp() override {
        broker = std::make_shared<PubSubBroker>();
        publisher = std::make_shared<Publisher>(broker);

        sub1 = std::make_shared<MockSubscriber>();
        sub2 = std::make_shared<MockSubscriber>();
        asyncSub = std::make_shared<MockAsyncSubscriber>();

        broker->subscribe<TestEvent>(sub1);
        broker->subscribe<TestEvent, AnotherEvent>(sub2);
        broker->subscribe<TestEvent, AnotherEvent>(asyncSub);

        asyncSub->start();
    }

    void TearDown() override {
        asyncSub->cancel();
        asyncSub->join();

        broker.reset();
        publisher.reset();
        sub1.reset();
        sub2.reset();
        asyncSub.reset();
    }

    std::shared_ptr<PubSubBroker> broker;
    std::shared_ptr<Publisher> publisher;
    std::shared_ptr<MockSubscriber> sub1;
    std::shared_ptr<MockSubscriber> sub2;
    std::shared_ptr<MockAsyncSubscriber> asyncSub;
};

// -----------------------------
// Tests
// -----------------------------

TEST_F(PubSubFixture, SingleEvent) {
    auto ev = std::make_shared<const TestEvent>("Event1");
    publisher->publish(ev);

    EXPECT_EQ(sub1->received_events.size(), 1);
    EXPECT_EQ(sub1->received_events[0], "Event1");

    EXPECT_EQ(sub2->received_events.size(), 1);
    EXPECT_EQ(sub2->received_events[0], "Event1");

    // Give async subscriber some time to process
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    EXPECT_EQ(asyncSub->received_events.size(), 1);
    EXPECT_EQ(asyncSub->received_events[0], "Event1");
}

TEST_F(PubSubFixture, MultipleEvents) {
    auto ev1 = std::make_shared<const TestEvent>("Event1");
    auto ev2 = std::make_shared<const AnotherEvent>("Event2");

    publisher->publish(ev1);
    publisher->publish(ev2);

    EXPECT_EQ(sub1->received_events.size(), 1);
    EXPECT_EQ(sub1->received_events[0], "Event1");

    EXPECT_EQ(sub2->received_events.size(), 2);
    EXPECT_EQ(sub2->received_events[0], "Event1");
    EXPECT_EQ(sub2->received_events[1], "Event2");

    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    EXPECT_EQ(asyncSub->received_events.size(), 2);
    EXPECT_EQ(asyncSub->received_events[0], "Event1");
    EXPECT_EQ(asyncSub->received_events[1], "Event2");
}

TEST_F(PubSubFixture, Unsubscribe) {
    broker->unsubscribe<TestEvent>(sub1);

    auto ev = std::make_shared<const TestEvent>("Event1");
    publisher->publish(ev);

    EXPECT_EQ(sub1->received_events.size(), 0); // unsubscribed
    EXPECT_EQ(sub2->received_events.size(), 1);
    EXPECT_EQ(sub2->received_events[0], "Event1");

    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    EXPECT_EQ(asyncSub->received_events.size(), 1);
}

TEST_F(PubSubFixture, SubscribeMultipleEvents) {
    class ThirdEvent : public PubSubEvent {
    public:
        std::string name() const override { return "ThirdEvent"; }
    };

    auto multiSub = std::make_shared<MockSubscriber>();
    broker->subscribe<TestEvent, AnotherEvent, ThirdEvent>(multiSub);

    auto ev1 = std::make_shared<const TestEvent>("Event1");
    auto ev2 = std::make_shared<const AnotherEvent>("Event2");
    auto ev3 = std::make_shared<const ThirdEvent>();

    publisher->publish(ev1);
    publisher->publish(ev2);
    publisher->publish(ev3);

    EXPECT_EQ(multiSub->received_events.size(), 3);
    EXPECT_EQ(multiSub->received_events[0], "Event1");
    EXPECT_EQ(multiSub->received_events[1], "Event2");
    EXPECT_EQ(multiSub->received_events[2], "ThirdEvent");
}

TEST_F(PubSubFixture, ConcurrentPublish) {
    auto ev1 = std::make_shared<const TestEvent>("Event1");
    auto ev2 = std::make_shared<const AnotherEvent>("Event2");

    auto thread1 = std::thread([&]() { publisher->publish(ev1); });
    auto thread2 = std::thread([&]() { publisher->publish(ev2); });

    thread1.join();
    thread2.join();

    // Check subscribers received events (order may vary)
    EXPECT_EQ(sub1->received_events.size(), 1);
    EXPECT_EQ(sub2->received_events.size(), 2);

    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    EXPECT_EQ(asyncSub->received_events.size(), 2);
}
