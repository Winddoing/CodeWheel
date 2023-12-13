#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define PART_NAME_LEN 32
#define PART_TYPE_LEN 32
#define UUID_STR_LEN  36

typedef unsigned long ulong;
typedef unsigned char uchar;

typedef struct disk_partition {
	ulong	start;	/* # of first block in partition	*/
	ulong	size;	/* number of blocks in partition	*/
	ulong	blksz;		/* block size in bytes			*/
	uchar	name[PART_NAME_LEN];	/* partition name			*/
	uchar	type[PART_TYPE_LEN];	/* string type description		*/
	int	bootable;	/* Active/Bootable flag is set		*/
	char	uuid[UUID_STR_LEN + 1];	/* filesystem UUID as string, if exists	*/
	uchar	sys_ind;	/* partition type 			*/
} disk_partition_t;




void usage(char *name){
	printf("%s file\n", name);
	exit(0);
}

void parseOne(disk_partition_t *part, char *p){
    char *start, *size, *name;
    // trim size leading spaces
    size = p;
    while (isspace(*size))
        ++size;
    
    start = strchr(size, '@');
    p = start++;
    p--;
    // trim size ending spaces
    while (isspace(*p))
        p--;
    p[1] = 0;
    
    // trim start leading spaces
    while (isspace(*start))
        ++start;    
    name = strchr(start, '(');
    p = name++;
    p--;
    // trim len ending spaces
    while (isspace(*p))
        p--;
    p[1] = 0;
    
    // trim name leading spaces
    while (isspace(*name))
        ++name;
    
    p = strchr(name, ')');
    p--;
    // trim name ending spaces
    while (isspace(*p))
        p--;
    p[1] = 0;
    p = strchr(name, ':');
    if (p){
        *p = 0;
    }
    
    strncpy(part->name, name, sizeof(part->name));
    if ((strcmp(size, "-") == 0)) {
        /* Let part efi module to auto extend the size */
        part->size = 0;
    } else {
        part->size = strtoul(size, NULL, 0);
    }
    part->start = strtoul(start, NULL, 0);
    
    printf("part name:%s start:%lx size:%lx\n", part->name, part->start, part->size);
    
    return;
}

int parsePartitions(disk_partition_t *part, char *mtdparts){
    int index = 0;
    char *p, *next;
    
    p = strchr(mtdparts, ':');
    p++;    //skip ':'
    
    while(next = strchr(p, ',')){
        *next = 0;
        next++;
        parseOne(part + index, p);
        p = next;
        index++;
    }
    next = strchr(p, '\n');
    *next = 0;
    parseOne(part + index, p);
    
    return index + 1;
}

void parseUUID(disk_partition_t *part, char *uuid, int count){
    char *name;
    char *p;
    
    name = strchr(uuid, ':');
    name++; //skip ':';
    
    //Trim name leading spaces
    while (isspace(*name))
        ++name;
    uuid = strchr(name, '=');
    p = uuid++;
    p--;
    //Trim name ending spaces
    while (isspace(*p))
        p--;
    p[1] = 0;
    
    //Trim uuid leading spaces
    while (isspace(*uuid))
        ++uuid;
    p = strchr(uuid, '\n');
    p--;
    //Trim uuid ending spaces
    while (isspace(*p))
        p--;
    p[1] = 0;
    
    printf("set part:%s uuid to:%s\n", name ,uuid);
    
    
}

int main(int argc, char *argv[]){
	FILE *fp;
    int len;
    char *buf;    
    char *mtdparts, *uuid;
    disk_partition_t parts[8];
    int count;
    
	if (argc != 2)
		usage(argv[0]);

    memset(parts, 0, sizeof(parts));
    
	fp = fopen(argv[1], "rb");

	if (fp == NULL){
		printf("can't open parameter file %s\n", argv[1]);
	}
    fseek(fp, 0, SEEK_END);
    len = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    
    printf("file len is %d bytes\n", len);
    
    buf = (char *)calloc(1, len + 1);
    
    fread(buf, len, 1, fp);
    
    mtdparts = strstr(buf, "mtdparts");
    uuid = strstr(buf, "uuid");
    
    count = parsePartitions(parts, mtdparts);
    parseUUID(parts, uuid, count);
    printf("Total %d partitions\n", count);
    return 0;
}
