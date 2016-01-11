#include <iostream>
#include <seqan/sequence.h>
#include <seqan/basic.h>
#include <seqan/index.h>
#include <seqan/seq_io.h>
#include <list>
#include <map>
#include <chrono>
#include <stdlib.h>

using namespace seqan;
using namespace std;
using namespace chrono;

int N = 32;
int M = 13;
int N_MINUS_M = N - M;

typedef Infix<DnaString>::Type  DnaInfix;

DnaInfix findMinSegmentOfLengthM(DnaString &in, int begin, const int end)
{
	DnaInfix minSegment = infixWithLength(in, begin, M);
	for (begin = begin + 1; begin + M <= end; begin++)
	{
		minSegment = min(infixWithLength(in, begin, M), minSegment);
	}
	return minSegment;
}

void addSegmentToMap(multimap<DnaString, int> &m, const Segment<DnaString> &segment)
{
	m.insert(pair<DnaString, int>(segment, segment.data_begin_position));
}

multimap<DnaString, int> getMapWithMinSegments(DnaString &in)
{
	multimap<DnaString, int> m;
	DnaInfix lastMinSegment = findMinSegmentOfLengthM(in, 0, N);
	addSegmentToMap(m, lastMinSegment);
	for (int i = N_MINUS_M + 1; i + M <= length(in); i++)
	{
		if (lastMinSegment.data_begin_position < i - N_MINUS_M)
		{
			lastMinSegment = findMinSegmentOfLengthM(in, i - N_MINUS_M, i + M);
			addSegmentToMap(m, lastMinSegment);
		}
		else
		{
			DnaInfix segment = infixWithLength(in, i, M);
			if (isLess(segment, lastMinSegment))
			{
				lastMinSegment = segment;
				addSegmentToMap(m, lastMinSegment);
			}

		}
	}
	return m;
}

bool isCorrect(DnaString &in, DnaString &query, int beginSegment, DnaInfix &queryMinSegment)
{
	return (isEqual(infixWithLength(query, 0, queryMinSegment.data_begin_position),
					infixWithLength(in, beginSegment - queryMinSegment.data_begin_position, queryMinSegment.data_begin_position)
					)
			&&
			isEqual(infixWithLength(query, queryMinSegment.data_end_position, N - queryMinSegment.data_end_position),
					infixWithLength(in, beginSegment + M, N - queryMinSegment.data_end_position)
					)
			);
}

void findQuery(DnaString &in, DnaString &query, multimap<DnaString, int> &m, list<int> &out)
{
	DnaInfix queryMinSegment = findMinSegmentOfLengthM(query, 0, N);
	pair<multimap<DnaString, int>::iterator, multimap<DnaString, int>::iterator> mapIteratorPair = m.equal_range(queryMinSegment);
	for (multimap<DnaString, int>::iterator mapIterator = mapIteratorPair.first; mapIterator != mapIteratorPair.second; ++mapIterator)
	{
		if (isCorrect(in, query, (*mapIterator).second, queryMinSegment))
			out.push_back( (*mapIterator).second - queryMinSegment.data_begin_position);
	}
}

list<int> findQueries(DnaString &in, list<DnaString> &queries)
{
	list<int> out;
	multimap<DnaString, int> m = getMapWithMinSegments(in);
	for (list<DnaString>::iterator listIterator = queries.begin(); listIterator != queries.end(); listIterator++)
	{
		findQuery(in, *listIterator, m, out);
	}
	return out;
}

void printTestResults(CharString algoritmus, int vysledok, int time)
{
	cout << algoritmus << " - Hotovo" << endl;
	cout << "Pocet vyskytov dotazov: " << vysledok << endl;
	cout << "Priemerny cas: " << time << "ms" << endl;
}

