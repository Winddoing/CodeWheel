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

#ifndef _PROGRAM_H_
#define _PROGRAM_H_
#define NAME_LENGTH_MAX 512
#define PROGRAM_MAX 128
#define AUDIION_MAX 20

typedef struct PROGRAM_INFO
{
	unsigned int uiProgramNumber :16;
	unsigned int uiPMT_PID :13;
	unsigned int uiVideoPID :13;
	unsigned int auiAudioPID[AUDIION_MAX];
	unsigned int uiFreeCA_Mode :1;
	unsigned int uiEIT_ScheduleFlag :1;
	unsigned int uiEIT_PresentFollowingFlag :1;
	unsigned char aucProgramNanme[NAME_LENGTH_MAX];
} PROGRAM_INFO;

/*********************************************************************************************************************************
 * Function Name : CombinationAllInfo
 * Description   :  combination all information
 * Parameters    :
 *				pstProgramInfo -- the program array
 *				iProgramCount -- the program count
 *				pstPAT_Info -- the PAT_INFO array
 *				pstPMT_Info -- the PMT_INFO array
 *				pstSDT_Info -- the SDT_INFO array
 * Returns       : void
 *********************************************************************************************************************************/
void CombinationAllInfo(PROGRAM_INFO *pstProgramInfo, int iProgramCount, PAT_INFO *pstPAT_Info, PMT_INFO *pstPMT_Info,
        SDT_INFO *pstSDT_Info);

/*********************************************************************************************************************************
 * Function Name : PrintAllProgramInfo
 * Description   :  out put all of the program information
 * Parameters    :
 *				pstProgramInfo -- the program information array
 * Returns       : void
 *********************************************************************************************************************************/
void PrintAllProgramInfo(PROGRAM_INFO *pstProgramInfo, int iProgramCount);

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
        unsigned int uiInServiceId);

/*********************************************************************************************************************************
 * Function Name : PrintEIT_ExistState
 * Description   :  print the exist state of the EIT table(1 exist 0 not exist)
 * Parameters    :
 *				pstProgramInfo -- the program information array
 *				iProgramCount -- program count
 * Returns       : void
 *********************************************************************************************************************************/
void PrintEIT_ExistState(PROGRAM_INFO *pstProgramInfo, int iProgramCount);

#endif /* PROGRAM_H_ */
