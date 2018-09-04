/********************************************************************************************************************************
 *									 All Rights GLOBALSAT Reserved													            *
 ********************************************************************************************************************************
 ********************************************************************************************************************************
 * Filename	   	: ParsePAT.c
 * Description  : parse the PAT
 * Version	   	: 1.0
 * History      :
 * WuYunXing 	: 2013-3-22  Create
 *********************************************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "GetSection.h"
#include "ParsePAT.h"

#define PAT_PID 0x0000
#define PAT_TABLE_ID 0x00

/*********************************************************************************************************************************
 * Function Name : ParsePAT_SectionHead
 * Description   :  parse the PAT section head
 * Parameters    :
 * 				pstTS_PAT -- the TS_PAT
 *				pucSectionBuffer -- the PAT section buffer
 * Returns       : void
 *********************************************************************************************************************************/
static void ParsePAT_SectionHead(TS_PAT *pstTS_PAT, unsigned char *pucSectionBuffer)
{
	int iPAT_Length = 0;

	pstTS_PAT->table_id = pucSectionBuffer[0];
	pstTS_PAT->section_syntax_indicator = pucSectionBuffer[1] >> 7;
	pstTS_PAT->zero = (pucSectionBuffer[1] >> 6) & 0x1;
	pstTS_PAT->reserved_1 = (pucSectionBuffer[1] >> 4) & 0x3;
	pstTS_PAT->section_length = ((pucSectionBuffer[1] & 0x0F) << 8) | pucSectionBuffer[2];
	pstTS_PAT->transport_stream_id = (pucSectionBuffer[3] << 8) | pucSectionBuffer[4];
	pstTS_PAT->reserved_2 = pucSectionBuffer[5] >> 6;
	pstTS_PAT->version_number = (pucSectionBuffer[5] >> 1) & 0x1F;
	pstTS_PAT->current_next_indicator = (pucSectionBuffer[5] << 7) >> 7;
	pstTS_PAT->section_number = pucSectionBuffer[6];
	pstTS_PAT->last_section_number = pucSectionBuffer[7];
	iPAT_Length = 3 + pstTS_PAT->section_length;
	pstTS_PAT->CRC_32 = (pucSectionBuffer[iPAT_Length - 4] << 24) | (pucSectionBuffer[iPAT_Length - 3] << 16)
	        | (pucSectionBuffer[iPAT_Length - 2] << 8) | pucSectionBuffer[iPAT_Length - 1];
}

/*********************************************************************************************************************************
 * Function Name : ParsePAT_Section
 * Description   :  parse the PAT section
 * Parameters    :
 * 				pstTS_PAT -- the TS_PAT
 *				pucSectionBuffer -- the PAT section buffer
 * Returns       : the PAT_PROGRAM count
 *********************************************************************************************************************************/
static int ParsePAT_Section(TS_PAT *pstTS_PAT, unsigned char *pucSectionBuffer)
{
	int iPAT_Length = 0;
	int iPatProgramPosition = 8;
	int iPAT_ProgramCount = 0;

	memset(pstTS_PAT, 0, sizeof(TS_PAT));
	ParsePAT_SectionHead(pstTS_PAT, pucSectionBuffer);
	iPAT_Length = 3 + pstTS_PAT->section_length;
	for (iPatProgramPosition = 8; iPatProgramPosition < iPAT_Length - 4; iPatProgramPosition += 4)
	{
		if (0x00 == ((pucSectionBuffer[iPatProgramPosition] << 8) | pucSectionBuffer[1 + iPatProgramPosition]))
		{
			pstTS_PAT->network_PID = ((pucSectionBuffer[2 + iPatProgramPosition] & 0x1F) << 8) | pucSectionBuffer[3 + iPatProgramPosition];
			printf("the network_PID %0x\n", pstTS_PAT->network_PID);
		}
		else
		{
			pstTS_PAT->astPAT_Program[iPAT_ProgramCount].program_number = (pucSectionBuffer[iPatProgramPosition] << 8)
			        | pucSectionBuffer[1 + iPatProgramPosition];
			pstTS_PAT->astPAT_Program[iPAT_ProgramCount].reserved_3 = pucSectionBuffer[2 + iPatProgramPosition] >> 5;
			pstTS_PAT->astPAT_Program[iPAT_ProgramCount].program_map_PID = ((pucSectionBuffer[2 + iPatProgramPosition] & 0x1F) << 8)
			        | pucSectionBuffer[3 + iPatProgramPosition];
			iPAT_ProgramCount++;
		}
	}
	return iPAT_ProgramCount;
}

/*********************************************************************************************************************************
 * Function Name : GetPAT_Info
 * Description   :  get the PAT information (program_number and PMT_PID)
 * Parameters    :
 * 				pstTS_PAT -- the TS_PAT
 * 				iPAT_ProgramCount -- PAT_PROGRAM count
 *				pstPAT_Info -- the PAT_INFO array
 *				iInfoCount -- the PAT_INFO count
 * Returns       : void
 *********************************************************************************************************************************/
static void GetPAT_Info(TS_PAT *pstTS_PAT, int iPAT_ProgramCount, PAT_INFO *pstPAT_Info, int *iInfoCount)
{
	int iLoopTime = 0;

	for (iLoopTime = 0; iLoopTime < iPAT_ProgramCount; iLoopTime++)
	{
		pstPAT_Info[*iInfoCount].uiPMT_PID = pstTS_PAT->astPAT_Program[iLoopTime].program_map_PID;
		pstPAT_Info[*iInfoCount].uiProgramNumber = pstTS_PAT->astPAT_Program[iLoopTime].program_number;
		(*iInfoCount)++;
	}
}

/*********************************************************************************************************************************
 * Function Name : CleanPAT_Info
 * Description   :  clean PAT information
 * Parameters    :
 *				pstPAT_Info -- the PAT_INFO array
 *				piInfoCount -- the PAT_INFO count
 * Returns       : void
 *********************************************************************************************************************************/
static void CleanPAT_Info(PAT_INFO *pstPAT_Info, int *piInfoCount)
{
	*piInfoCount = 0;
	memset(pstPAT_Info, 0, sizeof(TS_PAT_PROGRAM) * PAT_PROGRAM_MAX);
}

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
int ParsePAT_Table(FILE *pfTsFile, int iTsPosition, int iTsLength, PAT_INFO *pstPAT_Info)
{
	int iTemp = 0;
	int iInfoCount = 0;
	int iPatProgramCount = 0;
	TS_PAT stTS_PAT = {0};
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
		iTemp = GetOneSection(pfTsFile, iTsLength, aucSectionBuffer, PAT_PID, PAT_TABLE_ID, &uiVersion);
		if (0 == iTemp) // version number change
		{
			uiVersion = INITIAL_VERSION;
			memset(auiRecordSectionNumber, 0, sizeof(char) * SECTION_COUNT_256); // clean the record array
			fseek(pfTsFile, 0 - iTsLength, SEEK_CUR);
			CleanPAT_Info(pstPAT_Info, &iInfoCount);
		}
		if (1 == iTemp) //one section over
		{
			if (0 == IsSectionGetBefore(aucSectionBuffer, auiRecordSectionNumber))
			{
				iPatProgramCount = ParsePAT_Section(&stTS_PAT, aucSectionBuffer);
				GetPAT_Info(&stTS_PAT, iPatProgramCount, pstPAT_Info, &iInfoCount);
			}
			if (1 == IsAllSectionOver(aucSectionBuffer, auiRecordSectionNumber))
			{
				return iInfoCount;
			}
		}
		if (-1 == iTemp) // no section
		{
			return 0;
		}
	}
	return 0;
}

