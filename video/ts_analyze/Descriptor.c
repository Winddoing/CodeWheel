/********************************************************************************************************************************
 *									 All Rights GLOBALSAT Reserved													            *
 ********************************************************************************************************************************
 ********************************************************************************************************************************
 * Filename	   	: Descriptor.c
 * Description  : the function about the descriptor
 * Version	   	: 1.0
 * History      :
 * WuYunXing 	: 2013-4-15  Create
 *********************************************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Descriptor.h"

#define SDT_SERVICE_DESCRIPTOR 0x48
#define EIT_SHORT_EVENT_DESCRIPTOR 0x4d
#define NIT_STATELLITE_DELIVERY_SYSTEM_DESCRIPTOR 0x43
#define NIT_NETWORK_NAME_DESCRIPTOR 0x40
#define NIT_SERVICE_LIST_DESCRIPTOR 0x41

/*********************************************************************************************************************************
 * Function Name : GetDescriptorTagPosition
 * Description   : get the descriptor tag position
 * Parameters    :
 *				pucDescriptors -- the descriptor buffer
 *				uiDescriprotLength -- the descriptor length
 *				uiDescriptorTagType -- the descriptor tag type (0x4d)
 * Returns       : -1 -- failure >0 -- success
 *********************************************************************************************************************************/
static int GetDescriptorTagPosition(unsigned char *pucDescriptor, unsigned int uiDescriprotLength, unsigned int uiDescriptorTagType)
{
	int iDescriptorTagPosition = 0;
	unsigned int iDescriptorTagLength = 0;
	unsigned int uiDescriptorTagTemp = 0;

	while (iDescriptorTagPosition < (int)uiDescriprotLength)
	{
		uiDescriptorTagTemp = pucDescriptor[iDescriptorTagPosition];
		if (uiDescriptorTagType == uiDescriptorTagTemp)
		{
			return iDescriptorTagPosition;
		}
		iDescriptorTagLength = pucDescriptor[iDescriptorTagPosition + 1];
		iDescriptorTagPosition = iDescriptorTagPosition + 2 + iDescriptorTagLength;
	}

	return -1;
}

/*********************************************************************************************************************************
 * Function Name : GetServiceDescriptor
 * Description   : get the service_descriptor (tag is 0x48)
 * Parameters    :
 *				pstServiceDescriptor -- the service_descriptor
 *				pucDescriptor -- the descriptor buffer
 *				uiDescriprotLength -- the descriptor length
 * Returns       : -1 -- failure 0 -- success
 *********************************************************************************************************************************/
int GetServiceDescriptor(SERVICE_DESCRIPTOR *pstServiceDescriptor, unsigned char *pucDescriptor, unsigned int uiDescriprotLength)
{
	int iServiceDespritorPosition = -1;

	if (NULL == pucDescriptor)
	{
		printf("no descriptor information\n");
		return -1;
	}

	iServiceDespritorPosition = GetDescriptorTagPosition(pucDescriptor, uiDescriprotLength, SDT_SERVICE_DESCRIPTOR);
	if (-1 == iServiceDespritorPosition)
	{
		printf("no service_descriptor information\n");
		return -1;
	}

	memset(pstServiceDescriptor, 0, sizeof(SERVICE_DESCRIPTOR));
	pstServiceDescriptor->descriptor_tag = pucDescriptor[iServiceDespritorPosition];
	pstServiceDescriptor->descriptor_length = pucDescriptor[iServiceDespritorPosition + 1];
	pstServiceDescriptor->service_type = pucDescriptor[iServiceDespritorPosition + 2];
	pstServiceDescriptor->service_provider_name_length = pucDescriptor[iServiceDespritorPosition + 3];
	memcpy(pstServiceDescriptor->service_provider_name, pucDescriptor + iServiceDespritorPosition + 4,
	        pstServiceDescriptor->service_provider_name_length);
	pstServiceDescriptor->service_name_length = pucDescriptor[4 + pstServiceDescriptor->service_provider_name_length];
	memcpy(pstServiceDescriptor->service_name,
	        pucDescriptor + iServiceDespritorPosition + 5 + pstServiceDescriptor->service_provider_name_length,
	        pstServiceDescriptor->service_name_length);

	return 0;
}

/*********************************************************************************************************************************
 * Function Name : GetShortEventDescriptor
 * Description   : get short_event_descriptor (tag is 0x4d)
 * Parameters    :
 *				pstShortEventDescriptor -- the short_event_descriptor
 *				pucDescriptors -- the descriptor buffer
 *				iDescriprotLength -- the descriptor length
 * Returns       : -1 -- failure 0 -- success
 *********************************************************************************************************************************/
