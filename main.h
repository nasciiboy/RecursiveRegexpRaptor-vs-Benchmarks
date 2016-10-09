#ifdef WIN32

#include <windows.h>

#define TIME_TYPE LARGE_INTEGER
#define GET_TIME(res) do { QueryPerformanceCounter(&(res)); } while(0)
#define GET_TIME_RESOLUTION(res) do { QueryPerformanceFrequency(&(res)); } while(0)
#define TIME_DIFF_IN_MS(begin, end, resolution) ((int)(((end).QuadPart - (begin).QuadPart) * 1000 / (resolution).QuadPart))

#else

#include <time.h>

#define TIME_TYPE clock_t
#define GET_TIME(res) do { res = clock(); } while(0)
#define GET_TIME_RESOLUTION(res) do { res = CLOCKS_PER_SEC; } while(0)
#define TIME_DIFF_IN_MS(begin, end, resolution) ((int)((double)((end) - (begin)) * 1000 / (resolution)))

#endif

// Common result print
void printResult(char* name, int time, int found);

// 0 - interpreter
// 1 - dfa
// 2 - jit
void pcre2_find_all(char* pattern, char* subject, int subject_len, int repeat, int mode);
void tre_find_all(char* pattern, char* subject, int subject_len, int repeat);
void re2_find_all(char* pattern, char* subject, int subject_len, int repeat);
void onig_find_all(char* pattern, char* subject, int subject_len, int repeat);
void regexp3_find_all(char* pattern, char* subject, int subject_len, int repeat );
void regexp4_find_all(char* pattern, char* subject, int subject_len, int repeat );
