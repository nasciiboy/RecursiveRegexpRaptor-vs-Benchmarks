#include "common-c.h"
#include "tre/tre.h"

#include <string.h>

const char* engine = "tre";
const char* type   = "std";

int main( int argc, char *argv[] ){
  if( argc < 4 ){
    fprintf( stderr, "%s: wrong args", engine );
    return 1;
  }

  load_data( argv[ 1 ], engine );

  int n = atoi( argv[ 2 ] );
  if( n < 1 ){
    fprintf( stderr, "%s: n < 0", engine );
    return 1;
  }

  char buff[1024];
  sprintf( buff, "result.%s.%s", type, engine );

  FILE* file_output = fopen( buff, "a" );
  if (!file_output) {
    fprintf( stderr, "%s: Cannot open '%s'!\n", engine, buff );
    exit( 1 );
  }

  for( int i = 3; i < argc; i++ ){
    if( strcmp( argv[ i ], "---" ) == 0 ){
      fprintf( file_output, "---\n" );
      continue;
    }

    int repeat = n;
    const char* srexp = save_rexp( argv[i] );

    int err_val;
    regex_t regex;
    regmatch_t match[1];
    char *ptr;
    int len;
    TIME_TYPE start, end;
    int found, time, best_time = 0;

    err_val = tre_regcomp(&regex, argv[ i ], REG_EXTENDED | REG_NEWLINE);
    if (err_val != 0) {
      fprintf( stderr, "TRE compilation failed with error %d \"%s\"\n", err_val, srexp );
      fprintf( file_output, "---\n" );
      continue;
    }

    do {
      found = 0;
      ptr = data;
      len = data_len;
      GET_TIME(start);
      while (1) {
        err_val = tre_regnexec(&regex, ptr, len, 1, match, 0);
        if (err_val != 0)
          break;

        // printf("%.*s %ld %ld\n", ( (ptr - subject) + match[0].rm_eo) - ((ptr - subject) + match[0].rm_so), subject + ((ptr - subject) + match[0].rm_so), (ptr - subject) + match[0].rm_so, (ptr - subject) + match[0].rm_eo);

        found++;
        ptr += match[0].rm_eo;
        len -= match[0].rm_eo;
      }
      GET_TIME(end);
      time = TIME_DIFF_IN_MS( start, end );
      if (!best_time || time < best_time)
        best_time = time;
    } while (--repeat > 0);

    tre_regfree(&regex);

    printf( "loops %d, best time %dms, matches %d, rexp \"%s\"\n", n, best_time, found, srexp );
    fprintf( file_output, "%d %d \"%s\"\n", best_time, found, srexp );
  }

  fclose(file_output);
  if( data ) free( data );
  return 0;
}
