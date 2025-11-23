#include <printf/printf.h>
#include "internal.h"

/*
 * We can't use putchar_ as is, since our output gadget
 * only takes pointers to functions with an extra argument
 */
static inline void putchar_wrapper(char c, void* unused)
{
  (void) unused;
  putchar_(c);
}

static inline output_gadget_t extern_putchar_gadget(void)
{
  return function_gadget(putchar_wrapper, NULL);
}


int vprintf_(const char* format, va_list arg)
{
  output_gadget_t gadget = extern_putchar_gadget();
  return eyalroz_vsnprintf_impl(&gadget, format, arg);
}

int vsprintf_(char* s, const char* format, va_list arg)
{
  return vsnprintf_(s, PRINTF_MAX_POSSIBLE_BUFFER_SIZE, format, arg);
}

int vsnprintf_(char* s, size_t n, const char* format, va_list arg)
{
  output_gadget_t gadget = buffer_gadget(s, n);
  return eyalroz_vsnprintf_impl(&gadget, format, arg);
}

int printf_(const char *format, ...) {
  int ret;
  va_list args;
  va_start(args, format);
  ret = vprintf_(format, args);
  va_end(args);
  return ret;
}

int sprintf_(char *s, const char *format, ...) {
  int ret;
  va_list args;
  va_start(args, format);
  ret = vsprintf_(s, format, args);
  va_end(args);
  return ret;
}

int snprintf_(char *s, size_t n, const char *format, ...) {
  int ret;
  va_list args;
  va_start(args, format);
  ret = vsnprintf_(s, n, format, args);
  va_end(args);
  return ret;
}

int fctprintf(void (*out)(char c, void *extra_arg), void *extra_arg,
              const char *format, ...) {
  int ret;
  va_list args;
  va_start(args, format);
  ret = vfctprintf(out, extra_arg, format, args);
  va_end(args);
  return ret;
}
