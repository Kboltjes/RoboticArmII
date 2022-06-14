#include <stdarg.h>
#include <stdio.h>


#define PRINT_TO_SERIAL
//#define PRINT_TO_LCD

void Print(const char *fmt, ...)
{
  va_list args;
  va_start(args, fmt);
  while (*fmt != '\0') {
    if (*fmt == '%') { 
      if (*(fmt + 1) == 'd') {
        int i = va_arg(args, int);
        Printer(i);
        ++fmt;
      } else if (*(fmt + 1) == 'l' && *(fmt + 2) == 'd') {
        long i = va_arg(args, long);
        Printer(i);
        fmt += 2;
      } else if (*(fmt + 1) == 's') {
        char *s = va_arg(args, char*);
        Printer(s);
        ++fmt;
      } else if (*(fmt + 1) == 'f') {
        double f = va_arg(args, double);
        Printer(f);
        ++fmt;
      }
    }
    else if (*fmt == '\\') {
      if (*(fmt + 1) == 'n') {
        Printer("\n");
        ++fmt;
      }
    }
    else {
      Printer(*fmt);
    }
    ++fmt;
  }
  va_end(args);
}

template<typename Args>
void Printer(Args arg) {
#ifdef PRINT_TO_SERIAL
  Serial.print(arg);
#endif

#ifdef PRINT_TO_LCD
  //lcd.print(arg);
#endif
}
