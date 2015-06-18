#include <wave.h>
#include <mymysql.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_SQL_LENGTH	(4096 * 1024)

int insertWaveFile(const char *fileName)
{
	WaveInfo info;
	char buffer[MAX_SQL_LENGTH];
	int id;
	char **row;

	//取得WAVE音频文件信息
	if(-1 == getWaveFile(fileName, &info))
		return -2;
	//找到baseinfo表中的最大ID值
	strcpy(buffer, "SELECT MAX(id) + 1 FROM baseinfo");
	if(-1 == querySql(buffer)) return -1;
	row = getRow(0);
	if(NULL == row) return -1;	//如果20行的语句有错
	if(NULL == row[0])	//如果表中没有任何记录，row[0] == NULL
		id = 1;
	else
		id = atoi(row[0]);
	//生成插入baseinfo表的SQL语句
	sprintf(buffer, "INSERT INTO baseinfo(id, channels, bits, rate, "
					"name, length)VALUES(%d, %d, %d, %d, '%s', %u)",
					id, info.head->channels, info.head->bits,
					info.head->rate, info.fileName,	
					info.head->voiceLength);
	if(-1 == execSql(buffer)) return -1;
	sprintf(buffer, "INSERT INTO voicedata(bid, data)"
					"VALUES(%d, :DATA)", id);
	bindValue(buffer, ":DATA", info.data, info.head->voiceLength);
	if(-1 == execSql(buffer)) return -1;

	freeWaveFile(&info);

	return 0;
}
int selectWaveFile(const char *fileName)
{
	int i, j;
	char **row;

	char buffer[512] = "SELECT id, name, rate, bits, channels, "
					   "length FROM baseinfo ";
	if(fileName != NULL)
	{
		sprintf(buffer, "%s WHERE name LIKE '%%%s%%'",
						buffer, fileName);
	}
	if(-1 == querySql(buffer)) return -1;

	for(i = 0; NULL != (row = getRow(i)); i++)
	{
		for(j = 0; j < 6; j++)
		{
			fprintf(stdout, "%s\t", row[j]);
		}
		fprintf(stdout, "\n");
	}
	return 0;
}

int deleteWaveFile(const char *id)
{
	char c;
	char buffer[512];
	char *p;
	int bid;

	bid = strtol(id, &p, 10);
	if(*p != '\0')
	{
		fprintf(stderr, "Error id (%s)\n", id);
		return -1;
	}
	fprintf(stderr, "Delete id = %s recoder ? (y/n)", id);
	c = getchar();
	while(getchar() != '\n');
	if(c != 'y' && c != 'Y') return -1;
	sprintf(buffer, "DELETE baseinfo,voicedata FROM baseinfo,voicedata WHERE "
					"baseinfo.id=%d AND voicedata.bid=%d", bid, bid);
	if(-1 == execSql(buffer)) return -1;

	return 0;
}
int playWaveFile(const char *id)
{
	unsigned long *lengths;
	char** row;
	char *p;
	WaveInfo info;
	WaveHead head;
	int bid;
	char buffer[512] = "SELECT baseinfo.id, baseinfo.name, "
						"baseinfo.rate, baseinfo.bits, baseinfo.channels, "
					    "baseinfo.length, voicedata.data FROM "
						"baseinfo, voicedata WHERE baseinfo.id=voicedata.bid "
						"AND baseinfo.id=";

	bid = strtol(id, &p, 10);
	if(*p != '\0')
	{
		fprintf(stderr, "Error id (%s)\n", id);
		return -1;
	}
	strcat(buffer, id);
	strcat(buffer, " LIMIT 1");
	if(-1 == querySql(buffer)) return -1;

	row = getRow(0);
	lengths = getLengths(0);
	if(NULL == lengths) return -1;
	info.head = &head;
	info.fileName = row[1];
	head.rate = atoi(row[2]);
	head.bits = atoi(row[3]);
	head.channels = atoi(row[4]);
	info.data = row[6];
	info.fileLength = lengths[6];

	return playWave(&info);
}

