#include <iostream>
#include <seqan/sequence.h>
#include <seqan/seq_io.h>

using namespace std;

class IOtool
{
	public:
		char * file;
		seqan::StringSet<seqan::CharString> ids;
		seqan::StringSet<seqan::CharString> seqs;

		IOtool(char * file);

		//Reading sequence from file and stores it in the variable "out".
		seqan::DnaString readDnaString(seqan::DnaString &out);

		//Reading queries from file and stores them in the variable "out".
		void readQueries(vector<seqan::DnaString> &out);

		//Writing queries to file.
		int writeQueries(seqan::StringSet<seqan::DnaString> &queries);

	private:
		int readFromFile();
		void IOtool::addCharToDnaString(char ch, seqan::DnaString &s);
};