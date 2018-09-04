/********************************************************************************************************************************
 *									 All Rights GLOBALSAT Reserved													            *
 ********************************************************************************************************************************
 ********************************************************************************************************************************
 * Filename	   	: GetSection.c
 * Description  : using the file for get the section of the PID package which we want
 * Version	   	: 1.0
 * History      :
 * WuYunXing 	: 2013-4-11  Create
 *********************************************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "GetSection.h"
#include "Descriptor.h"
#include "ParseNIT.h"

#define NIT_PID 0x0010
#define NIT_TABLE_ID 0x40

/*********************************************************************************************************************************
 * Function Name : ParseSDT_SectionHead
 * Description   :  parse the SDT section head
 * Parameters    :
 * 				pstTS_SDT -- the TS_SDT
 *				pucSectionBuffer -- the SDT section buffer
 * Returns       : void
 *********************************************************************************************************************************/
static void ParseNIT_SectionHead(TS_NIT *pstTS_NIT, unsigned char *pucSectionBuffer)
{
	int iNetworkDescriptorLen = 0;
	int iNIT_Length = 0;

	pstTS_NIT->table_id = pucSectionBuffer[0];
	pstTS_NIT->section_syntax_indicator = pucSectionBuffer[1] >> 7;
	pstTS_NIT->reserved_future_use_1 = (pucSectionBuffer[1] >> 6) & 0x01;
	pstTS_NIT->reserved_1 = (pucSectionBuffer[1] >> 4) & 0x03;
	pstTS_NIT->section_length = ((pucSectionBuffer[1] & 0x0F) << 8) | pucSectionBuffer[2];
	pstTS_NIT->network_id = (pucSectionBuffer[3] << 8) | pucSectionBuffer[4];
	pstTS_NIT->reserved_2 = pucSectionBuffer[5] >> 6;
	pstTS_NIT->version_number = (pucSectionBuffer[5] >> 1) & 0x1F;
	pstTS_NIT->current_next_indicator = (pucSectionBuffer[5] << 7) >> 7;
	pstTS_NIT->section_number = pucSectionBuffer[6];
	pstTS_NIT->last_section_number = pucSectionBuffer[7];
	pstTS_NIT->reserved_future_use_2 = pucSectionBuffer[8] >> 4;
	pstTS_NIT->network_descriptors_length = ((pucSectionBuffer[8] & 0x0F) << 8) | pucSectionBuffer[9];
	iNetworkDescriptorLen = pstTS_NIT->network_descriptors_length;
	memcpy(pstTS_NIT->network_descriptor, pucSectionBuffer + 10, iNetworkDescriptorLen);
	pstTS_NIT->reserved_future_use_2 = pucSectionBuffer[10 + iNetworkDescriptorLen] >> 4;
	pstTS_NIT->transport_stream_loop_length = ((pucSectionBuffer[10 + iNetworkDescriptorLen] & 0x0F) << 8)
	        | pucSectionBuffer[10 + iNetworkDescriptorLen + 1];
	iNIT_Length = pstTS_NIT->section_length + 3;
	pstTS_NIT->CRC_32 = (pucSectionBuffer[iNIT_Length - 4] << 24) | (pucSectionBuffer[iNIT_Length - 3] << 16)
	        | (pucSectionBuffer[iNIT_Length - 2] << 8) | pucSectionBuffer[iNIT_Length - 1];
}

/*********************************************************************************************************************************
 * Function Name : ParseNIT_Section
 * Description   : parse the NIT table
 * Parameters    :
 *				pTS_NIT -- the TS_NIT
 *				pucSectionBuffer -- the buffer of NIT section
 * Returns       : the count of TS_NIT_TRANSPORT
 *********************************************************************************************************************************/
