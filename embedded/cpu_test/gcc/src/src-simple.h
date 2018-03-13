#define MAXLINE  8
static char in_buf[MAXLINE][256] = {
"main()",
"{",
"  int a=2, i;",
"  int b=3;",
"  i = a + b;",
"  return i;",
"}",
"",
};
int l_p=0, c_p=0;
static int cols_in_line[MAXLINE];
static int first = 0;
