/**
  ******************************************************************************
  * @file file.h 
  * @author leon.xie
  * @version v1.0.0
  * @date 2018-10-6 11:10:39
  * @brief This file contains all the functions prototypes for the BOARD 
  *  file 
  ******************************************************************************
  * @attention
  *
  * File For Yunin Software Team Only
  *
  * Copyright (C), 2017-2027, Yunin Software Team
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __FILE_H
#define __FILE_H

/* Includes ------------------------------------------------------------------*/

#include <stdio.h>

#ifdef  __cplusplus
extern "C" {
#endif

/* Exported typedef ----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
int file_write(const char *path,const void *data,unsigned int len);
int file_read(const char *path,void *data,unsigned int len);

#ifdef  __cplusplus
}
#endif

#endif  /* __FILE_H */


