# Lpattern

This code was extracted by the official Lua 5.3 source code. It contains the Lua
pattern matching algorithm to be used standalone in your project. The original
MIT license was kept.

A wrapper function is provided to match a pattern against a string. A simple
example application is provided also. It works on standard input/output. All
the new code is relesed under the unlicense.org.

All the code is provided without any warranty.

# Build

No build system is provided. You should be able to compile a library with:

```
gcc -std=c99 -shared lpattern.c -o liblpattern.so
```

or the example application with:


```
gcc -std=c99 lpattern.c main.c
```

# Usage

The wrapper function is called in this way:

```
#include "lpattern.h"

// ...

  lpattern_t match = lpattern_find (argv[2], strlen(argv[2]), line, linelen);
  char * err = lpattern_error (&match);
  if (err) return -1;

  if (match.found)
    for (int i=0; i<match.level; i++)
      printf("%.*s", match.extra.capture[i].length, match.extra.capture[i].data);

// ...
```

The example application can be used in this way:

```
$ echo -e "12\nfoo34 bar56\n7890\n1a2" | ./a.exe -c '(%d+)[^%d]+(%d+)'
34      56
1       2
```
