#include <stdio.h>  
#include <string.h> 
#include <stdlib.h>  
#include <dirent.h>  
#include <unistd.h>  
#include <signal.h>
#include <errno.h>
#include <fcntl.h>
#include <paths.h>
#include <unistd.h>
#include <termios.h>
#include <time.h>
#include <pthread.h>
#include <linux/types.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <sys/types.h> 
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <hardware_legacy/power.h>
#include <cutils/properties.h>
#include <log/log.h>
#include <netinet/in.h>      
#include <arpa/inet.h>     
#include <netinet/tcp.h>
#include <netdb.h>

#define MAXLINE 4096
struct vx_input_event {
        uint16_t type;
        uint16_t code;
        union {
                uint32_t value;
                struct {
                        uint16_t x;
                        uint16_t y;
                        int16_t dx;
                        int16_t dy;
                } pos;
        } v;
};
struct vmt_event {
        unsigned int x;
        int xflag;
        unsigned int y;
        int yflag;
        int slot;
        int trackingid;
};


struct vmt_event touch_point_status;
struct vx_input_event  key_input;
struct vx_input_event *recive_input;
static int dev_fd = -1;
unsigned int trackingid_nr = 0;
int pre_slot = 0;
int connfd = -1;
char client_ip[NI_MAXHOST + 1] = {0};
char client_port[NI_MAXSERV + 1] = {0};
//char local_ip[NI_MAXHOST + 1] = {0};
pthread_t read_tid;

#define DEV_UINPUT "/dev/uinput"
#define UINPUT_VTST "VTST-MultiTouch"
#define LOCAL_LOOP_IP "127.0.0.1"


void inetAddressStr(const struct sockaddr *addr, socklen_t addrlen, char *output_ip, char *output_port) {
    char host[NI_MAXHOST], service[NI_MAXSERV];
    if(getnameinfo(addr, addrlen, host, NI_MAXHOST, service,
            NI_MAXSERV, NI_NUMERICSERV | NI_NUMERICHOST) == 0) {
        strcpy(output_ip, host);
        strcpy(output_port, service);
    }
}

static int device_writeEvent(int fd, uint16_t type, uint16_t keycode, int32_t value) {
    struct input_event ev;  
    memset(&ev, 0, sizeof(struct input_event)); 
    ev.type = type;  
    ev.code = keycode;  
    ev.value = value;
    // char property_value[PROPERTY_VALUE_MAX];
    // property_get("debug.touchscreen.write_event", property_value, "false");
    // bool debug_write = !strcmp(property_value, "true");
    if (write(fd, &ev, sizeof(struct input_event)) < 0) {
        //if (debug_write) {
            //ALOGE("GSP device_writeEvent error ev.type = %04x, ev.code = %04x, ev.value = %08x", ev.type, ev.code, ev.value);
        //}
        ALOGE("GSP device_writeEvent error ev.type = %04x, ev.code = %04x, ev.value = %08x", ev.type, ev.code, ev.value);
        return 0;
    }
    //if (debug_write) {
        //ALOGD("GSP device_writeEvent ev.type = %04x, ev.code = %04x, ev.value = %08x", ev.type, ev.code, ev.value); //__FILE__,__FUNCTION__,__LINE__
    //}
    return 1;
}

 
static void createTouchScreen() {
    int uinp_fd;
    struct uinput_user_dev uinp;

    uinp_fd = open(DEV_UINPUT, O_WRONLY|O_NONBLOCK);
    if (uinp_fd < 0) {
        //printf("VTST Multi-Touch Unable to open /dev/uinput\n");
        ALOGE("GSP VTST Multi-Touch Unable to open /dev/uinput");
        return;
    }
    dev_fd = uinp_fd;
    memset(&uinp, 0, sizeof(uinp));
    //ALOGE("GSP UINPUT_MAX_NAME_SIZE = %d, %d", UINPUT_MAX_NAME_SIZE, sizeof(uinp.name));  //80 80
    //strncpy(uinp.name, UINPUT_VTST, UINPUT_MAX_NAME_SIZE);
    snprintf(uinp.name, sizeof(uinp.name), "%s", UINPUT_VTST);

    uinp.id.bustype = BUS_VIRTUAL;
    uinp.id.version = 1;
    uinp.absmin[ABS_MT_SLOT] = 0;
    uinp.absmax[ABS_MT_SLOT] = 9;
    uinp.absmin[ABS_MT_TRACKING_ID] = 0;
    uinp.absmax[ABS_MT_TRACKING_ID] = 65535;
    uinp.absmin[ABS_MT_POSITION_X] = 0;
    uinp.absmax[ABS_MT_POSITION_X] = 65535;
    uinp.absmin[ABS_MT_POSITION_Y] = 0;
    uinp.absmax[ABS_MT_POSITION_Y] = 65535;

    ioctl(uinp_fd, UI_SET_EVBIT, EV_ABS);
    ioctl(uinp_fd, UI_SET_EVBIT, EV_KEY);
    ioctl(uinp_fd, UI_SET_EVBIT, EV_SYN);
    ioctl(uinp_fd, UI_SET_ABSBIT, ABS_MT_SLOT);
    ioctl(uinp_fd, UI_SET_ABSBIT, ABS_MT_POSITION_X);
    ioctl(uinp_fd, UI_SET_ABSBIT, ABS_MT_POSITION_Y);
    ioctl(uinp_fd, UI_SET_ABSBIT, ABS_MT_TRACKING_ID);
    ioctl(uinp_fd, UI_SET_KEYBIT, KEY_BACK);
    ioctl(uinp_fd, UI_SET_KEYBIT, KEY_POWER);
    ioctl(uinp_fd, UI_SET_KEYBIT, KEY_HOMEPAGE);
    ioctl(uinp_fd, UI_SET_KEYBIT, KEY_MENU);
    ioctl(uinp_fd, UI_SET_KEYBIT, BTN_TOUCH);
    ioctl(uinp_fd, UI_SET_KEYBIT, KEY_VOLUMEDOWN);
    ioctl(uinp_fd, UI_SET_KEYBIT, KEY_VOLUMEUP);

    ioctl(uinp_fd, UI_SET_PROPBIT, INPUT_PROP_DIRECT);
    write(uinp_fd, &uinp, sizeof(uinp));
    ioctl(uinp_fd, UI_DEV_CREATE);
}
 
