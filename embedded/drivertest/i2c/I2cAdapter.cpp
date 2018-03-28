/*
 * i2c.c - the i2c library
 *
 * Copyright (c) 2016 Ingenic Semiconductor Co., Ltd.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA  02111-1307  USA
 */

#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <malloc.h>
#include "i2c-dev.h"
#include "i2c.h"
#include "I2cAdapter.h"



unsigned char slave_addr;


#define MAX_BUF 100



I2cAdapter::I2cAdapter(unsigned char i2c_adapter_nr)
{
	this->fd = I2cOpenAdapter(i2c_adapter_nr);
}

I2cAdapter::~I2cAdapter()
{
}

int I2cAdapter:: I2cOpenAdapter(unsigned char I2cAdapter_nr)
{
	char buf[MAX_BUF];
	int i2c_fd;
	snprintf(buf, sizeof(buf), "/dev/i2c-%d", I2cAdapter_nr);
	printf("open buf=%s\n", buf);
	if ((i2c_fd = open(buf, O_RDWR)) < 1) {
		perror("Failed to open adapter");
		return -1;
	}
	return i2c_fd;
}

unsigned char I2cAdapter::I2cSetSlave(unsigned char addr)
{
	if (ioctl(this->fd, I2C_SLAVE_FORCE, addr) < 0) {
		fprintf(stderr, "Failed to set slave address %d:", addr);
		perror("");
		return -1;
	}
	slave_addr = addr;
	return slave_addr;
}

int I2cAdapter::I2cWriteByte(unsigned char byte)
{
	if (I2cSmbusWriteByte(byte ) < 0) {
		perror("Failed to write byte to I2C slave");
		return -1;
	}
	return 0;
}

int I2cAdapter::I2cWriteBytes(unsigned char *bytes, unsigned char length)
{
	if(I2cSmbusWriteI2cBlockData(bytes[0], length-1, bytes+1)
									< 0) {
		perror("Failed to write bytes to I2C slave");
		return -1;
	}
	return 0;
}

int I2cAdapter::I2cReadByte()
{
	int byte;
	if ((byte = I2cSmbusReadByte()) < 0) {
		// perror("Failed to read byte from I2C slave");
		return -1;
	}
	return byte;
}

int I2cAdapter::I2cReadBytes(unsigned char *buf, int length)
{
	if (read(this->fd, buf, length) == length)
		return length;
	else
		return -1;
}



int I2cAdapter::I2cRdwrAccess(struct i2c_rdwr_ioctl_data *rdwr_data)
{

	if(ioctl(this->fd, I2C_RDWR, rdwr_data) < 0) {
		perror("Unable to send data");
		return -1;
	}
	return 0;
}

int I2cAdapter::I2cTransfer(struct i2c_msg *msgs, int num)
{
	struct i2c_rdwr_ioctl_data rdwr_data;
	rdwr_data.msgs = msgs;
	rdwr_data.nmsgs = num;

	/* ioctl rdwr */
	return I2cRdwrAccess(&rdwr_data);
}

int I2cAdapter::I2cMasterSend(unsigned char *buf, int count)
{
	int ret;
	struct i2c_msg msg;

	msg.addr = slave_addr;
	msg.flags = !I2C_M_RD;
	msg.len = count;
	msg.buf = buf;

	ret = I2cTransfer(&msg, 1);

	/*
	 *    If everything went ok (i.e. 1 msg transmitted), return #bytes
	 *    transmitted, else error code.
	 */
	return (ret == 1) ? count : ret;
}

int I2cAdapter::I2cMasterRecv(unsigned char *buf, int count)
{

	struct i2c_msg msg;
	int ret;

	msg.addr = slave_addr;
	msg.flags = I2C_M_RD;
	msg.len = count;
	msg.buf = buf;

	ret = I2cTransfer(&msg, 1);

	/*
	 *    If everything went ok (i.e. 1 msg received), return #bytes received,
	 *    else error code.
	 */
	return (ret == 1) ? count : ret;

}



signed int  I2cAdapter::I2cSmbusAccess(char read_write, unsigned char command,
                                     int size, union i2c_smbus_data *data)
{
	struct i2c_smbus_ioctl_data args;

	args.read_write = read_write;
	args.command = command;
	args.size = size;
	args.data = data;
	return ioctl(this->fd,I2C_SMBUS,&args);
}


signed int  I2cAdapter::I2cSmbusWriteQuick(unsigned char value)
{
	return I2cSmbusAccess(value,0,I2C_SMBUS_QUICK,NULL);
}

signed int  I2cAdapter::I2cSmbusReadByte()
{
	union i2c_smbus_data data;
	if (I2cSmbusAccess(I2C_SMBUS_READ,0,I2C_SMBUS_BYTE,&data))
		return -1;
	else
		return 0x0FF & data.byte;
}

