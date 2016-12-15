#include <time.h>

#define TIME_TYPE clock_t
#define GET_TIME( time ) time = clock()
#define GET_TIME_RESOLUTION( res ) res = CLOCKS_PER_SEC
#define TIME_DIFF_IN_MS( begin, end ) (int)(( end - begin ) * 1000 / CLOCKS_PER_SEC )

// Common result print
void printResult(char* name, int time, int found);

// 0 - interpreter
// 1 - dfa
// 2 - jit
void pcre2_find_all  ( char *pattern, char *subject, int subject_len, int repeat, int mode);

void tre_find_all    ( char *pattern, char *subject, int subject_len, int repeat );
void re2_find_all    ( char *pattern, char *subject, int subject_len, int repeat );
void onig_find_all   ( char *pattern, char *subject, int subject_len, int repeat );
void regexp3_find_all( char *pattern, char *subject, int subject_len, int repeat );
void regexp4_find_all( char *pattern, char *subject, int subject_len, int repeat );
