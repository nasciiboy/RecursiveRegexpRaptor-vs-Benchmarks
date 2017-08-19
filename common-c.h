#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#define TIME_TYPE clock_t
#define GET_TIME( time ) time = clock()
#define GET_TIME_RESOLUTION( res ) res = CLOCKS_PER_SEC
#define TIME_DIFF_IN_MS( begin, end ) (int)(( end - begin ) * 1000 / CLOCKS_PER_SEC )


static char *data = NULL;
static int   data_len = 0;

void load_data( const char* file_name, const char* engine ){
  FILE* f;
  f = fopen(file_name, "rb");
  if (!f) {
    fprintf( stderr, "%s: Cannot open '%s'!\n", engine, file_name);
    exit( 1 );
  }

  fseek(f, 0, SEEK_END);
  data_len = ftell(f);
  fseek(f, 0, SEEK_SET);

  data = (char*)malloc(data_len + 1);
  if (!data) {
    fprintf( stderr, "%s: Cannot allocate memory!\n", engine );
    fclose(f);
    exit( 1 );
  }

  data[data_len] = '\0';
  fread(data, data_len, 1, f);
  fclose(f);

  printf("%s: '%s' loaded. (Length: %d bytes)\n", engine, file_name, data_len);
}

char duff[1024];

const char* save_rexp( const char* rexp ){
  int i;

  for( i = 0; *rexp; rexp++, i++ ){
    switch( *rexp ){
    case '\a': duff[ i++ ] = '\\'; duff[ i ] = 'a'; break;
    case '\b': duff[ i++ ] = '\\'; duff[ i ] = 'b'; break;
    case '\t': duff[ i++ ] = '\\'; duff[ i ] = 't'; break;
    case '\n': duff[ i++ ] = '\\'; duff[ i ] = 'n'; break;
    case '\v': duff[ i++ ] = '\\'; duff[ i ] = 'v'; break;
    case '\f': duff[ i++ ] = '\\'; duff[ i ] = 'f'; break;
    case '\r': duff[ i++ ] = '\\'; duff[ i ] = 'r'; break;
    case '\\': duff[ i ] = '\\'; break;
    default: duff[ i ] = *rexp; break;
    }
  }

  duff[ i ] = '\0';

  return (const char*)duff;
}
