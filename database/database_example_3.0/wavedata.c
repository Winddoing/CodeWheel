#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>

#include <wave.h>
#include <mymysql.h>

#define MAX_SQL_LENGTH	(4096 * 1024)

int InsertWaveFile(const char *fileName)
{
	WaveInfo info;
	char buffer[MAX_SQL_LENGTH];
	int id;
	char **row;

	//取得WAVE音频文件信息
	if(-1 == getWaveFile(WAVEFILE, &info))
		return -2;
	//找到baseinfo表中的最大ID值
	strcpy(buffer, "SELECT MAX(id) + 1 FROM baseinfo");
	if(-1 == querySql(buffer)) return -1;
	row = getRow(0);
	if(NULL == row) return -1;
	id = atoi(row[0]);
	//生成插入baseinfo表的SQL语句
	sprintf(buffer, "INSERT INTO baseinfo(id, channels, bits, rate, "
					"name, length)VALUES(%d, %d, %d, %d, %s, %d)",
					id, info.head->channels, info.head->bits,
					info.head->rate, info.fileName,	info.head->voiceLength);
	if(-1 == execSql(buffer)) return -1;
	sprintf(buffer, "INSERT INTO voicedata(bid, data)"
					"VALUES(%d, :DATA)", id);
	bindValue(buffer, ":DATA", info.data, info.head->voiceLength);
	if(-1 == execSql(buffer)) return -1;

	freeWaveFile(&info);

	return 0;
}


