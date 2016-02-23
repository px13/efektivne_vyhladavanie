#include "io_tool.h"
#include "xy_index.h"
#include "fm_index.h"
#include "hash_table.h"
#include <chrono>
#include "windows.h"
#include "psapi.h"

void printTestResults(seqan::CharString algoritmus, long pocet, int memoryUsed, int preprocessTime, int searchingTime)
{
	cout << algoritmus << " - Done" << endl;
	cout << "Number of results: " << pocet << endl;
	cout << "Memory used: " << memoryUsed << " B" << endl;
	cout << "Preprocess time: " << preprocessTime << " ms" << endl;
	cout << "Average searching time: " << searchingTime << " ms" << endl;
	cout << "--------------------" << endl;
}

long pocetPrvkov(list<list<int>> &pole)
{
	long result = 0;
	for (list<list<int>>::iterator iter = pole.begin(); iter != pole.end(); iter++)
	{
		result += (*iter).size();
	}
	return result;
}

list<list<int>> testXYIndex(seqan::DnaString &in, list<seqan::DnaString> &queries, int pocetTestovani)
{
	list<list<int>> out;
	chrono::milliseconds ms = chrono::duration_cast< chrono::milliseconds >(chrono::system_clock::now().time_since_epoch());

	PROCESS_MEMORY_COUNTERS pmc;
	GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
	SIZE_T memoryUsed = pmc.WorkingSetSize;

	XYIndex index(in);
	index.prepare();

	GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
	memoryUsed = pmc.WorkingSetSize - memoryUsed;

	chrono::milliseconds ms2 = chrono::duration_cast< chrono::milliseconds >(chrono::system_clock::now().time_since_epoch());
	int preprocessTime = (ms2 - ms).count();
	ms = chrono::duration_cast< chrono::milliseconds >(chrono::system_clock::now().time_since_epoch());
	for (int i = 0; i < pocetTestovani; i++)
	{
		index.query(queries, out);
	}
	ms2 = chrono::duration_cast< chrono::milliseconds >(chrono::system_clock::now().time_since_epoch());
	printTestResults("XY-Index", pocetPrvkov(out), memoryUsed, preprocessTime, (ms2 - ms).count() / 10);
	return out;
}

list<list<int>> testFMIndex(seqan::DnaString &in, list<seqan::DnaString> &queries, int pocetTestovani)
{
	list<list<int>> out;
	chrono::milliseconds ms = chrono::duration_cast< chrono::milliseconds >(chrono::system_clock::now().time_since_epoch());

	PROCESS_MEMORY_COUNTERS pmc;
	GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
	SIZE_T memoryUsed = pmc.WorkingSetSize;

	FMIndex index(in);
	index.prepare();

	GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
	memoryUsed = pmc.WorkingSetSize - memoryUsed;

	chrono::milliseconds ms2 = chrono::duration_cast< chrono::milliseconds >(chrono::system_clock::now().time_since_epoch());
	int preprocessTime = (ms2 - ms).count();
	ms = chrono::duration_cast< chrono::milliseconds >(chrono::system_clock::now().time_since_epoch());
	for (int i = 0; i < pocetTestovani; i++)
	{
		index.query(queries, out);
	}
	ms2 = chrono::duration_cast< chrono::milliseconds >(chrono::system_clock::now().time_since_epoch());
	printTestResults("FM-Index", pocetPrvkov(out), memoryUsed, preprocessTime, (ms2 - ms).count() / 10);
	return out;
}

list<list<int>> testHashTable(seqan::DnaString &in, list<seqan::DnaString> &queries, int pocetTestovani)
{
	list<list<int>> out;
	chrono::milliseconds ms = chrono::duration_cast< chrono::milliseconds >(chrono::system_clock::now().time_since_epoch());

	PROCESS_MEMORY_COUNTERS pmc;
	GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
	SIZE_T memoryUsed = pmc.WorkingSetSize;

	HashTable hashTable(in);
	hashTable.prepare();

	GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
	memoryUsed = pmc.WorkingSetSize - memoryUsed;

	chrono::milliseconds ms2 = chrono::duration_cast< chrono::milliseconds >(chrono::system_clock::now().time_since_epoch());
	int preprocessTime = (ms2 - ms).count();
	ms = chrono::duration_cast< chrono::milliseconds >(chrono::system_clock::now().time_since_epoch());
	for (int i = 0; i < pocetTestovani; i++)
	{
		hashTable.query(queries, out);
	}
	ms2 = chrono::duration_cast< chrono::milliseconds >(chrono::system_clock::now().time_since_epoch());
	printTestResults("Hash table", pocetPrvkov(out), memoryUsed, preprocessTime, (ms2 - ms).count() / 10);
	return out;
}

int main()
{
	chrono::milliseconds ms = chrono::duration_cast< chrono::milliseconds >(chrono::system_clock::now().time_since_epoch());
	IOtool io("/genome.fasta");
	seqan::DnaString text = io.readDnaString();
	io = IOtool("/queries.fasta");
	list<seqan::DnaString> queries = io.readQueries();
	chrono::milliseconds ms2 = chrono::duration_cast< chrono::milliseconds >(chrono::system_clock::now().time_since_epoch());

	cout << "Input data loaded." << endl;
	cout << "String length: " << length(text) << endl;
	cout << "Queries number: " << queries.size() << endl;
	cout << "Time of data loading: " << (ms2 - ms).count() << "ms" << endl;
	cout << "====================" << endl;

	testXYIndex(text, queries, 10);
	testHashTable(text, queries, 10);
	testFMIndex(text, queries, 10);

	cin.get();
	return 0;
}

