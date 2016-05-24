## Efficient searching of short sequences in long sequence (Bachelor thesis)


### Examples of use.

Reading sequences/queries from file.


```C++
seqan::DnaString sequence;
IOtool(file).readDnaString(sequence);

vector<seqan::DnaString> queries;
IOtool(file).readQueries(queries);
```

#### Creating index and searching queries.

```C++
MinHashUM index(sequence, N, M); // N - queries length, M - minimal substring length
	// or
MinHashVP index(sequence, N, M);
	// or
HashTable index(sequence);
	// or
FMIndex index(sequence);

index.prepare() // Creating index

vector<deque<int>> result(queries.size());
idnex.find(queries, result) // Searching results are store in variable "result"
```


#### Run benchmark.

```C++
Benchmark benchmark(sequence, queries, C); // C - number of test cycles

benchmark.test(A, result, N, M); // A - algorithm number (0 - MinHash (UM), 1 - MinHash (VP), 2 - HashTable, 3 - FM-index)
```


#### Parameters to programs benchmark and querymaker. 

bechmark.exe A N M C PATH_1 [PATH_2, ...] PATH_N 
A - algorithm number
N - queries length
M - minimal substring length for MinHash
C - number of test cycles
PATH_(1 - N-1) - paths to input sequences
PATH_N - path to input queries

querymaker.exe N C PATH_1 [PATH_2, ...] PATH_N
N - queries length
C - number of queries
PATH_(1 - N-1) - paths to input sequences
PATH_N - path to output file with queries