static int ParseNIT_Section(TS_NIT *pstTS_NIT, unsigned char *pucSectionBuffer)
{
	int iNIT_Length = 0;
	int iTransportPosition = 0;
	int iTranportCount = 0;
	int iNetworkDescriptorLen = 0;

	memset(pstTS_NIT, 0, sizeof(TS_NIT));
	ParseNIT_SectionHead(pstTS_NIT, pucSectionBuffer);
	iNetworkDescriptorLen = pstTS_NIT->network_descriptors_length;
	iNIT_Length = pstTS_NIT->section_length + 3;

	for (iTransportPosition = 10 + iNetworkDescriptorLen + 2; iTransportPosition < iNIT_Length - 4; iTransportPosition += 6)
	{
		pstTS_NIT->astNIT_Transport[iTranportCount].transport_stream_id = (pucSectionBuffer[iTransportPosition] << 8)
		        | pucSectionBuffer[iTransportPosition + 1];
		pstTS_NIT->astNIT_Transport[iTranportCount].original_network_id = (pucSectionBuffer[iTransportPosition + 2] << 8)
		        | pucSectionBuffer[iTransportPosition + 3];
		pstTS_NIT->astNIT_Transport[iTranportCount].reserved_future_use = pucSectionBuffer[iTransportPosition + 4] >> 4;
		pstTS_NIT->astNIT_Transport[iTranportCount].transport_descriptors_length = ((pucSectionBuffer[iTransportPosition + 4] & 0x0F) << 8)
		        | pucSectionBuffer[iTransportPosition + 5];
		if (0 != pstTS_NIT->astNIT_Transport[iTranportCount].transport_descriptors_length)
		{
			memcpy(pstTS_NIT->astNIT_Transport[iTranportCount].descriptor, pucSectionBuffer + 6 + iTransportPosition,
			        pstTS_NIT->astNIT_Transport[iTranportCount].transport_descriptors_length);
			iTransportPosition += pstTS_NIT->astNIT_Transport[iTranportCount].transport_descriptors_length;
		}
		iTranportCount++;
	}
	return iTranportCount;
}

/*********************************************************************************************************************************
 * Function Name : DeleteHex0x
 * Description   : delete the hex 0x for example(0x47(hex) -> 47(decimal))
 * Parameters    :
 *				ucSource -- the hex value
 * Returns       : the decimal value delete the 0x
 *********************************************************************************************************************************/
static int DeleteHex0x(unsigned char ucSource)
{
	int iReturn = 0;
	int iTempTen = (int)(ucSource >> 4);
	int iTempBite = (int)(ucSource & 0x0F);

	iReturn = iTempTen * 10 + iTempBite;

	return iReturn;
}

/*********************************************************************************************************************************
 * Function Name : PrintSatelliteInfo
 * Description   : print the satellite information
 * Parameters    :
 *				ucDescriptor -- the descriptor buffer
 *				iDescriprotLength -- the descriptor length
 * Returns       : void
 *********************************************************************************************************************************/
