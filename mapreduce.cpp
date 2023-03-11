#include "mapreduce.h"
#include <algorithm>
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

void write_info(const std::string &path, const MapperResult &result) {}
void sort_mapper_result(MapperResult &result);

void Mapper::MapperWork(const std::string &workInfo) {
    std::string input;
    // TODO: read input info from workInfo
    MapInput _input(std::move(input));
    Map(_input);
    sort_mapper_result(_result);
    write_info(_path_pre + _name + ".", _result);
}

void sort_mapper_result(MapperResult &result) {
    for (auto &[ _0, mp ] : result) {
        for (auto &[ _, arr ] : mp) {
            std::sort(arr.begin(), arr.end());
        }
    }
}
} // namespace mapreduce
} // namespace pglang
