/*#############################################################
 *     File Name	: pci.c
 *     Author		: winddoing
 *     Created Time	: 2021年07月16日 星期五 16时11分29秒
 *     Description	:
 *############################################################*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/io.h>

#define PCI_MAX_BUS 255 /* 8 bits (0 ~ 255) */
#define PCI_MAX_DEV 31 /* 5 bits (0 ~ 31) */
#define PCI_MAX_FUN 7 /* 3 bits (0 ~ 7) */

#define CONFIG_ADDRESS 0xCF8
#define CONFIG_DATA 0xCFC

#define PCICFG_REG_VID 0x00 /* Vendor id, 2 bytes */
#define PCICFG_REG_DID 0x02 /* Device id, 2 bytes */
#define PCICFG_REG_CMD 0x04 /* Command register, 2 bytes */
#define PCICFG_REG_STAT 0x06 /* Status register, 2 bytes */
#define PCICFG_REG_RID 0x08 /* Revision id, 1 byte */


void list_pci_devices()
{
	unsigned int bus, dev, fun;
	unsigned int addr, data;

	//printf("BB:DD:FF VID:DID\n");


	for (bus = 0; bus <= PCI_MAX_BUS; bus++) {
		for (dev = 0; dev <= PCI_MAX_DEV; dev++) {
			for (fun = 0; fun <= PCI_MAX_FUN; fun++) {
				addr = 0x80000000L | (bus<<16) | (dev<<11) | (fun<<8);
				outl(addr, CONFIG_ADDRESS);
				data = inl(CONFIG_DATA);

				/* Identify vendor ID */
				if ((data != 0xFFFFFFFF) && (data != 0)) {
					printf("%02X:%02X:%02X ", bus, dev, fun);
					printf("%04X:%04X", data&0xFFFF, data>>16);
					addr = 0x80000000L | (bus<<16) | (dev<<11) | (fun<<8) | PCICFG_REG_RID;
					outl(addr, CONFIG_ADDRESS);
					data = inl(CONFIG_DATA);
					if (data&0xFF) {
						printf(" (rev %02X)\n", data&0xFF);
					} else {
						printf("\n");
					}
				}
			} //end func
		} // end device
	} // end bus
}


int main()
{
	int ret;

	/* Enable r/w permission of all 65536 ports */
	ret = iopl(3);
	if (ret < 0) {
		perror("iopl set error");
		return 1;
	}

	list_pci_devices();

	/* Disable r/w permission of all 65536 ports */
	ret = iopl(0);
	if (ret < 0) {
		perror("iopl set error");
		return 1;
	}

	return 0;
}

