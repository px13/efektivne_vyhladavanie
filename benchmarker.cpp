#include "benchmarker.h"

ms getTime()
{
	return chrono::duration_cast< ms >(chrono::system_clock::now().time_since_epoch());
}

Benchmarker::Benchmarker(seqan::DnaString &text, vector<seqan::DnaString> &queries, int numberOfTestingCycles) :
	queries(queries)
{
	this->text = text;
	this->numberOfTestingCycles = numberOfTestingCycles;
}

void Benchmarker::test(const int algorithm, vector<deque<int>> &out)
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
			algorithmName = "MinHash";
			start = getTime();
			memoryUsed = getCurrentMemoryUsed();
			MinHash index(text);
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
		case 2:
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
	printTestResults(algorithmName,
						vector<int>({	numberOfElements(out),
										(int) (memoryUsed / 1024 / 1024),
										preparingTime,
										averageValue(searchingTimes)})
						);

}

void Benchmarker::printTestResults(const string &algorithm, const vector<int> &resultsOfBenchmarking)
{
	cout << algorithm << ":" << endl;
	cout << "Number of results: " << resultsOfBenchmarking[0] << endl;
	cout << "Memory used: " << resultsOfBenchmarking[1] << " MB" << endl;
	cout << "Average preparing time: " << resultsOfBenchmarking[2] << " ms" << endl;
	cout << "Average searching time: " << resultsOfBenchmarking[3] << " ms" << endl;
	cout << "--------------------" << endl;
}

int Benchmarker::numberOfElements(const vector<deque<int>> &results)
{
	int result = 0;
	for (auto iter = results.begin(); iter != results.end(); ++iter)
	{
		result += iter->size();
	}
	return result;
}

size_t Benchmarker::getCurrentMemoryUsed()
{
	GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
	return pmc.WorkingSetSize;
}

int Benchmarker::averageValue(const vector<int> &values)
{
	int sum = 0;
	for (auto iter = values.begin(); iter != values.end(); ++iter)
	{
		sum += (*iter);
	}
	return (sum / numberOfTestingCycles);
}