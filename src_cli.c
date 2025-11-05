#include <stdio.h>

#include "src_lib_static.h"
#include "src_lib_shared.h"

int main(int argc, char *argv[])
{
  int i;

  printf("shared/static library test: %d %d\n",
    function_from_static_library(1),
    function_from_shared_library(1));

#ifdef __MSC_VER
  printf("Microsoft Compiler %s\n", _MSC_VER);
#endif
#ifdef __GNUC__
  printf("GNU compiler: %d\n", __GNUC__);
#endif
  for (i = 0; i < argc; i++) {
    printf("argv[%d] = \"%s\"\n", i, argv[i]);
  }
  return 0;
}
