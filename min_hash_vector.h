#include <seqan/sequence.h>
#include <deque>
#include <unordered_map>

using namespace std;

typedef seqan::Infix<seqan::DnaString>::Type  DnaInfix;

class MinHashVector
{
public:

	const int N = 32;
	const int M = 13;
	const int N_MINUS_M = N - M;

	bool preprocessDone;

	MinHashVector(seqan::DnaString &sequence, const int n = 32, const int m = 13);
	void prepare();
	void query(vector<seqan::DnaString> &queries, vector<deque<int>> &out);

	void setText(seqan::DnaString &sequence);

private:
	seqan::DnaString sequence;
	unordered_multimap<int, int> map;
	vector<vector<int>> pole;

	void addMinSegmentToMap(DnaInfix &segment1, DnaInfix &segment2);
	void findMinSegmentOfLengthM(DnaInfix &segment1, DnaInfix &segment2);
	void minSegment(const DnaInfix &segment1, DnaInfix &segment2);
	void addSegmentToMap(const DnaInfix &segment);
	void spracuj();

	void findQueries(vector<seqan::DnaString> &queries, vector<deque<int>> &out);
	void findQuery(seqan::DnaString &query, deque<int> &out);
	int isCorrect(seqan::DnaString &query, const int &beginSegment, const DnaInfix &queryMinSegment);

	int segmentToNumber(const DnaInfix &segment);
};