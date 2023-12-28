/* ifconfig.c - Configure network interface.
 *
 * Copyright 2012 Ranjan Kumar <ranjankumar.bth@gmail.com>
 * Copyright 2012 Kyungwan Han <asura321@gamil.com>
 * Reviewed by Kyungsu Kim <kaspyx@gmail.com>
 *
 * Not in SUSv4.

USE_IFCONFIG(NEWTOY(ifconfig, "^?a", TOYFLAG_SBIN))

config IFCONFIG
  bool "ifconfig"
  default y
  help
    usage: ifconfig [-a] [INTERFACE [ACTION...]]

    Display or configure network interface.

    With no arguments, display active interfaces. First argument is interface
    to operate on, one argument by itself displays that interface.

    -a	Show all interfaces, not just active ones

    Additional arguments are actions to perform on the interface:

    ADDRESS[/NETMASK] - set IPv4 address (1.2.3.4/5)
    default - unset ipv4 address
    add|del ADDRESS[/PREFIXLEN] - add/remove IPv6 address (1111::8888/128)
    up - enable interface
    down - disable interface

    netmask|broadcast|pointopoint ADDRESS - set more IPv4 characteristics
    hw ether|infiniband ADDRESS - set LAN hardware address (AA:BB:CC...)
    txqueuelen LEN - number of buffered packets before output blocks
    mtu LEN - size of outgoing packets (Maximum Transmission Unit)

    Flags you can set on an interface (or -remove by prefixing with -):
    arp - don't use Address Resolution Protocol to map LAN routes
    promisc - don't discard packets that aren't to this LAN hardware address
    multicast - force interface into multicast mode if the driver doesn't
    allmulti - promisc for multicast packets

    Obsolete fields included for historical purposes:
    irq|io_addr|mem_start ADDR - micromanage obsolete hardware
    outfill|keepalive INTEGER - SLIP analog dialup line quality monitoring
    metric INTEGER - added to Linux 0.9.10 with comment "never used", still true
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <net/ethernet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stddef.h>
#include <unistd.h>

#define ARRAY_LEN(array) (sizeof(array)/sizeof(*array))

struct string_list {
  struct string_list *next;
  char str[0];
};



int sockfd;
char toybuf[4096];

// Die with an error message.
void error_exit(char *msg, ...)
{
  va_list va;
  printf("error_exit:\n\t");
  va_start(va, msg);
  vprintf(msg, va);
  va_end(va);
  printf("\n");
  exit(-1);
}

// atol() with the kilo/mega/giga/tera/peta/exa extensions.
// (zetta and yotta don't fit in 64 bits.)
long atolx(char *numstr)
{
  char *c, *suffixes="cbkmgtpe", *end;
  long val;

  val = strtol(numstr, &c, 0);
  if (*c) {
    if (c != numstr && (end = strchr(suffixes, tolower(*c)))) {
      int shift = end-suffixes-2;
      if (shift >= 0) val *= 1024L<<(shift*10);
    } else {
      while (isspace(*c)) c++;
      if (*c) error_exit("not integer: %s", numstr);
    }
  }

  return val;
}

long atolx_range(char *numstr, long low, long high)
{
  long val = atolx(numstr);

  if (val < low) error_exit("%ld < %ld", val, low);
  if (val > high) error_exit("%ld > %ld", val, high);

  return val;
}

void poke(void *ptr, uint64_t val, int size)
{
  if (size & 8) {
    volatile uint64_t *p = (uint64_t *)ptr;
    *p = val;
  } else if (size & 4) {
    volatile int *p = (int *)ptr;
    *p = val;
  } else if (size & 2) {
    volatile short *p = (short *)ptr;
    *p = val;
  } else {
    volatile char *p = (char *)ptr;
    *p = val;
  }
}


// Convert hostname to binary address for AF_INET or AF_INET6
// return /prefix (or range max if none)
int get_addrinfo(char *host, sa_family_t af, void *addr)
{
  struct addrinfo hints, *result, *rp = 0;
  int status, len;
  char *from, *slash;

  memset(&hints, 0 , sizeof(struct addrinfo));
  hints.ai_family = af;
  hints.ai_socktype = SOCK_STREAM;

  slash = strchr(host, '/');
  if (slash) *slash = 0;

  status = getaddrinfo(host, NULL, &hints, &result);
  if (!status)
    for (rp = result; rp; rp = rp->ai_next)
      if (rp->ai_family == af) break;
  if (!rp) error_exit("bad address '%s' : %s", host, gai_strerror(status));

  // ai_addr isn't struct in_addr or in6_addr, it's struct sockaddr. Of course.
  // You'd think ipv4 and ipv6 would have some basic compatibility, but no.
  from = ((char *)rp->ai_addr) + 4;
  if (af == AF_INET6) {
    len = 16;
    from += 4;  // skip "flowinfo" field ipv6 puts before address
  } else len = 4;
  memcpy(addr, from, len);
  freeaddrinfo(result);

  len = -1;
  if (slash) len = atolx_range(slash+1, 0, (af == AF_INET) ? 32 : 128);

  return len;
}

static void display_ifconfig(char *name, int always, unsigned long long val[])
{
  struct ifreq ifre;
  struct {
    int type;
    char *title;
  } types[] = {
    {ARPHRD_LOOPBACK, "Local Loopback"}, {ARPHRD_ETHER, "Ethernet"},
    {ARPHRD_PPP, "Point-to-Point Protocol"}, {ARPHRD_INFINIBAND, "InfiniBand"},
    {ARPHRD_SIT, "IPv6-in-IPv4"}, {-1, "UNSPEC"}
  };
  int i;
  char *pp;
  FILE *fp;
  short flags;

  strncpy(ifre.ifr_name, name, IFNAMSIZ);
  if (ioctl(sockfd, SIOCGIFFLAGS, &ifre)<0) error_exit("%s", name);
  flags = ifre.ifr_flags;
  if (!always && !(flags & IFF_UP)) return;

  // query hardware type and hardware address
  i = ioctl(sockfd, SIOCGIFHWADDR, &ifre);

  for (i=0; i < (sizeof(types)/sizeof(*types))-1; i++)
    if (ifre.ifr_hwaddr.sa_family == types[i].type) break;

  printf("%-9s Link encap:%s  ", name, types[i].title);
  if(i >= 0 && ifre.ifr_hwaddr.sa_family == ARPHRD_ETHER) {
    printf("HWaddr ");
    for (i=0; i<6; i++) printf(":%02X"+!i, ifre.ifr_hwaddr.sa_data[i]);
  }
  putchar('\n');

  // If an address is assigned record that.

  ifre.ifr_addr.sa_family = AF_INET;
  memset(&ifre.ifr_addr, 0, sizeof(ifre.ifr_addr));
  ioctl(sockfd, SIOCGIFADDR, &ifre);
  pp = (char *)&ifre.ifr_addr;
  for (i = 0; i<sizeof(ifre.ifr_addr); i++) if (pp[i]) break;

  if (i != sizeof(ifre.ifr_addr)) {
    struct sockaddr_in *si = (struct sockaddr_in *)&ifre.ifr_addr;
    struct {
      char *name;
      int flag, ioctl;
    } addr[] = {
      {"addr", 0, 0},
      {"P-t-P", IFF_POINTOPOINT, SIOCGIFDSTADDR},
      {"Bcast", IFF_BROADCAST, SIOCGIFBRDADDR},
      {"Mask", 0, SIOCGIFNETMASK}
    };

    printf("%10c%s", ' ', (si->sin_family == AF_INET) ? "inet" :
        (si->sin_family == AF_INET6) ? "inet6" : "unspec");

    for (i=0; i < sizeof(addr)/sizeof(*addr); i++) {
      if (!addr[i].flag || (flags & addr[i].flag)) {
        if (addr[i].ioctl && ioctl(sockfd, addr[i].ioctl, &ifre))
          si->sin_family = 0;
        printf(" %s:%s ", addr[i].name,
          (si->sin_family == 0xFFFF || !si->sin_family)
            ? "[NOT SET]" : inet_ntoa(si->sin_addr));
      }
    }

    putchar('\n');
  }

  fp = fopen(pp = "/proc/net/if_inet6", "r");
  if (fp) {
    char iface_name[IFNAMSIZ];
    int plen, iscope;

    while (fgets(toybuf, sizeof(toybuf), fp)) {
      int nitems;
      char ipv6_addr[40];

      nitems = sscanf(toybuf, "%32s %*08x %02x %02x %*02x %15s\n",
                      ipv6_addr, &plen, &iscope, iface_name);
      if (nitems<0 && feof(fp)) break;
      if (nitems != 4) error_exit("bad %s", pp);

      if (!strcmp(name, iface_name)) {
        struct sockaddr_in6 s6;
        char *ptr = ipv6_addr+sizeof(ipv6_addr)-1;

        // convert giant hex string into colon-spearated ipv6 address by
        // inserting ':' every 4 characters. 
        for (i = 32; i; i--)
          if ((*(ptr--) = ipv6_addr[i])) if (!(i&3)) *(ptr--) = ':';

        // Convert to binary and back to get abbreviated :: version
        if (inet_pton(AF_INET6, ipv6_addr, (void *)&s6.sin6_addr) > 0) {
          if (inet_ntop(AF_INET6, &s6.sin6_addr, toybuf, sizeof(toybuf))) {
            char *scopes[] = {"Global","Host","Link","Site","Compat"},
                 *scope = "Unknown";

            for (i=0; i < sizeof(scopes)/sizeof(*scopes); i++)
              if (iscope == (!!i)<<(i+3)) scope = scopes[i];
            printf("%10cinet6 addr: %s/%d Scope: %s\n",
                    ' ', toybuf, plen, scope);
          }
        }
      }
    }
    fclose(fp);
  }

  printf("%10c", ' ');

  if (flags) {
    unsigned short mask = 1;
    char **s, *str[] = {
      "UP", "BROADCAST", "DEBUG", "LOOPBACK", "POINTOPOINT", "NOTRAILERS",
      "RUNNING", "NOARP", "PROMISC", "ALLMULTI", "MASTER", "SLAVE", "MULTICAST",
      "PORTSEL", "AUTOMEDIA", "DYNAMIC", NULL
    };

    for (s = str; *s; s++) {
      if (flags & mask) printf("%s ", *s);
      mask = mask << 1;
    }
  } else printf("[NO FLAGS] ");

  if (ioctl(sockfd, SIOCGIFMTU, &ifre) < 0) ifre.ifr_mtu = 0;
  printf(" MTU:%d", ifre.ifr_mtu);
  if (ioctl(sockfd, SIOCGIFMETRIC, &ifre) < 0) ifre.ifr_metric = 0;
  if (!ifre.ifr_metric) ifre.ifr_metric = 1;
  printf("  Metric:%d", ifre.ifr_metric);

  // non-virtual interface

  if (val) {
    char *label[] = {"RX bytes", "RX packets", "errors", "dropped", "overruns",
      "frame", 0, 0, "TX bytes", "TX packets", "errors", "dropped", "overruns",
      "collisions", "carrier", 0, "txqueuelen"};
    signed char order[] = {-1, 1, 2, 3, 4, 5, -1, 9, 10, 11, 12, 14, -1,
      13, 16, -1, 0, 8};
    int i;

    // Query txqueuelen
    if (ioctl(sockfd, SIOCGIFTXQLEN, &ifre) >= 0) val[16] = ifre.ifr_qlen;
    else val[16] = -1;

    for (i = 0; i < sizeof(order); i++) {
      int j = order[i];

      if (j < 0) printf("\n%10c", ' ');
      else printf("%s:%llu ", label[j], val[j]);
    }
  }
  putchar('\n');

  if(!ioctl(sockfd, SIOCGIFMAP, &ifre) && (ifre.ifr_map.irq ||
      ifre.ifr_map.mem_start || ifre.ifr_map.dma || ifre.ifr_map.base_addr))
  {
    printf("%10c", ' ');
    if(ifre.ifr_map.irq) printf("Interrupt:%d ", ifre.ifr_map.irq);
    if(ifre.ifr_map.base_addr >= 0x100) // IO_MAP_INDEX
      printf("Base address:0x%x ", ifre.ifr_map.base_addr);
    if(ifre.ifr_map.mem_start)
      printf("Memory:%lx-%lx ", ifre.ifr_map.mem_start, ifre.ifr_map.mem_end);
    if(ifre.ifr_map.dma) printf("DMA chan:%x ", ifre.ifr_map.dma);
    putchar('\n');
  }
  putchar('\n');
}

static void show_iface(char *iface_name)
{
  char *name;
  struct string_list *ifaces = 0, *sl;
  int i, j;
  FILE *fp;

  fp = fopen("/proc/net/dev", "r");

  for (i=0; fgets(toybuf, sizeof(toybuf), fp); i++) {
    char *buf = toybuf;
    unsigned long long val[17];

    if (i<2) continue;

    while (isspace(*buf)) buf++;
    name = strsep(&buf, ":");
    if(!buf) error_exit("bad name %s", name);

    errno = 0;
    for (j=0; j<16 && !errno; j++) val[j] = strtoll(buf, &buf, 0);
    if (errno) error_exit("bad %s at %s", name, buf);

    if (iface_name) {
      if (!strcmp(iface_name, name)) {
        display_ifconfig(iface_name, 1, val);

        return;
      }
    } else {
      sl = malloc(sizeof(*sl)+strlen(name)+1);
      strcpy(sl->str, name);
      sl->next = ifaces;
      ifaces = sl;

      display_ifconfig(sl->str, 1, val);
    }
  }
  fclose(fp);

  if (iface_name) display_ifconfig(iface_name, 1, 0);
  else {
    struct ifconf ifcon;
    struct ifreq *ifre;
    int num;

    // Loop until buffer's big enough
    ifcon.ifc_buf = NULL;
    for (num = 30;;num += 10) {
      ifcon.ifc_len = sizeof(struct ifreq)*num;
      ifcon.ifc_buf = realloc(ifcon.ifc_buf, ifcon.ifc_len);
      ioctl(sockfd, SIOCGIFCONF, &ifcon);
      if (ifcon.ifc_len != sizeof(struct ifreq)*num) break;
    }

    ifre = ifcon.ifc_req;
    for(num = 0; num < ifcon.ifc_len && ifre; num += sizeof(struct ifreq), ifre++)
    {
      // Skip duplicates
      for(sl = ifaces; sl; sl = sl->next)
        if(!strcmp(sl->str, ifre->ifr_name)) break;

      if(!sl) display_ifconfig(ifre->ifr_name, 1, 0);
    }

    free(ifcon.ifc_buf);
  }

  //llist_traverse(ifaces, free);

  while(sl = ifaces){
      ifaces = sl->next;
      free(sl);
  }
}

// Encode offset and size of field into an int, and make result negative
#define IFREQ_OFFSZ(x) -(int)((offsetof(struct ifreq, x)<<16) + sizeof(ifre.x))

void main(int argc, char *argv[])
{
  struct ifreq ifre;
  int i;

  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if(argc == 1) {
    show_iface(NULL);
    return;
  }else if(argc == 2){
      show_iface(argv[1]);
      return;
  }

  // Open interface
  memset(&ifre, 0, sizeof(struct ifreq));
  strncpy(ifre.ifr_name, *argv, IFNAMSIZ);

  // Perform operations on interface
  while(*++argv) {
    // Table of known operations
    struct argh {
      char *name;
      int on, off; // set, clear
    } try[] = {
      {0, IFF_UP|IFF_RUNNING, SIOCSIFADDR},
      {"up", IFF_UP|IFF_RUNNING, 0},
      {"down", 0, IFF_UP},
      {"arp", 0, IFF_NOARP},
      {"promisc", IFF_PROMISC, 0},
      {"allmulti", IFF_ALLMULTI, 0},
      {"multicast", IFF_MULTICAST, 0},
      {"pointopoint", IFF_POINTOPOINT, SIOCSIFDSTADDR},
      {"broadcast", IFF_BROADCAST, SIOCSIFBRDADDR},
      {"netmask", 0, SIOCSIFNETMASK},
      {"dstaddr", 0, SIOCSIFDSTADDR},
      {"mtu", IFREQ_OFFSZ(ifr_mtu), SIOCSIFMTU},
      {"keepalive", IFREQ_OFFSZ(ifr_data), SIOCDEVPRIVATE}, // SIOCSKEEPALIVE
      {"outfill", IFREQ_OFFSZ(ifr_data), SIOCDEVPRIVATE+2}, // SIOCSOUTFILL
      {"metric", IFREQ_OFFSZ(ifr_metric), SIOCSIFMETRIC},
      {"txqueuelen", IFREQ_OFFSZ(ifr_qlen), SIOCSIFTXQLEN},
      {"mem_start", IFREQ_OFFSZ(ifr_map.mem_start), SIOCSIFMAP},
      {"io_addr", IFREQ_OFFSZ(ifr_map.base_addr), SIOCSIFMAP},
      {"irq", IFREQ_OFFSZ(ifr_map.irq), SIOCSIFMAP},
      {"inet", 0, 0},
      {"inet6", 0, 0}
    };
    char *s = *argv;
    int rev = (*s == '-');

    s += rev;

    // "set hardware address" is oddball enough to special case
    if (!strcmp(*argv, "hw")) {
      char *hw_addr, *ptr, *p;
      struct sockaddr *sock = &ifre.ifr_hwaddr;
      int count = 6;

      ptr = p = (char *)sock->sa_data;
      memset(sock, 0, sizeof(struct sockaddr));
      if (argv[1]) {
        if (!strcmp("ether", *++argv)) sock->sa_family = ARPHRD_ETHER;
        else if (!strcmp("infiniband", *argv)) {
          sock->sa_family = ARPHRD_INFINIBAND;
          count = 20;
          p = ptr = toybuf;
        }
      }
      if (!sock->sa_family || !argv[1]) {
        error_exit("bad hw '%s'", *argv);
      }
      hw_addr = *++argv;

      // Parse and verify address.
      while (*hw_addr && (p-ptr) < count) {
        int val, len = 0;

        if (*hw_addr == ':') hw_addr++;
        sscanf(hw_addr, "%2x%n", &val, &len);
        if (!len || len > 2) break; // 1 nibble can be set e.g. C2:79:38:95:D:A 
        hw_addr += len;
        *p++ = val;
      }

      if ((p-ptr) != count || *hw_addr)
        error_exit("bad hw-addr '%s'", *argv);

      // the linux kernel's "struct sockaddr" (include/linux/socket.h in the
      // kernel source) only has 14 bytes of sa_data, and an infiniband address
      // is 20. So if we go through the ioctl, the kernel will truncate
      // infiniband addresses, meaning we have to go through sysfs instead.
      if (sock->sa_family == ARPHRD_INFINIBAND && !strchr(ifre.ifr_name, '/')) {
        int fd;

        sprintf(toybuf, "/sys/class/net/%s/address", ifre.ifr_name);
        fd = open(toybuf, O_RDWR);
        write(fd, *argv, strlen(*argv));
        close(fd);
      } else ioctl(sockfd, SIOCSIFHWADDR, &ifre);
      continue;

    // Add/remove ipv6 address to interface

    } else if (!strcmp(*argv, "add") || !strcmp(*argv, "del")) {
      struct ifreq_inet6 {
        struct in6_addr addr;
        unsigned prefix;
        int index;
      } ifre6;
      int plen, fd6 = socket(AF_INET6, SOCK_DGRAM, 0);

      if (!argv[1]) {
        error_exit("%s", *argv);
      }

      plen = get_addrinfo(argv[1], AF_INET6, &ifre6.addr);
      if (plen < 0) plen = 128;
      ioctl(fd6, SIOCGIFINDEX, &ifre);
      ifre6.index = ifre.ifr_ifindex;
      ifre6.prefix = plen;
      ioctl(fd6, **(argv++)=='a' ? SIOCSIFADDR : SIOCDIFADDR, &ifre6);

      close(fd6);
      continue;
    // Iterate through table to find/perform operation
    } else for (i = 0; i < ARRAY_LEN(try); i++) {
      struct argh *t = try+i;
      int on = t->on, off = t->off;

      if (!t->name) {
        if (isdigit(**argv) || !strcmp(*argv, "default")) argv--;
        else continue;
      } else if (strcmp(t->name, s)) continue;

      // Is this an SIOCSI entry?
      if ((off|0xff) == 0x89ff) {
        if (!rev) {
          if (!*++argv) error_exit("%s needs argument", t->name);

          // Assign value to ifre field and call ioctl? (via IFREQ_OFFSZ.)
          if (on < 0) {
            long l = strtoul(*argv, 0, 0);

            if (off == SIOCSIFMAP) ioctl(sockfd, SIOCGIFMAP, &ifre);
            on = -on;
            poke((on>>16) + (char *)&ifre, l, on&15);
            ioctl(sockfd, off, &ifre);
            break;
          } else {
            struct sockaddr_in *si = (struct sockaddr_in *)&ifre.ifr_addr;
            int mask = -1;

            si->sin_family = AF_INET;

            if (!strcmp(*argv, "default")) si->sin_addr.s_addr = INADDR_ANY;
            else mask = get_addrinfo(*argv, AF_INET, &si->sin_addr);
            ioctl(sockfd, off, &ifre);

            // Handle /netmask
            if (mask >= 0) {
              // sin_addr probably isn't unaligned, but just in case...
              mask = htonl((~0)<<(32-mask));
              memcpy(&si->sin_addr, &mask, 4);
              ioctl(sockfd, SIOCSIFNETMASK, &ifre);
            }
          }
        }
        off = 0;
      }

      // Set flags
      if (on || off) {
        ioctl(sockfd, SIOCGIFFLAGS, &ifre);
        ifre.ifr_flags &= ~(rev ? on : off);
        ifre.ifr_flags |= (rev ? off : on);
        ioctl(sockfd, SIOCSIFFLAGS, &ifre);
      }

      break;
    }
    if (i == sizeof(try)/sizeof(*try)) {
      error_exit("bad argument '%s'", *argv);
    }
  }
  close(sockfd);
}
