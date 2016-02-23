#include "hash_table.h"

vector<long long> nasobky4 = { 4611686018427387904, 1152921504606846976, 288230376151711744, 72057594037927936, 18014398509481984, 4503599627370496, 1125899906842624, 281474976710656, 70368744177664, 17592186044416, 4398046511104, 1099511627776, 274877906944, 68719476736, 17179869184, 4294967296, 1073741824, 268435456, 67108864, 16777216, 4194304, 1048576, 262144, 65536, 16384, 4096, 1024, 256, 64, 16, 4, 1 };

long long to4(const seqan::Segment<seqan::DnaString> &segment)
{
	long long result = 0;
	for (int i = 0; i != segment.data_end_position - segment.data_begin_position; i++)
	{
		result += (segment[i].value * nasobky4[i]);
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
		this->map.insert(pair<long long, int>(to4(seqan::infixWithLength(this->text, i, 32)), i));
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
	pair<unordered_multimap<long long, int>::iterator, unordered_multimap<long long, int>::iterator> mapIteratorPair = this->map.equal_range(to4(seqan::infixWithLength(query, 0, 32)));
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