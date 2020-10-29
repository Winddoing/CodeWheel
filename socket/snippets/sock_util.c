//#define _BSD_SOURCE  /* To get NI_MAXHOST and NI_MAXSERV definitions from <netdb.h> */

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>

#include "common.h"   /* To get true/false type */

/*
 *  Create and return a socket fd which connects to host:service.
 *  Used for v4v6 client.
 */
int inetConnect(const char *host, const char *service, int socktype)
{
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int sfd, s;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;
    hints.ai_family = AF_UNSPEC;   /* Allow IPv4 or IPv6 */
    hints.ai_socktype = socktype;  /* SOCK_STREAM or SOCK_DGRAM */

    s = getaddrinfo(host, service, &hints, &result);
    if(s != 0) {
        errno = ENOSYS;
        return -1;
    }

    /* Walk through returned list until we find an address structure
       that can be used to successfully connect a socket */
    for(rp = result; rp != NULL; rp = rp->ai_next) {
        sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if(sfd == -1)
            continue;    /* On error, try next address */

        if(connect(sfd, rp->ai_addr, rp->ai_addrlen) != -1)
            break;       /* Success */

        /* Connect failed: close this socket and try next address */
        close(sfd);
    }

    freeaddrinfo(result);

    return (rp == NULL) ? -1 : sfd;
}

/*
 *  Called by inetListen() and inetBind()
 */
static int inetPassiveSocket(const char *service, int type, socklen_t *addrlen,
                                bool doListen, int backlog)
{
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int sfd, optval, s;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;
    hints.ai_socktype = type;
    hints.ai_family = AF_UNSPEC;    /* Allows IPv4 or IPv6 */
    hints.ai_flags = AI_PASSIVE;    /* Use wildcard IP address */

    s = getaddrinfo(NULL, service, &hints, &result);
    if(s != 0)
        return -1;

    optval = 1;
    for(rp = result; rp != NULL; rp = rp->ai_next) {
        sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if(sfd == -1)
            continue;

        if(doListen) {
            if(setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1) {
                close(sfd);
                freeaddrinfo(result);
                return -1;
            }
        }

        if(bind(sfd, rp->ai_addr, rp->ai_addrlen) == 0)
            break;        /* Success */

        /* bind() failed: close this socket and try next address */
        close(sfd);
    }

    if(rp != NULL && doListen) {
        if(listen(sfd, backlog) == -1) {
            freeaddrinfo(result);
            return -1;
        }
    }

    if(rp != NULL && addrlen != NULL)
        *addrlen = rp->ai_addrlen;     /* Return address structure size */
    freeaddrinfo(result);

    return (rp == NULL) ? -1 : sfd;
}

/*
 *  Create and return a SOCK_STREAM socket fd which listens on [service].
 *  Used for v4v6 TCP server.
 */
int inetListen(const char *service, int backlog, socklen_t *addrlen)
{
    return inetPassiveSocket(service, SOCK_STREAM, addrlen, true, backlog);
}

/*
 *  Create and return a socket fd which binds on [service].
 */
int inetBind(const char *service, int socktype, socklen_t *addrlen)
{
    return inetPassiveSocket(service, socktype, addrlen, false, 0);
}

static ssize_t do_readn_sync(int fd, void *buffer, size_t n, int flags)
{
    ssize_t numRead;   /* # of bytes fetched by last read() */
    size_t totRead;    /* Total # of bytes read so far */
    char *buf;

    buf = buffer;
    for(totRead = 0; totRead < n;) {
        numRead = recvfrom(fd, buf, n - totRead, flags, NULL, 0);

        if(numRead == 0)        /* EOF */
            return totRead;

        if(numRead == -1) {
            if(errno == EINTR)
                continue;       /* Interrupted --> restart read() */
            else
                return -1;      /* Other error */
        }

        totRead += numRead;
        buf += numRead;
    }

    return totRead;             /* Must be 'n' bytes if we get here */
}


/*
 *  Returns number of bytes read, 0 on EOF, or -1 on error
 */
ssize_t readn_sync(int fd, void *buffer, size_t n)
{
    return do_readn_sync(fd, buffer, n, 0);
}

/*
 *  Returns number of bytes peek, 0 on EOF, or -1 on error.
 */
ssize_t peekn_sync(int fd, void *buffer, size_t count)
{
    return do_readn_sync(fd, buffer, count, MSG_PEEK);
}

/*
 *  Returns number of bytes written, or -1 on error
 */
ssize_t sendn_sync(int fd, void *buffer, size_t n)
{
    ssize_t numWritten;        /* # of bytes written by last write() */
    size_t totWritten;         /* Total # of bytes written so far */
    const char *buf;

    buf = buffer;
    for(totWritten = 0; totWritten < n;) {
        numWritten = write(fd, buf, n - totWritten);

        if(numWritten <= 0) {
            if(numWritten == -1 && errno == EINTR)
                continue;     /* Interrupted --> restart write() */
            else
                return -1;    /* Other error */
        }

        totWritten += numWritten;
        buf += numWritten;
    }

    return totWritten;        /* Must be 'n' bytes if we get here */
}

ssize_t sendn_sync_sock(int fd, void *buffer, size_t n)
{
    ssize_t numWritten;        /* # of bytes written by last write() */
    size_t totWritten;         /* Total # of bytes written so far */
    const char *buf;

    buf = buffer;
    for(totWritten = 0; totWritten < n;) {
        numWritten = write(fd, buf, n - totWritten);

        if(numWritten <= 0) {
            if(numWritten == -1 && errno == EINTR)
                continue;     /* Interrupted --> restart write() */
            else
                return -1;    /* Other error */
        }

        totWritten += numWritten;
        buf += numWritten;
    }

    return totWritten;        /* Must be 'n' bytes if we get here */
}

/*
 *  Returns pointer to addrStr, a string containing host and service name.
 */
void inetAddressStr(const struct sockaddr *addr, socklen_t addrlen, char *output_ip, char *output_port)
{
    char host[NI_MAXHOST], service[NI_MAXSERV];

    if(getnameinfo(addr, addrlen, host, NI_MAXHOST, service, NI_MAXSERV, NI_NUMERICSERV | NI_NUMERICHOST) == 0) {
        strcpy(output_ip, host);
        strcpy(output_port, service);
    }
}


