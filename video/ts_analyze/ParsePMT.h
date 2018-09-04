/********************************************************************************************************************************
 *									 All Rights GLOBALSAT Reserved													            *
 ********************************************************************************************************************************
 ********************************************************************************************************************************
 * Filename	   	: ParsePMT.c
 * Description  : parse the PAT
 * Version	   	: 1.0
 * History      :
 * WuYunXing 	: 2013-3-22  Create
 *********************************************************************************************************************************/

#ifndef _PARSE_PMT_H_
#define _PARSE_PMT_H_

#define PMT_DESCRIPTOR_MAX 1024
#define PMT_VUDIO_COUNT 20

#define PMT_STREAM_MAX 32

typedef struct TS_PMT_STREAM
{
	unsigned int stream_type :8;
	unsigned int reserved_5 :3;
	unsigned int elementary_PID :13;
	unsigned int reserved_6 :4;
	unsigned int ES_info_length :12;
	unsigned char descriptor[PMT_DESCRIPTOR_MAX];
} TS_PMT_STREAM;

typedef struct TS_PMT
{
	unsigned int table_id :8;
	unsigned int section_syntax_indicator :1;
	unsigned int zero :1;
	unsigned int reserved_1 :2;
	unsigned int section_length :12;
	unsigned int program_number :16;
	unsigned int reserved_2 :2;
	unsigned int version_number :5;
	unsigned int current_next_indicator :1;
	unsigned int section_number :8;
	unsigned int last_section_number :8;
	unsigned int reserved_3 :3;
	unsigned int PCR_PID :13;
	unsigned int reserved_4 :4;
	unsigned int program_info_length :12;
	unsigned char program_info_descriptor[PMT_DESCRIPTOR_MAX];

	TS_PMT_STREAM astPMT_Stream[PMT_STREAM_MAX];

	unsigned int CRC_32 :32;
} TS_PMT;

/*----------------------------------------------------------*/
/* the information we want to get form the PMT table */
/*----------------------------------------------------------*/
typedef struct PMT_INFO
{
	unsigned int uiProgramNumber :16;
	unsigned int uiVideoPID :13;
	unsigned int auiAudioPID[PMT_VUDIO_COUNT];
} PMT_INFO;

/*********************************************************************************************************************************
 * Function Name : ParsePMT_Table
 * Description   :  parse the PMT table
 * Parameters    :
 * 				pfTsFile -- the TS file descriptor
 * 				iTsPosition -- the TS package begin position
 *				iTsLength -- the TS package length
 *				uiWantPID -- the PMT_PID
 *				pstPMT_Info -- the PMT_INFO
 * Returns       : -1 failure 0 -- no the section in the transport stream >0 - the audio count in the PMT
 *********************************************************************************************************************************/
int ParsePMT_Table(FILE *pfTsFile, int iTsPosition, int iTsLength, unsigned int uiWantPID, PMT_INFO *pstPMT_Info);

#endif /* PARSEPMT_H_ */
