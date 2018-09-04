/********************************************************************************************************************************
 *									 All Rights GLOBALSAT Reserved													            *
 ********************************************************************************************************************************
 ********************************************************************************************************************************
 * Filename	   	: GetSection.h
 * Description  : using the file for get the section of the PID package which we want
 * Version	   	: 1.0
 * History      :
 * WuYunXing 	: 2013-3-22  Create
 *********************************************************************************************************************************/

#ifndef _GET_SECTION_H_
#define _GET_SECTION_H_

#define INITIAL_VERSION 0xffff

#define SECTION_COUNT_256 256
#define PACKAGE_MAX_LENGTH_204 204
#define SECTION_MAX_LENGTH_4096 1024*4

typedef struct SECTION_HEAD
{
	unsigned int table_id :8;
	unsigned int section_syntax_indicator :1;
	unsigned int zero :1;
	unsigned int reserved_1 :2;
	unsigned int section_length :12;
	unsigned int table_id_extension :16;
	unsigned int reserved_2 :2;
	unsigned int version_number :5;
	unsigned int current_next_indicator :1;
	unsigned int section_number :8;
	unsigned int last_section_number :8;
} SECTION_HEAD;

/*********************************************************************************************************************************
 * Function Name : GetOneSection
 * Description   :  get one section
 * Parameters    :
 * 				pfTsFile -- the TS file descriptor
 * 				iTsLength -- the TS package length
 *				pucSectionBuffer -- the section buffer
 *				uiWantPID -- the PID which we want
 *				uiTableId -- the table id which we want
 *				uiTableIdExtension -- the table id extension
 *				puiVersion -- the record version number
 * Returns       : -1 - reach the file end 0 - the version change 1 - one section over
 *********************************************************************************************************************************/
int GetOneSection(FILE *pfTsFile, int iTsLength, unsigned char *pucSectionBuffer, unsigned int uiPID, unsigned int uiTableId,
        unsigned int *puiVersion);

/*********************************************************************************************************************************
 * Function Name : GetEIT_OneSection
 * Description   :  get the EIT one section(we should point the table_id_extension)
 * Parameters    :
 * 				pfTsFile -- the TS file descriptor
 * 				iTsLength -- the TS package length
 *				pucSectionBuffer -- the section buffer
 *				uiTableId -- the table id which we want
 *				uiTableIdExtension -- the table id extension (the program number which we want to know the EIT information)
 *				puiVersion -- the record version number
 * Returns       : -1 - reach the file end 0 - the version change 1 - one section over
 *********************************************************************************************************************************/
int GetEIT_OneSection(FILE *pfTsFile, int iTsLength, unsigned char *pucSectionBuffer, unsigned int uiTableId, unsigned uiTableIdExtension,
        unsigned int *puiVersion);

/*********************************************************************************************************************************
 * Function Name : IsSectionGetBefore
 * Description   :  is the section number has been get before
 * Parameters    :
 * 				pucSectionBuffer -- the section buffer
 * 				puiRecordSectionNumber -- record the section number and the section count
 * Returns       : 1 - the section already get  0 - do not get
 *********************************************************************************************************************************/
int IsSectionGetBefore(unsigned char *pucSectionBuffer, unsigned int *puiRecordSectionNumber);

/*********************************************************************************************************************************
 * Function Name : IsAllSectionOver
 * Description   :  is all the section is over
 * Parameters    :
 *				pucSectionBuffer -- the section buffer
 *				puiRecordSectionNumber -- the buffer record the parse section number and the section count
 * Returns       : 1 - all section over 0 - do not over
 *********************************************************************************************************************************/
int IsAllSectionOver(unsigned char *pucSectionBuffer, unsigned int *puiRecordSectionNumber);

#endif /* GETSECTION_H_ */
