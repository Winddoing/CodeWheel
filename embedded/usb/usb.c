/* Copyright (C) 2022 wqshao All rights reserved.
 *
 *  File Name    : usb.c
 *  Author       : wqshao
 *  Created Time : 2022-07-13 17:33:29
 *  Description  : 下位机获取数据
 *  sudo apt install libusb-1.0-0-dev
 *  gcc usb.c `pkg-config --cflags --libs libusb-1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <libusb.h>

int main(int argc, char* argv[])
{
	libusb_context *usb_context = NULL;
	libusb_device **dev_list;
	libusb_device *dev = NULL;
	libusb_device *dev_temp;
	libusb_device_handle *dev_handle = NULL;

	int ret = -1;
	ssize_t cnt;

	if (ret = libusb_init(&usb_context) < 0) {
		printf("init error: %d\n",ret);
		return ret;
	}

	if (cnt = libusb_get_device_list(usb_context, &dev_list) < 0) {
		printf("get device list error: %ld\n", cnt);
		return cnt;
	}

	int i = 0;
	while ((dev_temp = dev_list[i++]) != NULL) {
		struct libusb_device_descriptor desc;
		int r = libusb_get_device_descriptor(dev_temp, &desc);
		if (r < 0) {
			printf("get device descriptor error:%d\n", r);
			return r;
		}

#if 1
		printf("%x:%x (bus %d, device %d)\n",
				desc.idVendor, desc.idProduct,
				libusb_get_bus_number(dev_temp), libusb_get_device_address(dev_temp));
#endif
		if ((desc.idVendor == 0x2207) && (desc.idProduct == 0x320c)) {
/*		if ((desc.idVendor == 0x9990) && (desc.idProduct == 0xeee0)) { */
			dev = dev_temp;
			break;
		}
	}

	if (dev == NULL) {
		printf("find dev error\n");
		return -1;
	}

	ret = libusb_open(dev, &dev_handle);
	if (dev_handle == NULL) {
		printf("open device error:%d\n", ret);
		return ret;
	}

	ret = libusb_kernel_driver_active(dev_handle, 0);
	if (ret == 1) {
		libusb_detach_kernel_driver(dev_handle, 0);
		printf("driver error\n");
	}

	if (ret = libusb_claim_interface(dev_handle, 0) < 0) {
		printf("claim interface error:%d\n",ret);
		libusb_free_device_list(dev_list, 1);
		libusb_exit(usb_context);
		return ret;
	}

	struct libusb_config_descriptor *config_descriptor_in = NULL, *config_descriptor_out = NULL;

	//libusb_get_active_config_descriptor(dev, &config_descriptor);
	int config = 0;

	libusb_get_configuration(dev_handle, &config);
	libusb_get_config_descriptor(dev, 0, &config_descriptor_in);
	libusb_get_config_descriptor(dev, 1, &config_descriptor_out);

	if (config_descriptor_in) {
		//1. 发送数据
		unsigned char data[4096] = "aaaaaffff";
		unsigned char data_rec[4096] = "\0";
		int length = 0;
		//ret = libusb_bulk_transfer(dev_handle, config_descriptor_in->interface->altsetting->endpoint->bEndpointAddress, data, 9, &length, 0);
		ret = libusb_bulk_transfer(dev_handle, 0x2, data, 4096, &length, 0);//host--------------->device
		if (ret < 0) {
			printf("bulk transfer error, ret=%d\n", ret);

		} else {
		//2. 接收数据
			for (int i = 0; i < 100000; i++) {
				memset(data_rec, 0, 4096);
				ret = libusb_bulk_transfer(dev_handle, 129, data_rec, 4096, &length, 0);//device--------->host
				printf("i = %d,receive data：%s",i, data_rec);
			}
		}
	} else {
		printf("get config_descriptor error\n");
	}

	libusb_free_device_list(dev_list,1);

	libusb_exit(usb_context);

	return 0;

}
