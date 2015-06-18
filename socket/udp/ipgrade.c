#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define IP_GRADE_A	0xFFFFFF00
#define IP_GRADE_B	0xFFFF0000
#define IP_GRADE_C	0xFF000000
#define IP_GRADE_O	4
#define IP_GRADE_N	5	//错误IP


int ipGrade(in_addr_t addr)
{
	if(INADDR_NONE == addr)
		return IP_GRADE_N;

	addr &= 0xFF;
	if(addr <= 127)
		return IP_GRADE_A;
	if(addr <= 191)
		return IP_GRADE_B;
	if(addr <= 223)
		return IP_GRADE_C;
	else
		return IP_GRADE_O;
}
in_addr_t broadcastAddr(in_addr_t addr)
{
	return addr | ipGrade(addr);
}
int main(int argc, char *argv[])
{
	const char *ip = "172.16.0.1";

	fprintf(stdout, "%08X\n", broadcastAddr(
					inet_addr(ip)));
	return EXIT_SUCCESS;
}

