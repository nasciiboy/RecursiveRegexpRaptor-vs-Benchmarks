#include "common-c.h"
#include "regexp4/regexp4.h"

#include "re2/c_helper.h"

#include <string.h>

const char* engine = "re2";
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

    void* obj = get_re2_object( argv[i] );
    TIME_TYPE start, end;
    int found, time, best_time = 0;

    const char* srexp = save_rexp( argv[i] );

    if (!obj) {
      fprintf( stderr, "RE2 compilation failed %s\n", srexp );
      fprintf( file_output, "---\n" );
      continue;
    }

    do {
      GET_TIME( start );
      found = search_all_re2( obj, data, data_len );
      GET_TIME(end);
      time = TIME_DIFF_IN_MS( start, end );
      if (!best_time || time < best_time)
        best_time = time;
    } while (--repeat > 0);

    free_re2_object(obj);

    printf( "loops %d, best time %dms, matches %d, rexp \"%s\"\n", n, best_time, found, srexp );
    fprintf( file_output, "%d %d \"%s\"\n", best_time, found, srexp );
  }

  fclose(file_output);
  if( data ) free( data );
  return 0;
}
