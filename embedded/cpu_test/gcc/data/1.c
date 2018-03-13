 #ifdef XXX_HARD
   int argc = 5;
   char *argv[] = {"sgcc.exe", "-dcfgj", "-g", "-O4", "../1.c"};
 #else // NORMAL or EASY
   int argc = 3;
   char *argv[]={"sgcc.exe", "-O4", "../1.c"};
 #endif

