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

void MinHashPair::addMinSegmentToMap(int segment1, int &segment2)
{
	findMinSegmentOfLengthM(sequence, segment1, segment2);
	addSegmentToMap(segment2);
}

void MinHashPair::findMinSegmentOfLengthM(const seqan::DnaString &text, int segment1, int &segment2)
{
	for (int i = 1; i <= N_MINUS_M; ++i, ++segment1)
	{
		minSegment(text, segment1, segment2);
	}
}

void MinHashPair::minSegment(const seqan::DnaString &text, const int segment1, int &segment2)
{
	if (isLessSegment(text, segment1, segment2))
	{
		segment2 = segment1;
	}
}

bool MinHashPair::isLessSegment(const seqan::DnaString &text, int segment1, int segment2)
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

void MinHashPair::addSegmentToMap(const int segment)
{
	map.insert(pair<int, int>(segmentToNumber(sequence, segment, segment + 13), segment));
}

void MinHashPair::spracuj()
{
	pole.push_back(pair<int, int>(-1, -1));
	for (auto iter1 = this->map.begin(), end = this->map.end();
		iter1 != end;
		iter1 = this->map.upper_bound(iter1->first)
		)
	{
		auto a = this->map.equal_range(iter1->first);
		for (multimap<int, int>::iterator iter2 = a.first; iter2 != a.second; ++iter2)
		{
			pole.push_back(pair<int, int>(iter1->first, iter2->second));
		}
	}
	pole.push_back(pair<int, int>(-1, -1));
	pole.shrink_to_fit();
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
	int queryMinSegment = 0;
	findMinSegmentOfLengthM(query, 1, queryMinSegment);
	int value = segmentToNumber(query, queryMinSegment, queryMinSegment + M);

	int begin = 1, end = pole.size() - 1;
	int ix = (begin + end) >> 1;
	while (begin <= end)
	{
		auto iter = pole.begin();
		advance(iter, ix);
		if (iter->first < value)
		{
			begin = ix + 1;
		}
		else if (iter->first > value)
		{
			end = ix - 1;
		}
		else
		{
			auto iter2 = iter - 1;
			do
			{
				if (int position = isCorrect(query, iter->second, queryMinSegment) != -1)
					out.push_back(position);
				++iter;
			} while (iter->first == value);

			while (iter2->first == value)
			{
				if (int position = isCorrect(query, iter2->second, queryMinSegment) != -1)
					out.push_back(position);
				--iter2;
			}
			return;
		}
		ix = (begin + end) >> 1;

	}
}

int MinHashPair::isCorrect(seqan::DnaString &query, const int beginSegment, const int queryMinSegment)
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

int MinHashPair::segmentToNumber(const seqan::DnaString &segment, int index, int end)
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