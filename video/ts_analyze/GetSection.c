/********************************************************************************************************************************
 *									 All Rights GLOBALSAT Reserved													            *
 ********************************************************************************************************************************
 ********************************************************************************************************************************
 * Filename	   	: GetSection.c
 * Description  : using the file for get the section of the PID package which we want
 * Version	   	: 1.0
 * History      :
 * WuYunXing 	: 2013-3-21  Create
 *********************************************************************************************************************************/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "GetPidPackage.h"
#include "GetSection.h"

/*********************************************************************************************************************************
 * Function Name : GetTheLoadBeginPostion
 * Description   :  get the package load begin position
 * Parameters    :
 *				pucPackageBuffer -- the package buffer
 * Returns       : -1 - get error >=0 - success
 *********************************************************************************************************************************/
static int GetTheLoadBeginPostion(unsigned char *pucPackageBuffer)
{
	int iLoadBeginPosition = -1;
	TS_PACKAGE_HEAD stTS_PackageHead = {0};

	ParseTS_PackageHead(&stTS_PackageHead, pucPackageBuffer);
	switch (stTS_PackageHead.adaption_field_control)
	{
		case 0:
			return -1;
		case 1:
			iLoadBeginPosition = 4;
			break;
		case 2:
			return -1;
		case 3:
			iLoadBeginPosition = 5 + pucPackageBuffer[4];
			break;
	}
	if (stTS_PackageHead.payload_unit_start_indicator)
	{
		iLoadBeginPosition = iLoadBeginPosition + pucPackageBuffer[iLoadBeginPosition] + 1;
	}
	return iLoadBeginPosition;
}

/*********************************************************************************************************************************
 * Function Name : PutTheBufferToSection
 * Description   :  put the buffer to the section
 * Parameters    :
 *				pucSectionBuffer -- the section buffer where we put buffer
 *				pucPackageBuffer -- the package buffer
 *				piAlreadyAddSection -- the already add length before
 *				pstSectionHead -- the section head structure
 * Returns       : void
 *********************************************************************************************************************************/
static void PutTheBufferToSection(unsigned char *pucSectionBuffer, unsigned char *pucPackageBuffer, int *piAlreadyAddSection,
        SECTION_HEAD *pstSectionHead)
{
	int iLoadBeginPosition = 0;
	int iCopyLength = 0;

	iLoadBeginPosition = GetTheLoadBeginPostion(pucPackageBuffer);
	iCopyLength = 188 - iLoadBeginPosition;
	if ((int)(pstSectionHead->section_length + 3) < (188 - iLoadBeginPosition + (*piAlreadyAddSection)))
	{
		iCopyLength = pstSectionHead->section_length + 3 - (*piAlreadyAddSection);
	}
	memcpy(pucSectionBuffer + (*piAlreadyAddSection), pucPackageBuffer + iLoadBeginPosition, iCopyLength);
	(*piAlreadyAddSection) = (*piAlreadyAddSection) + iCopyLength;
}

/*********************************************************************************************************************************
 * Function Name :  ParseSectionHeader
 * Description   :  parse the section head
 * Parameters    :
 * 				pstSectionHead -- the section head
 *				pucPackageBuffer -- the package buffer
 * Returns       : void
 *********************************************************************************************************************************/
static void ParseSectionHeader(SECTION_HEAD *pstSectionHead, unsigned char *pucPackageBuffer)
{
	int iPayloadPosition = -1;

	iPayloadPosition = GetTheLoadBeginPostion(pucPackageBuffer);

	pstSectionHead->table_id = pucPackageBuffer[iPayloadPosition];
	pstSectionHead->section_syntax_indicator = pucPackageBuffer[1 + iPayloadPosition] >> 7;
	pstSectionHead->zero = (pucPackageBuffer[1 + iPayloadPosition] >> 6) & 0x1;
	pstSectionHead->reserved_1 = (pucPackageBuffer[1 + iPayloadPosition] >> 4) & 0x3;
	pstSectionHead->section_length = (pucPackageBuffer[1 + iPayloadPosition] & 0xf) << 8 | pucPackageBuffer[2 + iPayloadPosition];
	pstSectionHead->table_id_extension = (pucPackageBuffer[3 + iPayloadPosition] << 8) | pucPackageBuffer[4 + iPayloadPosition];
	pstSectionHead->reserved_2 = pucPackageBuffer[5 + iPayloadPosition] >> 6;
	pstSectionHead->version_number = pucPackageBuffer[5 + iPayloadPosition] >> 1 & 0x1F;
	pstSectionHead->current_next_indicator = (pucPackageBuffer[5 + iPayloadPosition] << 7) >> 7;
	pstSectionHead->section_number = pucPackageBuffer[6 + iPayloadPosition];
	pstSectionHead->last_section_number = pucPackageBuffer[7 + iPayloadPosition];

}

