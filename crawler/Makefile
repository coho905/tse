OBJS = crawler.o ../libcs50/hashtable.o ../libcs50/bag.o ../libcs50/webpage.o ../common/pagedir.o ../libcs50/counters.o
LLIBS = ../libcs50/libcs50-given.a ../libcs50/libcs50.a ../common/common.a
CFLAGS= -Wall -pedantic -std=c11 -ggdb -I../libcs50 -I../common -L../common -L../libcs50
CC = gcc
MAKE= make
VALGRIND = myvalgrind


crawler:	$(OBJS)
	$(CC) $(CFLAGS) $^ $(LLIBS) $(LIBS) -o $@

crawler.o:	crawler.c 

test: 
	make crawler
	chmod +x testing.sh
	./testing.sh > testing.out

clean:
	rm -r crawler.o
	rm -r crawler

cleanALL:
	make clean
	rm -rf letters
	rm -rf testV
	rm -rf wiki
	rm -rf toScrape 
