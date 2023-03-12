#include "mapper_result.h"
#include <gtest/gtest.h>

using namespace pglang::mapreduce;
using namespace std;
vector<string> readValues(const string &path, const string &key) {
    std::unique_ptr<AFileReader> freader = make_unique<LocalFileReader>(path);
    MapperResultReader           reader{std::move(freader), key};
    vector<string>               read_result;
    while (!reader.done()) {
        read_result.push_back(reader.value());
        reader.NextValue();
    }
    return read_result;
}
// Demonstrate some basic assertions.
TEST(MapperResultTest, BasicAssertions) {
    vector<string> values{"value1", "value2", "value3"};
    MapperResult   result{{1, {{"key1", values}, {"key2", {}}}}};
    write_mapper_result("./mapper_result", result);
    auto k1v = readValues("./mapper_result.1", "key1");
    ASSERT_EQ(values, k1v);
    vector<string> empty_value;
    auto           k2v = readValues("./mapper_result.1", "key2");
    ASSERT_EQ(empty_value, k2v);
}
