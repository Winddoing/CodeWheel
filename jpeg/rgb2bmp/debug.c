#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "debug.h"


void HexaDump(unsigned char *pBuff, unsigned int len)
{
	unsigned int i;
	
	fprintf(stderr, "\n===========================================================\n");
	for( i = 0; i < len; i++ )
	{
		fprintf(stderr, "0x%02X ", pBuff[i]);
		if( (i != 0) && ((i % 16) == 0) )
		{
			fprintf(stderr, "\n");
		}
	}
	fprintf(stderr, "\n===========================================================\n");
}

void dbgMsg(const char *szFile, int line, const char *szFunc, const char *szFormat, ...)
{
	char szText[256];

	vsprintf(szText, szFormat, (char *)(&szFormat + 1));
	strtok(szText, "\n");
	fprintf(stderr, "%s %s @%s:%d\n", szText, szFunc, (strrchr(szFile,'/') ? strrchr(szFile,'/')+1 : szFile), line);
}
