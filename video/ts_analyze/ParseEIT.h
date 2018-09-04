/********************************************************************************************************************************
 *									 All Rights GLOBALSAT Reserved													            *
 ********************************************************************************************************************************
 ********************************************************************************************************************************
 * Filename	   	: ParseEIT.c
 * Description  : parse the SDT
 * Version	   	: 1.0
 * History      :
 * WuYunXing 	: 2013-3-22  Create
 *********************************************************************************************************************************/

#ifndef _PARSE_EIT_H_
#define _PARSE_EIT_H_

#define EIT_EVENT_MAX 128

#define EIT_PF_TABLE_ID 0x4e
#define EIT_SE_TABLE_ID 0x50
#define EIT__DESCRIPTOR_MAX 4096
typedef struct TS_EIT_EVENT
{
	unsigned int event_id :16;
	unsigned char start_time[5];
	unsigned int duration :24;
	unsigned int running_status :3;
	unsigned int free_CA_mode :1;
	unsigned int descriptors_loop_length :12;
	unsigned char descriptor[EIT__DESCRIPTOR_MAX];
} TS_EIT_EVENT;

typedef struct TS_EIT
{
	unsigned int table_id :8;
	unsigned int section_syntax_indicator :1;
	unsigned int reserved_future_use_1 :1;
	unsigned int reserved_1 :2;
	unsigned int section_length :12;
	unsigned int service_id :16;
	unsigned int reserved_2 :2;
	unsigned int version_number :5;
	unsigned int current_next_indicator :1;
	unsigned int section_number :8;
	unsigned int last_section_number :8;
	unsigned int transport_stream_id :16;
	unsigned int original_network_id :16;
	unsigned int segment_last_section_number :8;
	unsigned int last_table_id :8;

	TS_EIT_EVENT astEIT_Event[EIT_EVENT_MAX];

	unsigned int CRC_32 :32;
} TS_EIT;

/*********************************************************************************************************************************
 * Function Name : ParseEIT_Table
 * Description   :  parse the PAT table and get the information
 * Parameters    :
 * 				pfTsFile -- the TS file descriptor
 * 				iTsPosition -- the TS package begin position
 *				iTsLength -- the TS package length
 *				astProgram -- the program structure where we put the information in
 * Returns       : -1 failure 0 -- no the section in the transport stream 1 - all section over
 *********************************************************************************************************************************/
int ParseEIT_Table(FILE *pfTsFile, int iTsPosition, int iTsLength, unsigned int uiTableId, unsigned int uiTableIdExtension);

#endif /* PARSEEIT_H_ */
