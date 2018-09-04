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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "GetSection.h"
#include "Descriptor.h"
#include "ParseEIT.h"

#define EIT_PID 0x0012

/*********************************************************************************************************************************
 * Function Name : ParseEIT_SectionHead
 * Description   :  parse the EIT section head
 * Parameters    :
 * 				pstTS_EIT -- the TS_EIT
 *				pucSectionBuffer -- the SDT section buffer
 * Returns       : void
 *********************************************************************************************************************************/
static void ParseEIT_SectionHead(TS_EIT *pstTS_EIT, unsigned char *pucSectionBuffer)
{
	int iEIT_Length = 0;

	pstTS_EIT->table_id = pucSectionBuffer[0];
	pstTS_EIT->section_syntax_indicator = pucSectionBuffer[1] >> 7;
	pstTS_EIT->reserved_future_use_1 = (pucSectionBuffer[1] >> 6) & 0x01;
	pstTS_EIT->reserved_1 = (pucSectionBuffer[1] >> 4) & 0x03;
	pstTS_EIT->section_length = ((pucSectionBuffer[1] & 0x0F)) << 8 | pucSectionBuffer[2];
	pstTS_EIT->service_id = (pucSectionBuffer[3] << 8) | pucSectionBuffer[4];
	pstTS_EIT->reserved_2 = pucSectionBuffer[5] >> 6;
	pstTS_EIT->version_number = (pucSectionBuffer[5] >> 1) & 0x1F;
	pstTS_EIT->current_next_indicator = (pucSectionBuffer[5] << 7) >> 7;
	pstTS_EIT->section_number = pucSectionBuffer[6];
	pstTS_EIT->last_section_number = pucSectionBuffer[7];
	pstTS_EIT->transport_stream_id = (pucSectionBuffer[8] << 8) | pucSectionBuffer[9];
	pstTS_EIT->original_network_id = (pucSectionBuffer[10] << 8) | pucSectionBuffer[11];
	pstTS_EIT->segment_last_section_number = pucSectionBuffer[12];
	pstTS_EIT->last_table_id = pucSectionBuffer[13];
	iEIT_Length = pstTS_EIT->section_length + 3;
	pstTS_EIT->CRC_32 = (pucSectionBuffer[iEIT_Length - 4] << 24) | (pucSectionBuffer[iEIT_Length - 3] << 16)
	        | (pucSectionBuffer[iEIT_Length - 2] << 8) | pucSectionBuffer[iEIT_Length - 1];

}

/*********************************************************************************************************************************
 * Function Name : ParseEIT_Section
 * Description   :  parse the EIT section
 * Parameters    :
 * 				pstTS_EIT -- the TS_EIT
 *				pucSectionBuffer -- the EIT section buffer
 * Returns       : the TS_EIT_EVENT count
 *********************************************************************************************************************************/
