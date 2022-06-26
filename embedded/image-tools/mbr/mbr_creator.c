/* Copyright (C) 2022 wqshao All rights reserved.
 *
 *  File Name    : mbr_creator.c
 *  Author       : wqshao
 *  Created Time : 2022-06-25 15:50:55
 *  Description  :
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>

#define LINUX_FS			0x83
#define FAT_FS				0x0b

#define MBR_SIZE			512
#define MBR_PARTITION_MAX	4
/* The signature.  */
#define MBR_SIGNATURE		0xaa55

/* The partition entry.  */
struct msdos_partition_entry
{
	/* If active, 0x80, otherwise, 0x00.  */
	uint8_t flag;

	/* The head of the start.  */
	uint8_t start_head;

	/* (S | ((C >> 2) & 0xC0)) where S is the sector of the start and C
	   is the cylinder of the start. Note that S is counted from one.  */
	uint8_t start_sector;

	/* (C & 0xFF) where C is the cylinder of the start.  */
	uint8_t start_cylinder;

	/* The partition type.  */
	uint8_t type;

	/* The end versions of start_head, start_sector and start_cylinder,
	   respectively.  */
	uint8_t end_head;
	uint8_t end_sector;
	uint8_t end_cylinder;

	/* The start sector. Note that this is counted from zero.  */
	uint32_t start;

	/* The length in sector units.  */
	uint32_t length;
} __attribute__ ((packed));

/* The structure of MBR.  */
struct msdos_partition_mbr
{
	/* The code area (actually, including BPB).  */
	uint8_t code[446];

	/* Four partition entries.  */
	struct msdos_partition_entry entries[4];

	/* The signature 0xaa55.  */
	uint16_t signature;
} __attribute__ ((packed));

/* from: include/grub/msdos_partition.h */

typedef struct _item {
	char * name;
	char * value;
} item_t;

typedef struct _group {
	char * name;
	item_t items[MBR_PARTITION_MAX];
} group_t;

static char file_buffer[2048] = { 0 };
static group_t group_list[10] = { 0 };

static char * str_trim(char * str) {
	char * p;
	p = str + strlen(str) - 1;
	while (*p && (*p <= ' ')) {
		p--;
	}
	*(++p) = 0;

	p = str;
	while (*p && (*p <= ' ')) {
		p++;
	}
	return p;
}

static int parse_config_file(char * buffer)
{
	char * line = buffer;
	int is_last_line = 0;
	int group_index = -1;
	int item_index = -1;
	char * p;

	while (line && *line && !is_last_line)
	{
		// get next line
		char * next_line = strchr(line, '\n');
		if (next_line) {
			*next_line = 0;
		} else {
			is_last_line = 1;
		}

		if ((p = strchr(line, '#'))) {
			*p = 0;
		}

		if ((p = strchr(line, ':'))) { //find group name
			char * group_name;
			*p = 0;
			group_name = str_trim(line);
			if (group_name[0] != 0) {
				group_index ++;
				item_index = -1;
				if (group_index >= (sizeof(group_list)/sizeof(group_list[0]))) {
					return -1;
				}
				group_list[group_index].name = group_name;
			}
		} else if ((p = strchr(line, '='))) { //find property name
			char * property_name;
			*(p++) = 0;
			property_name = str_trim(line);
			if ((property_name[0] != 0) && (group_index >= 0)) {
				item_index ++;
				if (item_index >= (sizeof(group_list[0].items)/sizeof(group_list[0].items[0]))) {
					return -1;
				}
				group_list[group_index].items[item_index].name = property_name;
				group_list[group_index].items[item_index].value = str_trim(p);
			}
		}

		if (next_line == NULL) break;

		line = next_line + 1;
	}

	return 0;
}

static group_t * get_group(char * group_name)
{
	int i;
	for(i=0; i<sizeof(group_list)/sizeof(group_list[0]); i++) {
		group_t * group = &group_list[i];

		if (group->name == NULL) continue;

		if (strcmp(group->name, group_name) == 0) {
			return group;
		}
	}
	return NULL;
}

static item_t * get_item(char * group_name, char * item_name)
{
	int i;
	group_t * group = get_group(group_name);

	if (group == NULL) return NULL;

	for(i=0; i<sizeof(group_list[0].items)/sizeof(group_list[0].items[0]); i++) {
		item_t * item = &(group->items[i]);

		if (item->name == NULL) continue;

		if (strcmp(item->name, item_name) == 0) {
			return item;
		}
	}

	return NULL;
}

static char * get_item_value(char * group_name, char * item_name)
{
	item_t * item = get_item(group_name, item_name);

	return (item == NULL) ? NULL : item->value;
}

static uint64_t parse_size(char *sz)
{
	uint64_t n;

	int l = strlen(sz);

	n = strtoull(sz, 0, 10);

	if (l) {
		switch(sz[l-1]){
		case 'k':
		case 'K':
			n *= 1024;
			break;
		case 'm':
		case 'M':
			n *= (1024 * 1024);
			break;
		case 'g':
		case 'G':
			n *= (1024 * 1024 * 1024);
			break;
		}
	}
	return n;
}

