/**********************************************************************************************************************************
 *										All Rights GLOBALSAT Reserved											                  *
 **********************************************************************************************************************************
 **********************************************************************************************************************************
 * Filename	   	: ParseTS_Length.c
 * Description  : using the file for get the package length
 * Version	   	: 1.0
 * History      :
 * WuYunXing   	: 2013-3-18  Create
 **********************************************************************************************************************************/
#include <stdio.h>
#include "ParseTS_Length.h"

#define PACKET_LENGTH_188	188
#define PACKET_LENGTH_204	204

/*********************************************************************************************************************************
 * Function Name : JudgmentPackage
 * Description   : judgment 10 package whether the package length is 188 or 204
 * Parameters    :
 *				pfTsFile -- the file descriptor
 *				iTsPosition -- the package begin position pay attention we define the first position is 0
 *              iTsLength -- the package length which we want to judgment
 * Returns       : -1 - judgment failure >0 - judgment success return the package length
 ********************************************************************************************************************************/
static int JudgmentPackageTenTimes(FILE *pfTsFile, int iTsPosition, int iTsLength)
{
	int iLoopTime = 0;
	int iFirstPackageByte = 0;

	if (-1 == fseek(pfTsFile, iTsPosition + 1, SEEK_SET))
	{
		return -1;
	}
	for (iLoopTime = 0; iLoopTime < 10; iLoopTime++)
	{
		if (-1 == fseek(pfTsFile, iTsLength - 1, SEEK_CUR))
		{
			return -1;
		}
		if (feof(pfTsFile))
		{
			return -1;
		}
		iFirstPackageByte = fgetc(pfTsFile);
		if (0x47 != iFirstPackageByte)
		{
			return -1;
		}
	}
	return iTsLength;
}

/************************************************************************************************************************************
 * Function Name : ParseTSPackageLength
 * Description   : parse the  package length
 * Parameters    :
 *				pfTsFile -- the file descriptor
 *				piTsPosition -- the package position(return the package begin position)
 * Returns       : -1 - the file is not the transport stream >0 - success and return the  package length
 ************************************************************************************************************************************/
int ParseTsLength(FILE *pfTsFile, int *piTsPosition)
{
	int iFirstPackageByte = 0;

	while (!feof(pfTsFile))
	{
		iFirstPackageByte = fgetc(pfTsFile);
		if (0x47 == iFirstPackageByte)
		{
			if (PACKET_LENGTH_188 == JudgmentPackageTenTimes(pfTsFile, *piTsPosition, PACKET_LENGTH_188))
			{
				return PACKET_LENGTH_188;
			}
			if (PACKET_LENGTH_204 == JudgmentPackageTenTimes(pfTsFile, *piTsPosition, PACKET_LENGTH_204))
			{
				return PACKET_LENGTH_204;
			}
		}
		/*----------------------------------------------------------*/
		/* jump to the iTS_Position position for the next judgment*/
		/*----------------------------------------------------------*/
		(*piTsPosition)++;
		if (-1 == fseek(pfTsFile, *piTsPosition, SEEK_SET))
		{
			printf("the file error\n");
			return -1;
		}
	}
	printf("the file is not the transport stream\n");
	return -1;

}
