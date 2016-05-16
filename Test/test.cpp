#include <iostream>
#include <seqan/sequence.h>
#include <seqan/modifier.h>
#include "min_hash_um.h"
#include "min_hash_vp.h"

using namespace std;

void findQueries(seqan::DnaString &sequence, vector<seqan::DnaString> &queries, vector<deque<int>> &result)
{
	for (int i = 0; i < queries.size(); i++)
	{
		for (int j = 0; j <= length(sequence) - 32; j++)
		{
			if (seqan::isEqual(seqan::infixWithLength(sequence, j, 32), seqan::infixWithLength(queries[i], 0, 32)))
			{
				result[i].push_back(j);
			}
		}
	}
}

bool isEqualResults(vector<deque<int>> &result, vector<deque<int>> &testResult)
{
	for (int i = 0; i < result.size(); i++)
	{
		if (result[i].size() != testResult[i].size())
		{
			return false;
		}
		sort(result[i].begin(), result[i].end());
		sort(testResult[i].begin(), testResult[i].end());
		if (!equal(result[i].begin(), result[i].end(), testResult[i].begin()))
		{
			return false;
		}
	}
	return true;
}

int findMinSegment(seqan::DnaString &sequence, int begin)
{
	int min = begin;
	seqan::Segment<seqan::DnaString> seg = seqan::infixWithLength(sequence, begin, 16);
	for (int i = begin + 1; i <= begin + 16; i++)
	{
		seqan::Segment<seqan::DnaString> temp = seqan::infixWithLength(sequence, i, 16);
		if (seqan::isLess(temp, seg))
		{
			min = i;
			seg = temp;
		}
	}
	return min;
}

bool testFindMinSegment(seqan::DnaString &sequence, int begin)
{
	int res = begin;
	int testRes;

	MinHashUM index(sequence, 32, 16);
	index.findMinSegment(begin + 1, begin + 16, res);

	testRes = findMinSegment(sequence, begin);

	return res == testRes;
}

seqan::DnaString numberToSegment(unsigned int c)
{
	seqan::DnaString result = "";
	while (length(result) != 16)
	{
		int temp = c & 1;
		c >>= 1;
		temp <<= 1;
		temp += (c & 1);
		switch (temp)
		{
		case 0:
			result += "A";
			break;
		case 1:
			result += "C";
			break;
		case 2:
			result += "G";
			break;
		case 3:
			result += "T";
			break;
		}
		c >>= 1;
	}
	return result;
}

bool testSegmentToNumber(seqan::DnaString &sequence, int begin)
{
	MinHashUM index(sequence, 32, 16);
	seqan::DnaString s = numberToSegment(index.segmentToNumber(sequence, begin, begin + 15));
	reverse(s);
	return seqan::isEqual(seqan::infixWithLength(sequence, begin, 16), s);
}

bool isCorrect(seqan::DnaString &sequence, seqan::DnaString &query, int queryMinSegPos, int minSegPos)
{
	return seqan::isEqual(seqan::infixWithLength(query, 0, 32), seqan::infixWithLength(sequence, minSegPos - queryMinSegPos, 32));
}

bool testIsCorrect(seqan::DnaString &sequence, seqan::DnaString &query, int queryMinSegPos, int minSegPos)
{
	bool res, testRes;

	MinHashUM index(sequence, 32, 16);
	res = index.isCorrect(query, queryMinSegPos, minSegPos);

	testRes = isCorrect(sequence, query, queryMinSegPos, minSegPos);

	return res == testRes;
}

bool testMinHashUM(seqan::DnaString &sequence, vector<seqan::DnaString> &queries)
{
	vector<deque<int>> result(queries.size());
	vector<deque<int>> testResult(queries.size());

	MinHashUM index(sequence, 32, 16);
	index.prepare();
	index.query(queries, result);

	findQueries(sequence, queries, testResult);

	return isEqualResults(result, testResult);
}

bool testMinHashVP(seqan::DnaString &sequence, vector<seqan::DnaString> &queries)
{
	vector<deque<int>> result(queries.size());
	vector<deque<int>> testResult(queries.size());

	MinHashVP index(sequence, 32, 16);
	index.prepare();
	index.query(queries, result);

	findQueries(sequence, queries, testResult);

	return isEqualResults(result, testResult);
}

void printTestingResult(string testName, bool result)
{
	cout << testName << " - " << ((result) ? "OK" : "X") << endl;
}

seqan::DnaString stringGenerator(int length)
{
	seqan::DnaString str = "";
	for (int i = 0; i < length; i++)
	{
		str += rand() % 4;
	}
	return str;
}