list<int> findQueriesTest(DnaString &in, list<DnaString> &queries, int pocetTestovani)
{
	list<int> out;
	multimap<DnaString, int> m = getMapWithMinSegments(in);
	milliseconds ms = duration_cast< milliseconds >(
		system_clock::now().time_since_epoch()
		);
	for (int i = 0; i < pocetTestovani; i++)
	{
		for (list<DnaString>::iterator listIterator = queries.begin(); listIterator != queries.end(); listIterator++)
		{
			findQuery(in, *listIterator, m, out);
		}
	}
	milliseconds ms2 = duration_cast< milliseconds >(
		system_clock::now().time_since_epoch()
		);
	printTestResults("Moj algoritmus", out.size(), (ms2 - ms).count() / 10);
	return out;
}

list<int> testFMIndex(DnaString in, list<DnaString> queries, int pocetTestovani)
{
	Index<DnaString, FMIndex<> > fmIndex(in);
	Finder<Index<DnaString, FMIndex<> > > fmFinder(fmIndex);
	while (find(fmFinder, "AAAAAAAAAA")) { position(fmFinder); } // donutenie vytvorenia indexu pred zacatim merania casu
	list<int> out;
	milliseconds ms = duration_cast< milliseconds >(
		system_clock::now().time_since_epoch()
		);
	for (int i = 0; i < pocetTestovani; i++)
	{
		for (list<DnaString>::iterator iter = queries.begin(); iter != queries.end(); iter++)
		{

			while (find(fmFinder, *iter))
			{
				out.push_back(position(fmFinder));
			}
			clear(fmFinder);
		}
	}
	milliseconds ms2 = duration_cast< milliseconds >(
		system_clock::now().time_since_epoch()
		);
	printTestResults("FM-Index", out.size(), (ms2 - ms).count() / 10);
	return out;
}

int readDnaStringFromFile(DnaString &out, const char *file)
{
	CharString seqFileName = getAbsolutePath(file);
	SeqFileIn seqFileIn;
	if (!open(seqFileIn, toCString(seqFileName)))
	{
		cerr << "ERROR: Could not open the file.\n";
		return 1;
	}

	StringSet<CharString> ids;
	StringSet<CharString> seqs;

	try
	{
		readRecords(ids, seqs, seqFileIn);
	}
	catch (Exception const & e)
	{
		cout << "ERROR: " << e.what() << endl;
		return 1;
	}

	for (int i = 0; i < length(ids); ++i)
	{
		for (int j = 0; j < length(seqs[i]); j++)
		{
			if (seqs[i][j] == 'A' || seqs[i][j] == 'T' || seqs[i][j] == 'G' || seqs[i][j] == 'C')
			{
				out += seqs[i][j];
			}
		}
	}
	return 0;
}

int readQueriesFromFile(list<DnaString> &out, const char *file)
{
	CharString seqFileName = getAbsolutePath(file);
	SeqFileIn seqFileIn;
	if (!open(seqFileIn, toCString(seqFileName)))
	{
		cerr << "ERROR: Could not open the file.\n";
		return 1;
	}

	StringSet<CharString> ids;
	StringSet<CharString> seqs;

	try
	{
		readRecords(ids, seqs, seqFileIn);
	}
	catch (Exception const & e)
	{
		cout << "ERROR: " << e.what() << endl;
		return 1;
	}

	for (int i = 0; i < length(ids); ++i)
	{
		out.push_back(seqs[i]);
	}
	return 0;
}

int main()
{
	milliseconds ms = duration_cast< milliseconds >(
		system_clock::now().time_since_epoch()
		);
	DnaString text;
	readDnaStringFromFile(text, "/genome.fasta");
	//readDnaStringFromFile(text, "/genome2.fasta");

	list<DnaString> queries;
	readQueriesFromFile(queries, "/queries.fasta");

	milliseconds ms2 = duration_cast< milliseconds >(
		system_clock::now().time_since_epoch()
		);
	
	cout << "Nacitane vstupne data." << endl;
	cout << "Velkost retazca: " << length(text) << endl;
	cout << "Pocet queries: " << queries.size() << endl;
	cout << "time: " << (ms2 - ms).count() << endl;

	findQueriesTest(text, queries, 10);
	testFMIndex(text, queries, 10);

	cin.get();
	return 0;
}