#include <seqan/sequence.h>
#include <list>
#include <unordered_map>

using namespace std;

typedef seqan::Infix<seqan::DnaString>::Type  DnaInfix;

class HashTable
{
public:

	bool preprocessDone;

	HashTable(seqan::DnaString text);

	void query(list<seqan::DnaString> &queries, list<list<int>> &out);
	void prepare();

	void setText(seqan::DnaString text);
private:
	seqan::DnaString text;
	unordered_multimap<long long, int> map;

	void findQuery(seqan::DnaString &query, list<int> &out);
	void findQueries(list<seqan::DnaString> &queries, list<list<int>> &out);
};