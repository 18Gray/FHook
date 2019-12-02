/*================================================================
*   Copyright (C) 2019 Sangfor Ltd. All rights reserved.
*   
*   @File name：log.c
*   @Author: nathan
*   @Date: 2019年05月12日
================================================================*/

#include "Log.h"

#if LOG_LEVEL_DEBUG
ELogLevel g_eLogLevel = LogLevel_Debug;
#elif LOG_LEVEL_INFO
ELogLevel g_eLogLevel = LogLevel_Info;
#elif LOG_LEVEL_ERROR
ELogLevel g_eLogLevel = LogLevel_Error;
#else
ELogLevel g_eLogLevel = LogLevel_Error;
#endif

//void androidPrintLog
#define LOGCAT(fmt, args...)	__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, fmt, ##args)

#if ANDROID_LOGCAT
#define LOG_TAG "ANDROID_INJECT"
void logcat(ELogLevel eLevel, char *pBuf) 
{
	switch (eLevel) {
		case LogLevel_Debug:
			__android_log_write(ANDROID_LOG_DEBUG, LOG_TAG, pBuf);
			break;
		
		case LogLevel_Info:
			__android_log_write(ANDROID_LOG_INFO, LOG_TAG, pBuf);
			break;
			
		case LogLevel_Error:
			__android_log_write(ANDROID_LOG_ERROR, LOG_TAG, pBuf);
			break;
		default:
			break;
	}
}
#endif
void PrintLog(ELogLevel eLevel, const char *pFileName, int iLine, \
		const char *pFunc, const char *pFormat, ...)
{
    char pBuf[4096] = {0};
    char sLogLevelName[64] = {0};

    switch (eLevel) {
        case LogLevel_Debug:
            strncpy(sLogLevelName, "DEBUG", sizeof(sLogLevelName));
            break;

        case LogLevel_Info:
            strncpy(sLogLevelName, "INFO", sizeof(sLogLevelName));
            break;

        case LogLevel_Error:
            strncpy(sLogLevelName, "ERROR", sizeof(sLogLevelName));
            break;

        default:
            break;
    }

    if (eLevel >= g_eLogLevel) {
        va_list args;
        va_start(args, pFormat);

//        sprintf(pBuf, "%lu:%s:%s:%d:%s():(%d):\t", time(NULL), sLogLevelName, pFileName, iLine, pFunc, gettid());

#if ANDROID_LOGCAT
		vsprintf(pBuf + strlen(pBuf), pFormat, args);
		logcat(eLevel, pBuf);
        
#else
        printf("%s", pBuf);
        fflush(stdout);
        vprintf(pFormat, args);
#endif
    }
}
