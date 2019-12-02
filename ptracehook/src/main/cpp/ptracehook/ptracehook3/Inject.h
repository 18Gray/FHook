/*================================================================
*   Copyright (C) 2019 Sangfor Ltd. All rights reserved.
*   
*   @File name：inject.h
*   @Author: nathan
*   @Date: 2019年05月09日
================================================================*/

#ifndef __INJECT_H__
#define __INJECT_H__

#include <stdio.h>
#include <linux/ptrace.h>
#include <sys/wait.h>
#include <asm/ptrace.h>
#include <sys/ptrace.h>
#include "Log.h"

#define CPSR_T_MASK        ( 1u << 5 )

#ifndef PAGE_SIZE
#define PAGE_SIZE	4096
#endif

#if __LP64__
#define ARM_sp sp
#define ARM_pc pc
#define ARM_cpsr	pstate
#define ARM_lr		regs[30]
#define ARM_r0		regs[0]
#define PTRACE_SETREGS PTRACE_SETREGSET
#define PTRACE_GETREGS PTRACE_GETREGSET
typedef struct user_pt_regs PT_REGS;
#define uregs regs

#define PARAM_REGS_NUM 8
#else 
typedef struct pt_regs PT_REGS;

#define PARAM_REGS_NUM 4
#endif

int PtraceReadData(pid_t pid, void *pTargetAddr, uint8_t *pData, size_t lSize);
int PtraceWriteData(pid_t pid, void *pTargetAddr, uint8_t *pData, size_t lSize);
int PtraceWriteString(pid_t pid, void *pTargetAddr, char *pStr);
int PtraceCallFunc(pid_t pid, void *pFuncAddr, long *pParams, uint32_t wParamsNum, PT_REGS *pRegs);
int PtraceGetRegs(pid_t pid, PT_REGS *pRegs);
int PtraceSetRegs(pid_t pid, PT_REGS *pRegs);
int PtraceContinue(pid_t pid);
int PtraceAttach(pid_t pid);
int PtraceDetach(pid_t pid);
long PtraceRetValue(PT_REGS *pRegs);

#endif
