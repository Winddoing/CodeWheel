#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <global.h>

typedef struct{
	char *cmd;
	size_t cmdLen;
	char *ret;		//执行命后返回的字符串存储地址
	size_t retLen;	//系统可以接受的返回字符串长度
}BinCmd;

int initBinCmd(BinCmd *binCmd, size_t cmdLen, size_t bufferLen)
{
	bzero(binCmd, sizeof(BinCmd));
	binCmd->cmd = (char *)malloc(cmdLen);
	if(NULL == binCmd->cmd) 
		return -1;
	binCmd->ret = (char *)malloc(bufferLen);
	if(NULL == binCmd->ret) 
	{
		free(binCmd->cmd);
		binCmd->cmd = NULL;
		return -1;
	}
	binCmd->retLen = bufferLen;
	binCmd->cmdLen = cmdLen;
	return 0;
}
void destroyBinCmd(BinCmd *binCmd)
{
	if(binCmd->ret != NULL)
		free(binCmd->ret);
	if(binCmd->cmd != NULL)
		free(binCmd->cmd);
	bzero(binCmd, sizeof(BinCmd));
}
int execCmd(BinCmd *binCmd)
{
	//1. 去掉cmd字符串最后的换行
	char *pTmp;
	FILE *fp;
	size_t ret;

	pTmp = strrchr(binCmd->cmd, '\n');
	if(pTmp != NULL)
		*pTmp = '\0';

	fp = popen(binCmd->cmd, "r");
	if(NULL == fp) return -1;
	bzero(binCmd->ret, binCmd->retLen);
	if(0 == (ret = fread(binCmd->ret, sizeof(char), 
				binCmd->retLen - 1, fp)))
	{
		pclose(fp);
		return -1;
	}
	pclose(fp);

	return ret;
}

int main(int argc, char *argv[])
{
	BinCmd binCmd;
	ssize_t ret;
	int fifoFd = -1;

	fifoFd = open(FIFO_NAME, O_WRONLY);
	if(-1 == fifoFd)
		goto Error;
	if(-1 == initBinCmd(&binCmd, BUFFER_SIZE, 
							BUFFER_SIZE))
		goto Error;
	while(1)
	{
		fprintf(stderr, "cmd> ");
		
		fgets(binCmd.cmd, binCmd.cmdLen, stdin);
		//检查是否为退出命令
		if(0 == strncasecmp(QUIT_CMD, binCmd.cmd, 
						strlen(QUIT_CMD)))
			break;
		if(-1 == (ret = execCmd(&binCmd)))
			goto Error;
		if(-1 == write(fifoFd, binCmd.ret, ret))
			goto Error;
	}
	close(fifoFd);
	destroyBinCmd(&binCmd);
	fprintf(stdout, "Bye-Bye !\n");
	return EXIT_SUCCESS;
Error:
	perror("Writefifo");
	if(-1 == fifoFd)
		return EXIT_FAILURE;
	close(fifoFd);
	destroyBinCmd(&binCmd);
	return EXIT_FAILURE;
}






