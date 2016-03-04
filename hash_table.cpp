#include "hash_table.h"

HashTable::HashTable(seqan::DnaString &sequence)
{
	this->sequence = sequence;
	this->preprocessDone = false;
}

void HashTable::prepare()
{
	map.clear();
	DnaInfix lastMinSegment = DnaInfix(sequence, 0, 32);
	for (int i = 1; i <= length(sequence) - 32; i++)
	{
		map.emplace(segmentToNumber(DnaInfix(sequence, i, i + 32)), i);
	}
	preprocessDone = true;
}

void HashTable::query(vector<seqan::DnaString> &queries, vector<deque<int>> &out)
{
	if (!preprocessDone)
	{
		prepare();
	}
	findQueries(queries, out);
}

void HashTable::findQueries(vector<seqan::DnaString> &queries, vector<deque<int>> &out)
{
	auto queryIter = queries.begin();
	auto outIter = out.begin();
	for (; queryIter != queries.end(); ++queryIter, ++outIter)
	{
		findQuery(*queryIter, *outIter);
	}
}

void HashTable::findQuery(seqan::DnaString &query, deque<int> &out)
{
	auto mapIteratorPair = map.equal_range(segmentToNumber(DnaInfix(query, 0, 32)));
	for (auto mapIterator = mapIteratorPair.first; mapIterator != mapIteratorPair.second; ++mapIterator)
	{
		out.push_back((*mapIterator).second);
	}
}

void HashTable::setText(seqan::DnaString sequence)
{
	this->sequence = sequence;
	this->preprocessDone = false;
}

long long HashTable::segmentToNumber(const seqan::Segment<seqan::DnaString> &segment)
{
	long long result = 0;
	for (int i = 0; i < 32; ++i)
	{
		switch (segment[i].value)
		{
		case 1:
			++result;
		case 0:
			result <<= 2;
			break;
		case 3:
			++result;
		case 2:
			result <<= 1;
			++result <<= 1;
		}
	}
	return result;
}
