#include <seqan/sequence.h>
#include <list>
#include <unordered_map>

using namespace std;

typedef seqan::Infix<seqan::DnaString>::Type  DnaInfix;

class XYIndex
{
	public:

		int N = 32;
		int M = 13;
		int N_MINUS_M = N - M;

		bool preprocessDone;

		XYIndex(seqan::DnaString text, int n = 32, int m = 13);

		void query(list<seqan::DnaString> &queries, list<list<int>> &out);
		void prepare();

		void setText(seqan::DnaString text);
	private:
		seqan::DnaString text;
		unordered_multimap<long, int> map;

		void findQuery(seqan::DnaString &query, list<int> &out);
		void findQueries(list<seqan::DnaString> &queries, list<list<int>> &out);

		DnaInfix findMinSegmentOfLengthM(seqan::DnaString &in, int begin, const int end);
		void addSegmentToMap(const seqan::Segment<seqan::DnaString> &segment);

		bool isCorrect(seqan::DnaString &query, int beginSegment, DnaInfix &queryMinSegment);
};