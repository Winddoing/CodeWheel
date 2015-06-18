#ifndef SHELL_H
#define SHELL_H

#include <tcp.h>

#define TYPE_REQPROMPT	'P'
#define TYPE_REQEXERES	'C'
#define TYPE_SNDPROMPT	'p'
#define TYPE_SNDEXERES	'c'
#define TYPE_QUITCMD    'q'

#define TYPE_ERRCMD		('z' + 1)
#define SHELL_ERRRUN	('z' + 2)
#define QUIT_CMD        "QUIT"

/* run in client */
// recive command prompt from server
int recivePrompt(const IpInfo *info, Data *data);
// recive command exec result from server
int reciveExecResult(const IpInfo *info, Data *data);

/* run in server */
int analyData(const IpInfo *info, Data *data);
// send command prompt to client
int sendPrompt(const IpInfo *info, Data *data);
// send command exec result to client
int sendExecResult(const IpInfo *info, Data *data);

#endif //SHELL_H

