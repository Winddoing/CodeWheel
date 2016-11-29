#include <iostream>

#include "gpio_device.h"

int main(){
	GPIO_device phoenix(12);
	phoenix.export_gpio();

	std::cout << "Set pin 12 with out\n";
	phoenix.write_gpio("direction", "out");
	std::cout << "Set pin 12 with low\n";
	phoenix.write_gpio("direction", "low");

/*	if(phoenix.poll_gpio()){*/
/*		std::cout << "Pin 12 pushd\n";*/
/*	}*/
	return 0;
}
