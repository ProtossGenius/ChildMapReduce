#pragma once
#include "mapreduce.h"
#include <iostream>
using namespace std;
namespace pglang {
namespace mapreduce {

class WordCounter : public Mapper {
  public:
    WordCounter(int part_size, const std::string &path_pre,
                const std::string &name)
        : Mapper(part_size, path_pre, name) {}

  public:
    void Map(const MapInput &input) {
        using namespace std;
        const string &text = input.value();
        const int     n    = text.size();
        for (int i = 0; i < n;) {
            while ((i < n) && isspace(text[ i ]))
                i++;
            int start = i;
            while ((i < n) && !isspace(text[ i ]))
                i++;
            if (start < i) Emit(text.substr(start, i - start), "1");
        }
    }
};

class Adder : public Reducer {
  public:
    Adder(const std::string &cachePath)
        : Reducer(cachePath) {}

  public:
    void Reduce(ReduceInput *input) override {
        using namespace std;
        int value = GetValue() == "" ? 0 : stoi(GetValue());
        while (!input->done()) {
            value += stoi(input->value());
            input->NextValue();
        }

        Emit(to_string(value));
    }
};
} // namespace mapreduce
} // namespace pglang
