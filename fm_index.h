#include <seqan/sequence.h>
#include <seqan/index.h>
#include <deque>

using namespace std;

typedef seqan::Infix<seqan::DnaString>::Type  DnaInfix;

class FMIndex
{
	public:

		FMIndex(seqan::DnaString &sequence = seqan::DnaString());
		void prepare();
		void query(vector<seqan::DnaString> &queries, vector<deque<int>> &out);
		
	private:

		seqan::Index<seqan::DnaString, seqan::FMIndex<> > fmIndex;
		seqan::Finder<seqan::Index<seqan::DnaString, seqan::FMIndex<> > > fmFinder;

};