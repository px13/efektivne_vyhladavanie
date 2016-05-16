#include <iostream>
#include <seqan/sequence.h>
#include <deque>
#include <chrono>
#include "windows.h"
#include "psapi.h"
#include "min_hash_um.h"
#include "min_hash_vp.h"
#include "fm_index.h"
#include "hash_table.h"

using namespace std;

typedef chrono::milliseconds ms;

ms getTime();

class Benchmark
{
public:

	Benchmark(seqan::DnaString &text, vector<seqan::DnaString> &queries, int numberOfTestingCycles);

	//Testing algorithm with a specified number and stores the result in the variable "out".
	//N is length of query and M is length of minimal substring for MinHash algorithm.
	void test(const int algorithm, vector<deque<int>> &out, int n, int m);

private:
	seqan::DnaString text;
	vector<seqan::DnaString> queries;
	int numberOfTestingCycles;

	PROCESS_MEMORY_COUNTERS pmc;

	void printTestResults(const string &algorithm, const pair<int, int> &parameters, const vector<int> &resultsOfBenchmarking);

	//Counts the number of all elements in 2-D array - checksum
	int numberOfElements(const vector<deque<int>> &results);

	size_t getCurrentMemoryUsed();

	//Calculates the arithmetic mean of the measured times
	int averageValue(const vector<int> &values);

};