static void touch_move(int fd, int x, int xflag, int y, int yflag, int slot) {
    //ALOGD("GSP touch_move slot = %d, x = %d, y = %d, xflag = %d, yflag = %d ", slot, x, y, xflag, yflag);
    fd = dev_fd;
    if (slot != pre_slot){
        device_writeEvent(fd, EV_ABS, ABS_MT_SLOT, slot);
        pre_slot = slot;
    }

    if (xflag == 1) {
        device_writeEvent(fd, EV_ABS, ABS_MT_POSITION_X, x);
    }
    if (yflag == 1) {
        device_writeEvent(fd, EV_ABS, ABS_MT_POSITION_Y, y);
    }
}
    

static void touch_down(int fd, int x, int xflag, int y, int yflag, int slot) {
    //ALOGD("GSP touch_down trackingid_nr = %d", trackingid_nr);
    if (trackingid_nr >= 65535) {
        trackingid_nr = 0;
    } else {
        trackingid_nr++;
    }
    //device_writeEvent(fd, EV_KEY, BTN_TOUCH, 1);

    if (xflag != 1 || yflag != 1) {
        ALOGD("GSP touch_down xflag = %d, yflag = %d", xflag, yflag);
    }

    fd = dev_fd;
    if (slot != pre_slot) {
        device_writeEvent(fd, EV_ABS, ABS_MT_SLOT, slot);
        pre_slot = slot;
    }

    device_writeEvent(fd, EV_ABS, ABS_MT_TRACKING_ID, trackingid_nr);
    if (xflag == 1) {
        device_writeEvent(fd, EV_ABS, ABS_MT_POSITION_X, x);
    }
    if (yflag == 1) {
        device_writeEvent(fd, EV_ABS, ABS_MT_POSITION_Y, y);
    }
} 

static void touch_up(int slot) {
    //ALOGD("GSP touch_up slot = %d", slot);
    int fd;
    fd = dev_fd;
    if (slot != pre_slot) {
        device_writeEvent(fd, EV_ABS, ABS_MT_SLOT, slot);
        pre_slot = slot;
    }
    device_writeEvent(fd, EV_ABS, ABS_MT_TRACKING_ID, -1);
}

static void initTouchPointStatus() {
    touch_point_status.x = 0;
    touch_point_status.y = 0;
    touch_point_status.slot = -1;
    touch_point_status.trackingid = 0;
    touch_point_status.xflag = 0;
    touch_point_status.yflag = 0;
    //ALOGD("GSP initTouchPointStatus");
}

static void initKeyInput() {
    key_input.type = 0;
    key_input.code = 0;
    key_input.v.value = 0;
}

