#ifndef READCONF_H
#define READCONF_H

#define CONF_FILE "rmshell.conf"

#define ITEM_ROOT		"root"
#define ITEM_LOGFILE	"logfile"

int creatDaemon(const char *workPath);

int getSetupItem(const char *confName, const char *itemName,
				char *value);
int appendLog(const char *logName, const char *ip, 
				unsigned short port, int stat);

#endif //READCONF_H

