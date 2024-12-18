#!/bin/bash
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/ letters 1 10 #too many args
echo "Invalid Number of Args"
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/ letters #too few args
echo "Invalid Number of Args"
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/ letters -9  #out of bounds depth
echo "Invalid Range for Depth"
./crawler https://cs50tse.cs.dartmouth.edu/tse/letters/ letters 1 #invalid URL (https)
echo "Invalid URL"
./crawler https://google.com letters 0 #outside URL
mkdir testV
mkdir letters
mkdir toScrape
mkdir wiki
valgrind --leak-check=full --show-leak-kinds=all ./crawler http://cs50tse.cs.dartmouth.edu/tse/toScrape/ testV 1
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/ letters 0
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/ letters 1 #tryinf on letters
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/ letters 2 
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/ letters 10 
./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/ toScrape 0 #trying on ToScrape
./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/ toScrape 1 #trying on ToScrape
./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/ toScrape 2 #trying on ToScrape
./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/ toScrape 3 #trying on ToScrape
./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/ wiki 0 #trying on wiki
./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/ wiki 1 #trying on wiki
./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/ wiki 2 #trying on wiki
