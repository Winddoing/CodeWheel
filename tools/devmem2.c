/*
 * Copyright (C) 2000, Jan-Derk Bakker (J.D.Bakker@its.tudelft.nl)
 * Copyright (C) 2008, BusyBox Team. -solar 4/26/08
 * Licensed under GPLv2 or later, see file LICENSE in this source tree.
 */

//usage:#define devmem_trivial_usage
//usage:	"ADDRESS [WIDTH [VALUE]]"
//usage:#define devmem_full_usage "\n\n"
//usage:       "Read/write from physical address\n"
//usage:     "\n	ADDRESS	Address to act upon"
//usage:     "\n	WIDTH	Width (8/16/...)"
//usage:     "\n	VALUE	Data to be written"

#define _GNU_SOURCE 1 /* for strchrnul */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <inttypes.h> //strchrnul

#define ENABLE_FEATURE_CLEAN_UP 1

#define ALIGN1 __attribute__((aligned(1)))
#define ALIGN2 __attribute__((aligned(2)))
#define ALIGN4 __attribute__((aligned(4)))

#define FATAL(str) do { \
	fprintf(stderr, "<%s> Error at line %d, file %s (%d) [%s]\n", \
			str, __LINE__, __FILE__, errno, strerror(errno)); \
	exit(1); \
} while(0)

static void bb_show_usage()
{
	printf("Read/write from physical address\n\n");
	printf("Usage: devmem2 ADDRESS [WIDTH [VALUE]]\n");
	printf("\tADDRESS Address to act upon\n");
	printf("\tWIDTH   Width (8/16/...)\n");
	printf("\tVALUE   Data to be written\n");
	printf("\n");
	exit(1);
}

int main(int argc, const char *argv[])
{
	void *map_base, *virt_addr;
	uint64_t read_result;
	off_t target;
	unsigned page_size, mapped_size, offset_in_page;
	int fd;
	unsigned width = 8 * sizeof(int);

	/* devmem ADDRESS [WIDTH [VALUE]] */
// TODO: options?
// -r: read and output only the value in hex, with 0x prefix
// -w: write only, no reads before or after, and no output
// or make this behavior default?
// Let's try this and see how users react.

	/* ADDRESS */
	if (!argv[1])
		bb_show_usage();
	errno = 0;
	target = strtoull(argv[1], NULL, 0); /* allows hex, oct etc */

	/* WIDTH */
	if (argv[2]) {
		if (isdigit(argv[2][0]) || argv[2][1])
			width = atoi(argv[2]);
		else {
			static const char bhwl[] ALIGN1 = "bhwl";
			static const uint8_t sizes[] ALIGN1 = {
				8 * sizeof(char),
				8 * sizeof(short),
				8 * sizeof(int),
				8 * sizeof(long),
				0 /* bad */
			};
			width = strchrnul(bhwl, (argv[2][0] | 0x20)) - bhwl;
			width = sizes[width];
		}
	} else { /* argv[2] == NULL */
		/* make argv[3] to be a valid thing to fetch */
		argv--;
	}
	if (errno)
		bb_show_usage(); /* one of bb_strtouXX failed */

	fd = open("/dev/mem", argv[3] ? (O_RDWR | O_SYNC) : (O_RDONLY | O_SYNC));
	mapped_size = page_size = getpagesize();
	offset_in_page = (unsigned)target & (page_size - 1);
	if (offset_in_page + width > page_size) {
		/* This access spans pages.
		 * Must map two pages to make it possible: */
		mapped_size *= 2;
	}
	map_base = mmap(NULL,
			mapped_size,
			argv[3] ? (PROT_READ | PROT_WRITE) : PROT_READ,
			MAP_SHARED,
			fd,
			target & ~(off_t)(page_size - 1));
	if (map_base == MAP_FAILED)
		FATAL("mmap");

//	printf("Memory mapped at address %p.\n", map_base);

	virt_addr = (char*)map_base + offset_in_page;

	if (!argv[3]) {
#ifdef __SIZEOF_INT128__
		if (width == 128) {
			unsigned __int128 rd =
				*(volatile unsigned __int128 *)virt_addr;
			printf("0x%016llX%016llX\n",
				(unsigned long long)(uint64_t)(rd >> 64),
				(unsigned long long)(uint64_t)rd
			);
		} else
#endif
		{
			switch (width) {
			case 8:
				read_result = *(volatile uint8_t*)virt_addr;
				break;
			case 16:
				read_result = *(volatile uint16_t*)virt_addr;
				break;
			case 32:
				read_result = *(volatile uint32_t*)virt_addr;
				break;
			case 64:
				read_result = *(volatile uint64_t*)virt_addr;
				break;
			default:
				FATAL("bad width");
			}
//			printf("Value at address 0x%"OFF_FMT"X (%p): 0x%llX\n",
//				target, virt_addr,
//				(unsigned long long)read_result);
			/* Zero-padded output shows the width of access just done */
			printf("0x%0*lX -> 0x%0*llX\n", (width >> 2), target, (width >> 2), (unsigned long long)read_result);
		}
	} else {
		/* parse VALUE */
#ifdef __SIZEOF_INT128__
		unsigned __int128 writeval = strtoumax(argv[3], NULL, 0);
#else
		uint64_t writeval = bb_strtoull(argv[3], NULL, 0);
#endif
		switch (width) {
		case 8:
			*(volatile uint8_t*)virt_addr = writeval;
//			read_result = *(volatile uint8_t*)virt_addr;
			break;
		case 16:
			*(volatile uint16_t*)virt_addr = writeval;
//			read_result = *(volatile uint16_t*)virt_addr;
			break;
		case 32:
			*(volatile uint32_t*)virt_addr = writeval;
//			read_result = *(volatile uint32_t*)virt_addr;
			break;
		case 64:
			*(volatile uint64_t*)virt_addr = writeval;
//			read_result = *(volatile uint64_t*)virt_addr;
			break;
#ifdef __SIZEOF_INT128__
		case 128:
			*(volatile unsigned __int128 *)virt_addr = writeval;
//			read_result = *(volatile uint64_t*)virt_addr;
			break;
#endif
		default:
			FATAL("bad width");
		}
//		printf("Written 0x%llX; readback 0x%llX\n",
//				(unsigned long long)writeval,
//				(unsigned long long)read_result);
	}

	if (ENABLE_FEATURE_CLEAN_UP) {
		if (munmap(map_base, mapped_size) == -1)
			FATAL("munmap");
		close(fd);
	}

	return EXIT_SUCCESS;
}
