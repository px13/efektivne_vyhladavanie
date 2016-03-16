#include "min_hash_pair.h"

MinHashPair::MinHashPair(seqan::DnaString &sequence, const int n, const int m) :
N(n),
M(m),
N_MINUS_M(n - m)
{
	this->sequence = sequence;
	this->preprocessDone = false;
}

void MinHashPair::prepare()
{
	map.clear();
	DnaInfix lastMinSegment(sequence, 0, M);
	addMinSegmentToMap(DnaInfix(sequence, 1, 1 + M), lastMinSegment);
	for (int i = N_MINUS_M + 1, j = 1; i <= length(sequence) - M; ++i, ++j)
	{
		if (lastMinSegment.data_begin_position < j)
		{
			lastMinSegment = DnaInfix(sequence, j, j + M);
			addMinSegmentToMap(DnaInfix(sequence, j + 1, lastMinSegment.data_end_position + 1), lastMinSegment);
		}
		else
		{
			DnaInfix segment = DnaInfix(sequence, i, i + M);
			if (isLess(segment, lastMinSegment))
			{
				lastMinSegment = segment;
				addSegmentToMap(lastMinSegment);
			}

		}
	}
	spracuj();
	preprocessDone = true;
}

void MinHashPair::query(vector<seqan::DnaString> &queries, vector<deque<int>> &out)
{
	if (!preprocessDone)
	{
		prepare();
	}
	findQueries(queries, out);
}

void MinHashPair::setText(seqan::DnaString &sequence)
{
	this->sequence = sequence;
	this->preprocessDone = false;
}

void MinHashPair::addMinSegmentToMap(DnaInfix &segment1, DnaInfix &segment2)
{
	findMinSegmentOfLengthM(segment1, segment2);
	addSegmentToMap(segment2);
}

void MinHashPair::findMinSegmentOfLengthM(DnaInfix &segment1, DnaInfix &segment2)
{
	for (int i = 1; i <= N_MINUS_M; ++i, ++segment1.data_begin_position, ++segment1.data_end_position)
	{
		minSegment(segment1, segment2);
	}
}

void MinHashPair::minSegment(const DnaInfix &segment1, DnaInfix &segment2)
{
	if (isLess(segment1, segment2))
	{
		segment2 = segment1;
	}
}

void MinHashPair::addSegmentToMap(const DnaInfix &segment)
{
	map.insert(pair<int, int>(segmentToNumber(segment), segment.data_begin_position));
}

void MinHashPair::spracuj()
{
	deque<pair<int, int>> temp;
	for (auto iter1 = this->map.begin(), end = this->map.end();
		iter1 != end;
		iter1 = this->map.upper_bound(iter1->first)
		)
	{
		auto a = this->map.equal_range(iter1->first);
		for (multimap<int, int>::iterator iter2 = a.first; iter2 != a.second; ++iter2)
		{
			temp.push_back(pair<int, int>(iter1->first, iter2->second));
			lenPole++;
		}
	}
	pole.reserve(lenPole);
	copy(temp.begin(), temp.end(), back_inserter(pole));
	map.clear();
}

void MinHashPair::findQueries(vector<seqan::DnaString> &queries, vector<deque<int>> &out)
{
	auto queryIter = queries.begin();
	auto outIter = out.begin();
	for (; queryIter != queries.end(); ++queryIter, ++outIter)
	{
		findQuery(*queryIter, *outIter);
	}
}

void MinHashPair::findQuery(seqan::DnaString &query, deque<int> &out)
{
	DnaInfix queryMinSegment = DnaInfix(query, 0, M);
	findMinSegmentOfLengthM(DnaInfix(query, 1, 1 + M), queryMinSegment);
	int value = segmentToNumber(queryMinSegment);
	int min = 0, max = lenPole - 1;
	int ix = (min + max) >> 1;
	while (min <= max)
	{
		int aaa = pole[ix].first;
		if (aaa < value)
		{
			min = ix + 1;
		}
		else if (aaa > value)
		{
			max = ix - 1;
		}
		else
		{
			vector<pair<int, int>>::iterator iter = pole.begin();
			advance(iter, ix);
			while (iter->first == value && iter != pole.end())
			{
				if (int position = isCorrect(query, iter->second, queryMinSegment) != -1)
					out.push_back(position);
				++iter;
			}
			iter = pole.begin();
			advance(iter, ix - 1);
			while (iter->first == value && iter != pole.begin())
			{
				if (int position = isCorrect(query, iter->second, queryMinSegment) != -1)
					out.push_back(position);
				--iter;
			}
			return;
		}
		ix = (min + max) >> 1;
	}
}

int MinHashPair::isCorrect(seqan::DnaString &query, const int &beginSegment, const DnaInfix &queryMinSegment)
{
	int begin = beginSegment - queryMinSegment.data_begin_position;
	if (
		isEqual(
		DnaInfix(query, 0, queryMinSegment.data_begin_position),
		DnaInfix(sequence, begin, beginSegment)
		)
		&&
		isEqual(
		DnaInfix(query, queryMinSegment.data_end_position, N),
		DnaInfix(sequence, beginSegment + M, begin + N)
		)
		)
		return begin;
	return -1;
}

int MinHashPair::segmentToNumber(const DnaInfix &segment)
{
	int result = 0;
	for (int i = 0; i < M; i++)
	{
		switch (segment[i].value)
		{
		case 1:
			result++;
		case 0:
			result <<= 2;
			break;
		case 3:
			result++;
		case 2:
			result <<= 1;
			++result <<= 1;
		}
	}
	return result;
}