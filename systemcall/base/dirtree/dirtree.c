#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>

#define MAX_PATH 512

extern int n;

void openDirList(char *dirName)
{
	DIR *pDir;
	struct dirent *pDirRent;

	if(NULL == (pDir = opendir(dirName))) 
		return;
	n += 4;
	while(NULL != (pDirRent = readdir(pDir)))
	{
		//fprintf(stdout, "d_type = %d\n", pDirRent->d_type);
		if(pDirRent->d_type & DT_DIR)
		{
			if(strcmp(pDirRent->d_name, ".") == 0 ||
					strcmp(pDirRent->d_name, "..") == 0)
				continue;

				fprintf(stdout, "%*c%s\n", n, ' ', 
						pDirRent->d_name);
				char *p = malloc(MAX_PATH);  
				bzero(p, sizeof(MAX_PATH));
			   	strcat(p, dirName);
			   	strcat(p, "/");  
			   	strcat(p, pDirRent->d_name);
				openDirList(p);
				free(p);
		}
	}
	n -= 4;
	closedir(pDir);
}

