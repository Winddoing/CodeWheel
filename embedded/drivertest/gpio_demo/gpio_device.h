#include <iostream>
#include <stdio.h>
#include <string.h>

#include <fcntl.h>
#include <unistd.h>

const char* const GPIOSysfsPath = "/sys/class/gpio";
// GPIO polling timeout in milliseconds
const int PollTimeoutMsecs = 3000; /* 3 seconds */

class GPIO_device{

public:
	GPIO_device(const int gpio);
	/**
	 * @brief  Export GPIO to user space
	 *
	 * @returns
	 *		success ---> 0
	 *		fail  ---> -1
	 */
	int export_gpio();
	/**
	 * @brief   Write to GPIO
	 *
	 * @param type what GPIO type i.e. direction, edge
	 * @param value value to be written i.e in, out, high, low
	 * @returns
	 *		success ----> 0
	 *		fail    ----> -1
	 */
	int write_gpio(const char *type, const char *value);
	/**
	 * @brief  This method polls GPIO pin for 0 -> 1 transition.
	 *
	 * @returns
	 *		true if polling reads back successfully
	 *		false if GPIO open error or polling error
	 */
	bool poll_gpio();

private:
	const int gpio_;
	const char *file_path_;
};
