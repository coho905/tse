# CS50 TSE Indexer
## Implementation Spec

In this document we reference the [Requirements Specification](REQUIREMENTS.md) and [Design Specification](DESIGN.md) and focus on the implementation-specific decisions.
The knowledge unit noted that an [implementation spec](https://github.com/cs50dartmouthwi24/home/blob/main/knowledge/units/design.md#implementation-spec) may include many topics; not all are relevant to the TSE or the Indexer.
Here we focus on the core subset:

-  Data structures
-  Control flow: pseudo code for overall flow, and for each of the functions
-  Detailed function prototypes and their parameters
-  Error handling and recovery
-  Testing plan

## Data structures 

We use an index data structures: a 'hashtable' of words that have a counter to (docID, instances of Words)
The size of the hashtable (slots) is impossible to determine in advance, so we use 900.

## Control flow

The indexer is implemented in one file `indexer.c`, with three functions.

### main

The `main` function simply calls `parseArgs` and `indexBuild`, then exits zero.

### parseArgs

Given arguments from the command line, extract them into the function parameters; return only if successful.

* for `indexFilename`, validate it is not NULL and exists; file to print to
* for `pageDirectory`, call make sure the pageDirectory/.crawler is readable
* if any trouble is found, print an error to stderr and exit non-zero.

### indexBuild

Does the real work of creating the index from the pageDirectory given.
Pseudocode:

	initialize the index
	for page in PageDirectory
 		create webpage from page
		if webpage is not null,
  			indexPage webpage
		if fetch was successful,
			save the webpage to pageDirectory
			if the webpage is not at maxDepth,
				pageScan that HTML
		delete webpage
	index save
	delete the index

### indexPage

Goes through each webpage word by word and adds to the index
Pseudocode:

	for each word in webpage
		if word length is greater than 3
			normalize the word
			add word to index
		free word

## Other modules

### index
We create a re-usable module `index.c` to handle the index datatype used throughout
We chose to write this as a separate module, in `../common`, to encapsulate all the knowledge about how to initialize, create, add, and load an index.

```c
typedef struct index index_t; //defines the structure of the new index
index_t* index_new(int size); //creates new index
bool index_add(index_t* index, const char* key, int docID);  // adds key + docID to index, increases current value at that docID by 1 (or equal to one if initalizing new key)
bool index_save(index_t* index, const char* path); //saves index to a FILE at path
void index_delete(index_t* index); //deletes index + associated memory
bool index_addWithCount(index_t* index, const char* key, int docID, int count); // adds key + docID to a counter at the key value in the hashtable for index
```

### word
We create a re-usable module `word.c` to normalize words via the normalizeWord method. This converts ever letter in a word to lowercase.
```c
void normalizeWord(char* word);
```

### pagedir

We create a re-usable module `pagedir.c` to handles the *pagesaver*  mentioned in the design (writing a page to the pageDirectory), and marking it as a Crawler-produced pageDirectory (as required in the spec).
We chose to write this as a separate module, in `../common`, to encapsulate all the knowledge about how to initialize and validate a pageDirectory, and how to write and read page files, in one place... anticipating future use by the Indexer and Querier.

Pseudocode for `pagedir_load`:

	reads in from index file
 	creates new index object


Pseudocode for `pagedir_valid`:

	takes in directory
 	checks if pagedirectory/.crawler exists

### libcs50

We leverage the modules of libcs50, most notably `counters`, `hashtable`, and `webpage`.
See that directory for module interfaces.
The new `webpage` module allows us to represent pages as `webpage_t` objects, to fetch a page from the Internet, and to scan a (fetched) page for URLs; in that regard, it serves as the *pagefetcher* described in the design.

## Function prototypes

### indexer

Detailed descriptions of each function's interface is provided as a paragraph comment prior to each function's implementation in `crawler.c` and is not repeated here.

```c
static void indexBuild (const char* pageDirectory, const char* indexFilename);
static void indexPage(index_t* index, webpage_t* page, int docID);
static int parseArgs(const char* pageDirectory, const char* indexFilename, int args);
```

### pagedir

Detailed descriptions of each function's interface is provided as a paragraph comment prior to each function's declaration in `pagedir.h` and is not repeated here.

```c
bool pagedir_valid(const char* fileName);
void pagedir_load(index_t* index, const char* oldIndexFilename);
```

## Error handling and recovery

All the command-line parameters are rigorously checked before any data structures are allocated or work begins; problems result in a message printed to stderr and a non-zero exit status.

Out-of-memory errors are handled by variants of the `mem_assert` functions, which result in a message printed to stderr and a non-zero exit status.
We anticipate out-of-memory errors to be rare and thus allow the program to crash (cleanly) in this way.

All code uses defensive-programming tactics to catch and exit (using variants of the `mem_assert` functions), e.g., if a function receives bad parameters.

That said, certain errors are caught and handled internally: for example, `pagedir_init` returns false if there is any trouble creating the `.crawler` file, allowing the Crawler to decide what to do; the `webpage` module returns false when URLs are not retrievable, and the Crawler does not treat that as a fatal error.

## Testing plan

Here is an implementation-specific testing plan.

### Unit testing

There are only two units (indexer and pagedir).
The indexer represents the whole system and is covered below.
Indextest is the testing C file that loads an index using the pagedir module, then prints out the index via the index module. Use the testing.sh script or make test to run this testing script.
The pagedir unit is tiny; it could be tested using a small C 'driver' to invoke its functions with various arguments, but it is likely sufficient to observe its behavior during the system test.

### Regression testing

The crawler can take a long time to run on some sites when `maxDepth` is more than 2.
For routine regression tests, we crawl the `letters` site at moderate depths; save the pageDirectory from one working run to compare (with `diff -r`) against future runs.

> For Lab 5, you are not required to script regression tests, though you may find the technique useful for your own testing/debugging process.

### Integration/system testing

Test indexer with various invalid arguments. 2. no arguments 3. one argument 4. three or more arguments 5. invalid pageDirectory (non-existent path) 5. invalid pageDirectory (not a crawler directory) 6. invalid indexFile (non-existent path) 7. invalid indexFile (read-only directory) 7. invalid indexFile (existing, read-only file)
Run indexer on a variety of pageDirectories and use indextest as one means of validating the resulting index.
Run valgrind on both indexer and indextest to ensure no memory leaks or errors.

> For Lab 5, as noted in the assignment, you may submit a smaller test run.
> Furthermore, we recommend turning off detailed logging output for these tests, as they make `testing.out` rather large!
