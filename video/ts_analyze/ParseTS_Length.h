/********************************************************************************************************************************
 *									 All Rights GLOBALSAT Reserved													            *
 ********************************************************************************************************************************
 ********************************************************************************************************************************
 * Filename	   	: ParseTS_Length.h
 * Description  : using the file for get the package length and package begin position
 * Version	   	: 1.0
 * History      :
 * WuYunXing 	: 2013-3-18  Create
 *********************************************************************************************************************************/

#ifndef _PARSETS_LENGTH_H_
#define _PARSETS_LENGTH_H_
/************************************************************************************************************************************
 * Function Name : ParseTSPackageLength
 * Description   : parse the  package length
 * Parameters    :
 *				pfTsFile -- the file descriptor
 *				piTsPosition -- the package position(return the package begin position)
 * Returns       : -1 - the file is not the transport stream >0 - success and return the  package length
 ************************************************************************************************************************************/
int ParseTsLength(FILE *pfTsFile, int *piTsPosition);

#endif /* PARSETS_LENGTH_H_ */
