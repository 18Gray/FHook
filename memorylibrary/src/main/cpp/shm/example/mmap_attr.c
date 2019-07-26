#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

typedef struct{
    char name[4];
    int  age;
}people;



// 把程序编译成两个版本，两个版本主要体现在文件被映射部分的大小不同。文件的大小介于一个页面与两个页面之间（大小为：pagesize*2-99），
// 版本1的被映射部分是整个文件，版本2的文件被映射部分是文件大小减去一个页面后的剩余部分，不到一个页面大小(大小为：pagesize-99)。程序中试图访问每一个页面边界，
// 两个版本都试图在进程空间中映射pagesize*3的字节数。
main(int argc, char** argv)
{
    int fd,i;
    int pagesize,offset;
    people *p_map;

    pagesize = sysconf(_SC_PAGESIZE);
    printf("pagesize is %d\n",pagesize);
    fd = open(argv[1],O_CREAT|O_RDWR|O_TRUNC,00777);
    lseek(fd,pagesize*2-100,SEEK_SET);
    write(fd,"",1);
    offset = 0; //此处offset = 0编译成版本1；offset = pagesize编译成版本2
    p_map = (people*)mmap(NULL,pagesize*3,PROT_READ|PROT_WRITE,MAP_SHARED,fd,offset);
    close(fd);

    for(i = 1; i<10; i++)
    {
        (*(p_map+pagesize/sizeof(people)*i-2)).age = 100;
        printf("access page %d over\n",i);
        (*(p_map+pagesize/sizeof(people)*i-1)).age = 100;
        printf("access page %d edge over, now begin to access page %d\n",i, i+1);
        (*(p_map+pagesize/sizeof(people)*i)).age = 100;
        printf("access page %d over\n",i+1);
    }
    munmap(p_map,sizeof(people)*10);
}