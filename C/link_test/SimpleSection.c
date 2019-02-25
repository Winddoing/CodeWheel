/*#############################################################
 *     File Name	: SimpleSection.c
 *     Author		: winddoing
 *     Created Time	: 2018年12月18日 星期二 15时17分55秒
 *     Description	:
 *          gcc -c SimpleSection.c -o SimpleSection.o
 *          gcc version 7.3.0 (Ubuntu 7.3.0-27ubuntu1~18.04) 
 *############################################################*/

int printf(const char* format, ...);

int global_init_var = 84;
int global_uninit_var;

void func1(int i)
{
    printf("%d\n", i);
}

int main(int argc, const char *argv[])
{
    static int static_var = 85;
    static int static_var2;

    int a = 1;
    int b;

    func1(static_var + static_var2 + a + b);
    
    return a;
}
