/********************************************************************************************************************************
 *									 All Rights GLOBALSAT Reserved													            *
 ********************************************************************************************************************************
 ********************************************************************************************************************************
 * Filename	   	: GetSection.c
 * Description  : using the file for get the section of the PID package which we want
 * Version	   	: 1.0
 * History      :
 * WuYunXing 	: 2013-4-11  Create
 *********************************************************************************************************************************/

#ifndef PARSENIT_H_
#define PARSENIT_H_

#define NIT_DESCRIPTOR_MAX 1024
#define NIT_TRANSPORT_MAX 128

typedef struct TS_NIT_TRANSPORT
{
	unsigned int transport_stream_id :16;
	unsigned int original_network_id :16;
	unsigned int reserved_future_use :4;
	unsigned int transport_descriptors_length :12;
	unsigned char descriptor[NIT_DESCRIPTOR_MAX];

} TS_NIT_TRANSPORT;

typedef struct TS_NIT
{
	unsigned int table_id :8;
	unsigned int section_syntax_indicator :1;
	unsigned int reserved_future_use_1 :1;
	unsigned int reserved_1 :2;
	unsigned int section_length :12;
	unsigned int network_id :16;
	unsigned int reserved_2 :2;
	unsigned int version_number :5;
	unsigned int current_next_indicator :1;
	unsigned int section_number :8;
	unsigned int last_section_number :8;
	unsigned int reserved_future_use_2 :4;
	unsigned int network_descriptors_length :12;
	unsigned char network_descriptor[NIT_DESCRIPTOR_MAX];
	unsigned int reserved_future_use_3 :4;
	unsigned int transport_stream_loop_length :12;

	TS_NIT_TRANSPORT astNIT_Transport[NIT_TRANSPORT_MAX];

	unsigned int CRC_32 :32;
} TS_NIT;

/*********************************************************************************************************************************
 * Function Name : ParseNIT_Table
 * Description   :  parse the PAT table and get the information
 * Parameters    :
 * 				pfTsFile -- the TS file descriptor
 * 				iTsPosition -- the TS package begin position
 *				iTsLength -- the TS package length
 * Returns       : -1 failure 0 -- no the section in the transport stream 1 - all section over
 *********************************************************************************************************************************/
int ParseNIT_Table(FILE *pfTsFile, int iTsPosition, int iTsLength);

#endif /* PARSENIT_H_ */
