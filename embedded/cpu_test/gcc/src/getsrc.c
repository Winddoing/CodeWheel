#include <stdio.h>
#include <ctype.h>

#ifdef SIMPLE
#include "src-simple.h"
#elif HARD
#include "src-hard.h"
#else // NORMAL
#include "src-normal.h"
#endif //SIMPLE/NORMAL/HARD

void report_error()
{
}

char getcc(inputfile)
     FILE *inputfile;
{
  if (MAXLINE==0) return EOF;
  if (first == 0)
    {
      int i, j;
      for (i = 0; i < MAXLINE; i++)
        {
          j = 0;
          while (in_buf[i][j++] != '\0');
          cols_in_line[i] = j - 1;
        }
      first = 1;  
    }
    
  if (in_buf[l_p][c_p] != '\0')
    return in_buf[l_p][c_p++];
  else
    {
      while (++l_p < MAXLINE && cols_in_line[l_p] == 0);
      if (l_p >= MAXLINE)
        return EOF;
      else
        {
          c_p = 1;
          return in_buf[l_p][0];
        }
    }
}

void ungetcc(c, inputfile)
     FILE *inputfile;
{
  if (c_p != 0)
    c_p--;
  else
    {
      while(--l_p >= 0 && cols_in_line[l_p] == 0);
      if (l_p == 0 && cols_in_line[l_p] == 0)
        report_error();
      else
        c_p = cols_in_line[l_p] - 1;
    }
}
