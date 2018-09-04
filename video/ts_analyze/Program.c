/********************************************************************************************************************************
 *									 All Rights GLOBALSAT Reserved													            *
 ********************************************************************************************************************************
 ********************************************************************************************************************************
 * Filename	   	: Program.c
 * Description  : some actions for program information
 * Version	   	: 1.0
 * History      :
 * WuYunXing 	: 2013-4-2  Create
 *********************************************************************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "GetPidPackage.h"
#include "ParsePAT.h"
#include "ParsePMT.h"
#include "ParseSDT.h"
#include "Program.h"

#define SAVE_PID_COUNT_MAX 20

/*********************************************************************************************************************************
 * Function Name : CombinationPAT_Info
 * Description   :  combination PAT information
 * Parameters    :
 *				pstProgramInfo -- the program array
 *				iProgramCount -- the program count
 *				pstPAT_Info -- the PAT_INFO array
 * Returns       : void
 *********************************************************************************************************************************/
static void CombinationPAT_Info(PROGRAM_INFO *pstProgramInfo, int iProgramCount, PAT_INFO *pstPAT_Info)
{
	int iLoopTime = 0;

	for (iLoopTime = 0; iLoopTime < iProgramCount; iLoopTime++)
	{
		pstProgramInfo[iLoopTime].uiProgramNumber = pstPAT_Info[iLoopTime].uiProgramNumber;
		pstProgramInfo[iLoopTime].uiPMT_PID = pstPAT_Info[iLoopTime].uiPMT_PID;
	}
}

/*********************************************************************************************************************************
 * Function Name : CombinationPMT_Info
 * Description   :  combination PMT information
 * Parameters    :
 *				pstProgramInfo -- the program array
 *				iProgramCount -- the program count
 *				pstPMT_Info -- the PMT_INFO array
 * Returns       : void
 *********************************************************************************************************************************/
static void CombinationPMT_Info(PROGRAM_INFO *pstProgramInfo, int iProgramCount, PMT_INFO *pstPMT_Info)
{
	int iLoopTime = 0;
	int iStreamIndex = 0;

	for (iLoopTime = 0; iLoopTime < iProgramCount; iLoopTime++)
	{
		for (iStreamIndex = 0; iStreamIndex < iProgramCount; iStreamIndex++)
		{
			if (pstProgramInfo[iLoopTime].uiProgramNumber == pstPMT_Info[iStreamIndex].uiProgramNumber)
			{
				pstProgramInfo[iLoopTime].uiVideoPID = pstPMT_Info[iStreamIndex].uiVideoPID;
				memcpy(pstProgramInfo[iLoopTime].auiAudioPID, pstPMT_Info[iStreamIndex].auiAudioPID, AUDIION_MAX * sizeof(unsigned int));
			}
		}
	}
}

/*********************************************************************************************************************************
 * Function Name : CombinationSDT_Info
 * Description   :  combination SDT information
 * Parameters    :
 *				pstProgramInfo -- the program information array
 *				iProgramCount -- the program count
 *				pstSDT_Info -- the SDT_INFO array
 * Returns       : void
 *********************************************************************************************************************************/
static void CombinationSDT_Info(PROGRAM_INFO *pstProgramInfo, int iProgramCount, SDT_INFO *pstSDT_Info)
{
	int iLoopTime = 0;
	int iServiceIndex = 0;

	for (iLoopTime = 0; iLoopTime < iProgramCount; iLoopTime++)
	{
		for (iServiceIndex = 0; iServiceIndex < iProgramCount; iServiceIndex++)
		{
			if (pstProgramInfo[iLoopTime].uiProgramNumber == pstSDT_Info[iServiceIndex].uiServiceId)
			{
				pstProgramInfo[iLoopTime].uiFreeCA_Mode = pstSDT_Info[iServiceIndex].uiFreeCA_Mode;
				pstProgramInfo[iLoopTime].uiEIT_PresentFollowingFlag = pstSDT_Info[iServiceIndex].uiEIT_PresentFollowingFlag;
				pstProgramInfo[iLoopTime].uiEIT_ScheduleFlag = pstSDT_Info[iServiceIndex].uiEIT_ScheduleFlag;
				memcpy(pstProgramInfo[iLoopTime].aucProgramNanme, pstSDT_Info[iServiceIndex].aucProgramNanme,
				        NAME_LENGTH_MAX * sizeof(unsigned char));
			}
		}
	}
}

