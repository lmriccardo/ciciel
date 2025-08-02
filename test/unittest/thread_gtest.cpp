#include "gtest/gtest.h"
#include <sys/concurrent/thread.hpp>
#include <chrono>
#include <string>
#include <vector>
#include <mutex>
#include <stdexcept>
#include <pthread.h>

// Using the correct namespace
using namespace ccl::sys::concurrent;

// --- Helper Classes for Testing ---

// A simple derived class to test the pure virtual run() method
class SimpleRunnableThread : public Thread {
public:
    std::atomic<bool> run_called;
    std::atomic<bool> tear_down_called;
    std::atomic<int> run_iterations;
    std::chrono::milliseconds work_duration;
    std::atomic<bool> should_exit_cooperatively; // For AT_CONDITION_CHECK

    SimpleRunnableThread(const std::string& name = "GenericThread", bool daemon = false,
                         CancellationPolicy policy = CancellationPolicy::DEFERRED,
                         std::chrono::milliseconds duration = std::chrono::milliseconds(0))
        : Thread(name, daemon, policy),
          run_called(false),
          tear_down_called(false),
          run_iterations(0),
          work_duration(duration),
          should_exit_cooperatively(false) {}

protected:
    void run() override {
        run_called.store(true);
        // std::cout << "[" << getName() << "] Thread running..." << std::endl; // For debugging
        while (!should_exit_cooperatively.load() && !isCancelled()) {
            
            // Simulate work
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            run_iterations++;
            // For AT_CANCELLATION_POINT, make sure there are cancellation points
            if (getCancellationPolicy() == CancellationPolicy::AT_CANCELLATION_POINT) {
                pthread_testcancel(); // Explicit cancellation point
            }
            if (work_duration.count() > 0 && run_iterations * 10 >= work_duration.count()) {
                break; // Stop after a certain duration for timed tests
            }
            
        }
        // std::cout << "[" << getName() << "] Thread finished run." << std::endl;
    }

    void tearDown() override {
        tear_down_called.store(true);
        // std::cout << "[" << getName() << "] TearDown called." << std::endl; // For debugging
    }
};

// --- Test Fixtures ---
class ThreadTest : public ::testing::Test {
protected:
    // Set up before each test
    void SetUp() override {
        // Any common setup if needed
    }

    // Tear down after each test
    void TearDown() override {
        // Any common cleanup if needed
    }
};

// --- Test Cases ---

// 1 Test default constructor and initial state
TEST_F(ThreadTest, DefaultConstructorInitialState) {
    SimpleRunnableThread t;
    EXPECT_EQ(t.getName(), "GenericThread");
    EXPECT_FALSE(t.isDaemon());
    EXPECT_EQ(t.getCancellationPolicy(), CancellationPolicy::DEFERRED);
    EXPECT_FALSE(t.isStarted());
    EXPECT_FALSE(t.isCancelled());
    EXPECT_FALSE(t.isJoinable());
    EXPECT_EQ(t.getId(), std::thread::id()); // Default constructed ID
}

// 2 Test parameterized constructor
TEST_F(ThreadTest, ParameterizedConstructor) {
    SimpleRunnableThread t("MyNamedThread", true, CancellationPolicy::AT_CONDITION_CHECK);
    EXPECT_EQ(t.getName(), "MyNamedThread");
    EXPECT_TRUE(t.isDaemon());
    EXPECT_EQ(t.getCancellationPolicy(), CancellationPolicy::AT_CONDITION_CHECK);
}

// 3 Test start() method and basic life cycle
TEST_F(ThreadTest, StartAndJoin) {
    SimpleRunnableThread t("TestThread", false, CancellationPolicy::DEFERRED, std::chrono::milliseconds(50));
    EXPECT_FALSE(t.isStarted());
    t.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Give thread a moment to start
    EXPECT_TRUE(t.isStarted());
    std::cout << t.run_called.load();
    EXPECT_TRUE(t.run_called.load());
    std::cout << t.getId() << " " << std::thread::id();
    EXPECT_NE(t.getId(), std::thread::id()); // Should have a valid ID
    EXPECT_TRUE(t.isJoinable());

    t.join();
    EXPECT_TRUE(t.isStarted()); // After join, it's considered not started (completed)
    EXPECT_FALSE(t.isJoinable()); // Should no longer be joinable after join
}

// 4 Test detach() functionality
TEST_F(ThreadTest, DetachBehavior) {
    SimpleRunnableThread t("DaemonThread", true, CancellationPolicy::DEFERRED, std::chrono::milliseconds(50));
    t.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Give thread a moment to start
    // t.detach(); // Explicitly detach
    EXPECT_TRUE(t.isDaemon()); // Should be daemon now
    EXPECT_FALSE(t.isJoinable()); // Should not be joinable
    // Give it time to finish
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    EXPECT_TRUE(t.run_called.load()); // Task should have run
}

