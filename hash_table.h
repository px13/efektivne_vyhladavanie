#include <seqan/sequence.h>
#include <deque>
#include <unordered_map>

using namespace std;

typedef seqan::Infix<seqan::DnaString>::Type  DnaInfix;

class HashTable
{
public:

	bool preprocessDone;

	HashTable(seqan::DnaString &sequence = seqan::DnaString());

	void query(vector<seqan::DnaString> &queries, vector<deque<int>> &out);
	void prepare();

	void setText(seqan::DnaString sequence);
private:
	seqan::DnaString sequence;
	unordered_multimap<long long, int> map;

	void findQuery(seqan::DnaString &query, deque<int> &out);
	void findQueries(vector<seqan::DnaString> &queries, vector<deque<int>> &out);
	long long segmentToNumber(const seqan::Segment<seqan::DnaString> &segment);
};