static void PrintSatelliteInfo(unsigned char *pucDescriptor, unsigned int uiDescriprotLength)
{
	int iTemp = 0;
	double dFrequency = 0;
	double dSymbolRate = 0;
	double dOrbitalPosition = 0;
	int aiFrequency[4] = {0};
	int aiSymbolRate[4] = {0};
	int aiOrbitalPosition[2] = {0};
	int iSatelliteDescriptorPosition = -1;
	SATELLITE_DELIVERY_SYSTEM_DESCRIPTOR stSatellitDeliveryDescriptor = {0};

	iTemp = GetSatelliteDeliverySystemDescriptor(&stSatellitDeliveryDescriptor, pucDescriptor, uiDescriprotLength);
	if (0 == iTemp)
	{
		/*----------------------------------------------------------*/
		/* transform frequency to decimal */
		/*----------------------------------------------------------*/
		aiFrequency[0] = DeleteHex0x((unsigned char)(stSatellitDeliveryDescriptor.frequency >> 24));
		aiFrequency[1] = DeleteHex0x((unsigned char)(stSatellitDeliveryDescriptor.frequency >> 16));
		aiFrequency[2] = DeleteHex0x((unsigned char)(stSatellitDeliveryDescriptor.frequency >> 8));
		aiFrequency[3] = DeleteHex0x((unsigned char)(stSatellitDeliveryDescriptor.frequency));

		dFrequency = aiFrequency[0] * 100 + aiFrequency[1] / 10.0 + aiFrequency[2] / 1000.0 + aiFrequency[3] / 100000.0;

		/*----------------------------------------------------------*/
		/* transform orbital_position to decimal */
		/*----------------------------------------------------------*/
		aiOrbitalPosition[0] = DeleteHex0x((unsigned char)(stSatellitDeliveryDescriptor.orbital_position >> 8));
		aiOrbitalPosition[1] = DeleteHex0x((unsigned char)(stSatellitDeliveryDescriptor.orbital_position));

		dOrbitalPosition = aiOrbitalPosition[0] * 100 + aiOrbitalPosition[1] / 10.0;

		/*----------------------------------------------------------*/
		/* transform symbol_rate to decimal */
		/*----------------------------------------------------------*/
		aiSymbolRate[0] = DeleteHex0x((unsigned char)(stSatellitDeliveryDescriptor.symbol_rate >> 20));
		aiSymbolRate[1] = DeleteHex0x((unsigned char)(stSatellitDeliveryDescriptor.symbol_rate >> 12));
		aiSymbolRate[2] = DeleteHex0x((unsigned char)(stSatellitDeliveryDescriptor.symbol_rate >> 4));
		aiSymbolRate[3] = DeleteHex0x((unsigned char)(stSatellitDeliveryDescriptor.symbol_rate));

		dSymbolRate = aiSymbolRate[0] * 100 + aiSymbolRate[1] / 10.0 + aiSymbolRate[2] / 1000.0 + (aiSymbolRate[3] / 10) / 100000.0;

		printf("the satellite_delevery_system_descriptor information is \n");
		printf("frequency: %.5f GHz,\n", dFrequency);
		printf("orbital_position: %.1f degree\n", dOrbitalPosition);
		printf("symbol_rate: %.4f symbol/s\n", dSymbolRate);
	}
}

/*********************************************************************************************************************************
 * Function Name : PrintServiceListInfo
 * Description   : print the information in the service_list_descriptor
 * Parameters    :
 *				ucDescriptor -- the descriptor buffer
 *				iDescriprotLength -- the descriptor length
 * Returns       : void
 *********************************************************************************************************************************/
static void PrintServiceListInfo(unsigned char *pucDescriptor, unsigned int uiDescriprotLength)
{
	int iListCount = 0;
	int iServiceListIndex = 0;
	SERVICE_LIST_DESCRIPTOR stServiceListDescriptor = {0};

	iListCount = GetServiceListDescriptor(&stServiceListDescriptor, pucDescriptor, uiDescriprotLength);
	if (-1 != iListCount)
	{
		printf("the service_list_descriptor information is \n");

		/*----------------------------------------------------------*/
		/* print the service list information */
		/*----------------------------------------------------------*/
		for (iServiceListIndex = 0; iServiceListIndex < iListCount; iServiceListIndex++)
		{
			printf("the service_id is %x\t", stServiceListDescriptor.astServieList[iServiceListIndex].service_id);
			printf("the service_type is %x\n", stServiceListDescriptor.astServieList[iServiceListIndex].service_type);
		}
		printf("\n");
	}
}

/*********************************************************************************************************************************
 * Function Name : PrintNetworkName
 * Description   : print the network name information
 * Parameters    :
 *				pucDescriptor -- the descriptor buffer
 *				uiDescriprotLength -- the descriptor length
 * Returns       : void
 *********************************************************************************************************************************/
static void PrintNetworkName(unsigned char *pucDescriptor, unsigned int uiDescriprotLength)
{
	int iTemp = 0;
	NETWORK_NAME_DESCRIPTOR stNetworkNameDescriptor = {0};

	/*----------------------------------------------------------*/
	/* mainly in order to get the network name */
	/*----------------------------------------------------------*/
	iTemp = GetNetworkNameDescriptor(&stNetworkNameDescriptor, pucDescriptor, uiDescriprotLength);
	if (0 == iTemp)
	{
		printf("the network name is %s\n", stNetworkNameDescriptor.network_name);
	}
}