// 5 Test Thread::start() static factory method with lambda
TEST_F(ThreadTest, StaticStartWithLambda) {
    std::atomic<bool> lambda_executed(false);
    Thread_ptr my_thread_ptr = Thread::start(
        [&]() {
            lambda_executed.store(true);
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        },
        false, // not daemon
        CancellationPolicy::DEFERRED
    );

    std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Give time to start
    ASSERT_TRUE(my_thread_ptr->isStarted());
    ASSERT_TRUE(my_thread_ptr->isJoinable());

    my_thread_ptr->join();
    EXPECT_TRUE(lambda_executed.load());
    EXPECT_FALSE(my_thread_ptr->isJoinable()); // Should not be joinable after join
}

// 6 Test cancellation policy: DEFERRED
TEST_F(ThreadTest, CancellationPolicyDeferred) {
    SimpleRunnableThread t("DeferredThread", false, CancellationPolicy::DEFERRED, std::chrono::milliseconds(200));
    t.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(50)); // Let it run
    t.cancel(); // Call cancel
    EXPECT_FALSE(t.isCancelled()) << "Deferred policy: isCancelled() should remain false on cancel() call.";
    // EXPECT_TRUE(t.isRunning()) << "Deferred policy: Thread should continue running after cancel().";

    // Wait for the thread to complete naturally
    EXPECT_FALSE(t.tear_down_called.load()) << "TearDown should not be called by cancel() in deferred.";
    t.join();
    EXPECT_TRUE(t.run_called.load()); // Should have run
    EXPECT_TRUE(t.tear_down_called.load()) << "TearDown should be called by join() in deferred.";
}

// 7 Test cancellation policy: AT_CONDITION_CHECK
TEST_F(ThreadTest, CancellationPolicyAtConditionCheck) {
    SimpleRunnableThread t("ConditionCheckThread", false, CancellationPolicy::AT_CONDITION_CHECK);
    t.should_exit_cooperatively.store(false); // Ensure it keeps running initially
    t.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(50)); // Let it run a bit

    EXPECT_FALSE(t.isCancelled());
    int initial_iterations = t.run_iterations.load();

    t.cancel(); // This should set m_cancelled to true
    EXPECT_TRUE(t.isCancelled()) << "AT_CONDITION_CHECK policy: isCancelled() should be true after cancel().";

    // Give the thread time to check and exit
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    EXPECT_TRUE(t.run_iterations.load() > initial_iterations) << "Thread should have run more after cancel.";
    EXPECT_TRUE(t.run_called.load()); // Should have run
    t.join(); // Ensure it finishes
    EXPECT_TRUE(t.tear_down_called.load()) << "TearDown should be called on stop/destruction.";
}

// 8 Test stop() method which calls cancel() then join() then tearDown()
TEST_F(ThreadTest, StopMethod) {
    SimpleRunnableThread t("StopTestThread", false, CancellationPolicy::AT_CONDITION_CHECK);
    t.should_exit_cooperatively.store(false);
    t.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(50)); // Let it run a bit

    EXPECT_FALSE(t.isCancelled());
    EXPECT_FALSE(t.tear_down_called.load());

    t.stop(); // This should trigger cancel, join, and tearDown

    EXPECT_TRUE(t.isCancelled()) << "stop() should call cancel() setting isCancelled.";
    EXPECT_TRUE(t.tear_down_called.load()) << "tearDown() should be called by stop().";
    EXPECT_FALSE(t.isJoinable()) << "Thread should not be joinable after stop() and join().";
    EXPECT_TRUE(t.isStarted()) << "Thread should not be started after stop().";
}


// Test that setAffinity doesn't crash (actual verification is hard in unit tests)
TEST_F(ThreadTest, SetAffinityDoesNotCrash) {
    SimpleRunnableThread t("AffinityThread", false,  CancellationPolicy::DEFERRED,  std::chrono::milliseconds(100));
    t.start();
    // Assuming a system with at least 2 CPUs, try setting to CPU 0 or 1
    // This mostly tests that the call doesn't throw or crash.
    // Real verification needs OS-level tools or mocks for pthread_setaffinity_np
    ASSERT_NO_THROW(t.setAffinity(0));
    // Try setting to an invalid CPU (may or may not throw depending on pthread behavior)
    // For this example, we assume it's graceful due to std::cerr.
    // If you need to test error cases, consider mocking pthread_setaffinity_np.
    ASSERT_NO_THROW(t.setAffinity(9999)); // Large invalid CPU
    t.join();
}

