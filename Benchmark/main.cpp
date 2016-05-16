#include "io_tool.h"
#include "benchmark.h"

int main(int argc, char *argv[])
{
	if (argc < 7)
	{
		return 1;
	}
	int a, n, m, c;
	if (sscanf(argv[1], "%d", &a) != 1)
	{
		cerr << "ERROR: Invalid number of algorithm.\n";
		return 1;
	}
	if (sscanf(argv[2], "%d", &n) != 1 || n < 22)
	{
		cerr << "ERROR: Invalid or unrecomended length of queries.\n";
		return 1;
	}
	if (sscanf(argv[3], "%d", &m) != 1 || m < 13 || m + 2 >= n || m > 16)
	{
		cerr << "ERROR: Invalid or unrecomended length of minimal substring.\n";
		return 1;
	}
	if (sscanf(argv[4], "%d", &c) != 1 || c < 1)
	{
		cerr << "ERROR: Invalid number of testing cycles.\n";
		return 1;
	}

	ms time = getTime();
	seqan::DnaString sequence;
	vector<seqan::DnaString> queries;
	for (int i = 5; i < argc - 1; i++)
	{
		IOtool(argv[i]).readDnaString(sequence);
	}
	IOtool(argv[argc - 1]).readQueries(queries);
	time = getTime() - time;

	cout << "Input data loaded." << endl;
	cout << "String length: " << length(sequence) << endl;
	cout << "Queries number: " << queries.size() << endl;
	cout << "Time of data loading: " << time.count() << " ms" << endl;
	cout << "====================" << endl;

	vector<deque<int>> result(queries.size());

	Benchmark benchmark(sequence, queries, c);

	benchmark.test(a, result, n, m);

	return 0;
}