/*********************************************************************************************************************************
 * Function Name : PrintNIT_Info
 * Description   : print the NIT information
 * Parameters    :
 *				pstTS_NIT -- the TS_NIT
 *				iTranportCount -- the count of the transport
 * Returns       : void
 *********************************************************************************************************************************/
static void PrintNIT_Info(TS_NIT *pstTS_NIT, int iTranportCount)
{
	int iTransportIndex = 0;

	printf("* * * the information of the NIT table * * * \n");
	printf("* * the network information * * \n");
	printf("the network_id is : %d\n", pstTS_NIT->network_id);

	PrintNetworkName(pstTS_NIT->network_descriptor, pstTS_NIT->network_descriptors_length);

	printf("* * the transport stream information * * \n");
	system("pause");

	for (iTransportIndex = 0; iTransportIndex < iTranportCount; iTransportIndex++)
	{
		printf("the transport_stream_id: %d\n", pstTS_NIT->astNIT_Transport[iTransportIndex].transport_stream_id);

		/*----------------------------------------------------------*/
		/* mainly in order to print the frequency  orbital_position and symbol_rate */
		/*----------------------------------------------------------*/
		PrintSatelliteInfo(pstTS_NIT->astNIT_Transport[iTransportIndex].descriptor,
		        pstTS_NIT->astNIT_Transport[iTransportIndex].transport_descriptors_length);

		/*----------------------------------------------------------*/
		/* mainly in order to print the service_id and service_type in the transport stream */
		/*----------------------------------------------------------*/
		PrintServiceListInfo(pstTS_NIT->astNIT_Transport[iTransportIndex].descriptor,
		        pstTS_NIT->astNIT_Transport[iTransportIndex].transport_descriptors_length);

		printf("\n");

	}
}

/*********************************************************************************************************************************
 * Function Name : ParseNIT_Table
 * Description   :  parse the PAT table and get the information
 * Parameters    :
 * 				pfTsFile -- the TS file descriptor
 * 				iTsPosition -- the TS package begin position
 *				iTsLength -- the TS package length
 * Returns       : -1 failure 0 -- no the section in the transport stream 1 - all section over
 *********************************************************************************************************************************/
int ParseNIT_Table(FILE *pfTsFile, int iTsPosition, int iTsLength)
{
	int iTemp = 0;
	unsigned int uiVersion = INITIAL_VERSION;
	unsigned int auiRecordGetSection[SECTION_COUNT_256] = {0};
	unsigned char aucSectionBuffer[SECTION_MAX_LENGTH_4096] = {0};
	TS_NIT stTS_NIT = {0};
	int iTranportCount = 0;

	if (-1 == fseek(pfTsFile, iTsPosition, SEEK_SET))
	{
		printf("parse table error\n");
		return -1;
	}
	while (!feof(pfTsFile))
	{
		iTemp = GetOneSection(pfTsFile, iTsLength, aucSectionBuffer, NIT_PID, NIT_TABLE_ID, &uiVersion);
		if (0 == iTemp) // version number change
		{
			uiVersion = INITIAL_VERSION;
			memset(auiRecordGetSection, 0, sizeof(char) * SECTION_COUNT_256); // clean the record array
			fseek(pfTsFile, 0 - iTsLength, SEEK_CUR); //has read the package already so we should return one package
		}
		if (1 == iTemp) //one section over
		{
			if (0 == IsSectionGetBefore(aucSectionBuffer, auiRecordGetSection)) //judgment the section whether been parsed before
			{
				iTranportCount = ParseNIT_Section(&stTS_NIT, aucSectionBuffer);
				PrintNIT_Info(&stTS_NIT, iTranportCount);
			}
			if (1 == IsAllSectionOver(aucSectionBuffer, auiRecordGetSection))
			{
				return 1;
			}
		}
		if (-1 == iTemp) //error
		{
			printf("no NIT table in the transport stream \n ");
			return 0;
		}
	}
	return 1;
}
