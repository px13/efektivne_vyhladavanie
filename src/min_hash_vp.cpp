#include "min_hash_vp.h"

MinHashVP::MinHashVP(seqan::DnaString &sequence, const int n, const int m) :
N(n),
M(m),
PRED_M(m - 1),
N_MINUS_M(n - m)
{
	this->sequence = sequence;
	this->preprocessDone = false;
}

void MinHashVP::prepare()
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
	prepare2();
	preprocessDone = true;
}

void MinHashVP::query(vector<seqan::DnaString> &queries, vector<deque<int>> &out)
{
	if (!preprocessDone)
	{
		prepare();
	}
	findQueries(queries, out);
}

void MinHashVP::setText(seqan::DnaString &sequence)
{
	this->sequence = sequence;
	this->preprocessDone = false;
}

void MinHashVP::addMinSegmentToMap(const int &i, int &result)
{
	findMinSegment(i, result + N_MINUS_M, result);
	addSegmentToMap(result);
}

void MinHashVP::findMinSegment(int i, const int &end, int &result)
{
	if (sequence[result].value != 0)
	{
		for (; i <= end; ++i)
		{
			if (sequence[i].value < sequence[result].value)
			{
				result = i;
				if (sequence[result].value == 0)
				{
					break;
				}
			}
		}
		i = result;
	}
	for (; i <= end; ++i)
	{
		if (sequence[i].value == sequence[result].value)
		{
			if (isLessSegment(sequence, i, result))
			{
				result = i;
			}
		}
	}
}

void MinHashVP::findMinSegmentInQuery(const seqan::DnaString &query, int i, int &result)
{
	if (query[result].value != 0)
	{
		while (i-- != 0)
		{
			if (query[i].value < query[result].value)
			{
				result = i;
				if (query[result].value == 0)
				{
					break;
				}
			}
		}
		i = result;
	}
	while (i-- != 0)
	{
		if (query[i].value == query[result].value)
		{
			if (isLessSegment(query, i, result))
			{
				result = i;
			}
		}
	}
}

bool MinHashVP::isLessSegment(const seqan::DnaString &text, int segment1, int segment2)
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
	}
	return false;
}

void MinHashVP::addSegmentToMap(const int &segment)
{
	map.push_back(pair<unsigned int, int>(segmentToNumber(sequence, segment, segment + PRED_M), segment));
}

void MinHashVP::prepare2()
{
	sort(map.begin(), map.end());
	map.shrink_to_fit();
}

void MinHashVP::findQueries(vector<seqan::DnaString> &queries, vector<deque<int>> &out)
{
	auto queryIter = queries.begin();
	auto outIter = out.begin();
	for (; queryIter != queries.end(); ++queryIter, ++outIter)
	{
		findQuery(*queryIter, *outIter);
	}
}

void MinHashVP::findQuery(seqan::DnaString &query, deque<int> &out)
{
	int queryMinSegment = N_MINUS_M;
	findMinSegmentInQuery(query, N_MINUS_M, queryMinSegment);
	unsigned int value = segmentToNumber(query, queryMinSegment, queryMinSegment + PRED_M);
	int begin = 0, end = map.size() - 1;
	int ix = (begin + end) >> 1;
	while (begin <= end)
	{
		auto iter = map.begin();
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
				if (isCorrect(query, queryMinSegment, iter->second))
				{
					out.push_back(iter->second - queryMinSegment);
				}
				++iter;
			} while (iter->first == value);

			while (iter2->first == value)
			{
				if (isCorrect(query, queryMinSegment, iter2->second))
				{
					out.push_back(iter2->second - queryMinSegment);
				}
				--iter2;
			}
			return;
		}
		ix = (begin + end) >> 1;
	}
}

bool MinHashVP::isCorrect(seqan::DnaString &query, const int &queryMinSegPos, const int &minSegPos)
{
	for (int i = queryMinSegPos, j = minSegPos; i--;)
	{
		if (query[i].value != sequence[--j].value)
		{
			return false;
		}
	}
	for (int i = queryMinSegPos + M, j = minSegPos + M; i < N; ++i, ++j)
	{
		if (query[i].value != sequence[j].value)
		{
			return false;
		}
	}
	return true;
}

unsigned int MinHashVP::segmentToNumber(const seqan::DnaString &segment, int index, const int &end)
{
	unsigned int result = 0;
	for (; index < end; ++index)
	{
		switch (segment[index].value)
		{
		case 1:
			++result;
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
	switch (segment[index].value)
	{
	case 1:
		++result;
	case 0:
		result <<= 1;
		break;
	case 3:
		result <<= 1;
		result += 3;
		break;
	case 2:
		result <<= 1;
		++result;
	}
	return result;
}