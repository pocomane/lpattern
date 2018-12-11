
/******************************************************************************
* This is free and unencumbered software released into the public domain.
* 
* Anyone is free to copy, modify, publish, use, compile, sell, or
* distribute this software, either in source code form or as a compiled
* binary, for any purpose, commercial or non-commercial, and by any
* means.
* 
* In jurisdictions that recognize copyright laws, the author or authors
* of this software dedicate any and all copyright interest in the
* software to the public domain. We make this dedication for the benefit
* of the public at large and to the detriment of our heirs and
* successors. We intend this dedication to be an overt act of
* relinquishment in perpetuity of all present and future rights to this
* software under copyright law.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
* OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
* ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
* OTHER DEALINGS IN THE SOFTWARE.
* 
* For more information, please refer to <http://unlicense.org/>
******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <malloc.h>

#include "lpattern.h"

void print_help(int argc, char** argv){
  printf("Usage:\n  %s [-c|-f|-o] PATTERN\n");
  printf("The PATTERN will be matched against each line in the standard input.\n");
  printf("The output is printed on the standard output.\n");
  printf("  -c will print the captures.\n");
  printf("  -f will print only the line matching.\n");
  printf("  -o will print only the line not matching.\n");
}

int main(int argc, char** argv){

  if (argc < 3) {
    print_help(argc,argv);
    return 0;
  }

  char mode;
  if (!strcmp(argv[1], "-c")) {
    mode = 'c';
  } else if (!strcmp(argv[1], "-f")) {
    mode = 'f';
  } else if (!strcmp(argv[1], "-o")) {
    mode = 'o';
  } else {
    print_help(argc,argv);
    return -1;
  }

  const char * test = "_pro_va";
  int testlen = strlen(test);
  const char * pat = "(%a+)";
  int patlen = strlen(pat);

  while (1) {
    char *line = NULL;
    size_t aux = 0;
    int linelen = getline(&line, &aux, stdin);
    if (-1 == linelen) {
      free(line);
      break;
    }

    lpattern_t ms = lpattern_find (argv[2], strlen(argv[2]), line, linelen);

    switch (mode){
      default:;

      break;case 'c':
        if (ms.found) {
          for (int i=0;i<ms.level;i++){
            if (i!=0) printf("\t");
            printf("%.*s", ms.capture[i].length, ms.capture[i].data);
          }
          printf("\n");
        }

      break;case 'f':
        if (ms.found) {
          printf("%s", line);
        }

      break;case 'o':
        if (!ms.found) {
          printf("%s", line);
        }
    }
    
    free(line);
  }

  return 0;
}
