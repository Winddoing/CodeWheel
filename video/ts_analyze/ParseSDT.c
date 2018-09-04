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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "GetSection.h"
#include "Descriptor.h"
#include "ParseSDT.h"

#define SDT_PID 0x0011
#define SDT_TABLE_ID 0x42

/*********************************************************************************************************************************
 * Function Name : ParseSDT_SectionHead
 * Description   :  parse the SDT section head
 * Parameters    :
 * 				pstTS_SDT -- the TS_SDT
 *				pucSectionBuffer -- the SDT section buffer
 * Returns       : void
 *********************************************************************************************************************************/
static void ParseSDT_SectionHead(TS_SDT *pstTS_SDT, unsigned char *pucSectionBuffer)
{
	int iSDT_Length = 0;

	pstTS_SDT->table_id = pucSectionBuffer[0];
	pstTS_SDT->section_syntax_indicator = pucSectionBuffer[1] >> 7;
	pstTS_SDT->reserved_future_use_1 = (pucSectionBuffer[1] >> 6) & 0x01;
	pstTS_SDT->reserved_1 = (pucSectionBuffer[1] >> 4) & 0x03;
	pstTS_SDT->section_length = ((pucSectionBuffer[1] & 0x0F) << 8) | pucSectionBuffer[2];
	pstTS_SDT->transport_stream_id = (pucSectionBuffer[3] << 8) | pucSectionBuffer[4];
	pstTS_SDT->reserved_2 = pucSectionBuffer[5] >> 6;
	pstTS_SDT->version_number = (pucSectionBuffer[5] >> 1) & 0x1F;
	pstTS_SDT->current_next_indicator = (pucSectionBuffer[5] << 7) >> 7;
	pstTS_SDT->section_number = pucSectionBuffer[6];
	pstTS_SDT->last_section_number = pucSectionBuffer[7];
	pstTS_SDT->original_network_id = (pucSectionBuffer[8] << 8) | pucSectionBuffer[9];
	pstTS_SDT->reserved_future_use_2 = pucSectionBuffer[10];
	iSDT_Length = pstTS_SDT->section_length + 3;
	pstTS_SDT->CRC_32 = (pucSectionBuffer[iSDT_Length - 4] << 24) | (pucSectionBuffer[iSDT_Length - 3] << 16)
	        | (pucSectionBuffer[iSDT_Length - 2] << 8) | pucSectionBuffer[iSDT_Length - 1];
}

/*********************************************************************************************************************************
 * Function Name : ParseSDT_Section
 * Description   :  parse the SDT section
 * Parameters    :
 * 				pstTS_SDT -- the TS_SDT
 *				pucSectionBuffer -- the SDT section buffer
 * Returns       : the TS_SDT_SERVICE count
 *********************************************************************************************************************************/
static int ParseSDT_Section(TS_SDT *pstTS_SDT, unsigned char *pucSectionBuffer)
{
	int iSDT_Length = 0;
	int iServiceCount = 0;
	int iServicePosition = 11;

	memset(pstTS_SDT, 0, sizeof(TS_SDT));
	ParseSDT_SectionHead(pstTS_SDT, pucSectionBuffer);
	iSDT_Length = pstTS_SDT->section_length;
	for (iServicePosition = 11; iServicePosition < iSDT_Length - 4; iServicePosition += 5)
	{
		pstTS_SDT->astSDT_Service[iServiceCount].service_id = (pucSectionBuffer[iServicePosition] << 8)
		        | pucSectionBuffer[iServicePosition + 1];
		pstTS_SDT->astSDT_Service[iServiceCount].reserved_future_use = pucSectionBuffer[iServicePosition + 2] >> 2;
		pstTS_SDT->astSDT_Service[iServiceCount].EIT_schedule_flag = (pucSectionBuffer[iServicePosition + 2] & 0x03) >> 1;
		pstTS_SDT->astSDT_Service[iServiceCount].EIT_present_following_flag = pucSectionBuffer[iServicePosition + 2] & 0x01;
		pstTS_SDT->astSDT_Service[iServiceCount].running_status = pucSectionBuffer[iServicePosition + 3] >> 5;
		pstTS_SDT->astSDT_Service[iServiceCount].free_CA_mode = (pucSectionBuffer[iServicePosition + 3] & 0x1F) >> 4;
		pstTS_SDT->astSDT_Service[iServiceCount].descriptors_loop_length = ((pucSectionBuffer[iServicePosition + 3] & 0x0F) << 8)
		        | pucSectionBuffer[iServicePosition + 4];
		if (pstTS_SDT->astSDT_Service[iServiceCount].descriptors_loop_length != 0)
		{
			memcpy(pstTS_SDT->astSDT_Service[iServiceCount].descriptor, pucSectionBuffer + 5 + iServicePosition,
			        pstTS_SDT->astSDT_Service[iServiceCount].descriptors_loop_length);
			iServicePosition += pstTS_SDT->astSDT_Service[iServiceCount].descriptors_loop_length;
		}
		iServiceCount++;
	}
	return iServiceCount;
}

