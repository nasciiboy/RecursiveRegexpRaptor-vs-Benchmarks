REGEXP3 = regexp3_utf8.c
CC      = gcc
CFLAGS  = -O3 -static

libregexp3.a : regexp3.o
	 ar rcs libregexp3.a regexp3.o

regexp3.o : $(REGEXP3) regexp3.h
	$(CC) $(CFLAGS) -c $(REGEXP3) -o regexp3.o

clean:
	rm regexp3.o libregexp3.a