/*********************************************************************************************************************************
 * Function Name : CombinationAllInfo
 * Description   :  combination all information
 * Parameters    :
 *				pstProgramInfo -- the program information array
 *				iProgramCount -- the program count
 *				pstPAT_Info -- the PAT_INFO array
 *				pstPMT_Info -- the PMT_INFO array
 *				pstSDT_Info -- the SDT_INFO array
 * Returns       : void
 *********************************************************************************************************************************/
void CombinationAllInfo(PROGRAM_INFO *pstProgramInfo, int iProgramCount, PAT_INFO *pstPAT_Info, PMT_INFO *pstPMT_Info,
        SDT_INFO *pstSDT_Info)
{
	CombinationPAT_Info(pstProgramInfo, iProgramCount, pstPAT_Info);
	CombinationPMT_Info(pstProgramInfo, iProgramCount, pstPMT_Info);
	CombinationSDT_Info(pstProgramInfo, iProgramCount, pstSDT_Info);
}

/*********************************************************************************************************************************
 * Function Name : PrintAllProgramInfo
 * Description   :  out put all of the program information
 * Parameters    :
 *				pstProgramInfo -- the program information array
 * Returns       : void
 *********************************************************************************************************************************/
void PrintAllProgramInfo(PROGRAM_INFO *pstProgramInfo, int iProgramCount)
{
	int iProgramIndex = 0;

	printf("out put all the program information \n");
	printf("ProgramId PMT_PID   FreeCA  ProgramName           VideoPID  AudioPID1 AudioPID2\n");
	for (iProgramIndex = 0; iProgramIndex < iProgramCount; iProgramIndex++)
	{
		printf("%5d  ", pstProgramInfo[iProgramIndex].uiProgramNumber);
		printf("   0x%5x", pstProgramInfo[iProgramIndex].uiPMT_PID);
		if (0 == pstProgramInfo[iProgramIndex].uiFreeCA_Mode)
		{
			printf("     yes");
		}
		else
		{
			printf("      no");
		}
		printf("    %-25s", pstProgramInfo[iProgramIndex].aucProgramNanme);
		printf("%5d", pstProgramInfo[iProgramIndex].uiVideoPID);
		printf("    %5d", pstProgramInfo[iProgramIndex].auiAudioPID[0]);
		printf("    %5d\n", pstProgramInfo[iProgramIndex].auiAudioPID[1]);
	}
}

/*********************************************************************************************************************************
 * Function Name : GetAllSavePID
 * Description   :  get the save PID and put in array (auiSavePid)
 * Parameters    :
 *				pstProgramInfo -- the program information array
 *				iProgramCount -- program count
 *				uiServiceId -- the service id we should save
 *				puiSavePid -- save the want PID
 * Returns       : void
 *********************************************************************************************************************************/
static void GetAllSavePID(PROGRAM_INFO *pstProgramInfo, unsigned int *puiSavePid)
{
	int iPidCount = 0;
	int iAudioIndex = 0;
	/*----------------------------------------------------------*/
	/* get the video PID */
	/*----------------------------------------------------------*/
	if (0 != pstProgramInfo->uiVideoPID)
	{
		puiSavePid[++iPidCount] = pstProgramInfo->uiVideoPID;
	}

	/*----------------------------------------------------------*/
	/* get the audio PID */
	/*----------------------------------------------------------*/
	for (iAudioIndex = 0; iAudioIndex < AUDIION_MAX; iAudioIndex++)
	{
		if (0 != pstProgramInfo->auiAudioPID[iAudioIndex])
		{
			puiSavePid[++iPidCount] = pstProgramInfo->auiAudioPID[iAudioIndex];
		}
	}
	puiSavePid[0] = iPidCount;
}

/*********************************************************************************************************************************
 * Function Name : GetProgram
 * Description   :  get the program
 * Parameters    :
 *				pstProgramInfo -- the program information array
 *				iProgramCount -- program count
 *				uiServiceId -- the service id we should save
 * Returns       : the get program
 *********************************************************************************************************************************/
static PROGRAM_INFO GetProgram(PROGRAM_INFO *pstProgramInfo, int iProgramCount, unsigned int uiServiceId)
{
	int iProgramIndex = 0;
	PROGRAM_INFO stProgramInfoTemp = {0};

	for (iProgramIndex = 0; iProgramIndex < iProgramCount; iProgramIndex++)
	{
		if (pstProgramInfo[iProgramIndex].uiProgramNumber == uiServiceId)
		{
			return pstProgramInfo[iProgramIndex];
		}
	}
	return stProgramInfoTemp;
}

