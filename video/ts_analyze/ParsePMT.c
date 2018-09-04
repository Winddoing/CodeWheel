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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Descriptor.h"
#include "GetSection.h"
#include "ParsePMT.h"

#define PMT_TABLE_ID 0x02

/*********************************************************************************************************************************
 * Function Name : ParsePMT_SectionHead
 * Description   :  parse the PMT section head
 * Parameters    :
 * 				pstTS_PAT -- the TS_PMT
 *				pucSectionBuffer -- the PMT section buffer
 * Returns       : void
 *********************************************************************************************************************************/
static void ParsePMT_SectionHead(TS_PMT *pstTS_PMT, unsigned char *pucSectionBuffer)
{
	int iPMT_Length = 0;

	pstTS_PMT->table_id = pucSectionBuffer[0];
	pstTS_PMT->section_syntax_indicator = pucSectionBuffer[1] >> 7;
	pstTS_PMT->zero = (pucSectionBuffer[1] >> 6) & 0x01;
	pstTS_PMT->reserved_1 = (pucSectionBuffer[1] >> 4) & 0x03;
	pstTS_PMT->section_length = ((pucSectionBuffer[1] & 0x0F) << 8) | pucSectionBuffer[2];
	pstTS_PMT->program_number = (pucSectionBuffer[3] << 8) | pucSectionBuffer[4];
	pstTS_PMT->reserved_2 = pucSectionBuffer[5] >> 6;
	pstTS_PMT->version_number = (pucSectionBuffer[5] >> 1) & 0x1F;
	pstTS_PMT->current_next_indicator = (pucSectionBuffer[5] << 7) >> 7;
	pstTS_PMT->section_number = pucSectionBuffer[6];
	pstTS_PMT->last_section_number = pucSectionBuffer[7];
	pstTS_PMT->reserved_3 = pucSectionBuffer[8] >> 5;
	pstTS_PMT->PCR_PID = ((pucSectionBuffer[8] << 8) | pucSectionBuffer[9]) & 0x1FFF;
	pstTS_PMT->reserved_4 = pucSectionBuffer[10] >> 4;
	pstTS_PMT->program_info_length = ((pucSectionBuffer[10] & 0x0F) << 8) | pucSectionBuffer[11];
	if (0 != pstTS_PMT->program_info_length)
	{
		memcpy(pstTS_PMT->program_info_descriptor, pucSectionBuffer + 12, pstTS_PMT->program_info_length);
	}
	iPMT_Length = pstTS_PMT->section_length + 3;
	pstTS_PMT->CRC_32 = (pucSectionBuffer[iPMT_Length - 4] << 24) | (pucSectionBuffer[iPMT_Length - 3] << 16)
	        | (pucSectionBuffer[iPMT_Length - 2] << 8) | pucSectionBuffer[iPMT_Length - 1];
}

/*********************************************************************************************************************************
 * Function Name : ParsePMT_Section
 * Description   :  parse the PMT section
 * Parameters    :
 * 				pstTS_PMT -- the TS_PMT
 *				pucSectionBuffer -- the PMT section buffer
 * Returns       : the TS_PMT_STREAM count
 *********************************************************************************************************************************/
static int ParsePMT_Section(TS_PMT *pstTS_PMT, unsigned char *pucSectionBuffer)
{

	int iStreamPosition = 12;
	int iPMT_Length = 0;
	int iStreamCount = 0;

	ParsePMT_SectionHead(pstTS_PMT, pucSectionBuffer);
	iPMT_Length = pstTS_PMT->section_length;
	for (iStreamPosition += pstTS_PMT->program_info_length; iStreamPosition < iPMT_Length - 4; iStreamPosition += 5)
	{
		pstTS_PMT->astPMT_Stream[iStreamCount].stream_type = pucSectionBuffer[iStreamPosition];
		pstTS_PMT->astPMT_Stream[iStreamCount].reserved_5 = pucSectionBuffer[iStreamPosition + 1] >> 5;
		pstTS_PMT->astPMT_Stream[iStreamCount].elementary_PID = ((pucSectionBuffer[iStreamPosition + 1] << 8)
		        | pucSectionBuffer[iStreamPosition + 2]) & 0x1FFF;
		pstTS_PMT->astPMT_Stream[iStreamCount].reserved_6 = pucSectionBuffer[iStreamPosition + 3] >> 4;
		pstTS_PMT->astPMT_Stream[iStreamCount].ES_info_length = ((pucSectionBuffer[iStreamPosition + 3] & 0x0F) << 8)
		        | pucSectionBuffer[iStreamPosition + 4];
		if (0 != pstTS_PMT->astPMT_Stream[iStreamCount].ES_info_length)
		{
			memcpy(pstTS_PMT->astPMT_Stream[iStreamCount].descriptor, pucSectionBuffer + 5 + iStreamPosition,
			        pstTS_PMT->astPMT_Stream[iStreamCount].ES_info_length);
			iStreamPosition += pstTS_PMT->astPMT_Stream[iStreamCount].ES_info_length;
		}
		iStreamCount++;
	}
	return iStreamCount;
}

