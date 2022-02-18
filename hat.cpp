#include <iostream>
#include <string>
#include <assert.h>

#include "Timer.h"
#include "common.h"
#include "tsl/htrie_map.h"
#include "tsl/htrie_set.h"

void build(tsl::htrie_set<char>& set, const std::vector<std::string>& lines) {
  set.insert(lines.begin(), lines.end());
}

class serializer {
 public:
  serializer(const char* file_name) {
    m_ostream.exceptions(m_ostream.badbit | m_ostream.failbit);
    m_ostream.open(file_name);
  }

  template <class T, typename std::enable_if<
                         std::is_arithmetic<T>::value>::type* = nullptr>
  void operator()(const T& value) {
    m_ostream.write(reinterpret_cast<const char*>(&value), sizeof(T));
  }

  void operator()(const char* value, std::size_t value_size) {
    m_ostream.write(value, value_size);
  }

 private:
  std::ofstream m_ostream;
};

class deserializer {
 public:
  deserializer(const char* file_name) {
    m_istream.exceptions(m_istream.badbit | m_istream.failbit |
                         m_istream.eofbit);
    m_istream.open(file_name);
  }

  template <class T, typename std::enable_if<
                         std::is_arithmetic<T>::value>::type* = nullptr>
  T operator()() {
    T value;
    m_istream.read(reinterpret_cast<char*>(&value), sizeof(T));

    return value;
  }

  void operator()(char* value_out, std::size_t value_size) {
    m_istream.read(value_out, value_size);
  }

 private:
  std::ifstream m_istream;
};

void save(const tsl::htrie_set<char>& set, const char* file) {
	serializer serial(file);
	set.serialize(serial);
}

tsl::htrie_set<char> load(const char* file) {
	deserializer deserial(file);
	return tsl::htrie_set<char>::deserialize(deserial, true);
}

int main(int argc, char* argv[]) {
  TimeRecorder tr("hat-bench", 0);

  std::string file = F;
  std::string dic = HatDIC;
  if (argc > 1) {
    replace(file, "100", argv[1]);
    replace(dic, "100", argv[1]);
  }
  if (argc > 2) {
    replace(file, "wiki", argv[2]);
    replace(dic, "wiki", argv[2]);
  }

	std::cout << file << std::endl;
	std::cout << dic << std::endl;

	if (argc > 3) {
		// test memory usage
		auto set = load(dic.c_str());
		std::cout << set.size() << std::endl;
		getchar();
		getchar();
		exit(0);
	}

  auto lines = read_file(file.c_str());
  tr.RecordSection("read file");

  tsl::htrie_set<char> set;
  build(set, lines);
  tr.RecordSection("build htrie set");

  for (int i = 0; i < ql; i++) {
    auto prefix_range = set.equal_prefix_range(qs[i]);
    std::cout << "prefix: " << qs[i] << std::endl;
    for (auto it = prefix_range.first; it != prefix_range.second; ++it) {
      // std::cout << it.key() << std::endl;
    }
  }
  tr.RecordSection("prefix search");

	save(set, dic.c_str());
	tr.RecordSection("save");

	auto set2 = load(dic.c_str());
	tr.RecordSection("load");

	assert(set == set2);

  return 0;
}
