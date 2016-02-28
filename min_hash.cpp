#include "min_hash.h"

int stringToNumber(const seqan::Segment<seqan::DnaString> &segment)
{
	int result = 0;
	for (int i = 0; i < 13; ++i)
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

MinHash::MinHash(seqan::DnaString text, int n, int m)
{
	this->text = text;
	this->N = n;
	this->M = m;
	this->N_MINUS_M = n - m;
}

DnaInfix MinHash::findMinSegmentOfLengthM(seqan::DnaString &in, int begin, int end)
{
	end = end - M;
	DnaInfix minSegment = seqan::infixWithLength(in, begin, M);
	for (begin = begin + 1; begin <= end; ++begin)
	{
		DnaInfix temp = seqan::infixWithLength(in, begin, M);
		if (isLess(temp, minSegment))
		{
			minSegment = temp;
		}
	}
	return minSegment;
}

void MinHash::addSegmentToMap(const seqan::Segment<seqan::DnaString> &segment)
{
	this->map.insert(pair<int, int>(stringToNumber(segment), segment.data_begin_position));
}

void MinHash::prepare()
{
	this->map.clear();
	DnaInfix lastMinSegment = findMinSegmentOfLengthM(this->text, 0, N);
	addSegmentToMap(lastMinSegment);
	for (int i = N_MINUS_M + 1; i + M <= length(this->text); i++)
	{
		if (lastMinSegment.data_begin_position < i - N_MINUS_M)
		{
			lastMinSegment = findMinSegmentOfLengthM(this->text, i - N_MINUS_M, i + M);
			addSegmentToMap(lastMinSegment);
		}
		else
		{
			DnaInfix segment = seqan::infixWithLength(this->text, i, M);
			if (isLess(segment, lastMinSegment))
			{
				lastMinSegment = segment;
				addSegmentToMap(lastMinSegment);
			}

		}
	}
	preprocessDone = true;
}

bool MinHash::isCorrect(seqan::DnaString &query, int beginSegment, DnaInfix &queryMinSegment)
{
	return (
			isEqual(
				seqan::infixWithLength(query, 0, queryMinSegment.data_begin_position),
				seqan::infixWithLength(this->text, beginSegment - queryMinSegment.data_begin_position, queryMinSegment.data_begin_position)
			)
			&&
			isEqual(
				seqan::infixWithLength(query, queryMinSegment.data_end_position, N - queryMinSegment.data_end_position),
				seqan::infixWithLength(this->text, beginSegment + M, N - queryMinSegment.data_end_position)
			)
		);
}

void MinHash::query(list<seqan::DnaString> &queries, list<list<int>> &out)
{
	if (!preprocessDone)
	{
		prepare();
	}
	findQueries(queries, out);
}

void MinHash::findQueries(list<seqan::DnaString> &queries, list<list<int>> &out)
{
	for (list<seqan::DnaString>::iterator listIterator = queries.begin(); listIterator != queries.end(); listIterator++)
	{
		list<int> temp;
		findQuery(*listIterator, temp);
		out.push_back(temp);
	}
}

void MinHash::findQuery(seqan::DnaString &query, list<int> &out)
{
	DnaInfix queryMinSegment = findMinSegmentOfLengthM(query, 0, N);
	pair<unordered_multimap<int, int>::iterator, unordered_multimap<int, int>::iterator> mapIteratorPair = this->map.equal_range(stringToNumber(queryMinSegment));
	for (unordered_multimap<int, int>::iterator mapIterator = mapIteratorPair.first; mapIterator != mapIteratorPair.second; ++mapIterator)
	{
		if (isCorrect(query, (*mapIterator).second, queryMinSegment))
			out.push_back(
			(*mapIterator).second - queryMinSegment.data_begin_position);
	}
}

void MinHash::setText(seqan::DnaString text)
{
	this->text = text;
	this->preprocessDone = false;
}