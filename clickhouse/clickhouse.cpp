#include <stdio.h>
#include <string.h>

#include "Timer.h"
#include "clickhouse/client.h"
#include "common.h"

using namespace clickhouse;

void fill_block(Block& block, const std::vector<std::string>& lines) {
  auto str = std::make_shared<ColumnString>();
  for (const auto& line : lines) {
    str->Append(line);
  }
  block.AppendColumn("str", str);
}

void insert(Client& client, const Block& block,
            const std::string& table = "tutorial.strbench") {
  client.Insert(table, block);
}

void prefix_search(Client& client, const std::string& sql) {
  client.Select(sql);
}

void prefix_search(Client& client, const std::string& sql, SelectCallback cb) {
  client.Select(sql, cb);
}

void drop(Client& client, const std::string& table = "tutorial.strbench") {
  client.Execute(std::string("drop table ") + table);
}

int main(int argc, char* argv[]) {
  std::string num_rows = "100";
  std::string file = F;
  if (argc > 1) {
    replace(file, "100", argv[1]);
    replace(num_rows, "100", argv[1]);
  }
  if (argc > 2) {
    replace(file, "wiki", argv[2]);
  }

  std::cout << file << std::endl;

  TimeRecorder tr("clickhouse-bench", 0);

  /// Initialize client connection.
  ClientOptions opts;
  opts.SetHost("localhost");
  opts.SetPort(9000);
  opts.SetUser("default");
  opts.SetPassword("ljq");
  Client client(opts);
  tr.RecordSection("create connection");

  auto lines = read_file(file.c_str());
  tr.RecordSection("read file");

  /// Create a table.
  const char* ddl =
      "CREATE TABLE IF NOT EXISTS tutorial.strbench (str String) ENGINE = "
      "Memory";
  std::cout << ddl << std::endl;
  client.Execute(ddl);
  tr.RecordSection("create table");

  Block block;
  fill_block(block, lines);
  tr.RecordSection("fill block");

  insert(client, block, "tutorial.strbench");
  tr.RecordSection("insert");

  auto sql = new char[1000];
  for (unsigned int i = 0; i < ql; i++) {
    sprintf(sql, "select startsWith(%s, '%s') from %s limit %d", "str", qs[i],
            "tutorial.strbench", std::atoi(num_rows.c_str()) * 10000);
    std::cout << sql << std::endl;
    prefix_search(client, std::string(sql));
  }
  delete[] sql;
  tr.RecordSection("prefix search");

  // for (unsigned int i = 0; i < ql; i++) {
  //   char* sql;
  //   sprintf(sql, "select startsWith(%s, %s) from %s", "str", qs[i],
  //           "tutorial.strbench");
  //   prefix_search(client, std::string(sql), [](const Block& block) {
  //     for (size_t i = 0; i < block.GetRowCount(); ++i) {
  //       std::cout << block[0]->As<ColumnUInt8>()->At(i) << std::endl;
  //     }
  //   });
  // }
  // tr.RecordSection("prefix search cb");

  // drop(client, "tutorial.strbench");
  tr.RecordSection("drop table");

  return 0;
}