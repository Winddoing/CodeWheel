#ifndef MYMYSQL_H
#define MYMYSQL_H

#include <mysql/mysql.h>

#define MYSQL_HOST	"localhost"
#define MYSQL_USER	"root"
#define MYSQL_PASS	"123456"
#define MYSQL_DBNM	"wavedata"
#define MYSQL_PORT	3306
#define MYSQL_SOCK	NULL
#define MYSQL_ARGS	0
//正常情况下，以上几个参数应写到配置文件中

void printError(void);
int connectToMysql(void);
int execSql(const char *sql);

#endif // MYMYSQL_H

