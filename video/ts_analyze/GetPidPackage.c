/********************************************************************************************************************************
 *									 All Rights GLOBALSAT Reserved													            *
 ********************************************************************************************************************************
 ********************************************************************************************************************************
 * Filename	   	: GetPidPackage.c
 * Description  : using the file for get package which the PID package we want
 * Version	   	: 1.0
 * History      :
 * WuYunXing 	: 2013-3-20  Create
 *********************************************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "GetPidPackage.h"
#define PACKAGE_MAX_LENGTH 204

/*********************************************************************************************************************************
 * Function Name : ParseTS_PackageHead
 * Description   : parse the TS package head
 * Parameters    :
 *				pstTS_PackageHead -- the package head
 *				pucBufferPackage -- the package buffer
 * Returns       : void
 *********************************************************************************************************************************/
void ParseTS_PackageHead(TS_PACKAGE_HEAD *pstTS_PackageHead, unsigned char *pucPackageBuffer)
{
	pstTS_PackageHead->sync_byte = pucPackageBuffer[0];
	pstTS_PackageHead->transport_error_indicator = pucPackageBuffer[1] >> 7;
	pstTS_PackageHead->payload_unit_start_indicator = (pucPackageBuffer[1] >> 6) & 0x01;
	pstTS_PackageHead->transport_priority = (pucPackageBuffer[1] >> 5) & 0x01;
	pstTS_PackageHead->PID = ((pucPackageBuffer[1] & 0x1F) << 8) | pucPackageBuffer[2];
	pstTS_PackageHead->transport_scrambling_control = pucPackageBuffer[3] >> 6;
	pstTS_PackageHead->adaption_field_control = (pucPackageBuffer[3] >> 4) & 0x03;
	pstTS_PackageHead->continuity_counter = pucPackageBuffer[3] & 0x0f;
}

/*********************************************************************************************************************************
 * Function Name : IsWantPid
 * Description   : is we want PID (the first position of the array save the PID count )
 * Parameters    :
 *				puiWantPID -- save the want PID
 *				uiPID -- PID
 * Returns       : 0 - not the want PID 1 - yes the want PID
 *********************************************************************************************************************************/
static int IsWantPid(unsigned int *puiWantPID, unsigned int uiPID)
{
	int iLoopTime = 0;
	int iNumber = puiWantPID[0];

	for (iLoopTime = 1; iLoopTime <= iNumber; iLoopTime++)
	{
		if (puiWantPID[iLoopTime] == uiPID)
		{
			return 1;
		}
	}
	return 0;
}

/*********************************************************************************************************************************
 * Function Name : SavePID_Packege
 * Description   : get all the PID package which we want and save they in the file
 * Parameters    :
 *				pfTsFile -- the TS file descriptor
 *				iTsPosition -- the TS package begin position
 *				iTsLength -- the TS package length
 *				puiWantPID -- the package PID which we want
 *				pfSaveFile -- the save file descriptor
 * Returns       : -1 - get package failure 0 - get package save success
 *********************************************************************************************************************************/
int SavePID_Packege(FILE *pfTsFile, int iTsPosition, int iTsLength, unsigned int *puiWantPID, FILE *pfSaveFile)
{
	unsigned char aucPackageBuffer[PACKAGE_MAX_LENGTH] = {0};
	TS_PACKAGE_HEAD stTS_PackageHead = {0};

	if (-1 == fseek(pfTsFile, iTsPosition, SEEK_SET))
	{
		return -1;
	}
	while (!feof(pfTsFile))
	{
		if ((sizeof(char) * iTsLength) != fread((unsigned char *)aucPackageBuffer, sizeof(char), iTsLength, pfTsFile))
		{
			return 0;
		}
		ParseTS_PackageHead(&stTS_PackageHead, aucPackageBuffer);
		if (0x47 != stTS_PackageHead.sync_byte)
		{
			continue;
		}
		if (1 == IsWantPid(puiWantPID, stTS_PackageHead.PID))
		{
			if ((sizeof(char) * iTsLength) != fwrite((unsigned char *)aucPackageBuffer, sizeof(char), iTsLength, pfSaveFile))
			{
				return -1;
			}
		}
	}
	return 0;
}

