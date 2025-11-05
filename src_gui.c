#define _CRT_SECURE_NO_DEPRECATE

#ifdef _WIN32
#include <windows.h>
#endif
#include <stdio.h>
#include <stdarg.h>

#include "src_lib_static.h"
#include "src_lib_shared.h"

#if defined(_MSC_VER) && _MSC_VER < 1400
#define vsnprintf my_vsnprintf
int my_vsnprintf(char *buffer, size_t bufferlen, const char *format, va_list ap)
{
    char *ptr_write = buffer;
    for (;;) {
        char c = *format++;
        if (c == '\0') {
            break;
        }
        if (c == '%') {
            c = *format++;
            switch (c) {
            case '\0':
                *ptr_write++ = '\0';
                format--;
                break;
            case '%':
                *ptr_write++ = '%';
                break;
            case 's':
                {
                    const char *arg = va_arg(ap, const char *);
                    while (*arg) {
                        *ptr_write++ = *arg++;
                    }
                }
                break;
            default:
                abort();
            }
        } else {
            *ptr_write++ = c;
        }
    }
    return ptr_write - buffer;
}
#endif

static void show_something(const char *format, ...)
{
#ifdef _WIN32
  char buffer[512];
  va_list ap;
  va_start(ap, format);
  vsnprintf(buffer, sizeof(buffer), format, ap);
  va_end(ap);
  buffer[sizeof(buffer) - 1] = '\0';
  MessageBoxA(NULL, buffer, "title", MB_OK);
#else
  va_list ap;
  va_start(ap, format);
  vprintf(format, ap);
  va_end(ap);
#endif
}

static void do_compiler_detection()
{
#ifdef __MSC_VER
  show_something("Microsoft Compiler %s\n", _MSC_VER);
#endif
#ifdef __GNUC__
  show_something("GNU compiler: %d\n", __GNUC__);
#endif
}

#ifdef _WIN32
int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char *lpCommandLine, int nShowCmd)
{
  do_compiler_detection();
  show_something("%s", lpCommandLine);
  return 0;
 
}
#else
int main(int argc, char *argv[])
{
  int i;
  do_compiler_detection();
  show_something("Shared/static library test: %d, %d\n",
    function_from_shared_library(1),
    function_from_static_library(1));
  for (i = 0; i < argc; i++) {
    show_something("argv[%d] = \"%s\"\n", i, argv[i]);
  }
  return 0;
}
#endif
