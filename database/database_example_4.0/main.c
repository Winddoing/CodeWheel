#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mymysql.h>
#include <wavedata.h>

typedef int (*Pf)(const char *);
static struct{
	const char *prompt;
	const char *context;
	Pf fp;
}ctlStruct[] = {
	{	"1. Insert to database.",
		"input file name: ",
		insertWaveFile,
	},{
		"2. Get database list.",
		"input key word: ",
		selectWaveFile,
	},{
		"3. Delete from database.",
		"input recoder id: ",
		deleteWaveFile,
	},{
		"4. Play wave file.",
		"input recoder id:",
		playWaveFile,
	}
};
#define PROJ_NUM	(sizeof(ctlStruct) / sizeof(ctlStruct[0]))
void getString(char *buffer, int length);

static int getId(void)
{
	char buffer[256];
	char *p;
	int id;

	getString(buffer, sizeof(buffer));
	
	id = strtol(buffer, &p, 10);
	if(*p != '\0') return -1;

	return id;
}
void getString(char *buffer, int length)
{
	char *p;

	bzero(buffer, length);
	fgets(buffer, length - 1, stdin);
	p = strrchr(buffer, '\n');
	if(NULL != p) *p = '\0';
}

int main(int argc, char *argv[])
{
	int i, id;
	char buffer[256];

	if(0 != connectToMysql())
		return -1;

	while(1){
		for(i = 0; i < PROJ_NUM; i++)
		{
			fprintf(stdout, "%s\n", ctlStruct[i].prompt);
		}
		fprintf(stdout, "0. exit\nEnter your choose:");
		id = getId();
		if(id < 0 || id > PROJ_NUM)
		{
			fprintf(stderr, "Invalid choose.\n");
			continue;
		}else if(0 == id)
		{
			fprintf(stdout, "bye-bye!\n");
			break;
		}
		id--;
		fprintf(stderr, ctlStruct[id].context);
		getString(buffer, sizeof(buffer));
		ctlStruct[id].fp(buffer);
	}

	return EXIT_SUCCESS;
}


