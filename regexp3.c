#include <stdio.h>

#include "regexp3/regexp3.h"
#include "main.h"

void regexp3_find_all(char* pattern, char* subject, int subject_len, int repeat ) {
  TIME_TYPE start, end, resolution;
  int found, time, best_time = 0;

  GET_TIME_RESOLUTION(resolution);

  do{
    found = 0;

    GET_TIME(start);

    found = regexp3( subject, pattern, subject_len );

    GET_TIME(end);
    time = TIME_DIFF_IN_MS(start, end, resolution);
    if (!best_time || time < best_time) best_time = time;
  } while (--repeat > 0);

  // printf("regexp3 log start\n" );

  // int i = 0, max = totCatch();
  // while( ++i <= max )
  //   printf( "%.*s %ld %ld\n", (int)lenCatch( i ), gpsCatch( i ), gpsCatch(i) - subject, (gpsCatch(i) - subject) + lenCatch(i));

  // printf("regexp3 log end\n" );

  printResult("regexp3", best_time, found);
}
