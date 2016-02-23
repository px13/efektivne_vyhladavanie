#include "fm_index.h"

FMIndex::FMIndex(seqan::DnaString text)
{
	this->fmIndex = seqan::Index<seqan::DnaString, seqan::FMIndex<> >(text);
	this->fmFinder = seqan::Finder<seqan::Index<seqan::DnaString, seqan::FMIndex<> > >(this->fmIndex);
	
	prepare();
	
}

void FMIndex::prepare()
{
	while (find(this->fmFinder, "AAAAAAAAAA")) { position(this->fmFinder); }
	clear(fmFinder);
}

void FMIndex::query(list<seqan::DnaString> &queries, list<list<int>> &out)
{
	for (list<seqan::DnaString>::iterator iter = queries.begin(); iter != queries.end(); iter++)
	{
		list<int> temp;
		while (find(this->fmFinder, *iter))
		{
			temp.push_back(position(this->fmFinder));
		}
		clear(fmFinder);
		out.push_back(temp);
	}
}

