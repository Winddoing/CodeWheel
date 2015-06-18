#include <stdlib.h>
#include <limits.h>
#include <stdio.h>

int
main (int argc, char *argv[])
{
  int base;
  char *endptr, *str;
  long val;
 char  buf[50];

  if (argc < 2)
    {
      fprintf (stderr, "Usage: %s str [base]\n", argv[0]);
      exit (EXIT_FAILURE);
    }
  str = argv[1];
  base = (argc > 2) ? atoi (argv[2]) : 10;
  val = strtol (str, &endptr, base);

  itoa(str, buf, 2);

  printf ("%s to %ld\n", str, val);
    printf ("_________ %s\n", buf);

  if (*endptr != '\0')	/* Not necessarily an error... */
    printf ("Further characters after number: %s\n", endptr);

  exit (EXIT_SUCCESS);
}
