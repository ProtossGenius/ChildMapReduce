#pragma once
#include <functional>
#include <string>
namespace pglang {
namespace mapreduce {
typedef std::function<void(const std::string &key, const std::string &files)>
    CBOnReduceWork;

class Master {
  public:
    Master(const std::string &createInfo)
        : _create_info(createInfo) {}

  public:
    virtual ~Master() {}

  public: // For Worker
          // Accept Mapper's upload key-file info.
    virtual void AcceptKeyInfo(const std::string &key,
                               const std::string &file) = 0;
    // List all key-files info.
    virtual void ListKeysInfo(CBOnReduceWork keyFilesInfo)    = 0;
    virtual void AcceptReduceResult(const std::string &key,
                                    const std::string &value) = 0;

  protected:
    const std::string &getCreateInfo() { return _create_info; }

  private:
    std::string _create_info;
};

Master *createMaster(const std::string &createInfo);

} // namespace mapreduce
} // namespace pglang
