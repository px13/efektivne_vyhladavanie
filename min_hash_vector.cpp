#include "min_hash_vector.h"

MinHashVector::MinHashVector(seqan::DnaString &sequence, const int n, const int m) :
N(n),
M(m),
N_MINUS_M(n - m)
{
	this->sequence = sequence;
	this->preprocessDone = false;
}

void MinHashVector::prepare()
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

void MinHashVector::query(vector<seqan::DnaString> &queries, vector<deque<int>> &out)
{
	if (!preprocessDone)
	{
		prepare();
	}
	findQueries(queries, out);
}

void MinHashVector::setText(seqan::DnaString &sequence)
{
	this->sequence = sequence;
	this->preprocessDone = false;
}

void MinHashVector::addMinSegmentToMap(DnaInfix &segment1, DnaInfix &segment2)
{
	findMinSegmentOfLengthM(segment1, segment2);
	addSegmentToMap(segment2);
}

void MinHashVector::findMinSegmentOfLengthM(DnaInfix &segment1, DnaInfix &segment2)
{
	for (int i = 1; i <= N_MINUS_M; ++i, ++segment1.data_begin_position, ++segment1.data_end_position)
	{
		minSegment(segment1, segment2);
	}
}

void MinHashVector::minSegment(const DnaInfix &segment1, DnaInfix &segment2)
{
	if (isLess(segment1, segment2))
	{
		segment2 = segment1;
	}
}

void MinHashVector::addSegmentToMap(const DnaInfix &segment)
{
	map.insert(pair<int, int>(segmentToNumber(segment), segment.data_begin_position));
}

void MinHashVector::spracuj()
{
	this->pole.resize(134217728);
	for (auto iter1 = this->map.begin(), end = this->map.end();
		iter1 != end;
		iter1 = this->map.upper_bound(iter1->first)
		)
	{
		auto a = this->map.equal_range(iter1->first);
		for (unordered_multimap<int, int>::iterator iter2 = a.first; iter2 != a.second; ++iter2)
		{
			this->pole[iter1->first].push_back(iter2->second);
		}
	}
	this->map.clear();
	this->pole.shrink_to_fit();
}

void MinHashVector::findQueries(vector<seqan::DnaString> &queries, vector<deque<int>> &out)
{
	auto queryIter = queries.begin();
	auto outIter = out.begin();
	for (; queryIter != queries.end(); ++queryIter, ++outIter)
	{
		findQuery(*queryIter, *outIter);
	}
}

void MinHashVector::findQuery(seqan::DnaString &query, deque<int> &out)
{
	DnaInfix queryMinSegment = DnaInfix(query, 0, M);
	findMinSegmentOfLengthM(DnaInfix(query, 1, 1 + M), queryMinSegment);
	vector<int> * ptr = &this->pole[segmentToNumber(queryMinSegment)];
	for (auto iter = ptr->begin(); iter != ptr->end(); ++iter)
	{
		if (int position = isCorrect(query, *iter, queryMinSegment) != -1)
			out.push_back(position);
	}
}

int MinHashVector::isCorrect(seqan::DnaString &query, const int &beginSegment, const DnaInfix &queryMinSegment)
{
	int begin = beginSegment - queryMinSegment.data_begin_position;
	for (int i = 0, j = begin; i < queryMinSegment.data_begin_position; ++i, ++j)
	{
		if (query[i].value != sequence[j].value)
		{
			return -1;
		}
	}
	for (int i = queryMinSegment.data_end_position, j = beginSegment + M; i < N; ++i, ++j)
	{
		if (query[i].value != sequence[j].value)
		{
			return -1;
		}
	}
	return begin;
}

int MinHashVector::segmentToNumber(const DnaInfix &segment)
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