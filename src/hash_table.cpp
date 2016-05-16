#include "hash_table.h"

HashTable::HashTable(seqan::DnaString &sequence)
{
	qIndex = MyQIndex(sequence);
	qFinder = MyQFinder(qIndex);
}

void HashTable::prepare()
{
	while (find(qFinder, "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA")) { position(qFinder); } //force index creation
	clear(qFinder);
}

void HashTable::query(vector<seqan::DnaString> &queries, vector<deque<int>> &out)
{
	auto queryIter = queries.begin();
	auto outIter = out.begin();
	for (; queryIter != queries.end(); ++queryIter, ++outIter)
	{
		while (find(qFinder, *queryIter))
		{
			outIter->push_back(position(qFinder));
		}
		clear(qFinder);
	}
}
