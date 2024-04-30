/*#############################################################
 *     File Name	: client.c
 *     Author		: wqshao
 *     Created Time	: 2018年09月18日 星期二 19时05分52秒
 *     Description	:
 *############################################################*/

#include <stdio.h>
#include <stdlib.h>

/*
 * **功能：客户端心跳包发送线程函数
 * **参数：线程传参（可传套接字）
 * **返回值：空
 * */
void *send_heart(void *addr)
{
	while(1){
		pd->data_type = HEART;  //HEART:数据包类型,pd为数据包结构体指针
		write(client_sockfd, pd, sizeof(DATA_PACK));
		sleep(3); //定时3秒
	}
	return NULL;
}

