/*================================================================
*   Copyright (C) 2019 Sangfor Ltd. All rights reserved.
*   
*   @File name：Ihook.h
*   @Author: nathan
*   @Date: 2019年06月16日
================================================================*/

#ifndef __IHOOK_H__
#define __IHOOK_H__

#include "../ptracehook3/Inject.h"
#include "../ptracehook3/Tools.h"
#include<malloc.h>

#ifndef PAGE_SIZE
#define PAGE_SIZE	4096
#endif

#define PAGE_ALINE(addr)	((addr) & (~(PAGE_SIZE - 1)))
#define SET_BIT0(addr)		(addr | 1)
#define BACKUP_FIX_INST_MAX 10
#define BACKUP_OPCODE_MAX_LEN   12

#if __LP64__
#define CLEAR_BIT(addr)		(addr & 0xFFFFFFFFFFFFFFFE)
#else
#define CLEAR_BIT(addr)		(addr & 0xFFFFFFFE)
#define ALIGN_PC(pc)	(pc & 0xFFFFFFFC)
#endif



typedef struct InlineHookInfo {
	void *pHookAddr;		// 要hook的地址
	void *pStubAddr;		// 跳转的桩的地址
	void **ppOriginalFuncAddr;	//保存 构造返回到hook点的函数地址
    unsigned char backupOpcodes[BACKUP_OPCODE_MAX_LEN];  // 备份原指令    
    int backupLength;   // 跳转到桩代码所需要的跳转指令长度（备份原指令长度）
    int backupFixLengthArray[BACKUP_FIX_INST_MAX];     // 备份的原指令如果是跳转指令， 则需要修复，这里保存修复指令需要的长度
	void (*onCallBack)(PT_REGS *);  // hook后调用的外部函数
    uint32_t *pNewEntryForOriginalFuncAddr;     // 构造返回到hook点的函数指针
} STInlineHookInfo;

typedef struct HookTargetInfo {
	int iTargetOffset;
	int iInstructMode;
	char pTargetSoname[512];
} STHookItem;



#endif
