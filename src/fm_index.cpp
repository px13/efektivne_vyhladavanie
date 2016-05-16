#include "fm_index.h"

FMIndex::FMIndex(seqan::DnaString &sequence)
{
	fmIndex = MyFMIndex1(sequence);
	fmFinder = MyFMFinder1(fmIndex);
}

void FMIndex::prepare()
{
	while (find(fmFinder, "AAAAAAAAAA")) { position(fmFinder); } //force index creation
	clear(fmFinder);
}

void FMIndex::query(vector<seqan::DnaString> &queries, vector<deque<int>> &out)
{
	auto queryIter = queries.begin();
	auto outIter = out.begin();
	for (; queryIter != queries.end(); ++queryIter, ++outIter)
	{
		while (find(fmFinder, *queryIter))
		{
			outIter->push_back(position(fmFinder));
		}
		clear(fmFinder);
	}
}