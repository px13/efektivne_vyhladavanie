#include "hash_table.h"

long long stringToNumber(const seqan::Segment<seqan::DnaString> &segment)
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

HashTable::HashTable(seqan::DnaString text)
{
	this->text = text;
}

void HashTable::prepare()
{
	this->map.clear();
	DnaInfix lastMinSegment = seqan::infixWithLength(this->text, 0, 32);
	for (int i = 0; i + 32 <= length(this->text); i++)
	{
		this->map.insert(pair<long long, int>(stringToNumber(seqan::infixWithLength(this->text, i, 32)), i));
	}
	preprocessDone = true;
}

void HashTable::query(list<seqan::DnaString> &queries, list<list<int>> &out)
{
	if (!preprocessDone)
	{
		prepare();
	}
	findQueries(queries, out);
}

void HashTable::findQueries(list<seqan::DnaString> &queries, list<list<int>> &out)
{
	for (list<seqan::DnaString>::iterator listIterator = queries.begin(); listIterator != queries.end(); listIterator++)
	{
		list<int> temp;
		findQuery(*listIterator, temp);
		out.push_back(temp);
	}
}

void HashTable::findQuery(seqan::DnaString &query, list<int> &out)
{
	pair<unordered_multimap<long long, int>::iterator, unordered_multimap<long long, int>::iterator> mapIteratorPair = this->map.equal_range(stringToNumber(seqan::infixWithLength(query, 0, 32)));
	for (unordered_multimap<long long, int>::iterator mapIterator = mapIteratorPair.first; mapIterator != mapIteratorPair.second; ++mapIterator)
	{
		out.push_back((*mapIterator).second);
	}
}

void HashTable::setText(seqan::DnaString text)
{
	this->text = text;
	this->preprocessDone = false;
}