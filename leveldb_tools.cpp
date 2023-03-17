#include "leveldb_tools.h"
#include <memory>
#include <stdexcept>
namespace pglang {
namespace mapreduce {
std::unique_ptr<leveldb::DB> open_level_db(const std::string &path) {
    leveldb::DB     *db;
    leveldb::Options options;
    options.create_if_missing = true;
    leveldb::Status status    = leveldb::DB::Open(options, path, &db);
    if (!status.ok())
        throw std::runtime_error("can't open db: " + path +
                                 ", reason is :" + status.ToString());
    return std::unique_ptr<leveldb::DB>(db);
}
} // namespace mapreduce
} // namespace pglang
