#include "common-c.h"

#include <string.h>

const char* engine = "pcre";
const char* type   = "std";

#define PCRE2_CODE_UNIT_WIDTH 8

#include "pcre2/config.h"
#include "pcre2/pcre2.h"

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

    pcre2_code *re;
    pcre2_compile_context *comp_ctx;
    pcre2_match_data *match_data;
    pcre2_match_context *match_ctx;
    int err_code;
    PCRE2_SIZE err_offset;
    pcre2_jit_stack *stack = NULL;
    PCRE2_SIZE *ovector;
    char *ptr;
    int len;
    TIME_TYPE start, end;
    int found, time, best_time = 0;

    comp_ctx = pcre2_compile_context_create(NULL);
    if (!comp_ctx) {
      printf("PCRE2 cannot allocate compile context\n");
      fprintf( file_output, "---\n" );
      continue;
    }

    pcre2_set_newline(comp_ctx, PCRE2_NEWLINE_ANYCRLF);

    re = pcre2_compile(
                       (PCRE2_SPTR8) argv[ i ],	/* the pattern */
                       PCRE2_ZERO_TERMINATED,	/* length */
                       PCRE2_MULTILINE,	        /* options */
                       &err_code,		        /* for error code */
                       &err_offset,		/* for error offset */
                       comp_ctx);		        /* use default character tables */

    if (!re) {
      printf("PCRE2 compilation failed at offset %d: [%d]\n", (int)err_offset, err_code);
      fprintf( file_output, "---\n" );
      continue;
    }

    pcre2_compile_context_free(comp_ctx);

    match_ctx = pcre2_match_context_create(NULL);
    if (!match_ctx) {
      printf("PCRE JIT cannot allocate match context\n");
      fprintf( file_output, "---\n" );
      continue;
    }


    match_data = pcre2_match_data_create(32, NULL);

    if (!match_data) {
      printf("PCRE2 cannot allocate match data\n");
      fprintf( file_output, "---\n" );
      continue;
    }

    ovector = pcre2_get_ovector_pointer(match_data);

    do {
      found = 0;
      ptr = data;
      len = data_len;

      GET_TIME( start );
      while (1) {
        err_code = pcre2_match(
                               re,			/* the compiled pattern */
                               (PCRE2_SPTR8) ptr,	/* the subject string */
                               len,			/* the length of the subject */
                               0,			/* start at offset 0 in the subject */
                               0,			/* default options */
                               match_data,		/* match data */
                               match_ctx);		/* match context */

        if (err_code <= 0) {
          if (err_code == PCRE2_ERROR_NOMATCH)
            break;
          printf("PCRE pcre_exec failed with: %d\n", err_code);
          break;
        }

        // printf("match: %d %d\n", (ptr - subject) + match[0], (ptr - subject) + match[1]);
        ptr += ovector[1];
        len -= ovector[1];
        found++;
      }

      GET_TIME( end );


      time = TIME_DIFF_IN_MS( start, end );
      if (!best_time || time < best_time)
        best_time = time;
    } while (--repeat > 0);

    if (stack)
      pcre2_jit_stack_free(stack);
    pcre2_match_context_free(match_ctx);
    pcre2_code_free(re);

    const char* srexp = save_rexp( argv[i] );

    printf( "loops %d, best time %dms, matches %d, rexp \"%s\"\n", n, best_time, found, srexp );
    fprintf( file_output, "%d %d \"%s\"\n", best_time, found, srexp );
  }

  fclose(file_output);
  if( data ) free( data );
  return 0;
}
