OBJS = indexer.o ../libcs50/hashtable.o ../libcs50/webpage.o ../common/pagedir.o ../common/index.o ../common/word.o
LLIBS = ../libcs50/libcs50-given.a ../libcs50/libcs50.a ../common/common.a
CFLAGS= -Wall -pedantic -std=c11 -ggdb -I../libcs50 -I../common -L../common -L../libcs50
CC = gcc
MAKE= make
VALGRIND = myvalgrind
OBJS2 = indextest.o ../libcs50/hashtable.o ../libcs50/webpage.o ../common/pagedir.o ../common/index.o ../common/word.o

.PHONY:	indexer indexer2 indextest test clean all cleanFile

indexer:
	make all

indexer2:	$(OBJS)
	$(CC) $(CFLAGS) $^ $(LLIBS) $(LIBS) -o indexer

indexer.o:	indexer.c 

indextest:	$(OBJS2)
	$(CC) $(CFLAGS) $^ $(LLIBS) $(LIBS) -o $@

indextest.o:	indextest.c 

all:
	make indexer2
	make indextest

test: 
	make all
	chmod +x testing.sh
	./testing.sh &> testing.out

clean:
	rm -r indexer.o
	rm -r indexer
	rm -r indextest.o
	rm -r indextest

cleanFile:
	rm -r toScrape1.index
	rm -r toScrape1Test.index
	rm -r toScrape2.index
	rm -r toScrape2Test.index
	rm -r wiki2.index
	rm -r wiki2test.index