/*********************************************************************************************************************************
 * Function Name : IsVersionChange
 * Description   :  judgment whether the version number change (if version is 0xff we should record new version number)
 * Parameters    :
 *				stSectionHead -- the section head structure
 *				puiRecordVersion -- the previous version number
 * Returns       : 1 - version number change 0 - version number not change
 *********************************************************************************************************************************/
static int IsVersionChange(SECTION_HEAD *pstSectionHead, unsigned int *puiRecordVersion)
{
	/*----------------------------------------------------------*/
	/*if the version is 0xff then record new version */
	/*----------------------------------------------------------*/
	if (INITIAL_VERSION == (*puiRecordVersion))
	{
		(*puiRecordVersion) = pstSectionHead->version_number;
	}
	if ((*puiRecordVersion) != pstSectionHead->version_number)
	{
		return 1;
	}
	return 0;
}

/*********************************************************************************************************************************
 * Function Name : IsOneSectionOver
 * Description   :  is one section is over
 * Parameters    :
 * 				stSectionHead -- the section head structure
 * 				iAddLength -- the add length of the section
 * Returns       : 1 - one section over 0 - not over
 *********************************************************************************************************************************/
static int IsOneSectionOver(SECTION_HEAD stSectionHead, int iAddLength)
{
	int iSectionLength = (int)(stSectionHead.section_length + 3);

	if (iAddLength == iSectionLength)
	{
		return 1;
	}
	return 0;
}

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
        unsigned int *puiVersion)
{
	int iPayloadPosition = -1;
	int iFlagSectionStart = 0;
	int iAlreadyAddSection = 0;
	SECTION_HEAD stSectionHead = {0};
	TS_PACKAGE_HEAD stTS_PackageHead = {0};
	unsigned char aucPackageBuffer[PACKAGE_MAX_LENGTH_204] = {0};

	memset(pucSectionBuffer, 0, sizeof(char) * SECTION_MAX_LENGTH_4096); // initial the section buffer
	while (!feof(pfTsFile))
	{
		if ((sizeof(char) * iTsLength) != (fread((unsigned char *)aucPackageBuffer, sizeof(char), iTsLength, pfTsFile)))
		{
			break;
		}
		ParseTS_PackageHead(&stTS_PackageHead, aucPackageBuffer);
		if ((stTS_PackageHead.PID == uiPID) && (0x47 == stTS_PackageHead.sync_byte))
		{
			iPayloadPosition = GetTheLoadBeginPostion(aucPackageBuffer);
			if (-1 == iPayloadPosition) // no effective data in the package
			{
				continue;
			}
			if ((stTS_PackageHead.payload_unit_start_indicator == 1) && (aucPackageBuffer[iPayloadPosition] == uiTableId))
			{
				ParseSectionHeader(&stSectionHead, aucPackageBuffer);
				if (1 == IsVersionChange(&stSectionHead, puiVersion))
				{
					return 0; //version number change again get the table
				}
				iFlagSectionStart = 1;
				PutTheBufferToSection(pucSectionBuffer, aucPackageBuffer, &iAlreadyAddSection, &stSectionHead);
			}
			else
			{
				if (1 == iFlagSectionStart)
				{
					PutTheBufferToSection(pucSectionBuffer, aucPackageBuffer, &iAlreadyAddSection, &stSectionHead);
				}
			}
		}
		if (1 == iFlagSectionStart)
		{
			if (1 == IsOneSectionOver(stSectionHead, iAlreadyAddSection))
			{
				return 1;
			}
		}
	}
	return -1;
}

