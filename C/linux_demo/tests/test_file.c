/**
  ******************************************************************************
  * @file test_file.c
  * @author  leon.xie
  * @version v1.0.0
  * @date 2018-10-6 11:10:44
  * @brief  This file provides all the test_file functions. 
  ******************************************************************************
  * @attention
  *
  * File For Yunin Software Team Only
  *
  * Copyright (C), 2017-2027, Yunin Software Team
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include <string.h>

#include "file.h"

#ifdef  __cplusplus
extern "C" {
#endif

/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


int main(int argc, char *argv[])
{
    char msg[50]={0};

	file_read("/mnt/hgfs/share/hello.txt",msg,sizeof(msg));
	printf("%s\n",msg);
 	return 0;
}

#ifdef  __cplusplus
}
#endif


