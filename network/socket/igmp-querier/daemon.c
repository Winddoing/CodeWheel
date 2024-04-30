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

#include <errno.h>
#include <fcntl.h>
#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#include "logging.h"

typedef enum daemon_error {
        DAEMON_ERROR_NONE,
        DAEMON_ERROR_PIDFILE_CREATE,
        DAEMON_ERROR_PIDFILE_LOCK,
        DAEMON_ERROR_CHDIR,
        DAEMON_ERROR_STDIN_CLOSE,
        DAEMON_ERROR_STDOUT_CLOSE,
        DAEMON_ERROR_STDERR_CLOSE,
        DAEMON_ERROR_STDIN_OPEN,
        DAEMON_ERROR_STDOUT_OPEN,
        DAEMON_ERROR_STDERR_OPEN,
} daemon_error_t;

typedef struct daemon_status {
    daemon_error_t error;
    int            errnum;
} daemon_status_t;

int
drop_privileges(char* username, char *groupname)
{
    struct passwd *passwd = NULL;
    struct group *group = NULL;

    if (groupname != NULL) {
        errno = 0;
        group = getgrnam(groupname);
        if (group == NULL) {
            if (errno) {
                logger(LOG_LEVEL_ERR, "Could not get GID for group '%s': %s",
                    groupname, strerror(errno));
            } else {
                logger(LOG_LEVEL_ERR, "Nonexistent group '%s'", groupname);
            }
            return -1;
        }

        if (setgid(group->gr_gid) < 0) {
            logger(LOG_LEVEL_ERR, "Could not drop privileges to group '%s' (GID %d): %s",
                groupname, group->gr_gid, strerror(errno));
            return -1;
        }
    }

    if (username != NULL) {
        errno = 0;
        passwd = getpwnam(username);
        if (passwd == NULL) {
            if (errno) {
                logger(LOG_LEVEL_ERR, "Could not get GID for user '%s': %s",
                    username, strerror(errno));
            } else {
                logger(LOG_LEVEL_ERR, "Nonexistent user '%s'", username);
            }
            return -1;
        }

        if (setuid(passwd->pw_uid) < 0) {
            logger(LOG_LEVEL_ERR, "Could not drop privileges to user '%s' (UID %d): %s",
                username, passwd->pw_uid, strerror(errno));
            return -1;
        }
    }

    return 0;
}

