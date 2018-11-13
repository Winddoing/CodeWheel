/*
 * ts_log.h
 *
 *  Created on: 2015-1-18
 *      Author: shenwei
 */

#ifndef TS_LOG_H_
#define TS_LOG_H_

#define ts_debug() printf("[Debug] Line:%d Func:%s File:%s\n", __LINE__, __FUNCTION__, __FILE__)
#define ts_trace(fmt, args...) do{printf("[Trace] Line:%d Func:%s File:%s ", __LINE__, __FUNCTION__, __FILE__);printf(fmt,##args);}while(0)
#define ts_info(fmt, args...) do{printf("[Info] Line:%d Func:%s File:%s ", __LINE__, __FUNCTION__, __FILE__);printf(fmt,##args);}while(0)
#define ts_warn(fmt, args...) do{printf("[Warn] Line:%d Func:%s File:%s ", __LINE__, __FUNCTION__, __FILE__);printf(fmt,##args);}while(0)

#endif /* TS_LOG_H_ */
