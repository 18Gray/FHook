#include "com_example_ashmem_JNIInterface.h"
#include <fcntl.h>    /* For O_RDWR */
#include <unistd.h>   /* For open(), creat() */
#include <linux/ashmem.h>
#include <sys/mman.h>
#include <android/log.h>
#include <stdio.h>
#include    <stdlib.h>
#include    <pthread.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <errno.h>

#define LOGD(...) 	\
	{				\
	__android_log_print(ANDROID_LOG_DEBUG, "JNI", __VA_ARGS__);	\
	}

int g_fd2ashmem;

JNIEXPORT jint JNICALL Java_com_example_ashmem_JNIInterface_initAndGetFd2Ashmem
  (JNIEnv *env, jclass thiz){

    jint fd = open("/dev/ashmem",O_RDWR);
    ioctl(fd,ASHMEM_SET_NAME,"MyAshmemName");
    ioctl(fd,ASHMEM_SET_SIZE,4096);

    pthread_mutex_t *p_mutex=(pthread_mutex_t*)mmap(NULL, sizeof(pthread_mutex_t), PROT_READ|PROT_WRITE, MAP_SHARED,fd,0);
	if( MAP_FAILED==p_mutex )
	{
		LOGD("mmap:[%d](%s)",errno,strerror(errno));
		return;
	}

	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	int ret=pthread_mutexattr_setpshared(&attr,PTHREAD_PROCESS_SHARED);
	if( ret!=0 )
	{
		LOGD("init_mutex pthread_mutexattr_setpshared");
		return;
	}
	pthread_mutex_init(p_mutex, &attr);

	munmap(p_mutex, sizeof(pthread_mutex_t));

    g_fd2ashmem = fd;
    LOGD("initAndGetFd2Ashmem open fd:%d",g_fd2ashmem);
    return fd;
}

JNIEXPORT void JNICALL Java_com_example_ashmem_JNIInterface_doOperaNow
  (JNIEnv *env, jclass thiz){
	pthread_mutex_t *p_mutex=(pthread_mutex_t*)mmap(NULL, sizeof(pthread_mutex_t), PROT_READ|PROT_WRITE, MAP_SHARED,g_fd2ashmem,0);
	if( MAP_FAILED==p_mutex )
	{
		LOGD("mmap:[%d](%s)",errno,strerror(errno));
		return;
	}
	int ret=pthread_mutex_lock(p_mutex);
	if( ret!=0 )
	{
		LOGD("doOperaNow pthread_mutex_lock");
		return;
	}
	//测试是否能够阻止其他进程的进入
	sleep(2);
	LOGD("this is process a.doing AAAAAAAA");
	sleep(1);
	LOGD("this is process a.doing BBBBBBBB");
	sleep(1);
	LOGD("this is process a.doing CCCCCCCC");
	sleep(1);
	LOGD("this is process a.doing DDDDDDDD");

	ret=pthread_mutex_unlock(p_mutex);
	if( ret!=0 )
	{
		LOGD("child pthread_mutex_unlock");
	}

    munmap(p_mutex, sizeof(pthread_mutex_t));
}


JNIEXPORT void JNICALL Java_com_example_ashmem_JNIInterface_doOperaLater
  (JNIEnv *env, jclass thiz, jint fd){
	pthread_mutex_t *p_mutex=(pthread_mutex_t*)mmap(NULL, sizeof(pthread_mutex_t), PROT_READ|PROT_WRITE, MAP_SHARED,fd,0);
	if( MAP_FAILED==p_mutex )
	{
		LOGD("mmap:[%d](%s)",errno,strerror(errno));
		return;
	}
	sleep(2);// 保证主进程先行
	int ret=pthread_mutex_lock(p_mutex);
	if( ret!=0 )
	{
		LOGD("child pthread_mutex_lock");
	}
	LOGD("this is process b.doing AAAAAAAA");
	sleep(1);
	LOGD("this is process b.doing BBBBBBBB");
	sleep(1);
	LOGD("this is process b.doing CCCCCCCC");
	sleep(1);
	LOGD("this is process b.doing DDDDDDDD");

	ret=pthread_mutex_unlock(p_mutex);
	if( ret!=0 )
	{
		LOGD("child pthread_mutex_unlock");
	}

	munmap(p_mutex, sizeof(pthread_mutex_t));

}


