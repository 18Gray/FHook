/*================================================================
*   Copyright (C) 2019 Sangfor Ltd. All rights reserved.
*   
*   @File name：inject.c
*   @Author: nathan
*   @Date: 2019年05月09日
================================================================*/

#include "Tools.h"
#include "Inject.h"

/**
 * ①.每个进程都在/proc目录下，以进程id为文件夹名，所以可以通过/proc/<pid>/cmdline文件中中读取进程名称，和我们需要注入的进程名称比较，获得进程id

　　②.以root身份运行注入程序，通过ptrace函数，传入PTRACE_ATIACH附加到目标进程，PTRACE_SETREGS设置进程寄存器，PTRACE_GETREGS获得目标寄存器.更多可以访问ptrace的使用

　　③.调用mmap在对方进程空间分配内存，保存要加载的so文件路径，so中函数的名称，so中函数需要传入的参数。

　　　由于每个模块在进程中加载地址不一致，所以我们首先获得目标进程中libc.so文件基址TargetBase，再获得自身libc.so基址SelfBase，再根据mmap-SelfBase+TargetBase获得目标进程中mmap的地址。

        同理获得目标进程中dlopen()函数地址、dlsym()函数地址、dlclose()函数地址

　　④.调用dlopen()函数加载so库，调用dlsym()函数获得so库中函数的地址，调用so库中函数的地址，测试注入成功！调用dlclose()函数卸载so库。
 */

int PtraceAttach(pid_t pid)
{
	int iRet = -1;
	
	if (ptrace(PTRACE_ATTACH, pid, NULL, NULL) < 0) {
		LOGE("attach %d failed: %s\n", pid, strerror(errno));
		goto out;
	}
	waitpid(pid, NULL, WUNTRACED);

	if (ptrace(PTRACE_SYSCALL, pid, NULL, NULL) < 0) {
		LOGE("ptrace syscall failed: %s\n", strerror(errno));
		goto out;
	}

	waitpid(pid, NULL, WUNTRACED);
	
	iRet = 0;
out:
	return iRet;
}


int PtraceReadData(pid_t pid, void *pTargetAddr, uint8_t *pData, size_t lSize)
{
	union u {
		long value;
		char chars[sizeof(long)];
	} uDataBuf;
	int loop = lSize / sizeof(long);
	int tail = lSize % sizeof(long);
	int i = 0; 
	int iRet = -1;
	uint8_t *pTmpData = pData;

	for (i = 0; i < loop; i++) {
		uDataBuf.value = ptrace(PTRACE_PEEKTEXT, pid, pTargetAddr, NULL);
        if (uDataBuf.value == -1) {
            LOGE("ptrace readData failed: %s\n", strerror(errno));
            goto out;
        }
        memcpy(pTmpData, uDataBuf.chars, sizeof(uDataBuf.value));
        pTargetAddr ++;
        pTmpData += sizeof(uDataBuf.value);
	}

    if (tail > 0) {
		uDataBuf.value = ptrace(PTRACE_PEEKTEXT, pid, pTargetAddr, NULL);
        if (uDataBuf.value == -1) {
            LOGE("ptrace readData failed: %s\n", strerror(errno));
            goto out;
        }
        memcpy(pTmpData, uDataBuf.chars, tail);
    }
    
    iRet = 0;
out:
	return iRet;
}

int PtraceWriteData(pid_t pid, void *pTargetAddr, uint8_t *pData, size_t lSize)
{
	union u {
		long value;
		char chars[sizeof(long)];
	} uDataBuf;
	int loop = lSize / sizeof(long);
	int tail = lSize % sizeof(long);
	int i = 0;
	int iRet = -1;
	uint8_t *pTmpData = pData;
    size_t lStepLength = sizeof(uDataBuf.value);

    for (i = 0; i < loop; i++) {
        memcpy(uDataBuf.chars, pTmpData, lStepLength);
        if (ptrace(PTRACE_POKETEXT, pid, pTargetAddr, uDataBuf.value) < 0) {
            LOGE("ptrace writeData failed: %s\n", strerror(errno));
            goto out;
        }
        pTmpData += lStepLength;
        pTargetAddr++;
    }

    if (tail > 0) {
        uDataBuf.value = ptrace(PTRACE_PEEKTEXT, pid, pTargetAddr, NULL);
        if (uDataBuf.value == -1) {
            LOGE("ptrace writeData(prepare write tail) failed: %s\n", strerror(errno));
            goto out;
        }

		for (i = 0; i < tail; i++) {
			uDataBuf.chars[i] = *pTmpData++;
		}

		if (ptrace(PTRACE_POKETEXT, pid, pTargetAddr, uDataBuf.value) == -1) {
			LOGE("ptrace writeData(write tail) failed: %s\n", strerror(errno));
			goto out;
		}
    }

    iRet = 0;
out:
    return iRet;
}

