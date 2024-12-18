./querier # no args
./querier few # too few args
./querier way too many # too many args
./querier nonCrawler/ ../../shared/tse/output/letters-1.index #bad crawler
./querier ../../shared/tse/output/letters-1 thisFileDNE # non existent index
./fuzzquery ~/cs50-dev/shared/tse/output/letters-1.index 10 0 | valgrind --leak-check=full --show-leak-kinds=all ./querier ~/cs50-dev/shared/tse/output/letters-1  ~/cs50-dev/shared/tse/output/letters-1.index     
./fuzzquery ~/cs50-dev/shared/tse/output/toscrape-2.index 10 0 | ./querier ~/cs50-dev/shared/tse/output/toscrape-2  ~/cs50-dev/shared/tse/output/toscrape-2.index      
./fuzzquery ~/cs50-dev/shared/tse/output/letters-2.index 10 0 | ./querier ~/cs50-dev/shared/tse/output/letters-2  ~/cs50-dev/shared/tse/output/letters-2.index
./fuzzquery ~/cs50-dev/shared/tse/output/toscrape-1.index 15 1 | ./querier ~/cs50-dev/shared/tse/output/toscrape-1  ~/cs50-dev/shared/tse/output/toscrape-1.index
