#ifndef _DEBUG_H_
#define _DEBUG_H_

#define DBG_MSG(_fmt, args...)	dbgMsg(__FILE__, __LINE__, __FUNCTION__, _fmt, ## args)

// #define DebugOut(fmt, args...)	fprintf(stderr, fmt, ## args)

extern void dbgMsg(const char *szFile, int line, const char *szFunc, const char *szFormat, ...);
extern void HexaDump(unsigned char *pBuff, unsigned int len);

#endif	//_DEBUG_H_
