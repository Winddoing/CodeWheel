/********************************************************************************************************************************
 *									 All Rights GLOBALSAT Reserved													            *
 ********************************************************************************************************************************
 ********************************************************************************************************************************
 * Filename	   	: MainTest.c
 * Description  : the main function of the parse TS
 * Version	   	: 1.0
 * History      :
 * WuYunXing 	: 2013-3-20  Create
 *********************************************************************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ParseTS_Length.h"
#include "ParsePAT.h"
#include "ParsePMT.h"
#include "ParseSDT.h"
#include "ParseEIT.h"
#include "ParseNIT.h"
#include "Program.h"

/*********************************************************************************************************************************
 * Function Name : ParseAllProgramPMT
 * Description   :  parse all program PMT table
 * Parameters    :
 *				pfTsFile -- the TS file descriptor
 *				iTsPosition -- the TS begin position
 *				iTsLength -- the TS length
 *				pstPAT_Info -- the PAT_INFO contain the PMT_PID
 *				iProgramCount -- the program count
 *				pstPMT_Info -- the PMT_INFO array all program PMT information put in it
 * Returns       : -1 - failure  0 - success
 *********************************************************************************************************************************/
static int ParseAllProgramPMT(FILE *pfTsFile, int iTsPosition, int iTsLength, PAT_INFO *pstPAT_Info, int iProgramCount,
        PMT_INFO *pstPMT_Info)
{
	int iProgramIndex = 0;
	unsigned int uiPMT_PID = 0;
	PMT_INFO stOnePMT_Info = {0};

	for (iProgramIndex = 0; iProgramIndex < iProgramCount; iProgramIndex++)
	{
		uiPMT_PID = pstPAT_Info[iProgramIndex].uiPMT_PID;
		if (-1 == ParsePMT_Table(pfTsFile, iTsPosition, iTsLength, uiPMT_PID, &stOnePMT_Info))
		{
			printf("get information form PMT error\n");
			return -1;
		}
		pstPMT_Info[iProgramIndex] = stOnePMT_Info;
	}
	return 0;
}

/*********************************************************************************************************************************
 * Function Name : ParseAllEIT
 * Description   :  parse EIT table and print the EIT information (table id is 0x4e 0x50)
 * Parameters    :
 *				pfTsFile -- the TS file descriptor
 *				iTsPosition -- the TS begin position
 *				iTsLength -- the TS length
 *				uiTableIdExtension -- the table_id_extension (the program id which we want to know the EPG information)
 * Returns       : -1 - failure 0 - success
 *********************************************************************************************************************************/
static int ParseAllEIT(FILE *pfTsFile, int iTsPosition, int iTsLength, unsigned int uiTableIdExtension)
{
	int iTemp = 0;
	unsigned int uiEIT_PF_TableId = EIT_PF_TABLE_ID; //0x4e
	unsigned int uiEIT_SE_TableId = EIT_SE_TABLE_ID; //0x50

	printf("show present/following EPG event information(0x4e) \n");
	iTemp = ParseEIT_Table(pfTsFile, iTsPosition, iTsLength, uiEIT_PF_TableId, uiTableIdExtension);
	if (-1 == iTemp) //EIT
	{
		printf("get the information form EIT error\n");
		return -1;
	}
	if (0 == iTemp)
	{
		printf("the %d program no EIT(0x4e) table\n", uiTableIdExtension);
	}

	printf("show schedule EPG event information(0x50) enter any key for continue \n");
	system("pause");
	iTemp = ParseEIT_Table(pfTsFile, iTsPosition, iTsLength, uiEIT_SE_TableId, uiTableIdExtension);
	if (-1 == iTemp) //EIT
	{
		printf("get the information form EIT error\n");
		return -1;
	}

	return 0;
}

/*********************************************************************************************************************************
 * Function Name : ParseTransportStream
 * Description   :  parse the transport stream,print the program information and save the audio and video
 * Parameters    :
 *				pfTsFile -- the TS file descriptor
 * Returns       : -1 - failure  >0 - success
 *********************************************************************************************************************************/
static int ParseTransportStream(FILE *pfTsFile)
{
	int iTsLength = 0;
	int iTsPosition = 0;
	int iProgramCount = 0;
	unsigned int uiInServiceId = 0xffff;
	PROGRAM_INFO astProgramInfo[PROGRAM_MAX] = {0};
	PAT_INFO astPAT_Info[PROGRAM_MAX] = {0};
	PMT_INFO astPMT_Info[PROGRAM_MAX] = {0};
	SDT_INFO astSDT_Info[PROGRAM_MAX] = {0};

	iTsLength = ParseTsLength(pfTsFile, &iTsPosition);
	if (-1 == iTsLength)
	{
		printf("the file is not the transport stream file\n");
		return -1;
	}
	printf("the position is %d\n", iTsPosition);
	printf("the package length is %d\n", iTsLength);

	iProgramCount = ParsePAT_Table(pfTsFile, iTsPosition, iTsLength, astPAT_Info); //PAT
	if (iProgramCount <= 0) //no PAT table in the transport stream no use do the next
	{
		return -1;
	}
	printf("PAT_INFO uiProgramNumber=%#x, uiPMT_PID=%#x\n", astPAT_Info[0].uiProgramNumber, astPAT_Info[0].uiPMT_PID);
	if (-1 == ParseSDT_Table(pfTsFile, iTsPosition, iTsLength, astSDT_Info)) //SDT
	{
		return -1;
	}
	if (-1 == ParseAllProgramPMT(pfTsFile, iTsPosition, iTsLength, astPAT_Info, iProgramCount, astPMT_Info)) //PMT
	{
		return -1;
	}

	CombinationAllInfo(astProgramInfo, iProgramCount, astPAT_Info, astPMT_Info, astSDT_Info);

	PrintAllProgramInfo(astProgramInfo, iProgramCount);

	while (0 != uiInServiceId)
	{
		printf("Enter service_id to save file and get the EPG information(0 exit) :");
		scanf("%d", &uiInServiceId);
		if (0 == uiInServiceId)
		{
			break;
		}
		if (-1 == SaveVideoAndAudio(astProgramInfo, iProgramCount, pfTsFile, iTsPosition, iTsLength, uiInServiceId)) //save video and audio
		{
			return -1;
		}

		if (-1 == ParseAllEIT(pfTsFile, iTsPosition, iTsLength, uiInServiceId)) //EIT
		{
			return -1;
		}
	}
	printf("state to parse the NIT table enter any key for continue \n");
	system("pause");
	if (-1 == ParseNIT_Table(pfTsFile, iTsPosition, iTsLength)) //NIT
	{
		return -1;
	}
	return 0;
}

/*********************************************************************************************************************************
 * Function Name : main
 * Description   :  the main function
 * Parameters    : void
 * Returns       : -1 - failure >0 - success
 *********************************************************************************************************************************/
int main(void)
{
	int iReturn = 0;
	FILE *pfTsFile = NULL;
	pfTsFile = fopen("188.ts", "rb");

	if (NULL == pfTsFile)
	{
		printf("the file does not exist \n");
		return 0;
	}
	iReturn = ParseTransportStream(pfTsFile);
	fclose(pfTsFile);
	return iReturn;
}
