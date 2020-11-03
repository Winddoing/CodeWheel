#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXNAME 20
#define MAXLENGTH 1024

static char *readfile(char *name)
{
	FILE *file = fopen(name, "r");
	char * buf = (char *)malloc(sizeof(char) * MAXLENGTH);

	if(file == NULL) {
		printf("File Cann't Be Open!");
		return 0;
	}

	while(fgets(buf, MAXLENGTH - 1, file) != NULL) {
		return buf;
	}

	return NULL;
}

struct file_data {
    char f_name[MAXNAME];
    char *str;
};

static int check_file(char *file_name, char *str, struct file_data *f_data)
{
    strcpy(f_data->f_name, file_name);

    f_data->str = str;

    return 0;
}


int main(int argc, const char *argv[])
{
	char name[MAXNAME];
    char *str = NULL;

	printf("Enter file name:");
	scanf("%s", name);

    str = readfile(name);

	printf("The file first row is: %s\n", str);


    struct file_data *_file_data = NULL;

    _file_data = (struct file_data*)malloc(sizeof(struct file_data));
    if (!_file_data)
        printf("malloc file_data error\n");

    memset(_file_data->f_name, 0 , sizeof(char)*MAXNAME);
    _file_data->str = (char*)malloc(sizeof(char)*MAXLENGTH);


    check_file(name, str, _file_data);


    printf("file_data f_name:%s, str:%s\n", _file_data->f_name, _file_data->str);


    return 0;
}

