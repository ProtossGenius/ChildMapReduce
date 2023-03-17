#pragma once
#include <leveldb/db.h>
#include <memory>
#include <string>
namespace leveldb {

class DB;
}
namespace pglang {
namespace mapreduce {

std::unique_ptr<leveldb::DB> open_level_db(const std::string &path);

}
} // namespace pglang
