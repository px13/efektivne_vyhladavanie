#include <seqan/sequence.h>
#include <deque>
#include <unordered_map>

using namespace std;

typedef seqan::Infix<seqan::DnaString>::Type  DnaInfix;

class MinHash
{
	public:

		const int N = 32;
		const int M = 13;
		const int N_MINUS_M = N - M;

		bool preprocessDone;

		MinHash(seqan::DnaString sequence, const int n = 32, const int m = 13);
		void query(vector<seqan::DnaString> &queries, vector<deque<int>> &out);
		void prepare();

		void setText(seqan::DnaString sequence);

	private:
		seqan::DnaString sequence;
		unordered_multimap<int, int> map;

		void findQuery(seqan::DnaString &query, deque<int> &out);
		void findQueries(vector<seqan::DnaString> &queries, vector<deque<int>> &out);

		void findMinSegmentOfLengthM(DnaInfix &temp, DnaInfix &out);
		void addSegmentToMap(const DnaInfix &segment);

		int isCorrect(seqan::DnaString &query, const int beginSegment, const DnaInfix &queryMinSegment);

		void minSegment(const DnaInfix &segment1, DnaInfix &segment2);
		int segmentToNumber(const DnaInfix &segment);
};