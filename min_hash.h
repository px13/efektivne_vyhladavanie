#include <seqan/sequence.h>
#include <deque>
#include <unordered_map>

using namespace std;

class MinHash
{
	public:

		const int N = 32;
		const int M = 13;
		const int N_MINUS_M = N - M;

		bool preprocessDone;

		MinHash(seqan::DnaString &sequence, const int n = 32, const int m = 13);
		void prepare();
		void query(vector<seqan::DnaString> &queries, vector<deque<int>> &out);

		void setText(seqan::DnaString &sequence);

	private:
		seqan::DnaString sequence;
		unordered_multimap<int, int> map;

		void addMinSegmentToMap(int segment1, int &segment2);
		void findMinSegmentOfLengthM(const seqan::DnaString &text, int segment1, int &segment2);
		void minSegment(const seqan::DnaString &text, const int segment1, int &segment2);
		bool isLessSegment(const seqan::DnaString &text, int segment1, int segment2);
		void addSegmentToMap(const int segment);

		void findQueries(vector<seqan::DnaString> &queries, vector<deque<int>> &out);
		void findQuery(seqan::DnaString &query, deque<int> &out);
		int isCorrect(seqan::DnaString &query, const int beginSegment, const int queryMinSegment);
		
		int segmentToNumber(const seqan::DnaString &segment, int index, int end);
};