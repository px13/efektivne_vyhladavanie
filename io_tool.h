#include <iostream>
#include <seqan/sequence.h>
#include <seqan/seq_io.h>

using namespace std;

class IOtool
{
	public:
		char *file;
		seqan::StringSet<seqan::CharString> ids;
		seqan::StringSet<seqan::CharString> seqs;
		IOtool(char *file);
		seqan::DnaString readDnaString(seqan::DnaString &out);
		list<seqan::DnaString> readQueries();
		int writeQueries(seqan::StringSet<seqan::DnaString> &queries);
	private:
		int readFromFile();
		void IOtool::addCharToDnaString(char ch, seqan::DnaString &s);
};