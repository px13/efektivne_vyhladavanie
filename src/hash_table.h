#include <seqan/sequence.h>
#include <seqan/index.h>
#include <deque>

using namespace std;

typedef seqan::Infix<seqan::DnaString>::Type  DnaInfix;
typedef seqan::Index<seqan::DnaString, seqan::IndexQGram< seqan::UngappedShape<32>, seqan::OpenAddressing > > MyQIndex;
typedef seqan::Finder<seqan::Index<seqan::DnaString, seqan::IndexQGram< seqan::UngappedShape<32>, seqan::OpenAddressing > > > MyQFinder;

class HashTable
{
public:

	HashTable(seqan::DnaString &sequence = seqan::DnaString());

	//Creating index.
	void prepare();

	//Searching queries and stores the result in the variable "out".
	void query(vector<seqan::DnaString> &queries, vector<deque<int>> &out);

private:

	MyQIndex qIndex;
	MyQFinder qFinder;
};