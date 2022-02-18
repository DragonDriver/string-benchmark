#include <marisa.h>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "Timer.h"
#include "common.h"

void fill_keyset(marisa::Keyset& keyset,
                 const std::vector<std::string>& lines) {
  for (const auto& line : lines) {
    keyset.push_back(line.c_str());
  }
}

void build(marisa::Trie& trie, marisa::Keyset& keyset) { trie.build(keyset); }

void save(marisa::Trie& trie, const char* file) { trie.save(file); }

void load(marisa::Trie& trie, const char* file) { trie.load(file); }

int main(int argc, char* argv[]) {
  marisa::Keyset keyset;
  marisa::Trie trie1;
  marisa::Trie trie2;

  TimeRecorder tr("marisa-bench", 0);

  std::string file = F;
  std::string dic = MarisaDIC;
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
    marisa::Trie trie3;
    load(trie3, dic.c_str());
    std::cout << trie3.num_keys() << std::endl;
    std::cout << trie3.num_nodes() << std::endl;
    getchar();
    getchar();
    exit(0);
  }

  auto lines = read_file(file.c_str());
  tr.RecordSection("read file");

  fill_keyset(keyset, lines);
  tr.RecordSection("fill keyset");

  std::cout << "num_keys: " << keyset.num_keys() << std::endl;

  build(trie1, keyset);
  tr.RecordSection("build trie1");

  std::cout << "num_keys: " << trie1.num_keys() << std::endl;
  std::cout << "num_nodes: " << trie1.num_nodes() << std::endl;

  auto q = lines[0];
  std::cout << "first line: " << q << std::endl;

  marisa::Agent agent;

  // lookup, find id
  agent.set_query(q.c_str());
  trie1.lookup(agent);
  auto qid = agent.key().id();
  std::cout << "lookup " << q << std::endl;
  std::cout << "id: " << qid << std::endl;
  std::cout << "id == 0: " << (qid == 0) << std::endl;
  tr.RecordSection("lookup");

  // reverse lookup, find string by id
  agent.set_query(qid);
  trie1.reverse_lookup(agent);
  std::cout << "reverse lookup " << qid << std::endl;
  std::cout.write(agent.key().ptr(), agent.key().length());
  std::cout << std::endl;
  tr.RecordSection("reverse lookup");

  // common prefix search
  agent.set_query("!!!");
  std::cout << "common prefix search" << std::endl;
  while (trie1.common_prefix_search(agent)) {
    std::cout.write(agent.key().ptr(), agent.key().length());
    std::cout << ": " << agent.key().id() << std::endl;
  }
  tr.RecordSection("common prefix search");

  // predictive search
  std::cout << "predictive search" << std::endl;
  for (int i = 0; i < ql; i++) {
    agent.set_query(qs[i]);
    while (trie1.predictive_search(agent)) {
      // std::cout.write(agent.key().ptr(), agent.key().length());
      // std::cout << ": " << agent.key().id() << std::endl;
    }
  }
  tr.RecordSection("predictive search");

  save(trie1, dic.c_str());
  tr.RecordSection("save trie1");

  load(trie2, dic.c_str());
  tr.RecordSection("load trie2");

  return 0;
}
