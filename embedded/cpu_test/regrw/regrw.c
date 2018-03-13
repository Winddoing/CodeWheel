#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

#include <ctype.h>
#include <stddef.h>
#include <sys/mman.h>


#if 0
#define dprintf printf
#else
#define dprintf(aaa, bbb...)							\
	do {		  \
	} while (0)
#endif


#define PAGE_SIZE 4096
#define PAGE_MASK_L 0xfff


#define REG32(addr) *((volatile unsigned int*)(addr))
#define REG16(addr) *((volatile unsigned short*)(addr))
#define REG8(addr) *((volatile unsigned char*)(addr))



enum ACCESS_ALIGN {
	WORD_ACCESS = 4,
	HALFWORD_ACCESS= 2,
	BYTE_ACCESS= 1,
};


enum ACCESS_TYPE {
	ACCESS_READ,
	ACCESS_WRITE,
};


static int memfd = 0;
static unsigned char * map_addr_base = NULL;

int mmap_physical_to_virtual(unsigned int addr, unsigned int * virt)
{

	int aligned_addr = addr & ~PAGE_MASK_L;
	int offset = addr & PAGE_MASK_L;
	int len = PAGE_SIZE;
	dprintf("mmap phy addr:0x%x  len %x offset=%d\n", aligned_addr, len, offset);

	if ( virt == NULL) {
		return -1;
	}

	int fd	= open("/dev/mem", O_RDWR | O_SYNC);
	//unsigned int map_addr = (unsigned int)mmap(NULL, (len + PAGE_MASK_L) & ~(PAGE_MASK_L), PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, aligned_addr);
	unsigned int map_addr = (unsigned int)mmap(NULL, (len + PAGE_MASK_L) & ~(PAGE_MASK_L), PROT_READ | PROT_WRITE, MAP_SHARED, fd, aligned_addr);

	dprintf("map_addr %x fd %x\n", map_addr, fd);

	if ((int)map_addr == -1 ) {
		perror("error");
		return -1;
	}

	map_addr_base = (unsigned char * )map_addr;

	map_addr = (int)map_addr + offset;
	dprintf("map_addr %x fd %x\n", map_addr, fd);

	*virt =(unsigned int) map_addr;
	memfd = fd;
	return 0;
}



int munmap_address()
{
	int len = PAGE_SIZE;

	munmap(map_addr_base, (len + PAGE_MASK_L) & ~(PAGE_MASK_L));
	close(memfd);

	return 0;
}


void show_usage( char *name)
{
	printf("usage: %s [-w -h] addr [value]\n", name);
	printf("       addr, physical address to be read or write.\n");
	printf("       value, write this value to address. if no value typed, indicate a read access.\n");
	printf("       -w word access(default)\n");
	printf("       -h half word access\n");
	printf("       -c byte access\n");

	printf("example:\n");
	printf("       %s 0x13000000\n", name);
	printf("       %s 0x13000000 0x002AAAAA\n", name);
	printf("       %s -h 0x13050100\n", name);
	printf("       %s -h 0x13050100 0x000D\n", name);

}


int main(int argc, char *argv[])
{
	int ret;
	if (argc < 2) {
		show_usage(argv[0]);
		return -1;
	}

	enum ACCESS_ALIGN access_align;
	access_align = WORD_ACCESS;

	enum ACCESS_TYPE access_type = ACCESS_READ; /* default read */

	unsigned int phys_address = 0;
	unsigned int write_value = 0;

	int ii = 1;
	while ( ii < argc ) {
		if ( '-' == *(argv[ii])) {
			if ( 'c' == *(argv[ii]+1)) {
				access_align = BYTE_ACCESS;
			}
			else if ( 'h' == *(argv[ii]+1)) {
				access_align = HALFWORD_ACCESS;
			}
			else if ( 'w' == *(argv[ii]+1)) {
				access_align = WORD_ACCESS;
			}
			else {
				printf("unknown args: %s\n", argv[ii]);
				show_usage(argv[0]);
				exit(-1);
			}

			++ii;
		}
		else {
			dprintf("argc=%d, ii=%d\n", argc, ii);
			phys_address = strtoul(argv[ii], NULL, 0);
			++ii;
			if ( argc == ii + 1) {
				access_type = ACCESS_WRITE;
				write_value = strtoul(argv[ii], NULL, 0);
				++ii;
			}
			else {
				access_type = ACCESS_READ;
			}

		}

	};

	dprintf("access_align = 0x%x\n", access_align);
	dprintf("access_type  = 0x%x\n", access_type);
	dprintf("phys_address = 0x%x\n", phys_address);
	dprintf("write_value  = 0x%x\n", write_value);

	if ( phys_address == 0 ) {
		show_usage(argv[0]);
		exit(1);
	}


	/* mmap address */
	//address
	unsigned int virt_addr;
	ret = mmap_physical_to_virtual(phys_address, &virt_addr);
	if ( ret != 0 ) {
		printf("mmap_physical_to_virtual(%08x, ) failed ret=%d\n", phys_address, ret);
		exit(-1);
	}
	dprintf("virt_addr= 0x%08X\n", virt_addr);

//	if (access_align == WORD_ACCESS) {
		//volatile unsigned int * addr = (unsigned int*) virt_addr;
	unsigned int addr = (unsigned int) virt_addr;
	dprintf("addr=%08x\n", addr);
	if (access_type == ACCESS_READ) {
		unsigned int val;
		switch (access_align) {
		case BYTE_ACCESS:
			val = REG8(addr);
			break;
		case HALFWORD_ACCESS:
			val = REG16(addr);
			break;
		default:
			val = REG32(addr);
			break;
		}
		printf("0x%08X: 0x%08X\n", phys_address, (int)val);
	}
	else if (access_type == ACCESS_WRITE) {
		dprintf("*0x%08X=0x%08x\n", addr, write_value);
		switch (access_align) {
		case BYTE_ACCESS:
			REG8(addr) = write_value;
			break;
		case HALFWORD_ACCESS:
			REG16(addr) = write_value;
			break;
		default:
			REG32(addr) = write_value;
			break;
		}
	}



	munmap_address();

	return 0;
}
