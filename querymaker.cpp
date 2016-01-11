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

typedef Infix<DnaString>::Type  DnaInfix;

DnaString stringGenerator(int length)
{
	DnaString str = "";
	for (int i = 0; i < length; i++)
	{
		str += rand() % 4;
	}
	return str;
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

int writeQueriesToFile(StringSet<DnaString> &queries, const char *file)
{
	CharString seqFileName = getAbsolutePath(file);
	SeqFileOut seqFileOut;
	if (!open(seqFileOut, toCString(seqFileName)))
	{
		cerr << "ERROR: Could not open the file.\n";
		return 1;
	}
	for (int i = 0; i < length(queries); i++)
	{
		try
		{
			writeRecord(seqFileOut, i, queries[i]);
		}
		catch (Exception const & e)
		{
			cout << "ERROR: " << e.what() << endl;
			return 1;
		}
	}
	return 0;
}

int main()
{
	DnaString in;
	readDnaStringFromFile(in, "/genome.fasta");
	readDnaStringFromFile(in, "/genome2.fasta");
	StringSet<DnaString> queries;
	for (int i = 0; i < 1000; i++)
	{
		appendValue(queries, stringGenerator(32));
	}
	for (int i = 0; i < 1000; i++)
	{
		appendValue(queries, infixWithLength(in, rand() % length(in), 32));
	}
	writeQueriesToFile(queries, "/queries.fasta");
	return 0;
}
