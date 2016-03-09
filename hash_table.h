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

	void query(vector<seqan::DnaString> &queries, vector<deque<int>> &out);
	void prepare();

	void setText(seqan::DnaString sequence);
private:

	MyQIndex qIndex;
	MyQFinder qFinder;
};