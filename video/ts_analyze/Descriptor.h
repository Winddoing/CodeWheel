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

#ifndef DESCRIPTOR_H_
#define DESCRIPTOR_H_

#define SERVICE_NAME_LANGTH_MAX 512
#define SERVICE_LIST_MAX 128

/*----------------------------------------------------------*/
/* service_descriptor  the descriptor_tag  0x48 */
/*----------------------------------------------------------*/
typedef struct SERVICE_DESCRIPTOR
{
	unsigned int descriptor_tag :8;
	unsigned int descriptor_length :8;
	unsigned int service_type :8;
	unsigned int service_provider_name_length :8;
	unsigned char service_provider_name[SERVICE_NAME_LANGTH_MAX];
	unsigned int service_name_length :8;
	unsigned char service_name[SERVICE_NAME_LANGTH_MAX];
} SERVICE_DESCRIPTOR;

/*----------------------------------------------------------*/
/* service_descriptor the descriptor_tag  0x4d */
/*----------------------------------------------------------*/
typedef struct SHORT_EVENT_DESCRIPTOR
{
	unsigned int descriptor_tag :8;
	unsigned int descriptor_length :8;
	unsigned int ISO_639_language_code :24;
	unsigned int event_name_length :8;
	unsigned char event_name_char[SERVICE_NAME_LANGTH_MAX];
	unsigned int text_length :8;
	unsigned char text_char[SERVICE_NAME_LANGTH_MAX];
} SHORT_EVENT_DESCRIPTOR;

/*----------------------------------------------------------*/
/* network_name_descriptor the descriptor_tag  0x40 */
/*----------------------------------------------------------*/
typedef struct NETWORK_NAME_DESCRIPTOR
{
	unsigned int descriptor_tag :8;
	unsigned int descriptor_length :8;
	unsigned char network_name[SERVICE_NAME_LANGTH_MAX];
} NETWORK_NAME_DESCRIPTOR;

/*----------------------------------------------------------*/
/* satellite_delivery_system_descriptor the descriptor_tag  0x43 */
/*----------------------------------------------------------*/
typedef struct SATELLITE_DELIVERY_SYSTEM_DESCRIPTOR
{
	unsigned int descriptor_tag :8;
	unsigned int descriptor_length :8;
	unsigned int frequency :32;
	unsigned int orbital_position :16;
	unsigned int west_east_flag :1;
	unsigned int polarization :2;
	unsigned int roll_off :2;
	unsigned int modulation_system :1;
	unsigned int modulation_type :2;
	unsigned int symbol_rate :28;
	unsigned int FEC_inner :4;
} SATELLITE_DELIVERY_SYSTEM_DESCRIPTOR;

/*----------------------------------------------------------*/
/* service_list_descriptor the descriptor_tag 0x41 */
/*----------------------------------------------------------*/
typedef struct SERVICE_LIST
{
	unsigned int service_id :16;
	unsigned int service_type :8;
} SERVICE_LIST;
typedef struct SERVICE_LIST_DESCRIPTOR
{
	unsigned int descriptor_tag :8;
	unsigned int descriptor_length :8;
	SERVICE_LIST astServieList[SERVICE_LIST_MAX];
} SERVICE_LIST_DESCRIPTOR;

/*********************************************************************************************************************************
 * Function Name : GetServiceDescriptor
 * Description   : get the service_descriptor (tag is 0x48)
 * Parameters    :
 *				pstServiceDescriptor -- the service_descriptor
 *				pucDescriptor -- the descriptor buffer
 *				uiDescriprotLength -- the descriptor length
 * Returns       : -1 -- failure 0 -- success
 *********************************************************************************************************************************/
int GetServiceDescriptor(SERVICE_DESCRIPTOR *pstServiceDescriptor, unsigned char *pucDescriptor, unsigned int uiDescriprotLength);

/*********************************************************************************************************************************
 * Function Name : GetShortEventDescriptor
 * Description   : get short_event_descriptor (tag is 0x4d)
 * Parameters    :
 *				pstShortEventDescriptor -- the short_event_descriptor
 *				pucDescriptors -- the descriptor buffer
 *				iDescriprotLength -- the descriptor length
 * Returns       : -1 -- failure 0 -- success
 *********************************************************************************************************************************/
int GetShortEventDescriptor(SHORT_EVENT_DESCRIPTOR *pstShortEventDescriptor, unsigned char *pucDescriptor, unsigned int uiDescriprotLength);

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
        unsigned int uiDescriprotLength);

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
        unsigned int uiDescriprotLength);

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
        unsigned int uiDescriprotLength);

#endif /* DESCRIPTOR_H_ */
