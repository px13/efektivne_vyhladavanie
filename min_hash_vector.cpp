#include "min_hash_vector.h"

MinHashVector::MinHashVector(seqan::DnaString sequence, const int n, const int m) :
N(n),
M(m),
N_MINUS_M(n - m)
{
	this->sequence = sequence;
	this->preprocessDone = false;
}

void MinHashVector::findMinSegmentOfLengthM(DnaInfix &temp, DnaInfix &out)
{
	minSegment(temp, out);
	for (int i = 2; i <= N_MINUS_M; i++)
	{
		++temp.data_begin_position;
		++temp.data_end_position;
		minSegment(temp, out);
	}
}

void MinHashVector::minSegment(const DnaInfix &segment1, DnaInfix &segment2)
{
	if (isLess(segment1, segment2))
	{
		segment2.data_begin_position = segment1.data_begin_position;
		segment2.data_end_position = segment1.data_end_position;
	}
}

void MinHashVector::addSegmentToMap(const DnaInfix &segment)
{
	map.emplace(segmentToNumber(segment), segment.data_begin_position);
}

void MinHashVector::prepare()
{
	map.clear();
	DnaInfix lastMinSegment(sequence, 0, M);
	findMinSegmentOfLengthM(DnaInfix(sequence, 1, 1 + M), lastMinSegment);
	addSegmentToMap(lastMinSegment);
	for (int i = N_MINUS_M + 1; i <= length(sequence) - M; i++)
	{
		int begin = i - N_MINUS_M;
		if (lastMinSegment.data_begin_position < begin)
		{
			int end = begin + M;
			lastMinSegment = DnaInfix(sequence, begin, end);
			findMinSegmentOfLengthM(DnaInfix(sequence, begin + 1, end + 1), lastMinSegment);
			addSegmentToMap(lastMinSegment);
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

int MinHashVector::isCorrect(seqan::DnaString &query, const int beginSegment, const DnaInfix &queryMinSegment)
{
	int begin = beginSegment - queryMinSegment.data_begin_position;
	int end = begin + N;
	if (
		isEqual(
		DnaInfix(query, 0, queryMinSegment.data_begin_position),
		DnaInfix(sequence, begin, beginSegment)
		)
		&&
		isEqual(
		DnaInfix(query, queryMinSegment.data_end_position, N),
		DnaInfix(sequence, beginSegment + M, end)
		)
		)
		return begin;
	return -1;
}

void MinHashVector::setText(seqan::DnaString sequence)
{
	this->sequence = sequence;
	this->preprocessDone = false;
}

void MinHashVector::query(vector<seqan::DnaString> &queries, vector<deque<int>> &out)
{
	if (!preprocessDone)
	{
		prepare();
	}
	findQueries(queries, out);
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
	findMinSegmentOfLengthM(seqan::infixWithLength(query, 1, M), queryMinSegment);
	vector<int> * ptr = &this->pole[segmentToNumber(queryMinSegment)];
	for (auto iter = ptr->begin(); iter != ptr->end(); ++iter)
	{
		if (int position = isCorrect(query, *iter, queryMinSegment) != -1)
			out.push_back(position);
	}
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