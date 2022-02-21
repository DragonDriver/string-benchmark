#include <marisa.h>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "Timer.h"
#include "common.h"

struct Bruteforce {
  std::vector<std::string> prefix_search(const std::string& prefix);

  std::vector<std::string> prefix_search(const char* prefix) {
    return prefix_search(std::string(prefix));
  }

  std::vector<std::string> lines_;
};

std::vector<std::string> Bruteforce::prefix_search(const std::string& prefix) {
  std::vector<std::string> ret;
  for (const auto& str : lines_) {
    if (prefix_match(str, prefix)) {
      ret.emplace_back(str);
    }
  }
  return ret;
}

int main(int argc, char* argv[]) {
  TimeRecorder tr("bruteforce-bench", 0);

  std::string file = F;
  if (argc > 1) {
    replace(file, "100", argv[1]);
  }
  if (argc > 2) {
    replace(file, "wiki", argv[2]);
  }

  std::cout << file << std::endl;

  Bruteforce bf;
  bf.lines_ = read_file(file.c_str());
  tr.RecordSection("read file");

  // prefix search
  std::cout << "prefix search" << std::endl;
  for (int i = 0; i < ql; i++) {
    // std::cout << "prefix: " << qs[i] << std::endl;
    auto prefix_results = bf.prefix_search(qs[i]);
    // for (const auto& result : prefix_results) {
    //   std::cout << result << std::endl;
    // }
  }
  tr.RecordSection("prefix search");

  return 0;
}
