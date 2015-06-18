#include <mymysql.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

static MYSQL *db;

int querySql(const char *sql)
{
	int ret;
	MYSQL_RES *res;
	MYSQL_ROW row;

	ret = execSql(sql);
	if(-1 == ret) return -1;
	res = mysql_store_result(db);
	if(NULL == res) return -1;

	while(NULL != (row = mysql_fetch_row(res)))
	{
		fprintf(stdout, "%s\t%s\t%s\t%s\n",
					row[0], row[1], row[2], row[3]);
	}

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

