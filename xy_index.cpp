#include "xy_index.h"

long to10(const seqan::Segment<seqan::DnaString> &segment)
{
	long result = segment[0].value;
	for (int i = 1; i != segment.data_end_position - segment.data_begin_position; i++)
	{
		result *= 10;
		result += segment[i].value;
	}
	return result;
}

XYIndex::XYIndex(seqan::DnaString text, int n, int m)
{
	this->text = text;
	this->N = n;
	this->M = m;
	this->N_MINUS_M = n - m;
}

DnaInfix XYIndex::findMinSegmentOfLengthM(seqan::DnaString &in, int begin, const int end)
{
	DnaInfix minSegment = seqan::infixWithLength(in, begin, M);
	for (begin = begin + 1; begin + M <= end; begin++)
	{
		minSegment = min(seqan::infixWithLength(in, begin, M), minSegment);
	}
	return minSegment;
}

void XYIndex::addSegmentToMap(const seqan::Segment<seqan::DnaString> &segment)
{
	this->map.insert(pair<long, int>(to10(segment), segment.data_begin_position));
}

void XYIndex::prepare()
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

bool XYIndex::isCorrect(seqan::DnaString &query, int beginSegment, DnaInfix &queryMinSegment)
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

void XYIndex::query(list<seqan::DnaString> &queries, list<list<int>> &out)
{
	if (!preprocessDone)
	{
		prepare();
	}
	findQueries(queries, out);
}

void XYIndex::findQueries(list<seqan::DnaString> &queries, list<list<int>> &out)
{
	for (list<seqan::DnaString>::iterator listIterator = queries.begin(); listIterator != queries.end(); listIterator++)
	{
		list<int> temp;
		findQuery(*listIterator, temp);
		out.push_back(temp);
	}
}

void XYIndex::findQuery(seqan::DnaString &query, list<int> &out)
{
	DnaInfix queryMinSegment = findMinSegmentOfLengthM(query, 0, N);
	pair<unordered_multimap<long, int>::iterator, unordered_multimap<long, int>::iterator> mapIteratorPair = this->map.equal_range(to10(queryMinSegment));
	for (unordered_multimap<long, int>::iterator mapIterator = mapIteratorPair.first; mapIterator != mapIteratorPair.second; ++mapIterator)
	{
		if (isCorrect(query, (*mapIterator).second, queryMinSegment))
			out.push_back((*mapIterator).second - queryMinSegment.data_begin_position);
	}
}

void XYIndex::setText(seqan::DnaString text)
{
	this->text = text;
	this->preprocessDone = false;
}