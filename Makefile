marisa:
	g++ marisa.cpp -lmarisa -o marisa.out
hat:
	g++ hat.cpp -o hat.out
bruteforce:
	g++ bruteforce.cpp -o bruteforce.out
ch:
	cd clickhouse && g++ clickhouse.cpp -o ../clickhouse.out -l clickhouse-cpp-lib

# cd clickhouse && g++ clickhouse.cpp -o ../clickhouse.out -Iclickhouse/clickhouse-cpp/clickhouse -Iclickhouse/clickhouse-cpp/contrib -l clickhouse-cpp-lib