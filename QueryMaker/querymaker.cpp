#include "io_tool.h"

using namespace std;

seqan::DnaString stringGenerator(int length)
{
	seqan::DnaString str = "";
	for (int i = 0; i < length; i++)
	{
		str += rand() % 4;
	}
	return str;
}

int main(int argc, char *argv[])
{
	if (argc < 5)
	{
		return 1;
	}
	int n = 1;
	if (sscanf(argv[1], "%d", &n) != 1)
	{
		cerr << "ERROR: Invalid length of queries.\n";
		return 1;
	}
	int queriesNumber = 1;
	if (sscanf(argv[2], "%d", &queriesNumber) != 1)
	{
		cerr << "ERROR: Invalid number of queries.\n";
		return 1;
	}
	cout << n << endl;
	cout << queriesNumber << endl;
	seqan::DnaString in;
	for (int i = 3; i < argc - 1; i++)
	{
		IOtool io(argv[i]);
		io.readDnaString(in);
	}
	seqan::StringSet<seqan::DnaString> queries;
	for (int i = 0; i < queriesNumber / 2; i++)
	{
		appendValue(queries, stringGenerator(n));
	}
	for (int i = 0; i < queriesNumber / 2; i++)
	{
		appendValue(queries, infixWithLength(in, rand() % (length(in) - n), n));
	}
	
	IOtool io(argv[argc - 1]);
	io.writeQueries(queries);
	return 0;
}