signed int  I2cAdapter::I2cSmbusWriteByte(unsigned char value)
{
	return I2cSmbusAccess(I2C_SMBUS_WRITE,value,
	                        I2C_SMBUS_BYTE,NULL);
}

signed int  I2cAdapter::I2cSmbusReadByteData(unsigned char command)
{
	union i2c_smbus_data data;
	if (I2cSmbusAccess(I2C_SMBUS_READ,command,
	                     I2C_SMBUS_BYTE_DATA,&data))
		return -1;
	else
		return 0x0FF & data.byte;
}

signed int  I2cAdapter::I2cSmbusWriteByteData(unsigned char command,
                                              unsigned char value)
{
	union i2c_smbus_data data;
	data.byte = value;
	return I2cSmbusAccess(I2C_SMBUS_WRITE,command,
	                        I2C_SMBUS_BYTE_DATA, &data);
}

signed int  I2cAdapter::I2cSmbusReadWordData(unsigned char command)
{
	union i2c_smbus_data data;
	if (I2cSmbusAccess(I2C_SMBUS_READ,command,
	                     I2C_SMBUS_WORD_DATA,&data))
		return -1;
	else
		return 0x0FFFF & data.word;
}

signed int  I2cAdapter::I2cSmbusWriteWordData(unsigned char command,
                                              unsigned short int value)
{
	union i2c_smbus_data data;
	data.word = value;
	return I2cSmbusAccess(I2C_SMBUS_WRITE,command,
	                        I2C_SMBUS_WORD_DATA, &data);
}

signed int  I2cAdapter::I2cSmbusProcessCall(unsigned char command, unsigned short int value)
{
	union i2c_smbus_data data;
	data.word = value;
	if (I2cSmbusAccess(I2C_SMBUS_WRITE,command,
	                     I2C_SMBUS_PROC_CALL,&data))
		return -1;
	else
		return 0x0FFFF & data.word;
}


/* Returns the number of read bytes */
signed int  I2cAdapter::I2cSmbusReadBlockData(unsigned char command,
                                              unsigned char *values)
{
	union i2c_smbus_data data;
	int i;
	if (I2cSmbusAccess(I2C_SMBUS_READ,command,
	                     I2C_SMBUS_BLOCK_DATA,&data))
		return -1;
	else {
		for (i = 1; i <= data.block[0]; i++)
			values[i-1] = data.block[i];
		return data.block[0];
	}
}

signed int  I2cAdapter::I2cSmbusWriteBlockData(unsigned char command,
                                               unsigned char length, const unsigned char *values)
{
	union i2c_smbus_data data;
	int i;
	if (length > 32)
		length = 32;
	for (i = 1; i <= length; i++)
		data.block[i] = values[i-1];
	data.block[0] = length;
	return I2cSmbusAccess(I2C_SMBUS_WRITE,command,
	                        I2C_SMBUS_BLOCK_DATA, &data);
}

/* Returns the number of read bytes */
/* Until kernel 2.6.22, the length is hardcoded to 32 bytes. If you
   ask for less than 32 bytes, your code will only work with kernels
   2.6.23 and later. */
signed int  I2cAdapter::I2cSmbusReadI2cBlockData(unsigned char command,
                                                  unsigned char length, unsigned char *values)
{
	union i2c_smbus_data data;
	int i;

	if (length > 32)
		length = 32;
	data.block[0] = length;
	if (I2cSmbusAccess(I2C_SMBUS_READ,command,
	                     length == 32 ? I2C_SMBUS_I2C_BLOCK_BROKEN :
	                      I2C_SMBUS_I2C_BLOCK_DATA,&data))
		return -1;
	else {
		for (i = 1; i <= data.block[0]; i++)
			values[i-1] = data.block[i];
		return data.block[0];
	}
}

signed int  I2cAdapter::I2cSmbusWriteI2cBlockData(unsigned char command,
                                                   unsigned char length,
                                                   const unsigned char *values)
{
	union i2c_smbus_data data;
	int i;
	if (length > 32)
		length = 32;
	for (i = 1; i <= length; i++)
		data.block[i] = values[i-1];
	data.block[0] = length;
	return I2cSmbusAccess(I2C_SMBUS_WRITE,command,
	                        I2C_SMBUS_I2C_BLOCK_BROKEN, &data);
}

/* Returns the number of read bytes */
signed int  I2cAdapter::I2cSmbusBlockProcessCall(unsigned char command,
                                                 unsigned char length, unsigned char *values)
{
	union i2c_smbus_data data;
	int i;
	if (length > 32)
		length = 32;
	for (i = 1; i <= length; i++)
		data.block[i] = values[i-1];
	data.block[0] = length;
	if (I2cSmbusAccess(I2C_SMBUS_WRITE,command,
	                     I2C_SMBUS_BLOCK_PROC_CALL,&data))
		return -1;
	else {
		for (i = 1; i <= data.block[0]; i++)
			values[i-1] = data.block[i];
		return data.block[0];
	}
}

