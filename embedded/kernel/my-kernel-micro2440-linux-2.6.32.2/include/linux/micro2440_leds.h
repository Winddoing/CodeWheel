#ifndef MICRO2440_lEDS_H
#define MICRO2440_lEDS_H

#include <asm/ioctl.h>

struct leds_stat{
	unsigned short led_nr;
#define LED_ON  0
#define LED_OFF 1
	unsigned short led_st;
};

#define LEDS_IOCTL_TYPE 0xF5

//none data
#define LEDS_ALL_ON    _IO(LEDS_IOCTL_TYPE, 0)
#define LEDS_ALL_OFF   _IO(LEDS_IOCTL_TYPE, 1)
#define LEDS_ALL_CHA   _IO(LEDS_IOCTL_TYPE, 2)

//write data
#define LEDS_SET_ONE   _IOW(LEDS_IOCTL_TYPE, 3, struct leds_stat)

//read data
#define LEDS_GET_ONE   _IOWR(LEDS_IOCTL_TYPE, 4, struct leds_stat)

#endif // MICRO2440_lEDS_H

