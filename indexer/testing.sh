#!/bin/bash
make all
./indexer #no params
./indexer thisOnlyHasOneParameter #one param
./indexer this has too many parameters #too many params
./indexer ../../shared/tse willFail #not created by crawler
./indexer thisIsNotADirectory/NotReal cool #not a directory
./indexer ../../shared/tse/output/toscrape-1/ tester/tester.index #not real
./indexer ../../shared/tse/output/toscrape-1/ toScrape1.index #toScrape 1
./indexer ../../shared/tse/output/wikipedia-2/ wiki2.index #wiki 2
valgrind --leak-check=full --show-leak-kinds=all ./indexer ../../shared/tse/output/toscrape-2/ toScrape2.index
valgrind --leak-check=full --show-leak-kinds=all ./indextest toScrape1.index toScrape1Test.index
./indextest toScrape2.index toScrape2Test.index
./indextest wiki2.index wiki2test.index
TOSCRAPE1=`./../../shared/tse/indexcmp toScrape1.index toScrape1Test.index`
TOSCRAPE2=`./../../shared/tse/indexcmp toScrape2.index toScrape2Test.index`
WIKI2=`./../../shared/tse/indexcmp wiki2.index wiki2test.index`

if [[ -z "${TOSCRAPE1}" ]]; then
  echo "to-scrape-1 passed"
fi

if [[ -z "${TOSCRAPE2}" ]]; then
  echo "to-scrape-2 passed"
fi

if [[ -z "${WIKI2}" ]]; then
  echo "wiki-2 passed"
fi
 