int
daemonize(char *pidfile)
{
    daemon_status_t status;
    int len, pidfd;
    pid_t pid;
    int exitstatus;
    int statusfds[2];

    if (pipe(statusfds) < 0) {
        logger(LOG_LEVEL_ERR, "Could not create status pipe: %s", strerror(errno));
        return -1;
    }

    pid = fork();
    if (pid < 0) {
        logger(LOG_LEVEL_ERR, "Could not create child process: %s", strerror(errno));
        return -1;
    } else if (pid > 0) {
        close(statusfds[1]);
        if (waitpid(pid, &exitstatus, 0) == pid) {
            if (exitstatus == EXIT_SUCCESS) {
                do {
                    errno = 0;
                    len = read(statusfds[0], &status, sizeof(status));
                } while(len == -1 && errno == EINTR);
                close(statusfds[0]);

                if (len == 0) {
                    logger(LOG_LEVEL_ERR, "Could not read daemon status, pipe broken");
                    return -1;
                } else if (len == -1) {
                    logger(LOG_LEVEL_ERR, "Could not read daemon status: %s", strerror(errno));
                    return -1;
                }

                switch (status.error) {
                case DAEMON_ERROR_NONE:
                    /* Successfully created grandchild, exit parent */
                    _exit(EXIT_SUCCESS);
                    break;

                case DAEMON_ERROR_PIDFILE_CREATE:
                    logger(LOG_LEVEL_ERR, "Could not create pid file '%s': %s",
                        pidfile, strerror(status.errnum));
                    break;

                case DAEMON_ERROR_PIDFILE_LOCK:
                    logger(LOG_LEVEL_ERR, "Could not lock pidfile '%s': %s",
                        pidfile, strerror(status.errnum));
                    break;

                case DAEMON_ERROR_CHDIR:
                    logger(LOG_LEVEL_ERR, "Could not change directory to '/': %s",
                        strerror(status.errnum));
                    break;

                case DAEMON_ERROR_STDIN_CLOSE:
                    logger(LOG_LEVEL_ERR, "Could not close standard input: %s",
                        strerror(status.errnum));
                    break;

                case DAEMON_ERROR_STDOUT_CLOSE:
                    logger(LOG_LEVEL_ERR, "Could not close standard output: %s",
                        strerror(status.errnum));
                    break;

                case DAEMON_ERROR_STDERR_CLOSE:
                    logger(LOG_LEVEL_ERR, "Could not close standard error: %s",
                        strerror(status.errnum));
                    break;

                case DAEMON_ERROR_STDIN_OPEN:
                    logger(LOG_LEVEL_ERR, "Could not open '/dev/null' as standard input: %s",
                        strerror(status.errnum));
                    break;

                case DAEMON_ERROR_STDOUT_OPEN:
                    logger(LOG_LEVEL_ERR, "Could not open '/dev/null' as standard output: %s",
                        strerror(status.errnum));
                    break;

                case DAEMON_ERROR_STDERR_OPEN:
                    logger(LOG_LEVEL_ERR, "Could not open '/dev/null' as standard error: %s",
                        strerror(status.errnum));
                    break;

                default:
                    logger(LOG_LEVEL_ERR, "Unknown error while launching daemon");
                    break;
                }

                return -1;
            } else {
                logger(LOG_LEVEL_ERR, "Could not create a new process group and fork daemon");
                return -1;
            }
        } else {
            logger(LOG_LEVEL_ERR, "Could not wait for child process with PID %d: %s",
                pid, strerror(errno));
            return -1;
        }
    }

    /* Child process starts here */

    if (setsid() < 0) {
        logger(LOG_LEVEL_ERR, "Could not create new session: %s", strerror(errno));
        /* Signal parent by exiting with failure */
        exit(EXIT_FAILURE);
    }

    pid = fork();
    if (pid < 0) {
        logger(LOG_LEVEL_ERR, "Could not create grandchild process: %s", strerror(errno));
        /* Signal parent by exiting with failure */
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        /* Succesfully created grandchild, exit child */
        exit(EXIT_SUCCESS);
    }

    /* Grandchild process starts here */

    close(statusfds[0]);
    umask(0133);

    status.error = DAEMON_ERROR_NONE;
    errno = 0;

    if (pidfile != NULL) {
        pidfd = open(pidfile, O_WRONLY|O_CREAT, 0644);
        if (pidfd < 0) {
            status.error = DAEMON_ERROR_PIDFILE_CREATE;
            goto out;
        }

        if (lockf(pidfd, F_TLOCK, 0) < 0) {
            status.error = DAEMON_ERROR_PIDFILE_LOCK;
            close(pidfd);
            goto out;
        }

        dprintf(pidfd, "%d\n", getpid());
    }

    if (chdir("/") < 0) {
        status.error = DAEMON_ERROR_CHDIR;
        goto out;
    }

    if (close(STDIN_FILENO) < 0) {
        status.error = DAEMON_ERROR_STDIN_CLOSE;
        goto out;
    }

    if (close(STDOUT_FILENO) < 0) {
        status.error = DAEMON_ERROR_STDOUT_CLOSE;
        goto out;
    }

    if (close(STDERR_FILENO) < 0) {
        status.error = DAEMON_ERROR_STDERR_CLOSE;
        goto out;
    }

    if (open("/dev/null", O_RDONLY) < 0) {
        status.error = DAEMON_ERROR_STDIN_OPEN;
        goto out;
    }

    if (open("/dev/null", O_WRONLY) < 0) {
        status.error = DAEMON_ERROR_STDOUT_OPEN;
        goto out;
    }

    if (open("/dev/null", O_WRONLY) < 0) {
        status.error = DAEMON_ERROR_STDERR_OPEN;
        goto out;
    }

out:
    status.errnum = errno;
    write(statusfds[1], &status, sizeof(status));
    close(statusfds[1]);

    return (status.error == DAEMON_ERROR_NONE ? 0 : -1);
}
