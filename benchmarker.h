#include <iostream>
#include <seqan/sequence.h>
#include <deque>
#include <vector>
#include <chrono>
#include "windows.h"
#include "min_hash.h"
#include "min_hash_vector.h"
#include "min_hash_pair.h"
#include "fm_index.h"
#include "hash_table.h"
#include "psapi.h"

using namespace std;

typedef chrono::milliseconds ms;

ms getTime();

class Benchmarker
{
	public:

		Benchmarker(seqan::DnaString &text, vector<seqan::DnaString> &queries, int numberOfTestingCycles);
		void test(const int algorithm, vector<deque<int>> &out);

	private:
		seqan::DnaString text;
		vector<seqan::DnaString> queries;
		int numberOfTestingCycles;

		PROCESS_MEMORY_COUNTERS pmc;

		void printTestResults(const string &algorithm, const vector<int> &resultsOfBenchmarking);
		int numberOfElements(const vector<deque<int>> &results);
		size_t getCurrentMemoryUsed();
		int averageValue(const vector<int> &values);
	
};