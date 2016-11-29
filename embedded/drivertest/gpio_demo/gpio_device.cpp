
#include "gpio_device.h"

#include <sys/poll.h>

GPIO_device::GPIO_device(const int gpio) :
	gpio_(gpio)
{
	std::cout << "Initialize the gpio:" << gpio << "\n";
	file_path_ = GPIOSysfsPath;
}

int GPIO_device::export_gpio(){
	char value[5];
	int rw, fd;
	char file_name[30];
	char gpio_name[30];

	sprintf(file_name, "%s/export", file_path_);
	fd = open(file_name, O_WRONLY);
	if(fd < 0){
		std::cout << "Could not open export file.\n";
		return -1;
	}

	sprintf(gpio_name, "%s/gpio%d", file_path_, gpio_);
	if(!access(gpio_name, 0))
		return 0;

	sprintf(value, "%d", gpio_);
	rw = write(fd, value, 5);
	if(rw < 0){
		std::cout << "wirte error.\n";
		return -1;
	}

	fsync(fd);
	close(fd);

	return 0;
}

int GPIO_device::write_gpio(const char *type, const char *value){
	/*	char value[5];*/
	int rw, fd;
	char file_name[100];

	sprintf(file_name, "%s/gpio%d/%s", file_path_, gpio_, type);
	fd = open(file_name, O_WRONLY);
	if(fd < 0){
		std::cout << "Could not open file" << type;
		return -1;
	}

	rw = write(fd, value, 8);
	if(rw < 0){
		std::cout << "write error\n";
		return -1;
	}

	fsync(fd);
	close(fd);

	return 0;
}

bool GPIO_device::poll_gpio(){
	char buf[10];
	struct pollfd fdset[1];
	int nfds = 1;
	int fd = -1;
	int rc = -1;
	int len = -1;
	char gpio_path[60];

	sprintf(gpio_path, "%s/gpio%d/value", file_path_, gpio_);
	fd = open(gpio_path, O_RDONLY | O_NONBLOCK);
	if(fd < 0){
		std::cout << "GPIO open error\n";
		return false;
	}
	// Use this sawZeroValue to ignore the first spurious interrupt.
	bool sawZeroValue = false;

	while(1) {
		memset((void*)fdset, 0, sizeof(fdset));
		fdset[0].fd = fd;
		fdset[0].events = POLLPRI;

		rc = poll(fdset, nfds, PollTimeoutMsecs);
		if (rc < 0) {
			std::cout << "poll() failed!\n";
			close(fd);
			return false;
		} else if (rc == 0) {
			std::cout << "polling...\n";
		} else if (fdset[0].revents & POLLPRI) {
			len = read(fdset[0].fd, buf, 10 - 1);
			lseek(fdset[0].fd, 0, SEEK_SET);
			if (len >= 0) {
				buf[len] = '\0';
			}
			// might result in multiple events being read.
			if (buf[0] == '0') {
				std::cout << "First interrupt with value 0 ignored.";
				sawZeroValue = true;
				continue;
			}
			if (sawZeroValue && buf[0] == '1') {
				std::cout<< "GPIO pin reset and interrupt with value 1 caught.";
				close(fd);
				return true;
			}
		}

	}
}
