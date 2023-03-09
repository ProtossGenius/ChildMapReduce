#include "param_parser.h"
#include <gtest/gtest.h>
#include <iostream>
// Demonstrate some basic assertions.
TEST(ParamParserTest, BasicAssertions) {
    using namespace std;
    const char *args[]{"-test", "test", "hello", "world", "-b", "-c", "search"};
    int         argv = 7;
    pglang::mapreduce::ParamParser parser(argv, args);
    map<string, string> mp{{"-test", "test"}, {"-b", "true"}, {"-c", "search"}};
    vector<string>      list{"hello", "world"};
    ASSERT_EQ(parser.paramsMap(), mp);
    ASSERT_EQ(parser.normalPrarms(), list);
}
