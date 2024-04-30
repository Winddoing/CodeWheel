/*下列几种方法均优先输出非回环IP:127.0.0.1
  inet_ntoa(): 系统函数,将网络的二进制数IP转化成点分十进制IP
  inet_ntop(): 系统函数,新型函数,作用与inet_ntoa()相似

  inet_aton(): 系统函数,将点分十进制IP转化成网络的二进制数IP
  inet_pton(): 系统函数,新型函数,inet_aton()相似
  */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

//使用 ifconf结构体和ioctl函数时需要用到该头文件
#include <net/if.h>
#include <sys/ioctl.h>

//使用ifaddrs结构体时需要用到该头文件
#include <ifaddrs.h>


#define PJ_MAX_HOSTNAME  (128)

#define RUN_SUCCESS 0
#define RUN_FAIL -1

/* 该方法一般取出的ip为 127.0.0.1 ,windows也可以使用此类方法,但是需要略为改动*/
int get_local_ip_using_hostname(char *str_ip)
{
    int status = RUN_FAIL;
    int i = 0;
    char buf[PJ_MAX_HOSTNAME] = {0};
    char *local_ip = NULL;
    if (gethostname(buf, sizeof(buf)) == 0)
    {
        struct hostent *temp_he;
        temp_he = gethostbyname(buf);
        if (temp_he)
        {
            for(i = 0; temp_he->h_addr_list[i]; i++)
            {
                local_ip = NULL;
                local_ip = inet_ntoa(*(struct in_addr *)(temp_he->h_addr_list[i]));
                if(local_ip)
                {
                    strcpy(str_ip, local_ip);
                    status = RUN_SUCCESS;
                    if(strcmp("127.0.0.1", str_ip))
                    {
                        break;
                    }
                }
            }
        }
    }
    return status;
}


/*该方法需要能够够ping通 1.1.1.1(DNS服务器IP) , 而且该服务器 53 端口是开着的
  (也可以用一个能够connect的远程服务器IP代替),windows也可以使用此类方法,但是需要略为改动*/
int get_local_ip_using_create_socket(char *str_ip)
{
    int status = RUN_FAIL;
    int af = AF_INET;
    int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in remote_addr;
    struct sockaddr_in local_addr;
    char *local_ip = NULL;
    socklen_t len = 0;

    remote_addr.sin_family = AF_INET;
    remote_addr.sin_port = htons(53);
    remote_addr.sin_addr.s_addr = inet_addr("1.1.1.1");

    len =  sizeof(struct sockaddr_in);
    status = connect(sock_fd, (struct sockaddr*)&remote_addr, len);
    if(status != 0 ){
        printf("connect err \n");
    }

    len =  sizeof(struct sockaddr_in);
    getsockname(sock_fd, (struct sockaddr*)&local_addr, &len);

    local_ip = inet_ntoa(local_addr.sin_addr);
    if(local_ip)
    {
        strcpy(str_ip, local_ip);
        status = RUN_SUCCESS;
    }
    return status;
}

/*linux上支持（Android上也支持）， 此函数不仅能获取IP，还可以获取MAC地址、掩码和广播地址等*/
//支持多网卡检测
int get_local_ip_using_ifconf(char local_ip[][INET_ADDRSTRLEN], int cnt)
{
    int sock_fd, intrface;
    struct ifreq buf[INET_ADDRSTRLEN];
    struct ifconf ifc;
    char* tmp_local_ip = NULL;
    int local_ip_cnt = 0;

    if ((sock_fd = socket(AF_INET, SOCK_DGRAM, 0)) >= 0) {
        ifc.ifc_len = sizeof(buf);
        ifc.ifc_buf = (caddr_t)buf;
        if (!ioctl(sock_fd, SIOCGIFCONF, (char *)&ifc)) {
            intrface = ifc.ifc_len/sizeof(struct ifreq);
            while (intrface-- > 0) {
                if (!(ioctl(sock_fd, SIOCGIFADDR, (char *)&buf[intrface]))) {
                    tmp_local_ip = NULL;
                    tmp_local_ip = inet_ntoa(((struct sockaddr_in*)(&buf[intrface].ifr_addr))->sin_addr);
                    if (tmp_local_ip) {
                        strcpy(local_ip[local_ip_cnt], tmp_local_ip);
                        local_ip_cnt++;
                    }
                }
            }
        }
        close(sock_fd);
    }
    return local_ip_cnt;
}

