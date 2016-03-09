#include "io_tool.cpp"

using namespace std;

typedef seqan::Infix<seqan::DnaString>::Type  DnaInfix;

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
	if (argc < 4)
	{
		return 1;
	}
	int queriesNumber = 1;
	if (sscanf(argv[1], "%d", &queriesNumber) != 1)
	{
		cerr << "ERROR: Invalid number of queries.\n";
		return 1;
	}
	seqan::DnaString in;
	for (int i = 2; i < argc - 1; i++)
	{
		IOtool io(argv[i]);
		io.readDnaString(in);
	}
	seqan::StringSet<seqan::DnaString> queries;
	for (int i = 0; i < queriesNumber / 2; i++)
	{
		appendValue(queries, stringGenerator(32));
	}
	for (int i = 0; i < queriesNumber / 2; i++)
	{
		appendValue(queries, infixWithLength(in, rand() % length(in), 32));
	}
	
	IOtool io(argv[argc - 1]);
	io.writeQueries(queries);
	return 0;
}