static int ParseEIT_Section(TS_EIT *pstTS_EIT, unsigned char *pucSectionBuffer)
{
	int iEIT_Length = 0;
	int iEventPosition = 0;
	int iEventCount = 0;

	memset(pstTS_EIT, 0, sizeof(TS_EIT));
	ParseEIT_SectionHead(pstTS_EIT, pucSectionBuffer);
	iEIT_Length = pstTS_EIT->section_length + 3;
	for (iEventPosition = 14; iEventPosition < iEIT_Length - 4; iEventPosition += 12)
	{
		pstTS_EIT->astEIT_Event[iEventCount].event_id = (pucSectionBuffer[iEventPosition]) << 8 | pucSectionBuffer[iEventPosition + 1];
		memcpy(pstTS_EIT->astEIT_Event[iEventCount].start_time, pucSectionBuffer + iEventPosition + 2, 5);
		pstTS_EIT->astEIT_Event[iEventCount].duration = (pucSectionBuffer[iEventPosition + 7] << 16)
		        | (pucSectionBuffer[iEventPosition + 8] << 8) | (pucSectionBuffer[iEventPosition + 9]);
		pstTS_EIT->astEIT_Event[iEventCount].running_status = pucSectionBuffer[iEventPosition + 10] >> 5;
		pstTS_EIT->astEIT_Event[iEventCount].free_CA_mode = (pucSectionBuffer[iEventPosition + 10] & 0x1F) >> 4;
		pstTS_EIT->astEIT_Event[iEventCount].descriptors_loop_length = ((pucSectionBuffer[iEventPosition + 10] & 0x0F) << 8)
		        | pucSectionBuffer[iEventPosition + 11];
		if (pstTS_EIT->astEIT_Event[iEventCount].descriptors_loop_length != 0)
		{
			memcpy(pstTS_EIT->astEIT_Event[iEventCount].descriptor, pucSectionBuffer + 12 + iEventPosition,
			        pstTS_EIT->astEIT_Event[iEventCount].descriptors_loop_length);
			iEventPosition += pstTS_EIT->astEIT_Event[iEventCount].descriptors_loop_length;
		}
		iEventCount++;
	}
	return iEventCount;
}

/*********************************************************************************************************************************
 * Function Name : PrintTheStartTime
 * Description   : out put the start time
 * Parameters    :
 *				pucStartTime -- the buffer of time
 * Returns       : void
 *********************************************************************************************************************************/
static void PrintTheStartTime(unsigned char *pucStartTime)
{
	int iTemp = 0;
	int iDay = 0;
	int iYear = 0;
	int iMonth = 0;
	unsigned int uiMJD = 0;
	unsigned char aucStartTime[25] = {0};

	if (NULL != pucStartTime)
	{
		uiMJD = (pucStartTime[0] << 8) | pucStartTime[1];

		iYear = (int)((uiMJD - 15078.2) / 365.25);
		iMonth = (int)((uiMJD - 14956.1 - (int)(iYear * 365.25)) / 30.6001);
		iDay = (int)(uiMJD - 14956 - (int)(iYear * 365.25) - (int)(iMonth * 30.6001));
		if ((14 == iMonth) || (15 == iMonth))
		{
			iTemp = 1;
		}
		iYear = iYear + iTemp;
		iMonth = iMonth - 1 - iTemp * 12;
		iYear = iYear + 1900;

		sprintf(aucStartTime, "%d/%d/%d   %x:%x:%x", iYear, iMonth, iDay, (unsigned char)pucStartTime[2], (unsigned char)pucStartTime[3],
		        (unsigned char)pucStartTime[4]);

		printf("the start time is %s\n", aucStartTime);
	}
}

/*********************************************************************************************************************************
 * Function Name : PrintTheDurationTime
 * Description   : out put the duration time
 * Parameters    :
 *				uiDuration -- the duration time
 * Returns       : void
 *********************************************************************************************************************************/
static void PrintTheDurationTime(unsigned int uiDurationTime)
{
	unsigned char aucDurationTime[25] = {0};
	unsigned char uiHour = uiDurationTime >> 16;
	unsigned char uiMinute = uiDurationTime >> 8;
	unsigned char uiSecond = uiDurationTime;

	sprintf(aucDurationTime, "%x:%x:%x", uiHour, uiMinute, uiSecond);

	printf("the duration time is %s\n", aucDurationTime);
}

/*********************************************************************************************************************************
 * Function Name : PrintShortEventDescriptor
 * Description   : out put the event information in the descriptor tag 0x4d
 * Parameters    :
 *				pucDescriptor -- the descriptor
 *				uiDescriprotLength -- the descriptor length
 * Returns       : void
 *********************************************************************************************************************************/
