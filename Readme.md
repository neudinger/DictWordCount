# Dict Wordcount


```md
Write a utility with the following syntax: 
wordcount dictfile [textfile1 textfile2 ...]
wordcount parses a dictionary file as follows:

- a line with a single word defines a word to be counted in the input stream.

- empty lines and lines starting with # are ignored.

In the following cases, the program will stop with an error:

- if a line of the dictionary has more than one word.- 

if a word appears more than once in the dictionary.

wordcount then processes its input stream: the concatenation of all text files specified on the command line after the dictionary file or standard input if none is given.

wordcount must count the number of occurrence of any word in the dictfile that appears in the input stream, and also count the total number of words. Matching is performed on a full word basis. Words are delimited by white space.

The words and their counts can be written in any order: no constraint.

The format for output lines should be "<count> TAB <word>" which would yield something like: 

10    foo

104  bar

20    frotz

19274 total words

Implementation should be simple and efficient, dictionary size should be limited only by memory. An arbitrary limit on word length is OK for the dictionary, but should be unnecessary on input stream.

You should not spend more than 3 or 4 hours on this exercise. We do not insist on a complete solution, however, we ask for a small document describing the work accomplished: data structure choice, program limitations, possible areas for improvement...

Your submission will be evaluated on the following aspects:

- Presentation, readability, conciseness.

- Complexity, memory complexity, efficiency.

- Correctness, sturdiness, memory sanity.

- Compilation without errors (reference compiler: gcc, language version: C99, system: linux).

- Documentation, tests.

The answer should be submitted as an archive file named NAME_SURNAME_wordcount.{zip,rar,7z,tar.gz,...} containing at least:

- The source code of the utility.

- The document explaining your work (in text or pdf format for example).

The archive file can also contain:

- Building tools files.

- Test files you built.

- Testing source code you developed.

- Versioning system files (.git directory for example).

- Any additional file you think is relevant.
```

## Limitation

The usage of multithreading is useless in this case and condition due to high IO operations, shared file descriptor and low computation intensity.

And thefore disabled by default in cmake configuration.

The speed is only 10% faster than the syncronous version.

OpenMP was used to perform asyncronous task across the cpu core.

To benefit from parallelism performance, you need to use a file format compatible with parallel io, such as parquet or hdf5.

## Complexity, memory complexity, efficiency.

To find if a word stored in the dict is present in the text document a hash table with a collision conflict resolution was built.
Inspired from the Python `Dict()` hash table indexation object.

The time complexity is linear $T=(O_n)$

Due to static memory instantiation 
The limitation of this project is as follows:
- A word cannot be longer than 100 characters
- A maximum of 1000 can be indexed
```C
#define MAX_WORD_LENGTH 100
#define MAX_WORDS_TABLE 1000
```
This limitation can be easely lift up with the usage of dynamic memory allocation (std::vector, malloc, realoc) in the Dictionary structure.

## Compilation without errors (reference compiler: gcc, language version: C99, system: linux).

### Requirement

- Linux (can work on mac and windows)
- Cmake
- gcc

### Build

In the project directory:
```bash
cmake -B build
cmake --build build
```

```
./build/bin/dict_word test_files/dictionary1 test_files/textfile
./build/bin/dict_word test_files/dictionary2 test_files/textfile
./build/bin/dict_word test_files/dictionary3 test_files/textfile
./build/bin/dict_word test_files/dictionary4 test_files/textfile
./build/bin/dict_word test_files/dictionary5 test_files/textfile
```

### Improvement

- [ ] unit testing will be added with Catch2
- [ ] bdd_test testing will be added with
- [ ] bench_test will be added with
- [ ] analyse performance results analysis with slt_bench

Fully integrated in Cmake with CTest
