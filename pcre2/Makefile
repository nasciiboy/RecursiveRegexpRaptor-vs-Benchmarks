include ../MakeGlobals

CFLAGS += -DHAVE_CONFIG_H -DPCRE2_CODE_UNIT_WIDTH=8

TARGET = libpcre2.a

OBJS = pcre2_auto_possess.o pcre2_chartables.o pcre2_compile.o \
	pcre2_config.o pcre2_context.o pcre2_dfa_match.o \
	pcre2_find_bracket.o pcre2_jit_compile.o pcre2_match.o \
	pcre2_match_data.o pcre2_newline.o pcre2_ord2utf.o \
	pcre2_string_utils.o pcre2_study.o pcre2_tables.o \
	pcre2_ucd.o pcre2_valid_utf.o pcre2_xclass.o

.PHONY: all
all: $(TARGET)

%.o : %.c
	$(CC) $(CFLAGS) $^ -c -o $@

$(TARGET) : $(OBJS)
	$(AR) $@ $^

clean:
	$(RM) $(TARGET) $(OBJS)
