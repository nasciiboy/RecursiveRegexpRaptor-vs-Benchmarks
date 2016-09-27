include MakeGlobals

TARGET = runtest$(EXEC)

OBJS = main.o onig.o pcre2.o re2.o tre.o regexp3.o
LIBS = onig$(DIRSEP)libonig.a pcre2$(DIRSEP)libpcre2.a re2$(DIRSEP)libre2.a tre$(DIRSEP)libtre.a regexp3$(DIRSEP)libregexp3.a

.PHONY: all
all: $(TARGET)

%.o : %.c
	$(CC) $(CFLAGS) $^ -c -o $@

.PHONY: onig$(DIRSEP)libonig.a
onig$(DIRSEP)libonig.a :
	$(MAKE) -C onig

.PHONY: onig$(DIRSEP)libonig.a
onig$(DIRSEP)libonig.a :
	$(MAKE) -C onig


.PHONY: pcre2$(DIRSEP)libpcre2.a
pcre2$(DIRSEP)libpcre2.a :
	$(MAKE) -C pcre2

.PHONY: re2$(DIRSEP)libre2.a
re2$(DIRSEP)libre2.a :
	$(MAKE) -C re2

.PHONY: tre$(DIRSEP)libtre.a
tre$(DIRSEP)libtre.a :
	$(MAKE) -C tre

.PHONY: regexp3$(DIRSEP)libregexp3.a
regexp3$(DIRSEP)libregexp3.a :
	$(MAKE) -C regexp3

$(TARGET) : $(OBJS) $(LIBS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS)  -Lonig -lonig -Lpcre2 -lpcre2 -Lre2 -lre2 -Ltre -ltre -Lregexp3 -lregexp3

clean:
	$(RM) $(TARGET) $(OBJS)
	$(MAKE) -C onig clean
	$(MAKE) -C pcre2 clean
	$(MAKE) -C re2 clean
	$(MAKE) -C tre clean
	$(MAKE) -C regexp3 clean