/*可以检测ip4也可以检测ip6，但是需要ifaddrs.h，某些Android系统上没有该头文件（可自己实现该头文件所带内容）
  该方法较为强大，可以通过网卡名（ifaddrs_struct->ifr_name）获取IP.
  */
int get_local_ip_using_ifaddrs(char local_ip[][INET_ADDRSTRLEN], int cnt)
{
    struct ifaddrs * ifaddrs_struct=NULL;
    void * tmp_addr_ptr=NULL;
    char tmp_local_ip[INET_ADDRSTRLEN] = {0};
    int local_ip_cnt = 0;
    int i;

    getifaddrs(&ifaddrs_struct);
    if (!ifaddrs_struct) {
        printf("%s: getifaddrs error.\n", __func__);
        return -1;
    }

    for (i = 0; i < cnt; i++) {
        if (ifaddrs_struct->ifa_addr->sa_family==AF_INET) {
            //ipv4
            tmp_addr_ptr = &((struct sockaddr_in *)ifaddrs_struct->ifa_addr)->sin_addr;
            if (inet_ntop(AF_INET, tmp_addr_ptr, tmp_local_ip, INET_ADDRSTRLEN)) {
                strcpy(local_ip[local_ip_cnt], tmp_local_ip);
                local_ip_cnt++;
            }
        } else if (ifaddrs_struct->ifa_addr->sa_family==AF_INET6){
            //ipv6
        }
        ifaddrs_struct=ifaddrs_struct->ifa_next;
    }

    return local_ip_cnt;
}

/*使用命令获取IP,此首要条件是需要系统支持相关命令,限制性条件较多,下面该方法是获取本机的ip地址*/
int get_netway_ip_using_res(char local_ip[][INET_ADDRSTRLEN], int cnt)
{
    FILE *fp = NULL;
    //char do_comment[] = "ifconfig | grep 'inet ' | awk '{print $2}' | sed 's/.*://g'";
    char do_comment[] = "ifconfig | grep 'inet ' | awk '{print $2}'";
    char tmp_local_ip[INET_ADDRSTRLEN] = {0};
    int local_ip_cnt = 0;

    fp = popen(do_comment, "r");
    if(!fp) {
        printf("%s: cmd[%s] run error.\n", __func__, do_comment);
        return -1;
    }

    while( !feof(fp) ) {
        fgets(local_ip[local_ip_cnt], 1024, fp);
        local_ip_cnt++;
    }

    fclose(fp);

    return local_ip_cnt;
}

int main()
{
    char local_ip1[INET_ADDRSTRLEN] = {0};
    char local_ip2[INET_ADDRSTRLEN] = {0};
    char local_ip3[INET_ADDRSTRLEN] = {0};
    char local_ip4[INET_ADDRSTRLEN] = {0};
    char local_ip5[INET_ADDRSTRLEN] = {0};

    if( get_local_ip_using_hostname(local_ip1) ==  RUN_SUCCESS)
    {
        printf("get_local_ip_using_hostname() get local ip : %s \n", local_ip1);
    }else{
        printf("get_local_ip_using_hostname() err \n");
    }

    if( get_local_ip_using_create_socket(local_ip2) == RUN_SUCCESS)
    {
        printf("get_local_ip_using_socket() get local ip : %s \n", local_ip2);
    }else{
        printf("get_local_ip_using_socket() err \n");
    }

    char local_ip_array[10][INET_ADDRSTRLEN] = {0};
    int cnt, i;

    cnt = get_local_ip_using_ifconf(local_ip_array, 10);
    for (i = 0; i < cnt; i++) {
        printf("%s: local_ip_%d=%s\n", __func__, i + 1, local_ip_array[i]);
    }


    printf("===> func: %s, line: %d\n", __func__, __LINE__);
    cnt = get_local_ip_using_ifaddrs(local_ip_array, 10);
    for (i = 0; i < cnt; i++) {
        printf("%s: local_ip_%d=%s\n", __func__, i + 1, local_ip_array[i]);
    }

    printf("===> func: %s, line: %d\n", __func__, __LINE__);
    cnt = get_netway_ip_using_res(local_ip_array, 10);
    for (i = 0; i < cnt; i++) {
        printf("%s: local_ip_%d=%s\n", __func__, i + 1, local_ip_array[i]);
    }


    return 0;
}
