include ../MakeGlobals

CFLAGS += -I. -DHAVE_CONFIG_H=1

TARGET = libtre.a

OBJS = regcomp.o regerror.o regexec.o tre-ast.o \
	tre-compile.o tre-match-backtrack.o tre-match-parallel.o \
	tre-mem.o tre-parse.o tre-stack.o xmalloc.o

.PHONY: all
all: $(TARGET)

%.o : %.c
	$(CC) $(CFLAGS) $^ -c -o $@

$(TARGET) : $(OBJS)
	$(AR) $@ $^

clean:
	$(RM) $(TARGET) $(OBJS)
