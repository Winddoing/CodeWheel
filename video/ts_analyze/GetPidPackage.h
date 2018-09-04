/********************************************************************************************************************************
 *									 All Rights GLOBALSAT Reserved													            *
 ********************************************************************************************************************************
 ********************************************************************************************************************************
 * Filename	   	: GetPidPackage.h
 * Description  : get all the PID package which we want and put it in the file
 * Version	   	: 1.0
 * History      :
 * Bamboo 		: 2013-3-20  Create
 *********************************************************************************************************************************/

#ifndef GETPIDPACKAGE_H_
#define GETPIDPACKAGE_H_

typedef struct
{
	unsigned int sync_byte :8; //define 0x47
	unsigned int transport_error_indicator :1;
	unsigned int payload_unit_start_indicator :1;
	unsigned int transport_priority :1;
	unsigned int PID :13;
	unsigned int transport_scrambling_control :2;
	unsigned int adaption_field_control :2;
	unsigned int continuity_counter :4;
} TS_PACKAGE_HEAD;

/*********************************************************************************************************************************
 * Function Name : ParseTS_PackageHead
 * Description   : parse the TS package head
 * Parameters    :
 *				pstTS_PackageHead -- the package head structure
 *				pucBufferPackage -- the package buffer
 * Returns       : void
 *********************************************************************************************************************************/
void ParseTS_PackageHead(TS_PACKAGE_HEAD *pstTS_PackageHead, unsigned char *pucPackageBuffer);

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
int SavePID_Packege(FILE *pfTsFile, int iTsPosition, int iTsLength, unsigned int *puiWantPID, FILE *pfSaveFile);

#endif /* GETPIDPACKAGE_H_ */
