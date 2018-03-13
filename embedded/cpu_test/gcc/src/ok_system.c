#include <instructions.h>
#include <debug.h>
#include <exp_supp.h>
#include <test.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/_types.h>
#define __need_size_t
#define __need_ptrdiff_t
#include <stddef.h>

#define EXIT_PASS 0xa00ffff8
#define EXIT_FAIL 0xa00ffff4

#define FPGA_EXIT_LED0	0xa2100058
#define FPGA_EXIT_LED1	0xa2100059
#define FPGA_EXIT_LED2	0xa210005a
#define FPGA_EXIT_LED3  0xa210005b 
#define FPGA_EXIT_LED4  0xa210005c
#define FPGA_EXIT_LED5  0xa210005d
#define FPGA_EXIT_LED6  0xa210005e
#define FPGA_EXIT_LED7  0xa210005f

#define readb(addr) (*(volatile unsigned char *) (addr))
#define readw(addr) (*(volatile unsigned short *) (addr))
#define readl(addr) (*(volatile unsigned long *) (addr))
#define writeb(b,addr) (*(volatile unsigned char *) (addr) = (b))
#define writew(b,addr) (*(volatile unsigned short *) (addr) = (b))
#define writel(b,addr) (*(volatile unsigned int *) (addr) = (b))

#ifdef SIM_FPGA
volatile unsigned char *fpga_led=(unsigned char *)FPGA_EXIT_LED0;
int fpga_num=0;
#else
unsigned char fpga_led_data;
volatile unsigned char *fpga_led=&fpga_led_data;
#endif

void show_fpga_num(char *str)
{
#ifdef SIM_FPGA
  int i,len,m,n,k;
  extern int fpga_num;
  len = strlen (str);
  for (i=0; i<len; i++)
    fpga_led[i]=str[i];
  m = fpga_num / 100;
  n = fpga_num % 100 / 10;
  k = fpga_num % 10;
  fpga_led[5]=m+'0';
  fpga_led[6]=n+'0';
  fpga_led[7]=k+'0';
  fpga_num++;
#endif  
}

void exit(int i)
{
#ifdef SIM_FPGA
  show_fpga_num("FAIL!");
#else
  show_string("GCC FAIL!");
#endif
  REG32(EXIT_FAIL) = 1;
}
void pass()
{
#ifdef SIM_FPGA
  show_fpga_num("PASS!");
#else
  show_string("GCC PASS!");
#endif
  REG32(EXIT_PASS) = 1;
}
/*
char *printf()
{
  return (char *)0;
}

int fprintf()
{
  return 1;
}
*/

void free(){}
char *alloc_end;
int first_malloc = 1;
void *malloc(int size)
{
  extern long _end[3];
  char *p;
  int align;
  if (first_malloc) {
    first_malloc = 0;
    alloc_end = (char *)_end;
  }
  align = (int) alloc_end & 3;
  if (align)
    alloc_end += 4 - align;
  p = alloc_end;
  alloc_end += size;
  return p;
}

/*int _impure_ptr=0;*/
void Init_bss()
{
  unsigned int *idest, *iend_dest;
  unsigned char *dest, *end_dest;
  extern long _bss_start[5];
  extern long _end[3];
	idest = (unsigned int*)_bss_start;
	iend_dest = (unsigned int*)_end;	
	while (idest<iend_dest-32) 
	{
		idest[0] = 0;
		idest[1] = 0;
		idest[2] = 0;
		idest[3] = 0;
		idest[4] = 0;
		idest[5] = 0;
		idest[6] = 0;
		idest[7] = 0;
		idest += 8;
	}

	while (idest<iend_dest-4) 
	{
		*idest = 0;
		idest++;
	}
	dest = (unsigned char*)idest;
	end_dest = (unsigned char*)_end;	
	while (dest<end_dest) 
	{
		*dest++ = 0;
	}
}

int __errno()
{
  return 1;
}