int GetShortEventDescriptor(SHORT_EVENT_DESCRIPTOR *pstShortEventDescriptor, unsigned char *pucDescriptor, unsigned int uiDescriprotLength)
{
	int iShotEventDespritorPosition = -1;

	if (NULL == pucDescriptor)
	{
		printf("no descriptor information\n");
		return -1;
	}

	iShotEventDespritorPosition = GetDescriptorTagPosition(pucDescriptor, uiDescriprotLength, EIT_SHORT_EVENT_DESCRIPTOR);
	if (-1 == iShotEventDespritorPosition)
	{
		printf("no short_event_descriptor information\n");
		return -1;
	}

	memset(pstShortEventDescriptor, 0, sizeof(SHORT_EVENT_DESCRIPTOR));
	pstShortEventDescriptor->descriptor_tag = pucDescriptor[iShotEventDespritorPosition];
	pstShortEventDescriptor->descriptor_length = pucDescriptor[iShotEventDespritorPosition + 1];
	pstShortEventDescriptor->ISO_639_language_code = (pucDescriptor[iShotEventDespritorPosition + 2] << 16)
	        | (pucDescriptor[iShotEventDespritorPosition + 3] << 8) | (pucDescriptor[iShotEventDespritorPosition + 4]);
	pstShortEventDescriptor->event_name_length = pucDescriptor[iShotEventDespritorPosition + 5];
	memcpy(pstShortEventDescriptor->event_name_char, pucDescriptor + iShotEventDespritorPosition + 6,
	        pstShortEventDescriptor->event_name_length);
	pstShortEventDescriptor->text_length = pucDescriptor[6 + pstShortEventDescriptor->event_name_length];
	memcpy(pstShortEventDescriptor->text_char, pucDescriptor + iShotEventDespritorPosition + 7 + pstShortEventDescriptor->event_name_length,
	        pstShortEventDescriptor->text_length);

	return 0;
}

/*********************************************************************************************************************************
 * Function Name : GetNetworkNameDescriptor
 * Description   : get the network_name_descriptor (tag is 0x40)
 * Parameters    :
 *				pstNetworkNameDescriptor -- the network_name_descriptor
 *				pucDescriptors -- the descriptor buffer
 *				uiDescriprotLength -- the descriptor length
 * Returns       : -1 -- failure 0 -- success
 *********************************************************************************************************************************/
int GetNetworkNameDescriptor(NETWORK_NAME_DESCRIPTOR *pstNetworkNameDescriptor, unsigned char *pucDescriptor,
        unsigned int uiDescriprotLength)
{
	int iNetworkNamePosition = -1;

	if (NULL == pucDescriptor)
	{
		printf("no descriptor information\n");
		return -1;
	}

	iNetworkNamePosition = GetDescriptorTagPosition(pucDescriptor, uiDescriprotLength, NIT_NETWORK_NAME_DESCRIPTOR);
	if (-1 == iNetworkNamePosition)
	{
		printf("no network_name_descriptor information\n");
		return -1;
	}

	memset(pstNetworkNameDescriptor, 0, sizeof(NETWORK_NAME_DESCRIPTOR));
	pstNetworkNameDescriptor->descriptor_tag = pucDescriptor[iNetworkNamePosition];
	pstNetworkNameDescriptor->descriptor_length = pucDescriptor[iNetworkNamePosition + 1];
	memcpy(pstNetworkNameDescriptor->network_name, pucDescriptor + iNetworkNamePosition + 2, pstNetworkNameDescriptor->descriptor_length);

	return 0;
}

/*********************************************************************************************************************************
 * Function Name : GetSatelliteDeliverySystemDescriptor
 * Description   : get satellite_delivery_system_descriptor (tag is 0x43)
 * Parameters    :
 *				pstSatellitDeliveryDescriptor -- the satellite_delivery_system_descriptor
 *				pucDescriptor -- the descriptor buffer
 *				uiDescriprotLength -- the descriptor length
 * Returns       : -1 -- failure 0 -- success
 *********************************************************************************************************************************/
