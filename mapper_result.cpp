#include "mapper_result.h"

namespace pglang {
namespace mapreduce {
MapperResultReader::MapperResultReader(std::unique_ptr<AFileReader> reader,
                                       const std::string           &key)
    : _reader(std::move(reader))
    , _key(key)

{
    readHeader();
}

void MapperResultReader::readHeader() {
    f_int32 size;
    _reader->readsome(size.arr, 4);
}

} // namespace mapreduce
} // namespace pglang
