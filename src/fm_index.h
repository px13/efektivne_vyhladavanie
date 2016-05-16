#include <seqan/sequence.h>
#include <seqan/index.h>
#include <deque>

//Settings for different SAMPLING.
namespace seqan
{
	template <typename TSpec = void, typename TLengthSum = size_t>
	struct FMIndexConfig1
	{
		typedef TLengthSum                                  LengthSum;
		typedef WaveletTree<TSpec, WTRDConfig<LengthSum> >  Bwt;
		typedef Levels<TSpec, LevelsRDConfig<LengthSum> >   Sentinels;

		static const unsigned SAMPLING = 1;
	};
}

using namespace std;

typedef seqan::Index<seqan::DnaString, seqan::FMIndex<void, seqan::FMIndexConfig1<void, size_t>> > MyFMIndex1;
typedef seqan::Finder<seqan::Index<seqan::DnaString, seqan::FMIndex<void, seqan::FMIndexConfig1<void, size_t>> > > MyFMFinder1;

class FMIndex
{
	public:

		FMIndex(seqan::DnaString &sequence = seqan::DnaString());

		//Creating index.
		void prepare();

		//Searching queries and stores the result in the variable "out".
		void query(vector<seqan::DnaString> &queries, vector<deque<int>> &out);
		
	private:

		MyFMIndex1 fmIndex;
		MyFMFinder1 fmFinder;

};
