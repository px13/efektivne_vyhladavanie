#include "fm_index.h"

FMIndex::FMIndex(seqan::DnaString &sequence)
{
	fmIndex = seqan::Index<seqan::DnaString, seqan::FMIndex<> >(sequence);
	fmFinder = seqan::Finder<seqan::Index<seqan::DnaString, seqan::FMIndex<> > >(this->fmIndex);
	
	prepare();	
}

void FMIndex::prepare()
{
	while (find(fmFinder, "AAAAAAAAAA") != NULL) { position(fmFinder); }
	clear(fmFinder);
}

void FMIndex::query(vector<seqan::DnaString> &queries, vector<deque<int>> &out)
{
	auto queryIter = queries.begin();
	auto outIter = out.begin();
	for (; queryIter != queries.end(); ++queryIter, ++outIter)
	{
		while (find(fmFinder, *queryIter) != NULL)
		{
			outIter->push_back(position(fmFinder));
		}
		clear(fmFinder);
	}
}

