include ../MakeGlobals

#CFLAGS += -DHAVE_CONFIG_H

TARGET = libonig.a

OBJS = ascii.o regcomp.o regenc.o regerror.o regexec.o regparse.o st.o

.PHONY: all
all: $(TARGET)

%.o : %.c
	$(CC) $(CFLAGS) $^ -c -o $@

$(TARGET) : $(OBJS)
	$(AR) $@ $^

clean:
	$(RM) $(TARGET) $(OBJS)
