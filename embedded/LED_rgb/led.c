#include <stdio.h>
#include "led.h"

#define LED_POS 32
#define STATE_POS 5

static led_state[3] = {0};

void led_ctrl(char led, char state)
{
	if((state != '0') && (state != '1'))
		return;
	if(led != 'r' && led != 'g' &&  led != 'b')
		return;
	char command[] = "echo 1 > /sys/class/leds/wl_led_b/brightness";
	command[LED_POS] = led;
	command[STATE_POS] = state;
	if(led == 'r')
		if(state == '0')led_state[0] = 0;
		else led_state[0] = 1;
	if(led == 'g')
		if(state == '0')led_state[1] = 0;
		else led_state[1] = 1;
	if(led == 'b')
		if(state == '0')led_state[2] = 0;
		else led_state[2] = 1;
	system(command);
}
void get_led_state(char *state)
{
	state[0] = led_state[0];
	state[1] = led_state[1];
	state[2] = led_state[2];
}
