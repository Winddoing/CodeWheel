/*
    I2cAadapter.h - i2c-adapter-class, i2c device interface

	Copyright (c) 2016 Ingenic Semiconductor Co., Ltd.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
    MA 02110-1301 USA.
*/


class I2cAdapter{

	int fd;

public:

	I2cAdapter(unsigned char i2c_adapter_nr);

	~I2cAdapter();

	/**
	 * @brief open i2c adapter
	 *
	 * @param i2c_adapter_nr i2c number
	 *
	 * @return fd
	 */
	int I2cOpenAdapter(unsigned char i2c_adapter_nr);

	/**
	 * @brief set i2c slave
	 *
	 * @param addr slave device addr
	 *
	 * @return slave device addr
	 */
	unsigned char I2cSetSlave(unsigned char addr);

	/**
	 * @brief write byte to i2c slave
	 *
	 * @param byte being written
	 *
	 * @return
	 *      success ---> 0
	 *      fail  ---> -1
	 */
	int I2cWriteByte(unsigned char byte);

	/**
	 * @brief write bytes to i2c slave
	 *
	 * @param bytes being written
	 * @param length being written
	 *
	 * @return
	 *      success ---> 0
	 *      fail  ---> -1
	 */
	int I2cWriteBytes(unsigned char *bytes, unsigned char length);

	/**
	 * @brief read byte from i2c slave
	 *
	 * @param byte being read
	 *
	 * @return
	 *      success ---> byte
	 *      fail  ---> -1
	 */
	int I2cReadByte();

	/**
	 * @brief read bytes from i2c slave
	 *
	 * @param bytes being read
	 * @param length being read
	 *
	 * @return
	 *      success ---> length
	 *      fail  ---> -1
	 */
	int I2cReadBytes(unsigned char *buf, int length);

private:
	/**
	 * @brief I2C_RDWR ioctl()
	 *
	 * @param rdwr_data for ioctl
	 *
	 * @return
	 *      success ---> 0
	 *      fail  ---> -1
	 */
	int I2cRdwrAccess(struct i2c_rdwr_ioctl_data *rdwr_data);

public:
	/**
	 * @brief transfer rdwr_data
	 *
	 * @param msgs to transfer
	 * @param num of msg
	 *
	 * @return
	 *      success ---> 0
	 *      fail  ---> -1
	 */
	int I2cTransfer(struct i2c_msg *msgs, int num);

	/**
	 * @brief issue a single I2C message in master transmit mode
	 *
	 * @param buf Data that will be written to the slave
	 * @param count How many bytes to write, must be less than 64k since msg.len is u16
	 *
	 * @return negative errno, or else the number of bytes written.
	 */
	int I2cMasterSend(unsigned char *buf, int count);

	/**
	 * @brief issue a single I2C message in master receive mode
	 *
	 * @param buf Where to store data read from slave
	 * @param count How many bytes to read, must be less than 64k since msg.len is u16
	 *
	 * @return negative errno, or else the number of bytes read.
	 */
	int I2cMasterRecv(unsigned char *buf, int count);

	/**
	 * @brief I2C_SMBUS ioctl
	 *
	 * @param read_write I2C_SMBUS_READ or I2C_SMBUS_WRITE
	 * @param command Byte interpreted by slave, for protocols which use such bytes
	 * @param size Size to be read or written
	 * @param data Data to be read or written
	 *
	 * @return negative errno code else zero on success.
	 */
	signed int I2cSmbusAccess(char read_write, unsigned char command,
                                     int size, union i2c_smbus_data *data);

	/**
	 * @brief I2C_SMBUS ioctl quick mode
	 *
	 * @param value write command
	 *
	 * @return negative errno code else zero on success.
	 */
	signed int I2cSmbusWriteQuick(unsigned char value);

	/**
	 * @brief SMBus "receive byte" protocol
	 *
	 * @return negative errno else the byte received from the device.
	 */
	signed int I2cSmbusReadByte();

	/**
	 * @brief SMBus "send byte" protocol
	 *
	 * @param value Byte to be sent
	 *
	 * @return negative errno  else zero on success.
	 */
	signed int I2cSmbusWriteByte(unsigned char value);

	/**
	 * @brief SMBus "read byte" protocol
	 *
	 * @param command Byte interpreted by slave
	 *
	 * @return negative errno else a data byte received from the device.
	 */
	signed int I2cSmbusReadByteData(unsigned char command);

	/**
	 * @brief SMBus "write byte" protocol
	 *
	 * @param command Byte interpreted by slave
	 * @param value Byte being written
	 *
	 * @return negative errno else zero on success.
	 */
	signed int I2cSmbusWriteByteData(unsigned char command,
                                              unsigned char value);

	/**
	 * @brief SMBus "read word" protocol
	 *
	 * @param command Byte interpreted by slave
	 *
	 * @return negative errno else a 16-bit unsigned "word" received from the device.
	 */
	signed int I2cSmbusReadWordData(unsigned char command);

	/**
	 * @brief SMBus "write word" protocol
	 *
	 * @param command Byte interpreted by slave
	 * @param value 16-bit "word" being written
	 *
	 * @return negative errno else zero on success.
	 */
	signed int I2cSmbusWriteWordData(unsigned char command,
                                              unsigned short int value);

	/**
	 * @brief SMBus "process call" protocol
	 *
	 * @param command Byte interpreted by slave
	 * @param value Byte being written
	 *
	 * @return negative errno else zero on success.
	 */
	signed int I2cSmbusProcessCall(unsigned char command, unsigned short int value);

	/**
	 * @brief SMBus "block read" protocol
	 *
	 * @param command Byte interpreted by slave
	 * @param values  Byte array into which data will be read; big enough to hold
	 * the data returned by the slave.  SMBus allows at most 32 bytes.
	 *
	 * @return negative errno else the number of data bytes in the slave's response.
	 */
	signed int I2cSmbusReadBlockData(unsigned char command,
                                              unsigned char *values);

	/**
	 * @brief SMBus "block write" protocol
	 *
	 * @param command Byte interpreted by slave
	 * @param length Size of data block; SMBus allows at most 32 bytes
	 * @param values Byte array which will be written.
	 *
	 * @return negative errno else zero on success.
	 */
	signed int I2cSmbusWriteBlockData(unsigned char command,
                                               unsigned char length, const unsigned char *values);
	/**
	 * @brief I2C "block read" protocol
	 *
	 * @param command Byte interpreted by slave
	 * @param values  Byte array into which data will be read; big enough to hold
	 * the data returned by the slave.  SMBus allows at most 32 bytes.
	 *
	 * @return the number of read bytes
	 */
	signed int I2cSmbusReadI2cBlockData(unsigned char command,
                                                  unsigned char length, unsigned char *values);

	/**
	 * @brief I2C "block write" protocol
	 *
	 * @param command Byte interpreted by slave
	 * @param length Size of data block; SMBus allows at most 32 bytes
	 * @param values Byte array which will be written.
	 *
	 * @return negative errno else zero on success.
	 */
	signed int I2cSmbusWriteI2cBlockData(unsigned char command,
                                                   unsigned char length,
                                                   const unsigned char *values);

	/**
	 * @brief SMBus "block process call" protocol
	 *
	 * @param command Byte interpreted by slave
	 * @param value Byte being written
	 *
	 * @return negative errno else zero on success.
	 */
	signed int I2cSmbusBlockProcessCall(unsigned char command,
                                                 unsigned char length, unsigned char *values);

};


