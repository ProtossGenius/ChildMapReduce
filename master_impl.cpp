#include "master_impl.h"
#include "defer.h"
#include "leveldb_tools.h"
#include "thread_pool.h"
#include <boost/asio.hpp>
#include <iostream>
#include <memory>
#include <nlohmann/json.hpp>
namespace pglang {
namespace mapreduce {

class LocalMaster : public Master {
  public:
    LocalMaster(const std::string &createInfo);

  public:
    void ListKeysInfo(CBOnReduceWork keyFilesInfo) override;
    void AcceptKeyInfo(const std::string &key,
                       const std::string &file) override;

    void AcceptReduceResult(const std::string &key,
                            const std::string &value) override;
    void ListResult(CBOnReduceWork result) override;

  private:
    std::unique_ptr<leveldb::DB> _file_info_db;
    std::unique_ptr<leveldb::DB> _result_db;
    ThreadPool                   _pool;
};

LocalMaster::LocalMaster(const std::string &createInfo)
    : Master(createInfo)
    , _pool(1) {
    using namespace nlohmann;
    json info = json::parse(createInfo);
    auto iter = info.find("file_info_db_path");
    _file_info_db =
        open_level_db(iter == info.end() ? "/tmp/file_info" : iter.value());
    iter = info.find("result_db_path");
    _result_db =
        open_level_db(iter == info.end() ? "/tmp/result_info" : iter.value());
}

void LocalMaster::ListResult(CBOnReduceWork cb) {
    auto iter = _result_db->NewIterator(leveldb::ReadOptions());
    for (iter->SeekToFirst(); iter->Valid(); iter->Next()) {
        cb(iter->key().ToString(), iter->value().ToString());
    }
}

void LocalMaster::AcceptReduceResult(const std::string &key,
                                     const std::string &value) {
    auto status = _result_db->Put(leveldb::WriteOptions(), key, value);
    if (!status.ok()) {
        throw std::runtime_error("When AcceptReduceResult : Key = " + key +
                                 "; value = " + value + "\n" +
                                 status.ToString());
    }
}
void LocalMaster::AcceptKeyInfo(const std::string &key,
                                const std::string &file) {
    _pool.post([ this, key, file ]() {
        std::string value;
        auto status = _file_info_db->Get(leveldb::ReadOptions(), key, &value);
        if (status.ok()) value += ";";
        value += file;
        status = _file_info_db->Put(leveldb::WriteOptions(), key, value);
        if (!status.ok()) {
            throw std::runtime_error("can't write key " + key +
                                     ", reason = " + status.ToString());
        }
    });
}

void LocalMaster::ListKeysInfo(CBOnReduceWork keyFilesInfo) {
    leveldb::Iterator *it = _file_info_db->NewIterator(leveldb::ReadOptions());
    Defer              delete_iter([ it ]() { delete it; });
    for (it->SeekToFirst(); it->Valid(); it->Next()) {
        keyFilesInfo(it->key().ToString(), it->value().ToString());
    }
}
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

Master *createMaster(const std::string &createInfo) {
    return new LocalMaster(createInfo);
}
} // namespace mapreduce
} // namespace pglang
