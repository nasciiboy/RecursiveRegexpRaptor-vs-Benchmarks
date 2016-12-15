include ../MakeGlobals

CFLAGS += -std=c99

REGEXP3 = regexp3_ascii.c

libregexp3.a : regexp3.o
	 $(AR) libregexp3.a regexp3.o

regexp3.o : $(REGEXP3) regexp3.h
	$(CC) $(CFLAGS) -c $(REGEXP3) -o regexp3.o

clean:
	$(RM) regexp3.o libregexp3.a
