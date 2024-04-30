/*#############################################################
 *     File Name	: random_mac.c
 *     Author		: winddoing
 *     Created Time	: 2021年07月27日 星期二 20时10分23秒
 *     Description	: shell: echo $RANDOM|md5sum|sed 's/../&:/g'|cut -c 1-17
 *  dd if=/dev/urandom count=1 2>/dev/null | md5sum | sed 's/^\(.\)\(..\)\(..\)\(..\)\(..\)\(..\).*$/\14:\2:\3:\4:\5:\6/g' 
 *############################################################*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include<time.h>
#include<unistd.h>

#define RANDOM(x) (rand()%x)
#define MAC_ADDR_LENGTH 12
#define FMT_MAC_ADDR_LEN (MAC_ADDR_LENGTH+5)

unsigned char HEXCHAR[16]={'0','1','2','3','4','5','6','7','8','9','a','b','c', 'd','e','f'};
unsigned char genMACAddr[MAC_ADDR_LENGTH];
unsigned char fmtMACAddr[FMT_MAC_ADDR_LEN]={'0','0',':','0','0',':','0','0',':', '0','0',':','0','0',':','0','0'};

void formatMACAddr()
{
    unsigned short i=0;
    unsigned short n=0;
    for(i=0;i<MAC_ADDR_LENGTH;i++,n++) {
        if(fmtMACAddr[n]!=':') {
            fmtMACAddr[n]=genMACAddr[i];
        } else {
            n++;
            fmtMACAddr[n]=genMACAddr[i];
        }
    }
}

void main()
{
    unsigned short i=0;
    unsigned short n=0;

    srand(getpid());

    for(int i=0;i<MAC_ADDR_LENGTH;i++) {
        n=RANDOM(16);
        genMACAddr[i]=HEXCHAR[n];
    }

    formatMACAddr();

    printf("%s\n",fmtMACAddr);
}
