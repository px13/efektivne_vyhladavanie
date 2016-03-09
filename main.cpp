#include "io_tool.h"
#include "benchmarker.h"

int main()
{
	ms time = getTime();
	seqan::DnaString text;
	vector<seqan::DnaString> queries;
	IOtool io("/genome.fasta");
	io.readDnaString(text);
	//io = IOtool("/genome2.fasta");
	//io.readDnaString(text);
	io = IOtool("/queries1-1m.fasta");
	io.readQueries(queries);
	time = getTime() - time;

	cout << "Input data loaded." << endl;
	cout << "String length: " << length(text) << endl;
	cout << "Queries number: " << queries.size() << endl;
	cout << "Time of data loading: " << time.count() << " ms" << endl;
	cout << "====================" << endl;
	
	vector<deque<int>> result(queries.size());

	Benchmarker benchmark(text, queries, 10);

	benchmark.test(0, result);
	benchmark.test(3, result);
	benchmark.test(4, result);
	benchmark.test(1, result);
	benchmark.test(2, result);

	cin.get();
	return 0;
}