int PtraceWriteString(pid_t pid, void *pTargetAddr, char *pStr)
{
	return PtraceWriteData(pid, pTargetAddr, (uint8_t *)pStr, strlen(pStr) + 1);
}

int PtraceCallFunc(pid_t pid, void *pFuncAddr, long *pParams, uint32_t wParamsNum, PT_REGS *pRegs)
{
	int i;
	int iRet = -1;
	
	// 前8/4个参数存入r0 ~ r3
	for (i = 0; i < wParamsNum && i < PARAM_REGS_NUM; i++) {
		pRegs->uregs[i] = pParams[i];
	}

	// 多余8/4个参数的，入栈
	if (i < wParamsNum) {
		pRegs->ARM_sp -= (wParamsNum - i) * sizeof(long);
		PtraceWriteData(pid, (void *)pRegs->ARM_sp, (uint8_t *)&pParams[i], (wParamsNum - i) *sizeof(long));
	}

	// 把要执行的函数地址赋给pc寄存器
	pRegs->ARM_pc = (unsigned long)pFuncAddr;
	if (pRegs->ARM_pc & 1) {
		// thumb
		pRegs->ARM_pc &= (~1u);
		pRegs->ARM_cpsr |= CPSR_T_MASK;
	}
	else {
		// arm
		pRegs->ARM_cpsr &= ~CPSR_T_MASK;
	}

	pRegs->ARM_lr = 0; //置子程序的返回地址为空，以便函数执行完后，返回到null地址，产生SIGSEGV错误

	if (PtraceSetRegs(pid, pRegs) < 0) {
		LOGE("PtraceSetRegs failed.\n");
		goto out;
	}

	if (PtraceContinue(pid) < 0) {
		LOGE("PtraceContinue failed.\n");
		goto out;
	}
	
	int status = 0;
	waitpid(pid, &status, WUNTRACED);	// 等待目标进程中的函数执行完成后返回（返回到null地址，产生SIGSEGV信号）
	if (WSTOPSIG (status) != SIGSEGV) {
		 iRet = PtraceContinue(pid);
		 if (iRet < 0) {
			LOGE("ptrace call err.\n");
			goto out;
		}
		waitpid(pid, &status, WUNTRACED);
	}
	
	iRet = 0;
out:
	return iRet;
}

long PtraceRetValue(PT_REGS *pRegs)
{
	return pRegs->ARM_r0;
}

int PtraceGetRegs(pid_t pid, PT_REGS *pRegs)
{
	if (ptrace(PTRACE_GETREGS, pid, NULL, pRegs) < 0) {
		LOGE("ptrace getregs: %s\n", strerror(errno));
		return -1;
	}

	return 0;
}

int PtraceSetRegs(pid_t pid, PT_REGS *pRegs)
{
	if (ptrace(PTRACE_SETREGS, pid, NULL, pRegs) < 0) {
		LOGE("ptrace setregs: %s\n", strerror(errno));
		return -1;
	}

	return 0;
}

int PtraceContinue(pid_t pid)
{
	if (ptrace(PTRACE_CONT, pid, NULL, NULL) < 0) {
		LOGE("ptrace continue: %s\n", strerror(errno));
		return -1;
	}

	return 0;
}

int PtraceDetach(pid_t pid)
{
	if (ptrace(PTRACE_DETACH, pid, NULL, NULL) < 0) {
		LOGE("ptrace detach: %s\n", strerror(errno));
		return -1;
	}

	return 0;
}


int main(int argc, char *argv[])
{
	if (argc < 2) {
		LOGE("please input the target app name...\n");
		goto out;
	}
	int iRet = -1, ret = -1;
	char *targetName = argv[1];
	pid_t targetPid = -1;

	targetPid = GetPidFromName(targetName);
	if (targetPid == -1) {
		LOGE("err: getPidFromName found pid failed.\n");
		goto out;
	}
	LOGD("pid: %d\n", targetPid);

	if (ptrace(PTRACE_ATTACH, targetPid, NULL, NULL) < 0) {
		LOGE("attach %d failed: %s\n", targetPid, strerror(errno));
		goto out;
	}
	wait(NULL);
	LOGD("attach target ok.\n");

	if (ptrace(PTRACE_CONT, targetPid, NULL, NULL) < 0) {
		LOGE("continue %d failed: %s\n", targetPid, strerror(errno));
	}

//	sleep(50);

	iRet = 0;
out:
	return iRet;
}
