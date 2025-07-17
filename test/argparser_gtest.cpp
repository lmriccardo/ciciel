#include <gtest/gtest.h>
#include <cli/argparse.hpp>

using namespace ccl::cli::argparse;

class ArgumentParserTest : public ::testing::Test {
protected:
    ArgumentParser parser{"test_program"};

    void SetUp() override {
        // Clean parser state between tests
        parser.clean();
    }
};

TEST_F(ArgumentParserTest, ProgramNameIsStoredCorrectly) {
    EXPECT_EQ(parser.getProgramName(), "test_program");
}

TEST_F(ArgumentParserTest, AddAndParseBooleanArgument) {
    parser.addArgument<bool>({ "verbose", "v", "Enable verbose output", false }, false);
    const char* argv[] = { "test_program", "--verbose" };
    parser.parse(2, argv);

    EXPECT_TRUE(parser.getValue<bool>("verbose"));
}

TEST_F(ArgumentParserTest, AddAndParseStringArgument) {
    parser.addArgument<std::string>({ "name", "n", "User name", true });
    const char* argv[] = { "test_program", "--name", "Alice" };
    parser.parse(3, argv);

    EXPECT_EQ(parser.getValue<std::string>("name"), "Alice");
}

TEST_F(ArgumentParserTest, AddAndParseIntArgument) {
    parser.addArgument<int>({ "age", "a", "User age", true });
    const char* argv[] = { "test_program", "--age", "42" };
    parser.parse(3, argv);

    EXPECT_EQ(parser.getValue<int>("age"), 42);
}

TEST_F(ArgumentParserTest, AddAndParseDoubleArgument) {
    parser.addArgument<double>({ "score", "s", "Score value", true });
    const char* argv[] = { "test_program", "--score", "98.6" };
    parser.parse(3, argv);

    EXPECT_NEAR(parser.getValue<double>("score"), 98.6, 1e-6);
}

TEST_F(ArgumentParserTest, MissingRequiredArgumentThrows) {
    parser.addArgument<std::string>({ "config", "c", "Path to config", true });

    const char* argv[] = { "test_program" };
    EXPECT_THROW(parser.parse(1, argv), std::runtime_error);
}

TEST_F(ArgumentParserTest, OptionalArgumentDefaultsToProvidedValue) {
    parser.addArgument<int>({ "count", "c", "Item count", false }, 10);

    const char* argv[] = { "test_program" };
    parser.parse(1, argv);

    EXPECT_EQ(parser.getValue<int>("count"), 10);
}

TEST_F(ArgumentParserTest, OverrideDefaultOptionalArgument) {
    parser.addArgument<int>({ "count", "c", "Item count", false }, 10);

    const char* argv[] = { "test_program", "--count", "25" };
    parser.parse(3, argv);

    EXPECT_EQ(parser.getValue<int>("count"), 25);
}

TEST_F(ArgumentParserTest, ShortAndLongArgumentNamesBothWork) {
    parser.addArgument<std::string>({ "output", "o", "Output file", true });

    const char* argv1[] = { "test_program", "--output", "file.txt" };
    const char* argv2[] = { "test_program", "-o", "file.txt" };

    parser.parse(3, argv1);
    EXPECT_EQ(parser.getValue<std::string>("output"), "file.txt");

    parser.clean();
    parser.parse(3, argv2);
    EXPECT_EQ(parser.getValue<std::string>("output"), "file.txt");
}

TEST_F(ArgumentParserTest, GettingWrongTypeThrows) {
    parser.addArgument<int>({ "num", "n", "Number of items", true });
    const char* argv[] = { "test_program", "--num", "100" };
    parser.parse(3, argv);

    // We added an int, now try getting it as a double
    EXPECT_THROW({
        parser.getValue<double>("num");
    }, std::exception);
}
