#include "common-c.h"
#include "regexp4/regexp4.h"

#include <string.h>

const char* engine = "regexp4";
const char* type   = "raptor";

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

    TIME_TYPE start, end;
    int found, time, best_time = 0;
    int repeat = n;

    do{
      found = 0;

      GET_TIME( start );

      found = regexp4( data, argv[ i ], data_len );

      GET_TIME( end );
      time = TIME_DIFF_IN_MS( start, end );
      if( !best_time || time < best_time ) best_time = time;
    } while( --repeat > 0 );

    const char* srexp = save_rexp( argv[i] );

    printf( "loops %d, best time %dms, matches %d, rexp \"%s\"\n", n, best_time, found, srexp );
    fprintf( file_output, "%d %d \"%s\"\n", best_time, found, srexp );
  }

  fclose(file_output);
  if( data ) free( data );
  return 0;
}
