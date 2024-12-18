# tse
Tiny Search Engine was created for Dartmouth CS50. The project is has 3 components:

1. Crawler: Crawls through a website and retrieves all connected webpages up until a given depth
2. Indexer: Builds an index from the crawler directory
3. Querier: Returns webpages based on queries (supports 'and' and 'or' operators)
Please see individual directories for IMPLEMENTATION specs and more information.

To build: run make.</br>
To clean up: run make clean.</br>
There is NO global make test, to test the one of the components - you must go to the respective directory.</br>

### Use
To run it fully, first run the crawler, then the indexer, and lastly the querier. Running the querier allows for user input of strings to search for.

## Notes
+ Only run the crawler on:  https://cs50tse.cs.dartmouth.edu/tse/ or wherever the CS50 database is.
+ This code should NOT, in any capacity, be used in any future CS50 projects
+ Reach out if you want to learn more about it and/or get it running on your machine

MIT License, Copyright (c) Colin Wolfe - 2024
