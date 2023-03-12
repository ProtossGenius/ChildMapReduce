#include "mapreduce.h"
#include "defer.h"
#include "file.h"
#include "mapper_result.h"
#include "nlohmann/json.hpp"
#include <algorithm>
#include <fstream>
namespace pglang {
namespace mapreduce {

Mapper::Mapper(int part_size, const std::string &path_pre,
               const std::string &name)
    : _part_size(part_size)
    , _path_pre(path_pre)
    , _name(name) {}

void Mapper::Emit(const std::string &key, const std::string &value) {
    auto index = hash(key) % _part_size;
    _result[ index ][ key ].push_back(value);
}

void write_info(const std::string &path, const MapperResult &result);
void sort_mapper_result(MapperResult &result);
void read_input_by_work_info(std::string &input, const std::string &workInfo);
void Mapper::MapperWork(const std::string &workInfo) {
    std::string input;
    read_input_by_work_info(input, workInfo);
    MapInput _input(std::move(input));
    Map(_input);
    sort_mapper_result(_result);
    write_mapper_result(_path_pre + _name + ".", _result);
}

void sort_mapper_result(MapperResult &result) {
    for (auto &[ _0, mp ] : result) {
        for (auto &[ _, arr ] : mp) {
            std::sort(arr.begin(), arr.end());
        }
    }
}

void read_input_by_work_info(std::string &input, const std::string &workInfo) {
    using json            = nlohmann::json;
    json        data      = json::parse(workInfo);
    std::string uri       = data[ "uri" ];
    size_t      begin     = data[ "begin" ].get<size_t>();
    size_t      end       = data[ "end" ].get<size_t>();
    size_t      read_size = end - begin + 1;
    input.resize(read_size);

    std::unique_ptr<AFileReader> reader =
        std::make_unique<LocalFileReader>(uri);
    if (reader->size() < end)
        throw std::runtime_error(
            "try read file error, need file size >= " + std::to_string(end) +
            ";\nwork info is: " + workInfo);
    reader->seekg(begin);
    reader->read(input);
}
} // namespace mapreduce
} // namespace pglang
