#include "../Timer.h"
#include "../common.h"
#include <string.h>
#include <stdio.h>
#include <clickhouse/client.h>

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

void
prefix_search(Client& client, const std::string& sql) {
	client.Select(sql);
}

void prefix_search(Client& client, const std::string& sql, SelectCallback cb) {
	client.Select(sql, cb);
}

void drop(Client& client,
          const std::string& table = "tutorial.strbench") {
  client.Execute(std::string("drop table ") + table);
}

int main(int argc, char* argv[]) {
  std::string file = F;
  if (argc > 1) {
    replace(file, "100", argv[1]);
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

  auto lines = read_file();
  tr.RecordSection("read file");

  /// Create a table.
  client.Execute(
      "CREATE TABLE IF NOT EXISTS tutorial.strbench (str String) ENGINE = "
      "Memory");
  tr.RecordSection("create table");

  Block block;
  fill_block(block, lines);
  tr.RecordSection("fill block");

  insert(client, block, "tutorial.strbench");
  tr.RecordSection("insert");

	for (int i = 0; i < ql; i++) {
		char* sql;
		sprintf(sql, "select startsWith(%s, %s) from %s", "str", qs[i], "tutorial.strbench");
		prefix_search(client, std::string(sql));
	}
	tr.RecordSection("prefix search");

	for (int i = 0; i < ql; i++) {
		char* sql;
		sprintf(sql, "select startsWith(%s, %s) from %s", "str", qs[i], "tutorial.strbench");
		prefix_search(client, std::string(sql), [](const Block& block) {
			for (size_t i = 0; i < block.GetRowCount(); ++i) {
            std::cout << block[0]->As<ColumnUInt8>()->At(i) << std::endl;
        }
		});
	}
	tr.RecordSection("prefix search cb");

	drop(client, "tutorial.strbench");
	tr.RecordSection("drop table");

  return 0;
}