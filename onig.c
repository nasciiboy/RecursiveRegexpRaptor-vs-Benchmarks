#include "common-c.h"
#include "onig/oniguruma.h"

#include <string.h>

const char* engine = "onig";
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

    regex_t* reg;
    OnigRegion *region;
    TIME_TYPE start, end;
    unsigned char *ptr;
    int res, len, found, time, best_time = 0;

    int repeat = n;

    res = onig_new(&reg, (unsigned char *)argv[ i ], (unsigned char *)argv[ i ] + strlen((char* )argv[ i ]),
                   ONIG_OPTION_DEFAULT, ONIG_ENCODING_ASCII, ONIG_SYNTAX_DEFAULT, NULL);
    if (res != ONIG_NORMAL) {
      fprintf( stderr, "Onig: compilation failed \"%s\"\n", argv[ i ] );
      fprintf( file_output, "---\n" );
      continue;
    }
    region = onig_region_new();
    if (!region) {
      fprintf( stderr, "Onig: Cannot allocate region\n");
      fprintf( file_output, "---\n" );
      continue;
    }

    do {
      found = 0;
      ptr = (unsigned char *)data;
      len = data_len;

      GET_TIME( start );
      while (1) {
        res = onig_search(reg, ptr, ptr + len, ptr, ptr + len, region, ONIG_OPTION_NONE);
        if (res < 0)
          break;
        // printf("match: %d %d\n", (ptr - (unsigned char *)subject) + region->beg[0], (ptr - (unsigned char *)subject) + region->end[0]);
        ptr += region->end[0];
        len -= region->end[0];
        found++;
      }
      GET_TIME( end );
      time = TIME_DIFF_IN_MS( start, end );
      if (!best_time || time < best_time)
        best_time = time;
    } while (--repeat > 0);

    onig_region_free(region, 1);
    onig_free(reg);

    const char* srexp = save_rexp( argv[i] );

    printf( "loops %d, best time %dms, matches %d, rexp \"%s\"\n", n, best_time, found, srexp );
    fprintf( file_output, "%d %d \"%s\"\n", best_time, found, srexp );
  }

  fclose(file_output);
  if( data ) free( data );
  return 0;
}
