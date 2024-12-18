# CS50 TSE Querier
## Implementation Spec

In this document we reference the [Requirements Specification](REQUIREMENTS.md) and [Design Specification](DESIGN.md) and focus on the implementation-specific decisions.
The knowledge unit noted that an [implementation spec](https://github.com/cs50dartmouthwi24/home/blob/main/knowledge/units/design.md#implementation-spec) may include many topics; not all are relevant to the TSE or the Querier.
Here we focus on the core subset:

-  Data structures
-  Control flow: pseudo code for overall flow, and for each of the functions
-  Detailed function prototypes and their parameters
-  Error handling and recovery
-  Testing plan

## Data structures 

We use an index data structures: a 'hashtable' of words that have a counter to (docID, instances of Words)
The size of the hashtable (slots) is the number of words in the index file.
We use counters to keep track of docID to instances of Words.
We have a struct of 2 counters used for manipulating the final values.
There is a struct of 2 integers that keep track of the key value and count of the max item of the counter.

## Control flow

The querier is implemented in one file `querier.c`, with many functions.

### main

The `main` function calls `parseArgs` to determine if the files are correct. Then it parses through the logic of exiting while the response is NULL, verifying all characters are in alphabetic or spaces, makes sure no "and" or "or" lead, end, or are adjacent in the string. Then determines the score counter, prints it in descending, and takes care of cleaning up the memory.

### parseArgs

Given arguments from the command line, extract them into the function parameters; return only if successful.

* for `indexFilename`, validate it is not NULL and exists; file to read from
* for `pageDirectory`, call make sure the pageDirectory/.crawler is readable
* if any trouble is found, print an error to stderr and exit non-zero.

### getScore

Does the real work of creating the counter from the words array given.
Pseudocode:

  instantiate results
  temp = NULL;
  for each word in the given array
    if word is not "and" or "or"
      curr = hashtable find at the word;
      if temp = NULL
        if curr != NULL
          temp = counters_copy(curr);
        else
          temp = counters_new();
      else
        tempCopy = copy of temp;
        if curr != NULL
          intersect tempCopy with curr
          delete temp
          temp is copy of tempCopy
          delete tempCopy
          tempCopy = NULL
        else{
          instatiate curr
          intersect tempCurr with curr;
          delete temp
          delete curr
          temp = copy of tempCopy
          delete tempCopy
          tempCopy = NULL
    else if word is "or"
      union result with temp
      delete temp
      temp = NULL
    else if word is and
      pass
  union on result from temp;
  delete temp;
  return result;

### sort

Prints each item in the counter in order by score
Pseudocode:

	instantiate max_t max
  	while max count > 0
    		max count = 0
    		max key = 0
    		iterate over counter using find_biggest
    		if max key > 0
      			set counter at max key to 0
      			get URL for docID
      			print info
### counters_copy

copies the informatino from one counter to another
Pseudocode:
	copy = new counter
  	iterate over counter using copy_helper
  	return copy

### checkArray
Makes sure the order of words is valid
Pseudocode:
	check if "and" or "or" start or finish the array
 	make sure neither conjuction is back to back at any point

### matches 
Determines number of matches for a given query
Pseudocode:
  sum = 0
  iterate over counter with find_total
  return sum;

### checkString
Makes sure the string is all valid characters
Pseudocode:
	for each character in the string
 		if any character is not alpha or space
   			return false
      	return true

### numWords
Determines number of words in a given string
	 count = 0
	 length = length of given sentence
  	if length = 0
    		return 0
  	if length > 0) 
    		lchar = sentence[0];
  for each character in the sentence
    if character is a space and last character is an alpha char
     	 count+=1;
    	lchar = sentence[i];
  return count;

### getURl
gets docID's index from directory
Pseudocode:
	open file for docID
 	url = file read line
  	return url

### prompt
Provided my Prof. Palmer; this prints a prompt if stdin is the terminal 
Pseudocode:
	If stdin is the terminal
 	print: query?

### parser
parses the sentence into words into given array
pseudocode:
	malloc space for given array
	for each character
 		if new word
   			add word to location at tracker
      			tracker ++
	
### Helper Functions

### find_biggest

sets max_t count and key to new values if its bigger than the previous one.
Pseudocode:
	max = item
  	if count > max count)
    		max key = key;
    		max count = count;

### intersect_helper

helps the intersect functionality
Pseudocode:
	newer = arg;
    	set newer->result to the minimum count of (count, newer->result at word)

### union_helper

helps the union functionality
Pseudocode:
	newer = arg;
    	set newer->result to the summation of (count, newer->result at word)

### copy_help
helps the copy function
Pseudocode:
	copy = arg;
  	set key and count in copy

### print_helper
helps printing the document information
Pseudocode:
  	if count != 0
    		print score, doc, URL
      
### find_total
helps printing the calculation of counter size
Pseudocode:
  	point = arg;
  	point's value increases by one


## Other modules

### index
We create a re-usable module `index.c` to handle the index datatype used throughout
We chose to write this as a separate module, in `../common`, to encapsulate all the knowledge about how to initialize, create, add, and load an index.
'''c
hashtable_t* index_getHashtable(index_t* index); //This returns the hashtable at the given index
'''

### libcs50

We leverage the modules of libcs50, most notably `counters`, `hashtable`, and `webpage`.
See that directory for module interfaces.
The new `webpage` module allows us to represent pages as `webpage_t` objects, to fetch a page from the Internet, and to scan a (fetched) page for URLs; in that regard, it serves as the *pagefetcher* described in the design.

## Error handling and recovery

All the command-line parameters are rigorously checked before any data structures are allocated or work begins; problems result in a message printed to stderr and a non-zero exit status.

Out-of-memory errors are handled by variants of the `mem_assert` functions, which result in a message printed to stderr and a non-zero exit status.
We anticipate out-of-memory errors to be rare and thus allow the program to crash (cleanly) in this way.

All code uses defensive-programming tactics to catch and exit (using variants of the `mem_assert` functions), e.g., if a function receives bad parameters.

That said, certain errors are caught and handled internally: for example, `pagedir_init` returns false if there is any trouble creating the `.crawler` file, allowing the Crawler to decide what to do; the `webpage` module returns false when URLs are not retrievable, and the Crawler does not treat that as a fatal error.

## Testing plan

Here is an implementation-specific testing plan.

### Unit testing

There is only one units (querier).
run _make test_ to run the given system testing for querier

### Regression Testing
> For Lab 6, you are not required to script regression tests, though you may find the technique useful for your own testing/debugging process.

### Integration/system testing

Test querier with various invalid arguments.
Run querier on a variety of pageDirectories and use fuzzquery
Run valgrind on querier ensure no memory leaks or errors.

> For Lab 6, as noted in the assignment, you may submit a smaller test run.
> Furthermore, we recommend turning off detailed logging output for these tests, as they make `testing.out` rather large!
