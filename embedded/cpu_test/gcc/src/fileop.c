#include <stdarg.h>
#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include "_ansi.h"
#include <machine/setjmp.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>

#include "test.h"

extern _CONST char _ctype_[];
struct _reent *_impure_ptr=0;
char **environ;
extern unsigned char _ctype[];

int fprintf(FILE *fp, const char * format, ...) 
{
  int count;
  char buffer[1024];
  va_list ap;

  va_start(ap, format);
  count = vsprintf(buffer, format, ap);
  show_string("\nfprintf: ");
  show_string(buffer);
  va_end(ap);
  return count;
}

int fputc(int c, FILE *fp) 
{
  return 1;
}

int fputs(const char *s, FILE *fp) 
{
  return 1;
}

int printf(const char * format, ...) 
{
  int count;
  char buffer[1024];
  va_list ap;

  va_start(ap, format);
  count = vsprintf(buffer, format, ap);
  show_string("\nfprintf: ");
  show_string(buffer);
  va_end(ap);
  return count;
}

long ftell(FILE *fp) 
{
  return 1;
}

FILE * fopen(const char *name, const char *type) 
{
  return (FILE *) 4;
}

int fclose(FILE *fp) 
{
  return 0;
}

int fferror(FILE *fp)
{
  return 0;
}

int fflush(FILE *fp) 
{
  return 0;
}

int unlink(const char *path) 
{
  return 0;
}

void bzero(char *s, size_t l) 
{
  memset(s, '\0', l);
}
int bcmp(const char *s1, const char *s2, size_t l) 
{
  return strncmp(s1, s2, l);
}

char * index(const char *s, int c) 
{
  return strchr(s, c);
}


int __swbuf(int a, FILE *fp) {}

int setjmp(jmp_buf __jmpb) {}

char * getenv(const char *__string) {}

void perror(const char *s) {}
// _PTR realloc(_PTR r, size_t s) {}
_sig_func_ptr signal(int n, _sig_func_ptr s) {}

void longjmp(jmp_buf jb, int retval) {}

int stat(const char *path, struct stat *buf) {}
void abort() { exit (10); }

int system(const char *s) {}
