#include "min_hash.h"

MinHash::MinHash(seqan::DnaString &sequence, const int n, const int m) :
	N(n),
	M(m),
	N_MINUS_M(n-m)
{
	this->sequence = sequence;
	this->preprocessDone = false;
}

void MinHash::prepare()
{
	map.clear();
	int lastMinSegment = 0;
	addMinSegmentToMap(1, lastMinSegment);
	for (int i = N_MINUS_M + 1, j = 1; i <= length(sequence) - M; ++i, ++j)
	{
		if (lastMinSegment < j)
		{
			lastMinSegment = j;
			addMinSegmentToMap(j + 1, lastMinSegment);
		}
		else
		{
			if (isLessSegment(sequence, i, lastMinSegment))
			{
				lastMinSegment = i;
				addSegmentToMap(lastMinSegment);
			}

		}
	}
	preprocessDone = true;
}

void MinHash::query(vector<seqan::DnaString> &queries, vector<deque<int>> &out)
{
	if (!preprocessDone)
	{
		prepare();
	}
	findQueries(queries, out);
}

void MinHash::setText(seqan::DnaString &sequence)
{
	this->sequence = sequence;
	this->preprocessDone = false;
}

void MinHash::addMinSegmentToMap(int segment1, int &segment2)
{
	findMinSegmentOfLengthM(sequence, segment1, segment2);
	addSegmentToMap(segment2);
}

void MinHash::findMinSegmentOfLengthM(const seqan::DnaString &text, int segment1, int &segment2)
{
	for (int i = 1; i <= N_MINUS_M; ++i, ++segment1)
	{
		minSegment(text, segment1, segment2);
	}
}

void MinHash::minSegment(const seqan::DnaString &text, const int segment1, int &segment2)
{
	if (isLessSegment(text, segment1, segment2))
	{
		segment2 = segment1;
	}
}

bool MinHash::isLessSegment(const seqan::DnaString &text, int segment1, int segment2)
{
	for (int end = segment1 + M; segment1 < end; ++segment1, ++segment2)
	{
		if (text[segment1].value < text[segment2].value)
		{
			return true;
		}
		else if (text[segment2].value < text[segment1].value)
		{
			return false;
		}
		else
		{
			continue;
		}
	}
	return false;
}

void MinHash::addSegmentToMap(const int segment)
{
	map.insert(pair<int, int>(segmentToNumber(sequence, segment, segment + 13), segment));
}

void MinHash::findQueries(vector<seqan::DnaString> &queries, vector<deque<int>> &out)
{
	auto queryIter = queries.begin();
	auto outIter = out.begin();
	for (; queryIter != queries.end(); ++queryIter, ++outIter)
	{
		findQuery(*queryIter, *outIter);
	}
}

void MinHash::findQuery(seqan::DnaString &query, deque<int> &out)
{
	int queryMinSegment = 0;
	findMinSegmentOfLengthM(query, 1, queryMinSegment);
	auto mapIteratorPair = map.equal_range(segmentToNumber(query, queryMinSegment, queryMinSegment + M));
	for (auto mapIterator = mapIteratorPair.first; mapIterator != mapIteratorPair.second; ++mapIterator)
	{
		if (int position = isCorrect(query, (*mapIterator).second, queryMinSegment) != -1)
			out.push_back(position);
	}
}

int MinHash::isCorrect(seqan::DnaString &query, const int beginSegment, const int queryMinSegment)
{
	int begin = beginSegment - queryMinSegment;
	for (int i = 0, j = begin; i < queryMinSegment; ++i, ++j)
	{
		if (query[i].value != sequence[j].value)
		{
			return -1;
		}
	}
	for (int i = queryMinSegment + M, j = beginSegment + M; i < N; ++i, ++j)
	{
		if (query[i].value != sequence[j].value)
		{
			return -1;
		}
	}
	return begin;
}

int MinHash::segmentToNumber(const seqan::DnaString &segment, int index, int end)
{
	int result = 0;
	for (; index < end; ++index)
	{
		switch (segment[index].value)
		{
		case 1:
			result++;
		case 0:
			result <<= 2;
			break;
		case 3:
			result <<= 2;
			result += 6;
			break;
		case 2:
			result <<= 2;
			result += 2;
		}
	}
	return result;
}