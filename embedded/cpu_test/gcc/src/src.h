#define MAXLINE  22
static char in_buf[MAXLINE][256] = {
"double i;",
"main()",
"{",
"  int a=2, i;",
"  int b=3;",
"  char c;",
"  for(i=0; i<10; i++)",
"    {",
"      b=a+i;",
"    }",
"  printf(\"abcd!!!sss=%d\n\", a+b); ",
"  printf(\"Hello world\n\");",
"  c = getchar();",
"  switch (c) {",
"  case 0:",
"  case 1: printf(\"less than one\n\"); break;",
"  case 2: i = c < b ? 'a' : 'z'; break;",
"  default: i = a + b; printf(\"%d\n\", i); break;",
"  }",
"  return i;",
"}",
"",
};
int l_p=0, c_p=0;
static int cols_in_line[MAXLINE];
static int first = 0;
