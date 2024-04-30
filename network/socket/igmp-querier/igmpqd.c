/**
 * Copyright (c) 2013, Henrik Brix Andersen <henrik@brixandersen.dk>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <arpa/inet.h>
#include <errno.h>
#include <sys/types.h>
#include <netinet/igmp.h>
#include <limits.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "daemon.h"
#include "logging.h"

#define VERSION "0.2.0"

/* For compatibility with e.g. OS X 10.8 */
#ifndef IGMP_MEMBERSHIP_QUERY
#define IGMP_MEMBERSHIP_QUERY IGMP_HOST_MEMBERSHIP_QUERY
#endif

typedef struct igmpqd_options {
    int   debug;
    int   daemonize;
    int   help;
    int   use_syslog;
    int   version;
    long  interval;
    char *username;
    char *groupname;
    char *pidfile;
} igmpqd_options_t;

void
usage(char *command)
{
    printf("usage: %s [-dfhlv] [-m MGROUP] [-u USER] [-s INTERVAL] [-p PIDFILE]\n",
        command);
}

int
parse_command_line(int argc, char **argv, igmpqd_options_t *options)
{
    char *endptr = NULL;
    int c;

    while ((c = getopt(argc, argv, "dfg:hlp:s:u:v")) != -1) {
        switch (c) {
        case 'd':
            options->debug = 1;
            break;

        case 'f':
            options->daemonize = 0;
            break;

        case 'g':
            options->groupname = optarg;
            break;

        case 'h':
            options->help = 1;
            break;

        case 'l':
            options->use_syslog = 1;
            break;

        case 'p':
            options->pidfile = optarg;
            break;

        case 's':
            errno = 0;
            options->interval = strtol(optarg, &endptr, 10);
            if (*endptr != '\0' || options->interval <= 0 ||
                (errno == ERANGE && (options->interval == LONG_MAX || options->interval == LONG_MIN)) ||
                (errno != 0 && options->interval == 0)) {
                fprintf(stderr, "Error: Invalid interval '%s'\n", optarg);
                return -1;
            }
            break;

        case 'u':
            options->username = optarg;
            break;

        case 'v':
            options->version = 1;
            break;

        default:
            usage(argv[0]);
            return -1;
            break;
        }
    }

    /* Ensure no extra command line parameters were given */
    if (argc != optind) {
        usage(argv[0]);
        return -1;
    }

    return 0;
}

uint16_t
cksum(void *buf, size_t len)
{
    uint32_t cksum = 0;
    int i;
    int short_len = len / 2;

    for (i = 0; i < short_len; i++) {
        cksum += ((uint16_t*)buf)[i];
    }
    if (len % 2) {
        cksum += ((uint8_t*)buf)[len - 1];
    }
    cksum = (cksum >> 16) + (cksum & 0xFFFF);
    cksum = cksum + (cksum >> 16);

    return (~cksum & 0xFFFF);
}

int
main(int argc, char **argv)
{
    struct igmp igmp;
    struct in_addr mgroup, allhosts;
    struct sockaddr_in dst;
    igmpqd_options_t *options;
    int sockfd;

    /* Parse command line options */
    options = malloc(sizeof(igmpqd_options_t));
    if (options == NULL) {
        perror("Error: Could not allocate memory for options");
        exit(EXIT_FAILURE);
    }
    memset(options, 0, sizeof(*options));
    options->interval = 60; /* seconds */
    options->daemonize = 1;
    if (parse_command_line(argc, argv, options) != 0) {
        exit(EXIT_FAILURE);
    }

    /* Check for special options */
    if (options->help) {
        usage(argv[0]);
        exit(EXIT_SUCCESS);
    }
    if (options->version) {
        printf("%s\n", VERSION);
        exit(EXIT_SUCCESS);
    }

    /* Initialize logging */
    init_logger(options->use_syslog);

    /* Create socket */
    sockfd = socket(PF_INET, SOCK_RAW, IPPROTO_IGMP);
    if (sockfd == -1) {
        logger(LOG_LEVEL_ERR, "Could not open raw socket: %s", strerror(errno));
        goto fail;
    }

    /* Drop privileges */
    if (drop_privileges(options->username, options->groupname) != 0) {
        goto fail;
    }

    /* Multicast groups */
    mgroup.s_addr = inet_addr("0.0.0.0");
    if (mgroup.s_addr == INADDR_NONE) {
        logger(LOG_LEVEL_ERR, "Invalid multicast group '0.0.0.0'");
        goto fail;
    }
    allhosts.s_addr = inet_addr("224.0.0.1");
    if (allhosts.s_addr == INADDR_NONE) {
        logger(LOG_LEVEL_ERR, "Invalid multicast group '224.0.0.1'");
        goto fail;
    }

    /* IGMPv1 query */
    igmp.igmp_type = IGMP_MEMBERSHIP_QUERY;
    igmp.igmp_code = 0;
    igmp.igmp_cksum = 0;
    igmp.igmp_group = mgroup;
    igmp.igmp_cksum = cksum(&igmp, sizeof(igmp));

    /* Destination */
    dst.sin_family = AF_INET;
    dst.sin_port = htons(0);
    dst.sin_addr = allhosts;

    /* Daemonize */
    if (options->daemonize) {
        if (daemonize(options->pidfile) != 0) {
            goto fail;
        }
    }

    /* Transmit loop */
    while (1) {
        if (sendto(sockfd, &igmp, sizeof(igmp), 0, (struct sockaddr*)&dst, sizeof(dst)) == -1) {
            logger(LOG_LEVEL_ERR, "Could not send IGMP query: %s", strerror(errno));
        }
        sleep(options->interval);
    }

    free(options);
    exit(EXIT_SUCCESS);

fail:
    free(options);
    exit(EXIT_FAILURE);
}
