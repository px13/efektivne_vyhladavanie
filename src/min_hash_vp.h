#include <seqan/sequence.h>
#include <deque>

using namespace std;

class MinHashVP
{
public:

	const int N;
	const int M;
	const int N_MINUS_M;
	const int PRED_M;

	bool preprocessDone;

	MinHashVP(seqan::DnaString &sequence, const int n, const int m);

	//Creating index.
	void prepare();

	//Searching queries and stores the result in the variable "out".
	void query(vector<seqan::DnaString> &queries, vector<deque<int>> &out);

	void setText(seqan::DnaString &sequence);

	//Finding the minimal substring in sequence from position "i" to position "end" and stores the result in the variable "result".
	void findMinSegment(int i, const int &end, int &result);

	//Encoding substring of "segment" from position "index" to position "end".
	unsigned int segmentToNumber(const seqan::DnaString &segment, int index, const int &end);

	//Comparing sequence from position (minSegPos - queryMinSegPos) to  position minSegPos and query from position 0 to position queryMinSegPos.
	//Comparing sequence from position (minSegPos + M) to  position (minSegPos - queryMinSegPos + N) and query from position (queryMinSegPos + M) to position N.
	bool isCorrect(seqan::DnaString &query, const int &queryMinSegPos, const int &minSegPos);

private:
	seqan::DnaString sequence;
	vector<pair<unsigned int, int>> map;

	void addMinSegmentToMap(const int &i, int &result);
	void findMinSegmentInQuery(const seqan::DnaString &query, int i, int &result);
	bool isLessSegment(const seqan::DnaString &text, int segment1, int segment2);
	void addSegmentToMap(const int &segment);

	void prepare2();

	void findQueries(vector<seqan::DnaString> &queries, vector<deque<int>> &out);
	void findQuery(seqan::DnaString &query, deque<int> &out);
};