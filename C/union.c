//da_xiao_duan xi tong

#include <stdio.h>
#include <stdlib.h>

union st{
	char a[2];
	char b;
};

int main(int argc, char *argv[])
{
	union st s;

	//s.b = 0x1122;
	s.b = 0x0001;

	//if(s.a[0]  < s.a[1])
	//	fprintf(stdout, "da_duan\n");
	//else
	//	fprintf(stdout, "xiao_duan\n");		
	fprintf(stdout, "%s_duan\n", 
			s.a[1] ? "da" : "xiao");		
	
	return EXIT_SUCCESS;
}

