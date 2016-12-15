#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "main.h"

static char *data = NULL;
static int   data_len = 0;

void load( char* file_name ){
  int i;

  FILE* f;
  f = fopen(file_name, "rb");
  if (!f) {
    printf("Cannot open '%s'!\n", file_name);
    return;
  }

  fseek(f, 0, SEEK_END);
  data_len = ftell(f);
  fseek(f, 0, SEEK_SET);

  data = (char*)malloc(data_len + 1);
  if (!data) {
    printf("Cannot allocate memory!\n");
    fclose(f);
    return;
  }
  data[data_len] = '\0';
  fread(data, data_len, 1, f);
  fclose(f);

  for (i = 0; i < data_len; ++i)
    if (data[i] == '\r')
      data[i] = '\n';
  printf("'%s' loaded. (Length: %d bytes)\n", file_name, data_len);
}

#include "regexp3/regexp3.h"

char * toHTML( char *input, char *output ){
  char tmp[1024];
  regexp3( input, "<\n>", strlen( input ) );
  rplCatch( output, "\\n", 1 );

  regexp3( output, "<:<>", strlen( output ) );
  rplCatch( tmp, "&lt;", 1 );

  regexp3( tmp, "<:>>", strlen( tmp ) );
  rplCatch( output, "&gt;", 1 );

  return output;
}

void find_all( char *pattern, char *raptor3Pattern, char *raptor4Pattern, char *subject, int subject_len ){
  char str[3][1024];
  if( strcmp( raptor3Pattern, raptor4Pattern ) == 0 )
    printf("<tr><td class=\"pattern\"><table><tr><td>%s</td></tr><tr><td class=\"raptor3-pattern\">%s</td></tr></table></td>", toHTML( pattern, str[0] ), toHTML( raptor3Pattern, str[1] ));
  else
    printf("<tr><td class=\"pattern\"><table><tr><td>%s</td></tr><tr><td class=\"raptor3-pattern\">%s</td></tr><tr><td class=\"raptor4-pattern\">%s</td></tr></table></td>", toHTML( pattern, str[0] ), toHTML( raptor3Pattern, str[1] ), toHTML( raptor4Pattern, str[2] ));

#define LOOPS 5

  pcre2_find_all(pattern, subject, subject_len, LOOPS, 0);
  pcre2_find_all(pattern, subject, subject_len, LOOPS, 1);
  tre_find_all(pattern, subject, subject_len, LOOPS);
  onig_find_all(pattern, subject, subject_len, LOOPS);
  re2_find_all(pattern, subject, subject_len, LOOPS);
  pcre2_find_all(pattern, subject, subject_len, LOOPS, 2);
  regexp3_find_all(raptor3Pattern, subject, subject_len, LOOPS );
  regexp4_find_all(raptor4Pattern, subject, subject_len, LOOPS );

  printf("</tr>\n");
}

void printResult( char *name, int time, int found ){
  printf("<td class=\"time\">%d ms (%d)</td>", (int)time, found);
  fflush(stdout);
}

int main(){
  load("mark.txt");
  if( data_len == 0 ) return -1;

  // find_all("a(aa)+(y)",
  //          "a(aa)+(y)",
  //          "a(aa)+(y)", "xxaaaayyaaayssaayaaaay", 22); // Test function

  find_all(".|\n",
           ".",
           ".", data, data_len);
  find_all("\\w",
           ":w",
           ":w", data, data_len);
  find_all("\\d",
           ":d",
           ":d", data, data_len);
  find_all("\\S",
           ":S",
           ":S", data, data_len);
  find_all("\\S+",
           ":S+",
           ":S+", data, data_len);
  find_all("[a-zA-Z]+",
           "[a-zA-Z]+",
           "[a-zA-Z]+", data, data_len);
  find_all("[.\\s]+",
           "[:.:s]+",
           "[:.:s]+", data, data_len);
  find_all("([^\n]+)",
           "<[^\n]+>",
           "<[^\n]+>", data, data_len);
  find_all("e",
           "e",
           "e", data, data_len);
  find_all("(((((e)))))",
           "<<<<<e>>>>>",
           "<<<<<e>>>>>", data, data_len);
  find_all("((((((((((e))))))))))",
           "<<<<<<<<<<e>>>>>>>>>>",
           "<<<<<<<<<<e>>>>>>>>>>", data, data_len);
  find_all("Twain",
           "Twain",
           "Twain", data, data_len);
  find_all("(Twain)",
           "<Twain>",
           "<Twain>", data, data_len);
  find_all("(?i)Twain",
           "#*Twain",
           "#*Twain", data, data_len);
  find_all("((T|t)([wW])(a|A)i?I?([nN]))",
           "<<T|t><[wW]><a|A>i?I?<[nN]>>",
           "<<T|t><[wW]><a|A>i?I?<[nN]>>", data, data_len);
  find_all("(T+([w]?(a{1}(i+(n*))))){1}",
           "<T+<[w]?<a{1}<i+<n*>>>>>{1}",
           "<T+<[w]?<a{1}<i+<n*>>>>>{1}", data, data_len);
  find_all("(?:T+(?:[w]?(?:a{1}(?:i+(?:n*))))){1}",
           "(T+([w]?(a{1}(i+(n*))))){1}",
           "(T+([w]?(a{1}(i+(n*))))){1}", data, data_len);
  find_all("[a-z]shing",
           "[a-z]shing",
           "[a-z]shing", data, data_len);
  find_all("Huck[a-zA-Z]+|Saw[a-zA-Z]+",
           "Huck[a-zA-Z]+|Saw[a-zA-Z]+",
           "Huck[a-zA-Z]+|Saw[a-zA-Z]+", data, data_len);
  find_all("[a-q][^u-z]{13}x",
           "[a-q][^u-z]{13}x",
           "[a-q][^u-z]{13}x", data, data_len);
  find_all("Tom|Sawyer|Huckleberry|Finn",
           "Tom|Sawyer|Huckleberry|Finn",
           "Tom|Sawyer|Huckleberry|Finn", data, data_len);
  find_all("(Tom|Sawyer|Huckleberry|Finn)",
           "<Tom|Sawyer|Huckleberry|Finn>",
           "<Tom|Sawyer|Huckleberry|Finn>", data, data_len);
  find_all("[hHeELlOo][hHeELlOo][hHeELlOo][hHeELlOo][hHeELlOo]",
           "[hHeELlOo][hHeELlOo][hHeELlOo][hHeELlOo][hHeELlOo]",
           "[hHeELlOo][hHeELlOo][hHeELlOo][hHeELlOo][hHeELlOo]", data, data_len);
  find_all("Tom.{10,25}river|river.{10,25}Tom",
           "Tom([^(river|\n)]){10,25}river|river([^(Tom|\n)]){10,25}Tom",
           "Tom(river|\n){10,25}#!river|river(Tom|\n){10,25}#!Tom", data, data_len);
  find_all("ing[^a-zA-Z]",
           "ing[^a-zA-Z]",
           "ing[^a-zA-Z]", data, data_len);
  find_all("[a-zA-Z]ing[^a-zA-Z]",
           "[a-zA-Z]ing[^a-zA-Z]",
           "[a-zA-Z]ing[^a-zA-Z]", data, data_len);
  find_all("([a-zA-Z]+ing)",
           "<([^(ing|:A)])+ing(([^(ing|:A)])*ing)*>",
           "<(ing|:A)+#!ing(((ing|:A)*#!ing)*>", data, data_len);

  if( data ) free( data );
  return 0;
}