/*********************************************************************************************************************************
 * Function Name : GetPMT_Info
 * Description   :  get the PMT information (program_number video_pid audio_pid)
 * Parameters    :
 * 				pstTS_PMT -- the TS_PMT
 * 				iStreamCount -- the TS_PMT_STREAM count
 *				pstPMT_Info -- the PMT_INFO
 *				iVideoCount -- the video count
 * Returns       : void
 *********************************************************************************************************************************/
static void GetPMT_Info(TS_PMT *pstTS_PMT, int iStreamCount, PMT_INFO *pstPMT_Info, int *iVideoCount)
{
	int iLoopTime = 0;
	unsigned int uiStreamType = 0;
	unsigned int uiElementrayPID = 0;

	pstPMT_Info->uiProgramNumber = pstTS_PMT->program_number;
	for (iLoopTime = 0; iLoopTime < iStreamCount; iLoopTime++)
	{
		uiStreamType = pstTS_PMT->astPMT_Stream[iLoopTime].stream_type;
		uiElementrayPID = pstTS_PMT->astPMT_Stream[iLoopTime].elementary_PID;

		/*----------------------------------------------------------*/
		/* get the audio PID */
		/*----------------------------------------------------------*/
		if ((0x04 == uiStreamType) || (0x03 == uiStreamType) || (0x0F == uiStreamType) || (0x11 == uiStreamType)) //audio
		{
			pstPMT_Info->auiAudioPID[*iVideoCount] = uiElementrayPID;
			(*iVideoCount)++;
		}
		/*----------------------------------------------------------*/
		/* get the video PID */
		/*----------------------------------------------------------*/
		if ((0x01 == uiStreamType) || (0x02 == uiStreamType) || (0x1B == uiStreamType)) //video
		{
			pstPMT_Info->uiVideoPID = uiElementrayPID;
		}
	}
}
/*********************************************************************************************************************************
 * Function Name : CleanPMT_Info
 * Description   :  clean PMT information
 * Parameters    :
 *				pstPMT_Info -- the PMT_INFO
 *				piAudioCount -- the audio count
 * Returns       : void
 *********************************************************************************************************************************/
static void CleanPMT_Info(PMT_INFO *pstPMT_Info, int *piAudioCount)
{
	(*piAudioCount) = 0;
	memset(pstPMT_Info, 0, sizeof(TS_PMT_STREAM));
}

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
int ParsePMT_Table(FILE *pfTsFile, int iTsPosition, int iTsLength, unsigned int uiWantPID, PMT_INFO *pstPMT_Info)
{
	int iTemp = 0;
	TS_PMT stTS_PMT = {0};
	int iStreamCount = 0;
	int iAudioCount = 0;
	unsigned int uiVersion = INITIAL_VERSION;
	unsigned int auiRecordSectionNumber[SECTION_COUNT_256] = {0};
	unsigned char aucSectionBuffer[SECTION_MAX_LENGTH_4096] = {0};

	memset(pstPMT_Info, 0, sizeof(PMT_INFO));
	if (-1 == fseek(pfTsFile, iTsPosition, SEEK_SET))
	{
		printf("parse table error\n");
		return -1;
	}
	while (!feof(pfTsFile))
	{
		iTemp = GetOneSection(pfTsFile, iTsLength, aucSectionBuffer, uiWantPID, PMT_TABLE_ID, &uiVersion);
		if (0 == iTemp) // version number change
		{
			uiVersion = INITIAL_VERSION;
			memset(auiRecordSectionNumber, 0, sizeof(char) * SECTION_COUNT_256); // clean the record array
			fseek(pfTsFile, 0 - iTsLength, SEEK_CUR);
			CleanPMT_Info(pstPMT_Info, &iAudioCount);
		}
		if (1 == iTemp) //one section over
		{
			if (0 == IsSectionGetBefore(aucSectionBuffer, auiRecordSectionNumber)) //judgment the section whether been parsed before
			{
				iStreamCount = ParsePMT_Section(&stTS_PMT, aucSectionBuffer);
				GetPMT_Info(&stTS_PMT, iStreamCount, pstPMT_Info, &iAudioCount);
			}
			if (1 == IsAllSectionOver(aucSectionBuffer, auiRecordSectionNumber))
			{
				return iAudioCount;
			}
		}
		if (-1 == iTemp) //error
		{
			printf("%x no PMT table \n", uiWantPID);
			return 0;
		}
	}
	return 0;
}
