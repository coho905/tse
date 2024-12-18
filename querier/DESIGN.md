# CS50 TSE Querier
## Design Spec

In this document we reference the [Requirements Specification](REQUIREMENTS.md) and focus on the implementation-independent design decisions.
The knowledge unit noted that an [design spec](https://github.com/cs50dartmouthwi24/home/blob/main/knowledge/units/design.md#design-spec) may include many topics; not all are relevant to the TSE or the Querier.
Here we focus on the core subset:

- User interface
- Inputs and outputs
- Functional decomposition into modules
- Pseudo code (plain English-like language) for logic/algorithmic flow
- Major data structures
- Testing plan

## User interface

As described in the [Requirements Spec](REQUIREMENTS.md), the querier's first interface with the user is on the command-line; it must always have 2 arguments.

```bash
$ crawler pageDirectory indexFile
```

For example, to query one of the CS50 test sites, store the pages found in a subdirectory `data` in the current directory, use this command line:

``` bash
$ mkdir ../data/letters
$ ./querier ../data/data.index ../data/
```
Then, the query, if valid command-line arguments, will prompt for a query. Then the query should be sentences (or words) of strictly alphabetic characters and spaces

## Inputs and outputs

*Input:* there are no file inputs; there are command-line parameters described above. There are also queries, which is a combination of words.

*Output:* Per the requirements spec, the Querier will print documents in descending order for matches. The querier is build to not act on improper strings, strings with conjuctions at the beginning, end, or adjacent.
For each document the querier prints:

 * the score with the associated document and query
 * the docID
 * the full page URL,

## Functional decomposition into modules

We anticipate the following modules or functions:

 1. *main*, which parses arguments and initializes other modules
 2. *parser*, which parses the query for individual words
 3. *getScore*, which finds the counter for an associated query and its index
 4. *sort*, which prints the returned counter in descending order
 5. *checkArray, checkString, matches*, which all help parse information and direct the user to the correct output

And some helper modules that provide data structures:

  1. *counters_union* returns union of 2 counters
  2. *counters_intersect* returns intersection of 2 counters
  3. *getURL* returns URL for given docID
  4. Several other even smaller functions designed to help the ones mentioned above

## Pseudo code for logic/algorithmic flow

The querier will run as follows:

if parseArgs is successful
    prompt
    response = read from stdin
    load index from argument [1]
    while reponse is not NULL
        while response is not valid string
            prompt
            response = read from stdin
            if response == NULL
              exit  
        words = number of words in response
        if words> 0
          test is an array that the parser updated using the response
          normalize every word in test
          if checkArray for test is successful
            counter = getScore for the query
            number = number of matches           
            if number is greater than 0
              print counter in descending order
            else
              printf no matches
            prompt
            response = read from stdin 
          else
            print "Invalid Query"
            prompt
            response = read from stdin 
        else
          prompt
          response = read from stdin 
      free all memory
  else
    print to stderr and exit

## Major data structures

Helper modules provide all the data structures we need:

- We use an index data structures: a 'hashtable' of words that have a counter to (docID, instances of Words)
- The size of the hashtable (slots) is the number of words in the index file. We use counters to keep track of docID to instances of Words.
- We have a struct of 2 counters used for manipulating the final values.
- There is a struct of 2 integers that keep track of the key value and count of the max item of the counter.

## Testing plan

A sampling of tests that should be run:

1. Test the program with various forms of incorrect command-line arguments to ensure that its command-line parsing, and validation of those parameters, works correctly.
2. Query repeatedly using fuzz-query
3. When you are confident that your querier runs well, run it yourself to check.
4. *make test* does this in a script
