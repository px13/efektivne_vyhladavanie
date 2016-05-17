#include "io_tool.h"

using namespace std;

IOtool::IOtool(char * file)
{
	this->file = file;
}

int IOtool::readFromFile()
{
	seqan::SeqFileIn seqFileIn;
	if (!open(seqFileIn, file))
	{
		cerr << "ERROR: Could not open the file.\n";
		return 1;
	}

	try
	{
		readRecords(this->ids, this->seqs, seqFileIn);
	}
	catch (seqan::Exception const & e)
	{
		cout << "ERROR: " << e.what() << endl;
		return 1;
	}
	return 0;
}

seqan::DnaString IOtool::readDnaString(seqan::DnaString &out)
{
	seqan::StringSet<seqan::Dna> buffer;
	if (readFromFile() == 0)
	{
		for (int i = 0; i < length(this->ids); ++i)
		{
			for (int j = 0; j < length(this->seqs[i]); j++)
			{
				addCharToDnaString(this->seqs[i][j], out);
			}
		}
	}
	return out;
}

void IOtool::readQueries(vector<seqan::DnaString> &out)
{
	if (readFromFile() == 0)
	{
		for (int i = 0; i < length(this->ids); ++i)
		{
			seqan::DnaString query;
			for (int j = 0; j < length(this->seqs[i]); j++)
			{
				addCharToDnaString(this->seqs[i][j], query);
			}
			out.push_back(query);
		}	
	}
}

int IOtool::writeQueries(seqan::StringSet<seqan::DnaString> &queries)
{
	seqan::SeqFileOut seqFileOut;
	if (!open(seqFileOut, file))
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
		catch (seqan::Exception const & e)
		{
			cout << "ERROR: " << e.what() << endl;
			return 1;
		}
	}
	return 0;
}

void IOtool::addCharToDnaString(char ch, seqan::DnaString &s)
{
	if (ch == 'A' || ch == 'T' || ch == 'G' || ch == 'C')
	{
		seqan::appendValue(s, ch);
	}
}