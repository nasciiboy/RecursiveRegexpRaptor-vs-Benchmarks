include ../MakeGlobals

REGEXP4 = regexp4_ascii.c

libregexp4.a : regexp4.o
	 $(AR) libregexp4.a regexp4.o

regexp4.o : $(REGEXP4) regexp4.h
	$(CC) $(CFLAGS) -c $(REGEXP4) -o regexp4.o

clean:
	$(RM) regexp4.o libregexp4.a
