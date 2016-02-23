#include <seqan/sequence.h>
#include <seqan/index.h>
#include <list>

using namespace std;

typedef seqan::Infix<seqan::DnaString>::Type  DnaInfix;

class FMIndex
{
	public:

		FMIndex(seqan::DnaString text);
		void prepare();
		void query(list<seqan::DnaString> &queries, list<list<int>> &out);
		
	private:

		seqan::Index<seqan::DnaString, seqan::FMIndex<> > fmIndex;
		seqan::Finder<seqan::Index<seqan::DnaString, seqan::FMIndex<> > > fmFinder;

		
};