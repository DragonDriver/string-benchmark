#pragma once

#include <fstream>
#include <string>
#include <vector>

const char* F = "/home/ljq/work/dic/100w-wiki.txt";
const char* MarisaDIC = "/home/ljq/work/dic/100w-wiki.marisa.dic";
const char* HatDIC = "/home/ljq/work/dic/100w-wiki.hat.dic";

const char* qs[] = {"!!!",  "aha", "abc", "xyz", "Fuck", "You",
                    "fuck", "you", "123", "789", "1",    "9"};
auto ql = sizeof(qs) / sizeof(const char*);

bool replace(std::string& str, const std::string& from, const std::string& to) {
  size_t start_pos = str.find(from);
  if (start_pos == std::string::npos) return false;
  str.replace(start_pos, from.length(), to);
  return true;
}

std::vector<std::string> read_file(const char* file = F) {
  std::vector<std::string> ret;
  std::ifstream f(file);
  for (std::string line; std::getline(f, line);) {
    ret.emplace_back(line);
  }
  return ret;
}

static inline bool prefix_match(const std::string& str,
                                const std::string& prefix) {
  if (prefix.length() > str.length()) {
    return false;
  }
  for (int i = 0; i < prefix.length(); i++) {
    if (prefix[i] != str[i]) {
      return false;
    }
  }
  return true;
}
