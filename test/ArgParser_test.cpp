
#include "ArgParser.hh"
#include "gtest/gtest.h"
#include <iostream>
#include <string>

TEST(ArgParserTest, versionTest) {

  ExpUtils::ArgParser argParse{"versionTest", "v0.1"};

  argParse.addArgument("version", "v", ExpUtils::ArgParser::ArgType::version, true);

  ExpUtils::ArgParser::Help help;

  auto args = argParse.parse(std::vector<std::string>{"versionTest", "-v"}, help);

  EXPECT_FALSE(help.isSuccessful());
  EXPECT_EQ(help.report(), "versionTest: v0.1");
}

TEST(ArgParserTest, versionTest_exception) {

  ExpUtils::ArgParser argParse{"versionTest", "v0.1"};
  // ExpUtils::ArgParser argParse("ProgName", "v0.1");

  argParse.addArgument("version", "v", ExpUtils::ArgParser::ArgType::version, true);

  try {
    auto args = argParse.parse(std::vector<std::string>{"versionTest", "-v"});
  } catch (const std::runtime_error &e) {
    EXPECT_EQ(std::string(e.what()), "versionTest: v0.1");
  }
}

TEST(ArgParserTest, orderedTest) {
  ExpUtils::ArgParser argParse{"v0.1"};
  argParse.addOrderedArgument("test");
  argParse.addOrderedArgument("test2");

  auto args = argParse.parse(std::vector<std::string>{"versionTest", "1", "2.5"});

  EXPECT_EQ(args.retrieve<int>("test"), 1);
  EXPECT_EQ(args.retrieve<double>("test2"), 2.5);
}

TEST(ArgParserTest, orderedTest_toFew) {
  ExpUtils::ArgParser argParse{"v0.1"};
  argParse.addOrderedArgument("test");
  argParse.addOrderedArgument("test2");
  bool exception = false;

  try {
    auto args = argParse.parse(std::vector<std::string>{"versionTest", "1"});
  } catch (std::runtime_error &e) {
    // std::cerr<<e.what()<<std::endl;
    EXPECT_EQ(std::string(e.what()), "Format String Coming soon....");
    exception = true;
  }
  // EXPECT_EQ(args.retrieve<int>("test"), 1);
  // EXPECT_EQ(args.retrieve<double>("test2"), 2.5);
  EXPECT_TRUE(exception);
}

TEST(ArgParserTest, orderedTest_otherEarly) {
  ExpUtils::ArgParser argParse{"v0.1"};
  argParse.addOrderedArgument("test");
  argParse.addOrderedArgument("test2");
  argParse.addArgument("test3");

  bool exception = false;

  try {
    auto args = argParse.parse(
        std::vector<std::string>{"versionTest", "1", "--test3", "1"});
  } catch (std::runtime_error &e) {
    // std::cerr<<e.what()<<std::endl;
    EXPECT_EQ(std::string(e.what()), "Format String Coming soon....");
    exception = true;
  }
  // EXPECT_EQ(args.retrieve<int>("test"), 1);
  // EXPECT_EQ(args.retrieve<double>("test2"), 2.5);
  EXPECT_TRUE(exception);
}

TEST(ArgParserTest, storeTest) {

  ExpUtils::ArgParser argParse{"v0.1"};

  argParse.addArgument("test");

  auto args = argParse.parse(std::vector<std::string>{"storeTest", "--test", "1"});

  EXPECT_EQ(args.retrieve<int>("test"), 1);
}

TEST(ArgParserTest, storeTest_shortName) {

  ExpUtils::ArgParser argParse{"v0.1"};

  argParse.addArgument("test", "t");

  auto args = argParse.parse(std::vector<std::string>{"storeTest", "-t", "1"});

  EXPECT_TRUE(args.contains("test"));

  EXPECT_EQ(args.retrieve<int>("test"), 1);
}

TEST(ArgParserTest, storeTest_Multi) {

  ExpUtils::ArgParser argParse{"v0.1"};

  argParse.addArgument("test");

  auto args =
      argParse.parse(std::vector<std::string>{"storeTest", "--test", "1", "2"});
  std::vector<int> v{{1, 2}};
  std::array<int, 2> a{{1, 2}};

  // We exect to recieve the first when converting to a single integer, and then
  // both when converting to vector.
  EXPECT_EQ(args.retrieve<int>("test"), 1);
  EXPECT_EQ(args.retrieve<std::vector<int>>("test"), v);
  std::array<int, 2> a2 = args.retrieve<std::array<int, 2>>("test");
  EXPECT_EQ(a2, a);
}

TEST(ArgParserTest, storeTest_Exceptions) {
  // Mostly the array error....

  ExpUtils::ArgParser argParse{"v0.1"};

  argParse.addArgument("test");

  bool exception = false;

  auto args =
      argParse.parse(std::vector<std::string>{"storeTest", "--test", "1"});
  try {
    args.retrieve<std::array<int, 2>>("test");
  } catch (std::runtime_error &e) {
    exception = true;
    EXPECT_EQ(
        std::string(e.what()),
        "Something went wrong with parsing (probably not the right type) test");
  }
  EXPECT_TRUE(exception);
}

TEST(ArgParserTest, booleanFunctions) {

  ExpUtils::ArgParser argParse{"v0.1"};
  argParse.addArgument("runThis", "rt",
                       ExpUtils::ArgParser::ArgType::store_true);
  argParse.addArgument("dontRunThis", "drt",
                       ExpUtils::ArgParser::ArgType::store_false);

  auto args = argParse.parse({"bolleanTest", "-rt", "-drt"});
  EXPECT_TRUE(args.retrieve<bool>("runThis"));
  EXPECT_FALSE(args.retrieve<bool>("dontRunThis"));

  args = argParse.parse({"bolleanTest"});
  EXPECT_FALSE(args.retrieve<bool>("runThis"));
  EXPECT_TRUE(args.retrieve<bool>("dontRunThis"));
}

TEST(ArgParserTest, appendTest) {

  ExpUtils::ArgParser argParse{"v0.1"};
  argParse.addArgument("appendThis", "a", ExpUtils::ArgParser::ArgType::append);
  argParse.addArgument("store", "s");

  auto args = argParse.parse(
      {"appendTest", "-a", "1", "-s", "2", "3", "-a", "4", "5", "6"});

  auto v = std::vector<int>{{1, 4, 5, 6}};
  EXPECT_EQ(args.retrieve<std::vector<int>>("appendThis"), v);
}

TEST(ArgParserTest, multiArgument) {

  ExpUtils::ArgParser argParse{"v0.1"};
  argParse.addArgument("test", "t");

  argParse.addArgument("storeTrue", "st",
                       ExpUtils::ArgParser::ArgType::store_true);
  argParse.addArgument("store", "s", false);

  auto args = argParse.parse(
      {"multiArgument", "-s", "1", "3", "5", "-st", "--test", "10.0"});

  EXPECT_TRUE(args.retrieve<bool>("storeTrue"));
  auto a = args.retrieve<std::array<int, 3>>("store");
  auto a2 = std::array<int, 3>{{1, 3, 5}};
  EXPECT_EQ(a, a2);
}