void test()
{
	bool result = true;

	bool r;
	seqan::DnaString s;
	vector<seqan::DnaString> q;
	seqan::DnaString query;

	cout << "MinHash test" << endl;

	cout << "1. Testing findMinSegment method" << endl;

	s = stringGenerator(5000);
	r = true;
	for (int i = 0; i < length(s) - 16; i++)
	{
		r = r && testFindMinSegment(s, i);
	}
	result = result && r;
	printTestingResult("1.a", r);

	s = stringGenerator(10000);
	r = true;
	for (int i = 0; i < length(s) - 16; i++)
	{
		r = r && testFindMinSegment(s, i);
	}
	result = result && r;
	printTestingResult("1.b", r);

	cout << "2. Testing segmentToNumber method" << endl;

	s = stringGenerator(5000);
	r = true;
	for (int i = 0; i < length(s) - 16; i++)
	{
		r = r && testSegmentToNumber(s, i);
	}
	result = result && r;
	printTestingResult("2.a", r);

	s = stringGenerator(10000);
	r = true;
	for (int i = 0; i < length(s) - 16; i++)
	{
		r = r && testSegmentToNumber(s, i);
	}
	result = result && r;
	printTestingResult("2.b", r);

	cout << "3. Testing isCorrect method" << endl;

	s = "TATACGTAGTGTAGTCGGTAATGATGTGCTGTGATTA";
	query = "ACGTAGTGTAGTCGGTAATGATGTGCTGTGAT";
	r = testIsCorrect(s, query, 16, 19);
	result = result && r;
	printTestingResult("3.a", r);

	s = "TATACGTAGTGTAGTCAATGATGTGCTGTGATTA";
	r = testIsCorrect(s, query, 16, 16);
	result = result && r;
	printTestingResult("3.b", r);

	s = "TATACGTAGCAATGATGTGCTGTGATTACTGCCG";
	r = testIsCorrect(s, query, 16, 10);
	result = result && r;
	printTestingResult("3.c", r);

	s = "TATACGTAGTGTAGTCGGTAATGATGTGCTGTGATTA";
	query = "ACGTAGTGAATGATGTGCTGTGATTAGTCGGT";
	r = testIsCorrect(s, query, 8, 19);
	result = result && r;
	printTestingResult("3.d", r);

	cout << "4. Testing searching with MinHashUM" << endl;

	s = "ACGTAGTGTAGTCGGTAATGATGTGCTGTGAT";
	q = { "ACGTAGTGTAGTCGGTAATGATGTGCTGTGAT", "AATGATGTGCTGTGATACGTAGTGTAGTCGGT", "AGCTGAGTAATGATGTGCTGTGATAGCGAGTG", "AGCTGAGTATTGATGTGCTGTGATAGCGAGTG" };
	r = testMinHashUM(s, q);
	result = result && r;
	printTestingResult("4.a", r);

	s = "AATGATGTGCTGTGATACGTAGTGTAGTCGGT";
	r = testMinHashUM(s, q);
	result = result && r;
	printTestingResult("4.b", r);

	s = "AGCTGAGTAATGATGTGCTGTGATAGCGAGTG";
	r = testMinHashUM(s, q);
	result = result && r;
	printTestingResult("4.c", r);

	s = "AGCTGAGTATTGATGTGCTGTGATAGCGAGTG";
	r = testMinHashUM(s, q);
	result = result && r;
	printTestingResult("4.d", r);

	s = stringGenerator(1000);
	q = {};
	for (int i = 0; i < 100; i++) q.push_back(seqan::infixWithLength(s, rand() % (length(s) - 32), 32));
	r = testMinHashUM(s, q);
	result = result && r;
	printTestingResult("4.e", r);

	s = stringGenerator(20000);
	q = {};
	for (int i = 0; i < 10000; i++) q.push_back(seqan::infixWithLength(s, rand() % (length(s) - 32), 32));
	r = testMinHashUM(s, q);
	result = result && r;
	printTestingResult("4.f", r);

	cout << "5. Testing searching with MinHashVP" << endl;

	s = "ACGTAGTGTAGTCGGTAATGATGTGCTGTGAT";
	q = { "ACGTAGTGTAGTCGGTAATGATGTGCTGTGAT", "AATGATGTGCTGTGATACGTAGTGTAGTCGGT", "AGCTGAGTAATGATGTGCTGTGATAGCGAGTG", "AGCTGAGTATTGATGTGCTGTGATAGCGAGTG" };
	r = testMinHashVP(s, q);
	result = result && r;
	printTestingResult("5.a", r);

	s = "AATGATGTGCTGTGATACGTAGTGTAGTCGGT";
	r = testMinHashVP(s, q);
	result = result && r;
	printTestingResult("5.b", r);

	s = "AGCTGAGTAATGATGTGCTGTGATAGCGAGTG";
	r = testMinHashVP(s, q);
	result = result && r;
	printTestingResult("5.c", r);

	s = "AGCTGAGTATTGATGTGCTGTGATAGCGAGTG";
	r = testMinHashVP(s, q);
	result = result && r;
	printTestingResult("5.d", r);

	s = stringGenerator(1000);
	q = {};
	for (int i = 0; i < 100; i++) q.push_back(seqan::infixWithLength(s, rand() % (length(s) - 32), 32));
	r = testMinHashVP(s, q);
	result = result && r;
	printTestingResult("5.e", r);

	s = stringGenerator(20000);
	q = {};
	for (int i = 0; i < 10000; i++) q.push_back(seqan::infixWithLength(s, rand() % (length(s) - 32), 32));
	r = testMinHashVP(s, q);
	result = result && r;
	printTestingResult("5.f", r);

	cout << "-----------------------" << endl;
	printTestingResult("Results", result);

}

int main()
{
	test();

	cin.get();

	return 0;
}