int GetSatelliteDeliverySystemDescriptor(SATELLITE_DELIVERY_SYSTEM_DESCRIPTOR *pstSatellitDeliveryDescriptor, unsigned char *pucDescriptor,
        unsigned int uiDescriprotLength)
{
	int iSatelliteDeliveryPosition = -1;

	if (NULL == pucDescriptor)
	{
		printf("no descriptor information\n");
		return -1;
	}

	iSatelliteDeliveryPosition = GetDescriptorTagPosition(pucDescriptor, uiDescriprotLength, NIT_STATELLITE_DELIVERY_SYSTEM_DESCRIPTOR);
	if (-1 == iSatelliteDeliveryPosition)
	{
		printf("no service_satellite_delivery_system_descriptor information\n");
		return -1;
	}

	memset(pstSatellitDeliveryDescriptor, 0, sizeof(SATELLITE_DELIVERY_SYSTEM_DESCRIPTOR));
	pstSatellitDeliveryDescriptor->descriptor_tag = pucDescriptor[iSatelliteDeliveryPosition];
	pstSatellitDeliveryDescriptor->descriptor_length = pucDescriptor[iSatelliteDeliveryPosition + 1];
	pstSatellitDeliveryDescriptor->frequency = (pucDescriptor[iSatelliteDeliveryPosition + 2] << 24)
	        | (pucDescriptor[iSatelliteDeliveryPosition + 3] << 16) | (pucDescriptor[iSatelliteDeliveryPosition + 4] << 8)
	        | (pucDescriptor[iSatelliteDeliveryPosition + 5]);
	pstSatellitDeliveryDescriptor->orbital_position = (pucDescriptor[iSatelliteDeliveryPosition + 6] << 8)
	        | pucDescriptor[iSatelliteDeliveryPosition + 7];
	pstSatellitDeliveryDescriptor->west_east_flag = pucDescriptor[iSatelliteDeliveryPosition + 8] >> 7;
	pstSatellitDeliveryDescriptor->polarization = (pucDescriptor[iSatelliteDeliveryPosition + 8] & 0x7F) >> 5;
	pstSatellitDeliveryDescriptor->roll_off = (pucDescriptor[iSatelliteDeliveryPosition + 8] & 0x1F) >> 3;
	pstSatellitDeliveryDescriptor->modulation_system = (pucDescriptor[iSatelliteDeliveryPosition + 8] & 0x07) >> 2;
	pstSatellitDeliveryDescriptor->modulation_type = pucDescriptor[iSatelliteDeliveryPosition + 8] & 0x03;
	pstSatellitDeliveryDescriptor->symbol_rate = (pucDescriptor[iSatelliteDeliveryPosition + 9] << 20)
	        | (pucDescriptor[iSatelliteDeliveryPosition + 10] << 12) | (pucDescriptor[iSatelliteDeliveryPosition + 11] << 4)
	        | (pucDescriptor[iSatelliteDeliveryPosition + 12] >> 4);
	pstSatellitDeliveryDescriptor->FEC_inner = pucDescriptor[iSatelliteDeliveryPosition + 12] & 0x0F;

	return 0;
}

/*********************************************************************************************************************************
 * Function Name : GetServiceListDescriptor
 * Description   : get service_list_descriptor (tag is 0x41)
 * Parameters    :
 *				pstServiceListDescriptor -- the service_list_descriptor
 *				pucDescriptor -- the descriptor buffer
 *				uiDescriprotLength -- the descriptor length
 * Returns       : -1 -- failure >0 -- the count of the SERVICE_LIST
 *********************************************************************************************************************************/
int GetServiceListDescriptor(SERVICE_LIST_DESCRIPTOR *pstServiceListDescriptor, unsigned char *pucDescriptor,
        unsigned int uiDescriprotLength)
{
	int iServiceListPosition = -1;
	int iListIndex = 0;
	int iEachListPosition = 0;

	if (NULL == pucDescriptor)
	{
		printf("no descriptor information\n");
		return -1;
	}

	iServiceListPosition = GetDescriptorTagPosition(pucDescriptor, uiDescriprotLength, NIT_SERVICE_LIST_DESCRIPTOR);
	if (-1 == iServiceListPosition)
	{
		printf("no service_list_descriptor \n");
		return -1;
	}

	memset(pstServiceListDescriptor, 0, sizeof(SERVICE_LIST_DESCRIPTOR));
	pstServiceListDescriptor->descriptor_tag = pucDescriptor[iServiceListPosition];
	pstServiceListDescriptor->descriptor_length = pucDescriptor[iServiceListPosition + 1];

	/*----------------------------------------------------------*/
	/* get the list and put in array */
	/*----------------------------------------------------------*/
	for (iEachListPosition = 2 + iServiceListPosition;
	        iEachListPosition < (int)(pstServiceListDescriptor->descriptor_length + iServiceListPosition + 1); iEachListPosition += 3)
	        {
		pstServiceListDescriptor->astServieList[iListIndex].service_id = (pucDescriptor[iEachListPosition] << 8)
		        | pucDescriptor[iEachListPosition + 1];
		pstServiceListDescriptor->astServieList[iListIndex].service_type = pucDescriptor[iEachListPosition + 2];
		iListIndex++;
	}

	return iListIndex;
}