static uint64_t transfer_fstype(char * type_str)
{
	if (strcmp(type_str, "FAT_FS") == 0) {
		return FAT_FS;
	}else if (strcmp(type_str, "LINUX_FS") == 0) {
		return LINUX_FS;
	} else {
		return 0;
	}
}

static int parse_partition_item(char * value, uint64_t * start, uint64_t* size, uint64_t* type)
{
	char temp[128];
	char * str_start;
	char * str_size;
	char * str_type;
	char * sub_str;

	//strncpy(temp, value, sizeof(temp));
	strcpy(temp, value);
	sub_str = strchr(temp, ',');
	if (sub_str == NULL) return -1;
	*sub_str++ = 0;

	//To gain start field
	str_start = str_trim(temp);
	if (*str_start == 0) return -1;
	if (!isdigit(*str_start)) return -1;

	//To gain size field
	str_size = str_trim(sub_str);
	if (*str_size == 0) return -1;
	if (!isdigit(*str_size)) return -1;

	//To gain fstype field
	sub_str = strchr(sub_str, ',');
	if (sub_str == NULL) return -1;
	*sub_str++ = 0;
	str_type = str_trim(sub_str);
	if (*str_type == 0) return -1;

	*start = parse_size(str_start);
	*size = parse_size(str_size);
	*type = transfer_fstype(str_type);
	return 0;
}

static void add_ptn(struct msdos_partition_entry *entry, uint64_t type,
		uint64_t start, uint64_t size, const char *name)
{
	entry->type   = type;
	entry->start  = start;
	entry->length = size;
}

static void show(struct msdos_partition_mbr *mbr)
{
	int i;

	fprintf(stderr,"\nptn  start block     end block     size    fstype id\n");
	fprintf(stderr,"---- ------------- -------------  -------- ----------\n");

	for (i = 0; i < MBR_PARTITION_MAX; i++) {
		if (mbr->entries[i].start == 0)
			break;

		fprintf(stderr,"#%03d %13d %13d %6dMB    0x%x\n", i + 1,
				mbr->entries[i].start, mbr->entries[i].start + mbr->entries[i].length - 1,
				mbr->entries[i].length*512/1024/1024, mbr->entries[i].type);
	}
}

static int usage(void)
{
	fprintf(stderr,
			"\n"
			"usage: mbr_creator <partitions_config_file> <out_mbr_file> \n"
			"\n"
			"<partitions_config_file> sample: \n"
			"    property: \n"
			"        disk_size = 1024m \n"
			"    partition: \n"
			"        # name  start,  size,  fstype   \n"
			"        boot       3m,    9m,  EMPTY    \n"
			"        rootfs    12m,    9m,  LINUX_FS \n"
			"        data      21m,    5m,  LINUX_FS \n"
			"        msic      28m,   31m,  LINUX_FS \n"
		   );
	return 0;
}


int main(int argc, char *argv[])
{
	struct msdos_partition_mbr mbr;
	uint64_t disk_size, disk_blocks;
	char *config_file_name, *mbr_file_name;
	char *str_disk_size;
	group_t *partitions;
	FILE* fd = NULL;
	int ret = 0;

	if (argc != 3)
		return usage();

	config_file_name = argv[1];
	mbr_file_name = argv[2];

	fd = fopen(config_file_name, "r");
	if(fd == NULL) {
		fprintf(stderr, "cannot open config_file_name %s\n", config_file_name);
		return -1;
	}
	ret = fread(file_buffer, 1, sizeof(file_buffer), fd);
	if (ret <= 0 ) {
		fprintf(stderr, "cannot read partitions from '%s'\n", config_file_name);
		return -1;
	}
	parse_config_file(file_buffer);
	fclose(fd);
	fd = NULL;

	str_disk_size = get_item_value("property", "disk_size");
	if ((str_disk_size == NULL) || (str_disk_size[0] == 0)) {
		fprintf(stderr,"cannot get disk_size property\n");
		return -1;
	}
	disk_size = parse_size(str_disk_size);
	disk_blocks = disk_size/512;
	fprintf(stderr,"disk: size = %ld, blocks = %ld\n", disk_size, disk_blocks);

	memset(&mbr, 0, sizeof(mbr));
	mbr.signature = MBR_SIGNATURE;

	partitions = get_group("partition");
	if (partitions != NULL) {
		uint64_t total_sz = 0;
		int item_count = sizeof(group_list[0].items)/sizeof(group_list[0].items[0]);
		int i;
		for(i = 0; i < item_count; i++) {
			uint64_t start = 0, size = 0, type = 0;

			item_t * partition = &(partitions->items[i]);
			if ((partition->name == NULL) || (partition->value == NULL)) break;

			parse_partition_item(partition->value, &start, &size, &type);
			start /= 512;
			size  /= 512;

			add_ptn(&mbr.entries[i], type, start, size, partition->name);

			total_sz += size;
		}

		if (total_sz > disk_size) {
			fprintf(stderr, "Total partition size exceeds disk size\n");
			return -1;
		}
	}

	show(&mbr);

	fd = fopen(mbr_file_name, "wb");
	if(fd == 0) {
		fprintf(stderr, "open %s failed.\n", mbr_file_name);
		return -1;
	}

	fwrite(&mbr, 1, MBR_SIZE, fd);
	fclose(fd);

	return 0;
}
