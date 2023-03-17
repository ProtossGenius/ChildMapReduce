#include "file_spliter.h"
#include "mapper_result.h"
#include "mapreduce.h"
#include "mstr.h"
#include "nlohmann/json.hpp"
#include "unit_test/word_counter.h"
#include "worker_impl.h"
#include <gtest/gtest.h>
#include <iostream>
#include <master_impl.h>
using namespace pglang::mapreduce;
using namespace nlohmann;
using namespace std;
std::vector<std::pair<std::string, string>> getCount(const std::string &path) {
    std::unique_ptr freader(std::make_unique<LocalFileReader>(path));
    std::string     text;
    text.resize(freader->size());
    freader->read(text);
    std::map<std::string, int> counter;
    const int                  n = text.size();
    for (int i = 0; i < n;) {
        while ((i < n) && isspace(text[ i ]))
            i++;
        int start = i;
        while ((i < n) && !isspace(text[ i ]))
            i++;
        if (start < i) ++counter[ text.substr(start, i - start) ];
    }

    vector<pair<string, string>> result;
    for (const auto &[ key, val ] : counter)
        result.push_back({key, to_string(val)});
    return result;
}
// Demonstrate some basic assertions.
TEST(MapReduceTest, BasicAssertions) {
    const std::string inputPath = "out.txt";
    // step1. split the file & Mapper
    FileSpliter fs(std::make_unique<LocalFileReader>(inputPath), 20);
    int         counter = 0;
    std::shared_ptr<Worker> worker(createWorker());

    std::shared_ptr<Master> master(createMaster("{}"));
    worker->setMapper(
        std::make_shared<WordCounter>(5, "./word_counter.out", ""));
    worker->getMapper()->setOnFinish(
        [ master ](const auto &path, const auto &result) {
            for (const auto &[ idx, map ] : result) {
                for (const auto &key : map) {
                    master->AcceptKeyInfo(key.first,
                                          path + "." + std::to_string(idx));
                }
            }
        });
    fs.split([ &counter, worker ](const auto &str) {
        auto name = "m" + std::to_string(counter++);
        worker->getMapper()->reset(name);
        worker->MapperWork(str);
    });
    // setp2. sort the key & call reduce.
    worker->setReducer(std::make_unique<Adder>("/tmp/reducer.cache"));
    master->ListKeysInfo([ worker ](const auto &key, const auto &paths) {
        json value;
        value[ "key" ] = key;
        split(paths, ";", [ &value, worker ](const auto &path) {
            value[ "path" ] = path;
            worker->ReducerWork(value.dump());
        });
    });
    vector<pair<string, string>> result;
    worker->ReduceDone([ &result ](const auto &key, const auto &value) {
        result.push_back({key, value});
    });

    auto local_result = getCount(inputPath);

    ASSERT_EQ(result, local_result);
}
