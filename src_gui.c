#define _CRT_SECURE_NO_DEPRECATE

#ifdef _WIN32
#include <windows.h>
#endif
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "src_lib_static.h"
#include "src_lib_shared.h"

extern int my_vsnprintf(char *buffer, size_t bufferlen, const char *format, va_list ap)
{
    char *ptr_write = buffer;
    for (;;) {
        char c = *format++;
        if (c == '\0') {
            *ptr_write = '\0';
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
            case 'd':
                {
                    char digits[11];
                    int digits_pos;
                    int v = va_arg(ap, int);
                    if ((unsigned)v == 0x80000000) {
                        digits_pos = 0;
                        memcpy(digits, "-2147483648", 11);
                    } else {
                        int negative = 0;
                        if (v < 0) {
                            negative = 1;
                            v = -v;
                        }
                        digits[sizeof(digits)-1] = '0';
                        digits_pos = 10;
                        while (v) {
                            digits[digits_pos] = '0' + (v % 10);
                            v /= 10;
                            if (v) {
                                digits_pos--;
                            }
                        }
                        if (negative) {
                            digits_pos--;
                            digits[digits_pos] = '-';
                        }
                    }
                    while (digits_pos < 11) {
                        *ptr_write++ = digits[digits_pos++];
                    }
                }
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

static void show_something(const char *format, ...)
{
  char buffer[512];
  va_list ap;
  va_start(ap, format);
  my_vsnprintf(buffer, sizeof(buffer), format, ap);
  va_end(ap);
  buffer[sizeof(buffer) - 1] = '\0';
#ifdef _WIN32
  MessageBoxA(NULL, buffer, "title", MB_OK);
#else
  printf("%s\n", buffer);
#endif
}

static void do_compiler_detection()
{
#ifdef _MSC_VER
  show_something("Microsoft Compiler %d", _MSC_VER);
#endif
#ifdef __GNUC__
  show_something("GNU compiler: %d", __GNUC__);
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
  show_something("Shared/static library test: %d, %d",
    function_from_shared_library(1),
    function_from_static_library(1));
  for (i = 0; i < argc; i++) {
    show_something("argv[%d] = \"%s\"", i, argv[i]);
  }
  return 0;
}
#endif
