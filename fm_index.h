#include <seqan/sequence.h>
#include <seqan/index.h>
#include <deque>

namespace seqan
{
	template <typename TSpec = void, typename TLengthSum = size_t>
	struct FMIndexConfig2
	{
		typedef TLengthSum                                  LengthSum;
		typedef WaveletTree<TSpec, WTRDConfig<LengthSum> >  Bwt;
		typedef Levels<TSpec, LevelsRDConfig<LengthSum> >   Sentinels;

		static const unsigned SAMPLING = 1;
	};
}

using namespace std;

typedef seqan::Infix<seqan::DnaString>::Type  DnaInfix;
typedef seqan::Index<seqan::DnaString, seqan::FMIndex<void, seqan::FMIndexConfig2<void, size_t>> > MyFMIndex;
typedef seqan::Finder<seqan::Index<seqan::DnaString, seqan::FMIndex<void, seqan::FMIndexConfig2<void, size_t>> > > MyFMFinder;

class FMIndex
{
	public:

		FMIndex(seqan::DnaString &sequence = seqan::DnaString());
		void prepare();
		void query(vector<seqan::DnaString> &queries, vector<deque<int>> &out);
		
	private:

		MyFMIndex fmIndex;
		MyFMFinder fmFinder;

};