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
	if (argc < 3)
	{
		return 1;
	}
	IOtool io(argv[1]);
	seqan::DnaString in = io.readDnaString();
	seqan::StringSet<seqan::DnaString> queries;
	for (int i = 0; i < 1000; i++)
	{
		appendValue(queries, stringGenerator(32));
	}
	for (int i = 0; i < 1000; i++)
	{
		appendValue(queries, infixWithLength(in, rand() % length(in), 32));
	}
	
	io = IOtool(argv[2]);
	io.writeQueries(queries);
	return 0;
}