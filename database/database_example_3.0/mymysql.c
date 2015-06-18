#include <mymysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

static MYSQL *db;
static MYSQL_RES *res;

unsigned long bindValue(char *sql, const char *str,
			const char *binData, unsigned length)
{
	char *p, *pTail;
	unsigned long len;
	p = strstr(sql, str);//p = 在SQL语句中占位符所在的内存地址
	if(NULL == p) return 0;
	len = strlen(p + strlen(str));//len = 占位符后有效的SQL字符串长度
	//分配内存空间，用于存储占位符之后的字符串
	pTail = (char *)malloc(len + 1);
	//将占位符后的字符串复制到pTail指向的内存中
	strcpy(pTail, p + strlen(str));
	//由于占位符开始的位置将被替换为字符串，所以
	//先将占位符中的第一个字符替换为单引号
	*p++ = '\'';
	p += mysql_escape_string(p, binData, length);
	*p++ = '\'';
	//将原来保存在pTail中的SQL字符串后半段再复制回来
	strcpy(p, pTail);
	free(pTail);
	return p - sql + len;
}
int querySql(const char *sql)
{
	int ret;

	ret = execSql(sql);
	if(-1 == ret) return -1;
	if(res != NULL)//避免在提取结果集时出现内存泄漏
		mysql_free_result(res);
	res = mysql_store_result(db);
	if(NULL == res) return -1;
	
	return 0;
}
void freeResult(void)
{
	mysql_free_result(res);
	res = NULL;
}
char **getRow(int n)//n指写记录的行号
{
	if(NULL == res) return NULL;
	mysql_data_seek(res, n);//将结果集指针指行第n行
	return mysql_fetch_row(res);
}

int execSql(const char *sql)
{
	int ret;

	ret = mysql_real_query(db, sql, strlen(sql));
	if(ret != 0)
	{
		printError();
		return -1;
	}
	return 0;
}

void printError(void)
{
	fprintf(stderr, "Mysql Error(%d):%s\n",
			mysql_errno(db), mysql_error(db));
}

int connectToMysql(void)
{
	db = mysql_init(NULL);
	assert(db);

	if(NULL == mysql_real_connect(db, MYSQL_HOST,
					MYSQL_USER, MYSQL_PASS,
					MYSQL_DBNM, MYSQL_PORT,
					MYSQL_SOCK, MYSQL_ARGS))
	{
		printError();
		mysql_close(db);
		return -1;
	}
	return 0;
}

