#include <shell.h>
#include <stdio.h>
#include <string.h>

// recive command prompt from server
int recivePrompt(const IpInfo *info, Data *data)
{
	int ret;
	// 1. send 'P' data to client
	data->type = TYPE_REQPROMPT;
	data->dataLength = 1;
	ret = writeData(info, data);
	if(TCPERR_SUCCESS != ret) return ret;
	// 2. recive 'prompt' data from server.
	ret = readData(info, data);
	return ret;
}
// recive command exec result from server
int reciveExecResult(const IpInfo *info, Data *data)
{
	int ret;
	char *pTmp;

	bzero(data->data, data->maxLength);
	pTmp = fgets(data->data, data->maxLength - 1, stdin);
	pTmp = strrchr(data->data, '\n');
	if(NULL != pTmp)
		*pTmp = '\0';
	if(0 == strcasecmp(QUIT_CMD, data->data))
	{
		return TYPE_QUITCMD;
	}
	data->type = TYPE_REQEXERES;
	data->dataLength = strlen(data->data);
	ret = writeData(info, data);
	if(ret != TCPERR_SUCCESS)
		return ret;
	ret = readData(info, data);
	return ret;
}

// analy command and call function
int analyData(const IpInfo *info, Data *data)
{
	int ret;
	//1. read data from client
	bzero(data->data, data->maxLength);
	ret = readData(info, data);
	if(ret != TCPERR_SUCCESS)
		return ret;
	if(TYPE_REQPROMPT == data->type)
	{
		ret = sendPrompt(info, data);
		if(ret != TCPERR_SUCCESS)
			return ret;
	}else if(TYPE_REQEXERES == data->type)
	{
		ret = sendExecResult(info, data);
		if(ret != TCPERR_SUCCESS)
			return ret;
	}else
		return  TYPE_ERRCMD;
	return ret;
}
static int execCmd(Data *data)
{
	FILE *fp;
	int ret;
	//错误输出也要读到运行结果中
	strcat(data->data, " 2>&1");
	//打开读管道，执行用户命令
	if(NULL == (fp = popen(data->data, "r")))
	{
		strcpy(data->data, "Error!\n");
		data->dataLength = strlen(data->data);
		return 0;
	}
	//清空数据区，读取管道中的数据
	bzero(data->data, data->maxLength);
	ret = fread(data->data, 1, data->maxLength - 1, fp);
	pclose(fp);
	//在数据区添加一个长度的换行字符
	data->dataLength = ret + 1;
//	strcat(data->data, "\n");
	return 0;
}
// send command prompt to client
int sendPrompt(const IpInfo *info, Data *data)
{
	strcpy(data->data, "echo [`whoami`@`hostname -s` `basename $PWD`]");
	execCmd(data);
	data->data[data->dataLength - 2] = 
		strstr(data->data, "[root@") ? '#' : '$';
	data->type = TYPE_SNDPROMPT;

	return writeData(info, data);
}
// send command exec result to client
int sendExecResult(const IpInfo *info, Data *data)
{
	int ret;

	execCmd(data);
	data->type = TYPE_SNDEXERES;
	ret = writeData(info, data);

	return ret;
}