// Test calling start() multiple times (should only start once)
TEST_F(ThreadTest, MultipleStartsOnlyStartsOnce) {
    SimpleRunnableThread t("MultiStartThread", false,  CancellationPolicy::AT_CANCELLATION_POINT);
    EXPECT_FALSE(t.isStarted());
    t.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    EXPECT_TRUE(t.isStarted());
    std::thread::id original_id = t.getId();
    int initial_iterations = t.run_iterations.load();

    t.start(); // Call start again
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    EXPECT_EQ(t.getId(), original_id) << "Thread ID should not change on second start call.";
    EXPECT_TRUE(t.run_iterations.load() > initial_iterations) << "Thread should continue running, not restart.";
    EXPECT_TRUE(t.isStarted()) << "Thread should still be marked as started.";

    t.cancel();
}

// Test CancellationPolicy::AT_CANCELLATION_POINT
// WARNING: This can be tricky and may lead to resource leaks if not handled carefully
// The test ensures the thread stops and tearDown is called.
TEST_F(ThreadTest, CancellationPolicyAtCancellationPoint) {
    // Setup a runnable that has a cancellation point and handles cleanup
    class CancellableThread : public Thread {
    public:
        std::atomic<bool> run_entered;
        std::atomic<bool> tear_down_executed;
        std::atomic<bool> main_loop_exited;
        std::atomic<int> loop_iterations;

        CancellableThread(const std::string& name)
            : Thread(name, false, CancellationPolicy::AT_CANCELLATION_POINT),
              run_entered(false), tear_down_executed(false), main_loop_exited(false), loop_iterations(0) {}

    protected:
        void run() override {
            run_entered.store(true);
            // std::cout << "[" << getName() << "] Thread: run() entered." << std::endl;
            // Set thread to async cancellation for immediate effect if desired,
            // but AT_CANCELLATION_POINT means it will be deferred until a point.
            // pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, nullptr);
            // pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, nullptr);

            try {
                while (true) {
                    loop_iterations++;
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                    pthread_testcancel(); // Explicit cancellation point
                    // std::cout << "[" << getName() << "] Thread: Loop iteration " << loop_iterations.load() << std::endl;
                }
            } catch (const std::runtime_error& e) {
                // This catch block might not be reached if cancellation is abrupt.
                // It's for cooperative exit or if an exception is thrown.
                std::cout << "[" << getName() << "] Thread caught exception: " << e.what() << std::endl;
            } 
            catch (...) {
                std::cout << "[" << getName() << "] Thread caught unknown exception." << std::endl;
                main_loop_exited.store(true);
            
                throw;
            }
            // main_loop_exited.store(true);
            // std::cout << "[" << getName() << "] Thread: run() exited." << std::endl;
        }

        void tearDown() override {
            tear_down_executed.store(true);
            // std::cout << "[" << getName() << "] Thread: tearDown() executed." << std::endl;
        }
    };

    CancellableThread t("CancellationPointTest");
    t.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(50)); // Let it run
    
    EXPECT_TRUE(t.run_entered.load());
    EXPECT_TRUE(t.loop_iterations.load() > 0);
    EXPECT_FALSE(t.isCancelled()); // isCancelled is set to true AFTER pthread_cancel call
    EXPECT_FALSE(t.main_loop_exited.load());
    EXPECT_FALSE(t.tear_down_executed.load());

    t.cancel(); // Should send pthread_cancel signal
    EXPECT_TRUE(t.isCancelled()) << "isCancelled() should be true after cancel() call for AT_CANCELLATION_POINT.";

    // Give it time to hit cancellation point and terminate
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // The thread should have terminated due to cancellation
    t.join(); // Ensure cleanup for this test

    EXPECT_TRUE(t.main_loop_exited.load()) << "Thread run() should have exited due to cancellation.";
    EXPECT_TRUE(t.tear_down_executed.load()) << "tearDown() should have been called.";
    EXPECT_FALSE(t.isJoinable()) << "Thread should not be joinable after cancellation and join.";
    EXPECT_TRUE(t.isStarted()) << "Thread should not be started after cancellation and join.";
    std::cout << "Got here!\n";
}

// Test thread names and IDs
TEST_F(ThreadTest, NameAndId) {
    SimpleRunnableThread t("UniqueThreadName", false, CancellationPolicy::AT_CONDITION_CHECK);
    EXPECT_EQ(t.getName(), "UniqueThreadName");
    
    t.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    EXPECT_NE(t.getId(), std::thread::id()); // ID should be set
    
    // You could potentially try to get the OS-level thread name,
    // but that's platform-specific and beyond basic unit testing.
    
    t.cancel();
}

