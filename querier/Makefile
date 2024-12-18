OBJS = querier.o ../libcs50/hashtable.o ../libcs50/webpage.o ../common/pagedir.o ../common/index.o ../libcs50/mem.o ../common/word.o ../libcs50/file.o
OBJS2 = fuzzquery.o ../libcs50/hashtable.o ../libcs50/webpage.o ../common/pagedir.o ../common/index.o ../libcs50/mem.o ../common/word.o ../libcs50/file.o
LLIBS = ../libcs50/libcs50-given.a ../libcs50/libcs50.a ../common/common.a
CFLAGS= -Wall -pedantic -std=c11 -ggdb -I../libcs50 -I../common -L../common -L../libcs50 -lm 
CC = gcc
MAKE= make
VALGRIND = myvalgrind
# OBJS2 = indextest.o ../libcs50/hashtable.o ../libcs50/webpage.o ../common/pagedir.o ../common/index.o ../common/word.o


querier:	$(OBJS)
	$(CC) $(CFLAGS) $^ $(LLIBS) $(LIBS) -o $@


.PHONY:	test clean fuzzquery cleanALL

querier.o:	querier.c


test: 
	make querier
	make fuzzquery
	chmod +x testing.sh
	./testing.sh &> testing.out

fuzzquery.o:	fuzzquery.c

fuzzquery:	$(OBJS2)
	$(CC) $(CFLAGS) $^ $(LLIBS) $(LIBS) -o $@

clean:
	rm -r querier.o
	rm -r querier

cleanALL:
	make clean
	rm -r fuzzquery.o
	rm -r fuzzquery
	rm -r vgcore*
