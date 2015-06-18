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

char **getRow(int n);//n指写记录的行号
unsigned long *getLengths(int n);//n指写记录的行号
void freeResult(void);
void printError(void);
int connectToMysql(void);
int execSql(const char *sql);
int querySql(const char *sql);
/* sql是格式化SQL字符串，
 * str是在SQL字符串中为二进制数据占位的占位符
 * bindData是二进制数据的首地址
 * length是二进制数据的长度
 * 返回：转换后的SQL字符串长度 */
unsigned long bindValue(char *sql, const char *str,
			const char *binData, unsigned length);
/*ex:
 * 	sql = "Insert Into voicedata(5, :BinData)"
 *	str = ":BinData"
 *	binData = ...	//Length = 50
 *	binValue(sql, str, binData, 50);
 * 	sql = "Insert Into voicedata(5, '.....binData escape string........')"
 * */


#endif // MYMYSQL_H

