/********************************************************************************************************************************
 *									 All Rights GLOBALSAT Reserved													            *
 ********************************************************************************************************************************
 ********************************************************************************************************************************
 * Filename	   	: ParsePAT.h
 * Description  : parse the PAT
 * Version	   	: 2.0
 * History      :
 * WuYunXing 	: 2013-3-22  Create
 *********************************************************************************************************************************/

#ifndef _PARSE_PAT_H_
#define _PARSE_PAT_H_

#define PAT_PROGRAM_MAX 512

typedef struct TS_PAT_PROGRAM
{
	unsigned int program_number :16;
	unsigned int reserved_3 :3;
	unsigned int program_map_PID :13;
} TS_PAT_PROGRAM;

typedef struct TS_PAT
{
	unsigned int table_id :8;
	unsigned int section_syntax_indicator :1;
	unsigned int zero :1;
	unsigned int reserved_1 :2;
	unsigned int section_length :12;
	unsigned int transport_stream_id :16;
	unsigned int reserved_2 :2;
	unsigned int version_number :5;
	unsigned int current_next_indicator :1;
	unsigned int section_number :8;
	unsigned int last_section_number :8;

	TS_PAT_PROGRAM astPAT_Program[PAT_PROGRAM_MAX];

	unsigned int network_PID :13;
	unsigned int CRC_32 :32;
} TS_PAT;

/*----------------------------------------------------------*/
/* the information we want to get form the PAT table */
/*----------------------------------------------------------*/
typedef struct PAT_INFO
{
	unsigned int uiProgramNumber :16;
	unsigned int uiPMT_PID :13;
} PAT_INFO;

/*********************************************************************************************************************************
 * Function Name : ParsePAT_Table
 * Description   :  parse the PAT table put the information in PAT_INFO
 * Parameters    :
 * 				pfTsFile -- the TS file descriptor
 * 				iTsPosition -- the TS package begin position
 *				iTsLength -- the TS package length
 *				pstPAT_Info -- the PAT_INFO array
 * Returns       :-1 - failure  >=0 - the count of the program
 *********************************************************************************************************************************/
int ParsePAT_Table(FILE *pfTsFile, int iTsPosition, int iTsLength, PAT_INFO *pstPAT_Info);

#endif /* PARSEPAT_H_ */
