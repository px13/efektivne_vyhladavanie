#include "benchmark.h"

ms getTime()
{
	return chrono::duration_cast< ms >(chrono::system_clock::now().time_since_epoch());
}

Benchmark::Benchmark(seqan::DnaString &text, vector<seqan::DnaString> &queries, int numberOfTestingCycles) :
queries(queries)
{
	this->text = text;
	this->numberOfTestingCycles = numberOfTestingCycles;
}

void Benchmark::test(const int algorithm, vector<deque<int>> &out, int n, int m)
{
	out.clear(); out.resize(queries.size());
	size_t memoryUsed;
	int preparingTime;
	vector<int> searchingTimes(numberOfTestingCycles);
	ms start, end;

	string algorithmName = "Unkown";
	switch (algorithm)
	{
	case 0:
	{
			  algorithmName = "MinHash (UM)";
			  start = getTime();
			  memoryUsed = getCurrentMemoryUsed();
			  MinHashUM index(text, n, m);
			  index.prepare();
			  memoryUsed = getCurrentMemoryUsed() - memoryUsed;
			  end = getTime();
			  preparingTime = (end - start).count();
			  for (int i = 0; i < numberOfTestingCycles; i++)
			  {
				  out.clear(); out.resize(queries.size());
				  start = getTime();
				  index.query(queries, out);
				  end = getTime();
				  searchingTimes[i] = (end - start).count();
			  }
			  break;
	}
	case 1:
	{
			  algorithmName = "MinHash (VP)";
			  start = getTime();
			  memoryUsed = getCurrentMemoryUsed();
			  MinHashVP index(text, n, m);
			  index.prepare();
			  memoryUsed = getCurrentMemoryUsed() - memoryUsed;
			  end = getTime();
			  preparingTime = (end - start).count();
			  for (int i = 0; i < numberOfTestingCycles; i++)
			  {
				  out.clear(); out.resize(queries.size());
				  start = getTime();
				  index.query(queries, out);
				  end = getTime();
				  searchingTimes[i] = (end - start).count();
			  }
			  break;
	}
	case 2:
	{
			  algorithmName = "HashTable";
			  start = getTime();
			  memoryUsed = getCurrentMemoryUsed();
			  HashTable index(text);
			  index.prepare();
			  memoryUsed = getCurrentMemoryUsed() - memoryUsed;
			  end = getTime();
			  preparingTime = (end - start).count();
			  for (int i = 0; i < numberOfTestingCycles; i++)
			  {
				  out.clear(); out.resize(queries.size());
				  start = getTime();
				  index.query(queries, out);
				  end = getTime();
				  searchingTimes[i] = (end - start).count();
			  }
			  break;
	}
	case 3:
	{
			  algorithmName = "FMIndex";
			  start = getTime();
			  memoryUsed = getCurrentMemoryUsed();
			  FMIndex index(text);
			  index.prepare();
			  memoryUsed = getCurrentMemoryUsed() - memoryUsed;
			  end = getTime();
			  preparingTime = (end - start).count();
			  for (int i = 0; i < numberOfTestingCycles; i++)
			  {
				  out.clear(); out.resize(queries.size());
				  start = getTime();
				  index.query(queries, out);
				  end = getTime();
				  searchingTimes[i] = (end - start).count();
			  }
			  break;
	}
	default:
		cout << "Unkown algorithm." << endl;
		return;
	}
	printTestResults(algorithmName, pair<int, int>(n, m),
		vector<int>({ numberOfElements(out),
		(int)(memoryUsed / 1024 / 1024),
		preparingTime,
		averageValue(searchingTimes) })
		);

}

void Benchmark::printTestResults(const string &algorithm, const pair<int, int> &parameters, const vector<int> &resultsOfBenchmarking)
{
	cout << algorithm << " N = " << parameters.first;
	if (algorithm == "MinHash (UM)" || algorithm == "MinHash (VP)")
	{
		cout << " M = " << parameters.second;
	}
	cout << endl;
	cout << "Number of results: " << resultsOfBenchmarking[0] << endl;
	cout << "Memory used: " << resultsOfBenchmarking[1] << " MB" << endl;
	cout << "Average preparing time: " << resultsOfBenchmarking[2] << " ms" << endl;
	cout << "Average searching time: " << resultsOfBenchmarking[3] << " ms" << endl;
	cout << "--------------------" << endl;
}

int Benchmark::numberOfElements(const vector<deque<int>> &results)
{
	int result = 0;
	for (auto iter = results.begin(); iter != results.end(); ++iter)
	{
		result += iter->size();
	}
	return result;
}

size_t Benchmark::getCurrentMemoryUsed()
{
	GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
	return pmc.WorkingSetSize;
}

int Benchmark::averageValue(const vector<int> &values)
{
	int sum = 0;
	for (auto iter = values.begin(); iter != values.end(); ++iter)
	{
		sum += (*iter);
	}
	return (sum / numberOfTestingCycles);
}