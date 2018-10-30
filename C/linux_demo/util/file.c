/**
  ******************************************************************************
  * @file file.c
  * @author  leon.xie
  * @version v1.0.0
  * @date 2018-10-6 11:10:36
  * @brief  This file provides all the file functions. 
  ******************************************************************************
  * @attention
  *
  * File For Yunin Software Team Only
  *
  * Copyright (C), 2017-2027, Yunin Software Team
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "file.h"

#ifdef  __cplusplus
extern "C" {
#endif

/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/



//open/close/read/write

/*
 * 写文件, 原子操作
 */
int file_write(const char *path,const void *data,unsigned int len)
{
	int fd;
	int size = -1;
	fd = open(path,O_WRONLY|O_CREAT,0777);
	if(fd < 0)
	{
		printf("open %s fail...\n",path);
		return -1;
	}
	size = write(fd,data,len);
	if(size != len)
	{
		printf("write %s fail...\n",path);
		return -1;
	}
	close(fd);
	return 0;
}

/*
 * 读文件, 原子操作
 */
int file_read(const char *path,void *data,unsigned int len)
{
	int fd;
	int size = -1;
	fd = open(path,O_RDONLY,0777);
	if(fd < 0)
	{
		printf("open %s fail...\n",path);
		return -1;
	}
	size = read(fd,data,len);
	if(size < 0)
	{
		printf("read %s fail...\n",path);
		return -1;
	}
	close(fd);
	return 0;
}



#ifdef  __cplusplus
}
#endif


