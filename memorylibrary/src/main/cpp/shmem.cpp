#include <jni.h>
#include <string>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>


extern "C" JNIEXPORT jstring JNICALL
Java_com_eighteengray_fhook_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}


int shmRead(){
    void *shm = NULL;//分配的共享内存的原始首地址
    struct shared_use_st *shared;//指向shm
    int shmid;//共享内存标识符
    //创建共享内存
    shmid = shmget((key_t)12345, sizeof(struct shared_use_st), 0666|IPC_CREAT);
    //将共享内存映射到当前进程的地址空间
    shm = shmat(shmid, 0, 0);
    //设置共享内存
    shared = (struct shared_use_st*)shm;
    shared->written = 0;
    //访问共享内存
    while(1){
        if(shared->written != 0)  {
            printf("You wrote: %s", shared->text);
            if(strncmp(shared->text, "end", 3) == 0)
                break;
        }}
    //把共享内存从当前进程中分离
    if(shmdt(shm) == -1)  { }
    //删除共享内存
    if(shmctl(shmid, IPC_RMID, 0) == -1)   {  }
    exit(EXIT_SUCCESS);
}


int main()
{
    void *shm = NULL;
    struct shared_use_st *shared = NULL;
    char buffer[BUFSIZ + 1];//用于保存输入的文本
    int shmid;
    //创建共享内存
    shmid = shmget((key_t) 12345, sizeof(struct shared_use_st), 0666|IPC_CREAT);
    //将共享内存连接到当前进程的地址空间
    shm = shmat(shmid, (void*)0, 0);
    printf("Memory attached at %X\n", (int)shm);
    //设置共享内存
    shared = (struct shared_use_st*)shm;
    while(1)//向共享内存中写数据
    {
        //数据还没有被读取，则等待数据被读取,不能向共享内存中写入文本
        while(shared->written == 1)
        {
            sleep(1);
        }
        //向共享内存中写入数据
        fgets(buffer, BUFSIZ, stdin);
        strncpy(shared->text, buffer, 111);
        shared->written = 1;
        if(strncmp(buffer, "end", 3) == 0){

        }
//            running = 0;
    }
    //把共享内存从当前进程中分离
    if(shmdt(shm) == -1)   {    }
    sleep(2);
    exit(EXIT_SUCCESS);
}




















