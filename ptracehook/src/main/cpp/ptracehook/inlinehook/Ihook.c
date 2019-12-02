/*================================================================
*   Copyright (C) 2019 Sangfor Ltd. All rights reserved.
*   
*   @File name：Ihook.c
*   @Author: nathan
*   @Date: 2019年06月16日
================================================================*/

#include "Ihook.h"
#if ARM32
#include "arm32/InlineHookArm32.h"
#else
#include "arm64/InlineHookArm64.h"
#endif

int InlineHook(void *pHookAddr, void (*pCallBackFunc)(PT_REGS *), int iInsnMode);

//int IsTargetAddrInBackup(unsigned long ulTargetAddr, unsigned longulHookAddr, int iBackupLength);
/*
 *	@pHookAddr 要hook的目标地址
 *	@pCallBackFunc 要跳转到的自定义的函数地址
 *  @iInstMode	指令模式：0为arm，1为thumb 
 */
int InlineHook(void *pHookAddr, void (*pCallBackFunc)(PT_REGS *), int iInstMode)
{
	int i = 0;
	int iRet = -1;
	unsigned long ulHookAddr = 0;
	STInlineHookInfo *pstInlineHookInfo = NULL;

	if (pHookAddr == NULL || pCallBackFunc == NULL) {
		LOGE("check params.\n");
		goto out;
	}

	pstInlineHookInfo = (STInlineHookInfo *)malloc(sizeof(STInlineHookInfo));
	pstInlineHookInfo->pHookAddr = pHookAddr;
	pstInlineHookInfo->onCallBack = pCallBackFunc;

#if ARM32
	if (iInstMode == 0) {
		if (HookArm32(pstInlineHookInfo)) {
			LOGE("HookArm32 failed.\n");
			goto out;
		}
	}
	else {
//		if (HookThumb(pstInlineHookInfo)) {
//			LOGE("HookThumb failed.\n");
//			goto out;
//		}
	}
#else
    /*if (HookArm64(pstInlineHookInfo)) {
        LOGE("HookArm64 failed.\n");
        goto out;
    }*/
#endif

	iRet = 0;
out:
	if (pstInlineHookInfo != NULL) {
		free(pstInlineHookInfo);
	}
	return iRet;
}

int IsTargetAddrInBackup(unsigned long ulTargetAddr, unsigned long ulHookAddr, int iBackupLength)
{
    if ((ulTargetAddr <= ulHookAddr + iBackupLength) && (ulTargetAddr >= ulHookAddr)) {
        return 0;
    }
    else {
        return -1;
    }
}