static void PrintShortEventDescriptor(unsigned char *pucDescriptor, unsigned int uiDescriprotLength)
{
	int iTemp = 0;
	SHORT_EVENT_DESCRIPTOR stShortEventDescriptor = {0};

	iTemp = GetShortEventDescriptor(&stShortEventDescriptor, pucDescriptor, uiDescriprotLength);
	if (0 == iTemp)
	{
		printf("the event_name_char is : %s\n", stShortEventDescriptor.event_name_char);
		printf("the text_char is : %s\n", stShortEventDescriptor.text_char);
	}
}

/*********************************************************************************************************************************
 * Function Name : PrintEIT_Info
 * Description   : pint the EIT information
 * Parameters    :
 *				pstTS_EIT -- the TS_EIT
 * Returns       : void
 *********************************************************************************************************************************/
static void PrintEIT_Info(TS_EIT *pstTS_EIT, int iEventCount)
{
	int iEventIndex = 0;

	printf("* * * * * * * * * the information of the EIT table* * * * * * * * * *\n");
	printf("the EIT information about the program %d:\n", pstTS_EIT->service_id);
	printf("the section_number is %x\n", pstTS_EIT->section_number);

	if (0 == iEventCount)
	{
		printf("no event in the EIT section\n");
	}
	for (iEventIndex = 0; iEventIndex < iEventCount; iEventIndex++)
	{

		PrintTheStartTime(pstTS_EIT->astEIT_Event[iEventIndex].start_time);

		PrintTheDurationTime(pstTS_EIT->astEIT_Event[iEventIndex].duration);

		PrintShortEventDescriptor(pstTS_EIT->astEIT_Event[iEventIndex].descriptor,
		        pstTS_EIT->astEIT_Event[iEventIndex].descriptors_loop_length);
	}
	printf("* * * * * * * * * * * * * * end * * * * * * * * * * * * * * * * * * *\n");
}

/*********************************************************************************************************************************
 * Function Name : ParseEIT_Table
 * Description   :  parse the EIT table and print the information (start_time,duration_time,event)
 * Parameters    :
 * 				pfTsFile -- the TS file descriptor
 * 				iTsPosition -- the TS package begin position
 *				iTsLength -- the TS package length
 *				uiTableId -- the table id (0x4e or 0x50)
 *				uiTableIdExtension -- the program id which we want to know the EIT information
 * Returns       :-1 - failure  >=0 - the count of the program
 *********************************************************************************************************************************/
int ParseEIT_Table(FILE *pfTsFile, int iTsPosition, int iTsLength, unsigned int uiTableId, unsigned int uiTableIdExtension)
{
	int iTemp = 0;
	unsigned int uiVersion = INITIAL_VERSION;
	unsigned int auiRecordGetSection[SECTION_COUNT_256] = {0};
	unsigned char aucSectionBuffer[SECTION_MAX_LENGTH_4096] = {0};
	TS_EIT stTS_EIT = {0};
	int iEventCount = 0;

	if (-1 == fseek(pfTsFile, iTsPosition, SEEK_SET))
	{
		printf("parse table error\n");
		return -1;
	}
	while (!feof(pfTsFile))
	{
		iTemp = GetEIT_OneSection(pfTsFile, iTsLength, aucSectionBuffer, uiTableId, uiTableIdExtension, &uiVersion);
		if (0 == iTemp) // version number change
		{
			uiVersion = INITIAL_VERSION;
			memset(auiRecordGetSection, 0, sizeof(char) * SECTION_COUNT_256); // clean the record array
			fseek(pfTsFile, 0 - iTsLength, SEEK_CUR);
		}
		if (1 == iTemp) //one section over
		{
			if (0 == IsSectionGetBefore(aucSectionBuffer, auiRecordGetSection))
			{
				iEventCount = ParseEIT_Section(&stTS_EIT, aucSectionBuffer);
				PrintEIT_Info(&stTS_EIT, iEventCount);
			}
			if (1 == IsAllSectionOver(aucSectionBuffer, auiRecordGetSection))
			{
				return 1;
			}
		}
		if (-1 == iTemp) //error
		{
			return 0;
		}
	}
	return 1;
}
