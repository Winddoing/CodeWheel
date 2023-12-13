#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/utsname.h>
#include <dirent.h>
#include <linux/input.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>
#include <stdlib.h>
#include <linux/uinput.h>
#include <unistd.h>

#define UINPUT_PATH "/dev/uinput"

static int createKeyDevice(){
	int fd;
	int i;
	/* uInput device structure*/
	struct uinput_user_dev uinp;
	
	memset(&uinp,0,sizeof(uinp));

	fd = open(UINPUT_PATH, O_WRONLY | O_NDELAY);
	if (fd <0){
		printf("unable to open %s\n",UINPUT_PATH);
		return -1;
	}
	
		//Setup the uinput device
	if((ioctl(fd, UI_SET_EVBIT, EV_SYN) < 0)
		|| (ioctl(fd, UI_SET_EVBIT, EV_KEY) < 0)
		|| (ioctl(fd, UI_SET_KEYBIT, BTN_LEFT) < 0)
		|| (ioctl(fd, UI_SET_KEYBIT, BTN_MIDDLE) < 0)
		|| (ioctl(fd, UI_SET_KEYBIT, BTN_RIGHT) < 0)
		|| (ioctl(fd, UI_SET_EVBIT, EV_REL) < 0)
		|| (ioctl(fd, UI_SET_RELBIT, REL_X) < 0)
		|| (ioctl(fd, UI_SET_RELBIT, REL_Y) < 0)
		|| (ioctl(fd, UI_SET_RELBIT, REL_WHEEL) < 0)){
		printf("uninput ioctl set attr error\n");
		return -1;
	}

	for (i=0; i < 256; i++){
		if(ioctl(fd, UI_SET_KEYBIT, i) <0){
			printf("UI_SET_KEYBIT (%d) error\n",i);
			return -1;
		}
	}

	snprintf(uinp.name, UINPUT_MAX_NAME_SIZE, "virtual_button");
	uinp.id.vendor	= 0x1;
	uinp.id.product = 0x2;
	uinp.id.version = 1;
	uinp.id.bustype = BUS_I2C;
	
	if(write(fd, &uinp, sizeof(uinp)) < 0){
		printf("[%s]:uinput fd write args err.\n",__FUNCTION__);
		return -1;
	}

	if(ioctl(fd, UI_DEV_CREATE))
	{
		printf("create uinput device err\n");
		return -1;
	}

	return fd;


}

static void injectKey(int fd, int kval)
{
	struct input_event event;
	gettimeofday(&event.time, 0);
	
	//按下kval键
	event.type = EV_KEY;
	event.value = 1;
	event.code = kval;
	write(fd, &event, sizeof(event));
	
	//同步，也就是把它报告给系统
	event.type = EV_SYN;
	event.value = 0;
	event.code = SYN_REPORT;
	write(fd, &event, sizeof(event));

	usleep(200000);
	memset(&event, 0, sizeof(event));
	gettimeofday(&event.time, 0);

	//松开kval键
	event.type = EV_KEY;
	
	event.value = 0;
	event.code = kval;
	write(fd, &event, sizeof(event));
	
	//同步，也就是把它报告给系统
	event.type = EV_SYN;
	event.value = 0;
	event.code = SYN_REPORT;
	write(fd, &event, sizeof(event));
	
	return;
}
	
int main(int argc, char *argv[]){
	int fd;
	int key;
	int ch;
	fd = createKeyDevice();
	key = atoi(argv[1]);
	printf("send key value %d\n", key);
	injectKey(fd, key);
	
	while(ch = fgetc(stdin)){
		printf("send key value %d\n", 2);
		injectKey(fd, 2);
	}
}