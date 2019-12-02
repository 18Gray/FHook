/*================================================================
*   Copyright (C) 2019 Sangfor Ltd. All rights reserved.
*   
*   @File name：log.h
*   @Author: nathan
*   @Date: 2019年05月12日
================================================================*/

#ifndef __INJECT_LOG_H__
#define __INJECT_LOG_H__

#include <android/log.h>
#include <time.h>
#include <stdio.h>
#include <string.h>

typedef enum {
    LogLevel_Debug = 0,
    LogLevel_Info,
    LogLevel_Error,
} ELogLevel;

void PrintLog(ELogLevel eLevel, const char *pFileName, int iLine, const char *pFunc, const char *pFormat, ...);

#define LOGD(format, args...) do {\
    PrintLog(LogLevel_Debug, __FILE__, __LINE__, __FUNCTION__, format, ##args); \
} while(0)

#define LOGI(format, args...) do {\
    PrintLog(LogLevel_Info, __FILE__, __LINE__, __FUNCTION__, format, ##args); \
} while (0)

#define LOGE(format, args...) do {\
    PrintLog(LogLevel_Error, __FILE__, __LINE__, __FUNCTION__, format, ##args); \
} while (0)

//#if ANDROID_LOG
//#define LOG_TAG "ANDROID_INJECT"
//#define LOGD(fmt, args...)	__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, fmt, ##args)
//#else
//#define LOGD printf
//#endif



#endif