/*********************************************************************************************************************************
 * Function Name : NormalFileName
 * Description   :  normal the file name (the file name can't exist / \ : * ? " < > | )
 * Parameters    :
 *				pucFileName -- the file name
 * Returns       : void
 *********************************************************************************************************************************/
static void NormalFileName(unsigned char *pucFileName)
{
	unsigned char *pucTemp = pucFileName;

	/*----------------------------------------------------------*/
	/* the file name can't exist / \ : * ? " < > | */
	/*----------------------------------------------------------*/
	while ((*pucTemp) != '\0')
	{
		if (('/' == (*pucTemp)) || ('\\' == (*pucTemp)) || (':' == (*pucTemp)) || ('*' == (*pucTemp)) || ('?' == (*pucTemp))
		        || ('"' == (*pucTemp)) || ('<' == (*pucTemp)) || ('>' == (*pucTemp)) || ('|' == (*pucTemp)))
		{
			(*pucTemp) = ' ';
		}
		pucTemp++;
	}
}

/*********************************************************************************************************************************
 * Function Name : SaveVideoAndAudio
 * Description   :  save the video and audio of the program which we want
 * Parameters    :
 *				pstProgramInfo -- the program information array
 *				iProgramCount -- program count
 *				pfTsFile -- the TS file descriptor
 *				iTsPosition -- the TS package begin position
 *				iTsLength -- the TS package length
 *				uiInServiceId -- the service id we want to save
 * Returns       : -1 - failure 0 - success
 *********************************************************************************************************************************/
int SaveVideoAndAudio(PROGRAM_INFO *pstProgramInfo, int iProgramCount, FILE *pfTsFile, int iTsPosition, int iTsLength,
        unsigned int uiInServiceId)
{
	int iReturn = 0;
	FILE *pfSaveFile = NULL;
	unsigned int auiSavePid[SAVE_PID_COUNT_MAX] = {0};
	unsigned char aucSaveFileName[NAME_LENGTH_MAX] = {0};
	PROGRAM_INFO stSaveProgramInfo = {0};

	stSaveProgramInfo = GetProgram(pstProgramInfo, iProgramCount, uiInServiceId);
	if (stSaveProgramInfo.uiProgramNumber == uiInServiceId)
	{
		GetAllSavePID(&stSaveProgramInfo, auiSavePid);
		if (0 != auiSavePid[0]) //exist video or audio(auiSavePid[0] represent the count of PID)
		{
			sprintf(aucSaveFileName, "%s.ts", stSaveProgramInfo.aucProgramNanme);
			NormalFileName(aucSaveFileName);
			pfSaveFile = fopen(aucSaveFileName, "wb");

			if (NULL == pfSaveFile)
			{
				printf("open the save file error\n");
				return -1;
			}
			if (-1 == SavePID_Packege(pfTsFile, iTsPosition, iTsLength, auiSavePid, pfSaveFile))
			{
				iReturn = -1;
			}
			fclose(pfSaveFile);
			printf("\n save success and the save file name is    %s\n\n", aucSaveFileName);
		}
		else
		{
			printf("\n the %d program not exist video or audio input again \n\n", stSaveProgramInfo.uiProgramNumber);
		}
	}
	else
	{
		printf("no the program \n");
	}
	return iReturn;
}

/*********************************************************************************************************************************
 * Function Name : PrintEIT_ExistState
 * Description   :  print the exist state of the EIT table(1 exist 0 not exist)
 * Parameters    :
 *				pstProgramInfo -- the program information array
 *				iProgramCount -- program count
 * Returns       : void
 *********************************************************************************************************************************/
void PrintEIT_ExistState(PROGRAM_INFO *pstProgramInfo, int iProgramCount)
{
	int iProgramIndex = 0;

	printf("the state of the EIT table (1:exist 0:not exist)\n ");
	printf("Service_id EIT(0x4e) EIT(0x50)\n");
	for (iProgramIndex = 0; iProgramIndex < iProgramCount; iProgramIndex++)
	{
		printf("%5d", pstProgramInfo[iProgramIndex].uiProgramNumber);
		printf("         %d", pstProgramInfo[iProgramIndex].uiEIT_PresentFollowingFlag);
		printf("         %d\n", pstProgramInfo[iProgramIndex].uiEIT_ScheduleFlag);
	}
}