static void event_inject(struct vx_input_event *recive_input) {
    unsigned char buff[128] = {0};
    unsigned char datalen = 0;
    for ( ; ; ){
        if (connfd <= 0) {
            sleep(1);
            continue;
        }
        datalen = 0;
        int ret = recv(connfd, &datalen, 1, 0);
        if (ret <= 0) {
               //printf("recv datalen socket error!\n");
               ALOGE("GSP recv datalen socket %s!\n", ret == 0 ? "shutdown" : "error");
            close(connfd);
            connfd = -1;
            continue;
        }

        memset(buff, 0, sizeof(buff));
        ret = recv(connfd, buff, datalen, 0);
        if (ret <= 0) {
            //printf("recv data socket error!\n");
            ALOGE("GSP recv data socket %s!\n", ret == 0 ? "shutdown" : "error");
            close(connfd);
            connfd = -1;
            continue;
        }
        //ALOGD("GSP event_inject ret = %d, datalen = %u", ret, datalen);
        for (int i = 0; i < ret / 8; i++) {
            recive_input = (struct vx_input_event *)(buff + i * 8);
            //ALOGD("GSP recive_input->type = %04x", recive_input->type);
            if ((recive_input->type == EV_SYN) &&
                    (recive_input->code == SYN_REPORT) && (recive_input->v.value == 0)) {
                if (key_input.type == EV_KEY) {
                    device_writeEvent(dev_fd, key_input.type, key_input.code,key_input.v.value);
                    device_writeEvent(dev_fd, EV_SYN, SYN_REPORT, 0);
                    initKeyInput();
                }

                if (touch_point_status.slot != -1) {
                    //ALOGD("GSP touch_point_status : slot = %d, %d", touch_point_status.slot, pre_slot);
                    if (touch_point_status.trackingid == -1) {
                        touch_up(touch_point_status.slot);
                    } else if (touch_point_status.trackingid != 0) {
                         touch_down(0, touch_point_status.x, touch_point_status.xflag,
                            touch_point_status.y, touch_point_status.yflag, touch_point_status.slot);
                    } else {
                        touch_move(0, touch_point_status.x, touch_point_status.xflag,
                            touch_point_status.y, touch_point_status.yflag, touch_point_status.slot);
                    }
                    device_writeEvent(dev_fd, EV_SYN, SYN_REPORT, 0);
                    initTouchPointStatus();
                }
            } else if (recive_input->type == EV_KEY) {
                key_input.type = recive_input->type;
                key_input.code = recive_input->code;
                key_input.v.value = recive_input->v.value;
            } else if (recive_input->type == EV_ABS) {
                if (recive_input->code == ABS_MT_SLOT) {
                    touch_point_status.slot = recive_input->v.value;
                } else if (recive_input->code == ABS_MT_TRACKING_ID) {
                    touch_point_status.trackingid = recive_input->v.value;
                } else if (recive_input->code == ABS_MT_POSITION_X) {
                    touch_point_status.x = recive_input->v.value;
                    touch_point_status.xflag = 1;
                } else if (recive_input->code == ABS_MT_POSITION_Y) {
                    touch_point_status.y = recive_input->v.value;
                    touch_point_status.yflag = 1;
                }
                //ALOGD("GSP touch_point_status : %d, %d, %d, %d, %d, %d ", touch_point_status.slot, touch_point_status.trackingid,
                //    touch_point_status.x, touch_point_status.xflag, touch_point_status.y, touch_point_status.yflag);
            }
        }
    }
}

void* socketserver(void *arg) {
     arg=NULL;
     event_inject(recive_input);
     return NULL;
}

int main(void) {
    recive_input = (struct vx_input_event *)malloc(sizeof(struct vx_input_event));
    createTouchScreen();

    initKeyInput();
    initTouchPointStatus();

    //begin socket connect
    struct sockaddr_in remoteAddr;
    socklen_t nAddrlen = sizeof(remoteAddr);
    int listenfd,tmpfd;

    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(struct sockaddr_in));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(10001);

    if (-1 == (listenfd = socket(AF_INET, SOCK_STREAM, 0))) {
        //printf("Multi-Touch:can not create socket error:%s(errno:%d)\n",strerror(errno),errno);
        ALOGE("GSP Multi-Touch:can not create socket error:%s(errno:%d)\n",strerror(errno),errno);
        return 0;
    }

    if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1) {
        //printf("bind socket error: %s(errno: %d)\n", strerror(errno), errno);
        ALOGE("GSP bind socket error: %s(errno: %d)\n", strerror(errno), errno);
        return 0;
    }

    if (listen(listenfd, 1) == -1) {
        //printf("listen socket error : %s(errno: %d)\n", strerror(errno), errno);
        ALOGE("GSP listen socket error : %s(errno: %d)\n", strerror(errno), errno);
        return 0;
    }

    pthread_create(&read_tid, NULL, socketserver, NULL);

    while (1) {
        if ((tmpfd = accept(listenfd, (struct sockaddr *)&remoteAddr, &nAddrlen)) == -1) {
            //printf("accept socket error: %s(errno: %d)\n", strerror(errno), errno);
            ALOGE("GSP accept socket error: %s(errno: %d)\n", strerror(errno), errno);
            return 0;
        }
        //strcpy(local_ip, "127.0.0.1");
        inetAddressStr((struct sockaddr *)&remoteAddr, nAddrlen, client_ip, client_port);
        //ALOGE("GSP inetAddressStr client_ip : %s, client_port :%s", client_ip, client_port);
        if (!strcmp(client_ip, LOCAL_LOOP_IP)) {
            close(tmpfd);
        } else {
            connfd = tmpfd;
        }
        sleep(1);
    }
    close(listenfd);
    return 0;
}

