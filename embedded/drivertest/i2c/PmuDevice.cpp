#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "i2c.h"
#include "I2cAdapter.h"
#include "i2c-dev.h"


class PmuDevice{
public:
	int PmuSmbReadByte(unsigned char reg, unsigned char slave_addr);
};


	I2cAdapter Phoenix(1);

int PmuDevice:: PmuSmbReadByte(unsigned char reg, unsigned char slave_addr)
{
	unsigned char msgbuf0[1];
	unsigned char msgbuf1[1];
	unsigned char cmd=reg;

	struct i2c_msg msg[2];

			msg[0].addr = slave_addr,
			msg[0].flags = 0,
			msg[0].len = 1,
			msg[0].buf = &cmd,

			msg[1].addr = slave_addr,
			msg[1].flags = I2C_M_RD,
			msg[1].len = 1,
			msg[1].buf = msgbuf1,


	Phoenix.I2cTransfer(msg, 2);

	printf("msgbuf1=%x\n", msgbuf1[0]);

}



int main()
{
	PmuDevice Pmu;

	unsigned char slave_addr = Phoenix.I2cSetSlave(0x32);

	Pmu.PmuSmbReadByte(0x2c, slave_addr);

}
