include ../MakeGlobals

CFLAGS += -I. -I.. -I..$(DIRSEP).. -DNO_THREADS

TARGET = libre2.a

OBJS = bitstate.o c_helper.o compile.o dfa.o \
	filtered_re2.o mimics_pcre.o nfa.o onepass.o \
	parse.o perl_groups.o prefilter.o prefilter_tree.o \
	prog.o re2.o regexp.o set.o \
	simplify.o stringpiece.o tostring.o \
	unicode_casefold.o unicode_groups.o \
	util$(DIRSEP)hash.o util$(DIRSEP)rune.o \
	util$(DIRSEP)stringprintf.o util$(DIRSEP)strutil.o

.PHONY: all
all: $(TARGET)

%.o : %.cc
	$(CC) $(CFLAGS) $^ -c -o $@

$(TARGET) : $(OBJS)
	$(AR) $@ $^

clean:
	$(RM) $(TARGET) $(OBJS)