/*********************************************************************************************************************************
 * Function Name : GetSDT_Info
 * Description   :  get the SDT information (service_id and program_name)
 * Parameters    :
 * 				pstTS_SDT -- the TS_SDT
 * 				iServiceCount -- TS_SDT_SERVICE count
 *				pstSDT_Info -- the SDT_INFO array
 *				iInfoCount -- the SDT_INFO count
 * Returns       : void
 *********************************************************************************************************************************/
static void GetSDT_Info(TS_SDT *pstTS_SDT, int iServiceCount, SDT_INFO *pstSDT_Info, int *iInfoCount)
{
	int iTemp = 0;
	int iLoopTime = 0;
	SERVICE_DESCRIPTOR stServiceDescriptor = {0};

	for (iLoopTime = 0; iLoopTime < iServiceCount; iLoopTime++)
	{
		pstSDT_Info[*iInfoCount].uiServiceId = pstTS_SDT->astSDT_Service[iLoopTime].service_id;
		pstSDT_Info[*iInfoCount].uiFreeCA_Mode = pstTS_SDT->astSDT_Service[iLoopTime].free_CA_mode;
		pstSDT_Info[*iInfoCount].uiEIT_PresentFollowingFlag = pstTS_SDT->astSDT_Service[iLoopTime].EIT_present_following_flag;
		pstSDT_Info[*iInfoCount].uiEIT_ScheduleFlag = pstTS_SDT->astSDT_Service[iLoopTime].EIT_schedule_flag;
		iTemp = GetServiceDescriptor(&stServiceDescriptor, pstTS_SDT->astSDT_Service[iLoopTime].descriptor,
		        pstTS_SDT->astSDT_Service[iLoopTime].descriptors_loop_length);
		if (0 == iTemp)
		{
			memcpy(pstSDT_Info[*iInfoCount].aucProgramNanme, stServiceDescriptor.service_name, stServiceDescriptor.service_name_length);
		}
		(*iInfoCount)++;
	}
}

/*********************************************************************************************************************************
 * Function Name : CleanSDT_Info
 * Description   :  clean SDT information
 * Parameters    :
 *				pstSDT_Info -- the SDT_INFO array
 *				piInfoCount -- the SDT_INFO count
 * Returns       :>0 - the count of the program -1 - failure 0 - no the section in the transport stream
 *********************************************************************************************************************************/
static void CleanSDT_Info(SDT_INFO *pstSDT_Info, int *piInfoCount)
{
	*piInfoCount = 0;
	memset(pstSDT_Info, 0, sizeof(TS_SDT_SERVICE) * SDT_EVENT_MAX);
}

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
int ParseSDT_Table(FILE *pfTsFile, int iTsPosition, int iTsLength, SDT_INFO *pstSDT_Info)
{
	int iTemp = 0;
	int iServiceCount = 0;
	int iInfoCount = 0;
	TS_SDT stTS_SDT = {0};
	unsigned int uiVersion = INITIAL_VERSION;
	unsigned int auiRecordSectionNumber[SECTION_COUNT_256] = {0};
	unsigned char aucSectionBuffer[SECTION_MAX_LENGTH_4096] = {0};

	if (-1 == fseek(pfTsFile, iTsPosition, SEEK_SET))
	{
		printf("parse table error\n");
		return -1;
	}
	while (!feof(pfTsFile))
	{
		iTemp = GetOneSection(pfTsFile, iTsLength, aucSectionBuffer, SDT_PID, SDT_TABLE_ID, &uiVersion);
		if (0 == iTemp) // version number change
		{
			uiVersion = INITIAL_VERSION;
			memset(auiRecordSectionNumber, 0, sizeof(char) * SECTION_COUNT_256); // clean the record array
			fseek(pfTsFile, 0 - iTsLength, SEEK_CUR);
			CleanSDT_Info(pstSDT_Info, &iInfoCount);
		}
		if (1 == iTemp) //one section over
		{
			if (0 == IsSectionGetBefore(aucSectionBuffer, auiRecordSectionNumber)) //judgment the section whether been parsed before
			{
				iServiceCount = ParseSDT_Section(&stTS_SDT, aucSectionBuffer);
				GetSDT_Info(&stTS_SDT, iServiceCount, pstSDT_Info, &iInfoCount);
			}
			if (1 == IsAllSectionOver(aucSectionBuffer, auiRecordSectionNumber))
			{
				return iInfoCount;
			}
		}
		if (-1 == iTemp) //error
		{
			printf("no SDT table in the transport stream \n ");
			return 0;
		}
	}
	return 0;
}
