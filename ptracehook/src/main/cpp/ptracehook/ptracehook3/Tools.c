/*================================================================
*   Copyright (C) 2019 Sangfor Ltd. All rights reserved.
*   
*   @File name：tools.c
*   @Author: nathan
*   @Date: 2019年05月09日
================================================================*/

#include "Tools.h"
#include<stdio.h>
#include<malloc.h>
#include <unistd.h>
#include <sys/mman.h>

int GetPidFromName(char *pTargetName)
{
	if (pTargetName == NULL) {
		LOGE("pTargetName is NULL.\n");
		return -1;
	}

	int iPid = -1;
	DIR *pDir = NULL;
	struct dirent *psDirEntry;
	int iTmpPid = -1;
	char sCmdLine[128] = {0};
	char sAppName[1024] = {0};
	int fd = -1;

	pDir = opendir("/proc");

	while ((psDirEntry = readdir(pDir)) != NULL) {
		iTmpPid = atoi(psDirEntry->d_name);
		if (iTmpPid == 0) {
			continue;
		}
		sprintf(sCmdLine, "/proc/%d/cmdline", iTmpPid);
		
		fd = open(sCmdLine, O_RDONLY);
		if (fd < 0) {
			LOGE("open: %s\n", strerror(errno));
			goto out;
		}
		if (read(fd, sAppName, sizeof(sAppName)) < 0) {
			LOGE("read: %s\n", strerror(errno));
			goto out;
		}
		close(fd);

		if (strcmp(sAppName, pTargetName) == 0) {
			iPid = iTmpPid;
			break;
		}

		memset(sCmdLine, 0, sizeof(sCmdLine));
		memset(sAppName, 0, sizeof(sAppName));
	}
	
out:
	return iPid;
}

/*
 * 获取进程中某个模块的加载基址
 * pid为负数，则从本进程找
 */
long GetModuleBaseAddr(pid_t pid, char *pModuleName)
{
	long lModuleBase = -1;
	char pMaps[1024] = {0};
	char pLines[4096] = {0};
	FILE *pfMaps = NULL;

	if (pModuleName == NULL) {
		LOGE("param pModuleName is null.\n");
		goto out;
	}

	if (pid < 0) {
		sprintf(pMaps, "/proc/self/maps");
	}
	else {
		sprintf(pMaps, "/proc/%d/maps", pid);
	}

	pfMaps = fopen(pMaps, "r");
	if (pfMaps == NULL) {
		LOGE("fopen: %s. \n", strerror(errno));
		goto out;
	}

	while(fgets(pLines, sizeof(pLines), pfMaps) != NULL) {
		if (strstr(pLines, pModuleName)) {
			sscanf(pLines, "%l", &lModuleBase);
			goto out;
		}

		memset(pLines, 0, sizeof(pLines));
	}

out:
	if (pfMaps) {
		fclose(pfMaps);
	}
	return lModuleBase;
}

int IsExecAddr(unsigned long ulAddr)
{
	FILE *pfMaps = NULL;
	char pMaps[1024] = {0};
	char pLines[4096] = {0};
	unsigned long ulStartAddr = 0;
	unsigned long ulEndAddr = 0;
	int iRet = -1;

	pfMaps = fopen("/proc/self/maps", "r");
	if (pfMaps == NULL) {
		LOGE("fopen: %s. \n", strerror(errno));
		goto out;
	}

	while (fgets(pLines, sizeof(pLines), pfMaps) != NULL) {
		sscanf(pLines, "%lu-%lu", ulStartAddr, ulEndAddr);
		if (ulAddr > ulStartAddr && ulAddr < ulEndAddr) {
			iRet = 0;
			break;
		}

		memset(pLines, 0, sizeof(pLines));
	}
	
out:
	if (pfMaps) {
		fclose(pfMaps);
	}
	return iRet;
}

int ChangePageAttr(void *pAddr, size_t size, int port)
{
	int iRet = -1;
	unsigned long ulPageStartAddr = 0;
	unsigned long ulPageNum = 0;
	
	if ((unsigned long)pAddr & (PAGE_SIZE - 1)) {
		ulPageStartAddr = (unsigned long)pAddr & ~(PAGE_SIZE - 1);
		ulPageNum = size / PAGE_SIZE + 1;
	}
	else {
		ulPageStartAddr = (unsigned long)pAddr;
		ulPageStartAddr = size / PAGE_SIZE;
	}

	unsigned long i;
	for (i = 0; i < ulPageNum; i++) {
		if (mprotect(ulPageStartAddr + i * PAGE_SIZE, PAGE_SIZE, port) < 0) {
			LOGE("mprotect(): %s\n", strerror(errno));
			goto out;
		}	
	}

	iRet = 0;
out:
	return iRet;
}

