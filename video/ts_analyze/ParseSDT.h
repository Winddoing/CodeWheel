/********************************************************************************************************************************
 *									 All Rights GLOBALSAT Reserved													            *
 ********************************************************************************************************************************
 ********************************************************************************************************************************
 * Filename	   	: ParseSDT.c
 * Description  : parse the SDT
 * Version	   	: 1.0
 * History      :
 * WuYunXing 	: 2013-3-22  Create
 *********************************************************************************************************************************/

#ifndef _PARSE_SDT_H_
#define _PARSE_SDT_H_

#define SDT_DESCRIPTOR_MAX 1024
#define SDT_EVENT_MAX 128
#define SDT_PROGRAM_NAME_LENGTH 512

typedef struct TS_SDT_SERVICE
{
	unsigned int service_id :16;
	unsigned int reserved_future_use :6;
	unsigned int EIT_schedule_flag :1;
	unsigned int EIT_present_following_flag :1; //0x0F
	unsigned int running_status :3;
	unsigned int free_CA_mode :1;
	unsigned int descriptors_loop_length :12;
	unsigned char descriptor[SDT_DESCRIPTOR_MAX];
} TS_SDT_SERVICE;

typedef struct TS_SDT
{
	unsigned int table_id :8;
	unsigned int section_syntax_indicator :1;
	unsigned int reserved_future_use_1 :1;
	unsigned int reserved_1 :2;
	unsigned int section_length :12;
	unsigned int transport_stream_id :16;
	unsigned int reserved_2 :2;
	unsigned int version_number :5;
	unsigned int current_next_indicator :1;
	unsigned int section_number :8;
	unsigned int last_section_number :8;
	unsigned int original_network_id :16;
	unsigned int reserved_future_use_2 :8;

	TS_SDT_SERVICE astSDT_Service[SDT_EVENT_MAX];

	unsigned int CRC_32 :32;
} TS_SDT;

/*----------------------------------------------------------*/
/* the information we want to get form the SDT table */
/*----------------------------------------------------------*/
typedef struct SDT_INFO
{
	unsigned int uiServiceId :16;
	unsigned int uiFreeCA_Mode :1;
	unsigned int aucProgramNanme[SDT_PROGRAM_NAME_LENGTH];
	unsigned int uiEIT_ScheduleFlag :1;
	unsigned int uiEIT_PresentFollowingFlag :1;
} SDT_INFO;

/*********************************************************************************************************************************
 * Function Name : ParseSDT_Table
 * Description   :  parse the SDT table
 * Parameters    :
 * 				pfTsFile -- the TS file descriptor
 * 				iTsPosition -- the TS package begin position
 *				iTsLength -- the TS package length
 *				pstSDT_Info -- the SDT_INFO array
 * Returns       : the count of the SDT_INFO count
 *********************************************************************************************************************************/
int ParseSDT_Table(FILE *pfTsFile, int iTsPosition, int iTsLength, SDT_INFO *pstSDT_Info);

#endif /* PARSESDT_H_ */