/*********************************************************************************************************************************
 * Function Name : IsSectionGetBefore
 * Description   :  is the section number has been get before
 * Parameters    :
 * 				pucSectionBuffer -- the section buffer
 * 				puiRecordSectionNumber -- record the section number and the section count
 * Returns       : 1 - the section already get  0 - do not get
 *********************************************************************************************************************************/
int IsSectionGetBefore(unsigned char *pucSectionBuffer, unsigned int *puiRecordSectionNumber)
{
	int iLoopTime = 1;
	int iLength = (int)puiRecordSectionNumber[0];
	unsigned int uiSectioNumber = pucSectionBuffer[6];

	/*----------------------------------------------------------*/
	/* the first position save the length of the array */
	/*----------------------------------------------------------*/
	for (iLoopTime = 1; iLoopTime < iLength + 1; iLoopTime++)
	{
		if (puiRecordSectionNumber[iLoopTime] == uiSectioNumber)
		{
			return 1;
		}
	}
	puiRecordSectionNumber[iLoopTime] = uiSectioNumber;
	puiRecordSectionNumber[0]++; //the count + 1
	return 0;
}

/*********************************************************************************************************************************
 * Function Name : IsAllSectionOver
 * Description   :  is all the section is over
 * Parameters    :
 *				pucSectionBuffer -- the section buffer
 *				puiRecordSectionNumber -- the buffer record the parse section number and the section count
 * Returns       : 1 - all section over 0 - do not over
 *********************************************************************************************************************************/
int IsAllSectionOver(unsigned char *pucSectionBuffer, unsigned int *puiRecordSectionNumber)
{
	unsigned int uiSectionCount = puiRecordSectionNumber[0];
	unsigned int uiLastSectioNumber = pucSectionBuffer[7];

	if (uiSectionCount == (uiLastSectioNumber + 1))
	{
		return 1;
	}
	return 0;
}

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
        unsigned int *puiVersion)
{
	int iPayloadPosition = -1;
	int iFlagSectionStart = 0;
	int iAlreadyAddSection = 0;
	SECTION_HEAD stSectionHead = {0};
	TS_PACKAGE_HEAD stTS_PackageHead = {0};
	unsigned char aucPackageBuffer[PACKAGE_MAX_LENGTH_204] = {0};

	memset(pucSectionBuffer, 0, sizeof(char) * SECTION_MAX_LENGTH_4096); // initial the section buffer
	while (!feof(pfTsFile))
	{
		if ((sizeof(char) * iTsLength) != (fread((unsigned char *)aucPackageBuffer, sizeof(char), iTsLength, pfTsFile)))
		{
			break;
		}
		ParseTS_PackageHead(&stTS_PackageHead, aucPackageBuffer);
		if ((stTS_PackageHead.PID == 0x0012) && (0x47 == stTS_PackageHead.sync_byte))
		{
			iPayloadPosition = GetTheLoadBeginPostion(aucPackageBuffer);
			if (-1 == iPayloadPosition) // no effective data in the package
			{
				continue;
			}
			if ((stTS_PackageHead.payload_unit_start_indicator == 1) && (aucPackageBuffer[iPayloadPosition] == uiTableId))
			{
				ParseSectionHeader(&stSectionHead, aucPackageBuffer);

				if (stSectionHead.table_id_extension != uiTableIdExtension) // point the program which we want to know the EIT information
				{
					continue;
				}

				if (1 == IsVersionChange(&stSectionHead, puiVersion))
				{
					return 0; //version number change ready for next table
				}
				iFlagSectionStart = 1;
				PutTheBufferToSection(pucSectionBuffer, aucPackageBuffer, &iAlreadyAddSection, &stSectionHead);
			}
			else
			{
				if (1 == iFlagSectionStart)
				{
					PutTheBufferToSection(pucSectionBuffer, aucPackageBuffer, &iAlreadyAddSection, &stSectionHead);
				}
			}
		}
		if (1 == iFlagSectionStart)
		{
			if (1 == IsOneSectionOver(stSectionHead, iAlreadyAddSection))
			{
				return 1;
			}
		}
	}
	return -1;
}

