OBJS = pagedir.o webpage.o index.o word.o
LIB = common.a
CFLAGS = -I../libcs50

all: $(LIB)

pagedir.o: pagedir.c pagedir.h ../libcs50/webpage.h
	cc $(CFLAGS) -c -o pagedir.o pagedir.c

webpage.o: ../libcs50/webpage.c ../libcs50/webpage.h
	cc $(CFLAGS) -c -o webpage.o ../libcs50/webpage.c

index.o:	index.c index.h ../libcs50/webpage.h ../libcs50/hashtable.h ../libcs50/counters.h
	cc $(CFLAGS) -c -o index.o index.c	

word.o:	word.c word.h
	cc $ -c -o word.o word.c

$(LIB): $(OBJS)
	    ar cr $(LIB) $(OBJS)

clean:
	    rm -f $(OBJS) $(LIB)
