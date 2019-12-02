#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

// map_normalfile1.c及map_normalfile2.c。编译两个程序，可执行文件分别为map_normalfile1及map_normalfile2。
// 两个程序通过命令行参数指定同一个文件来实现共享内存方式的进程间通信。map_normalfile2试图打开命令行参数指定的一个普通文件，
// 把该文件映射到进程的地址空间，并对映射后的地址空间进行写操作。map_normalfile1把命令行参数指定的文件映射到进程地址空间，然后对映射后的地址空间执行读操作。
// 这样，两个进程通过命令行参数指定同一个文件来实现共享内存方式的进程间通信。
typedef struct{
    char name[4];
    int  age;
}people;


// map_normalfile1.c首先定义了一个people数据结构，（在这里采用数据结构的方式是因为，共享内存区的数据往往是有固定格式的，这由通信的各个进程决定，
// 采用结构的方式有普遍代表性）。map_normfile1首先打开或创建一个文件，并把文件的长度设置为5个people结构大小。然后从mmap()的返回地址开始，
// 设置了10个people结构。然后，进程睡眠10秒钟，等待其他进程映射同一个文件，最后解除映射。
main(int argc, char** argv) // map a normal file as shared mem:
{
    int fd,i;
    people *p_map;
    char temp;

    fd=open(argv[1],O_CREAT|O_RDWR|O_TRUNC,00777);
    lseek(fd,sizeof(people)*5-1,SEEK_SET);
    write(fd,"",1);

    p_map = (people*) mmap( NULL,sizeof(people)*10,PROT_READ|PROT_WRITE,
                            MAP_SHARED,fd,0 );
    close( fd );
    temp = 'a';
    for(i=0; i<10; i++)
    {
        temp += 1;
        memcpy( ( *(p_map+i) ).name, &temp,2);
        ( *(p_map+i) ).age = 20+i;
    }
    printf(" initialize over \n ");
    sleep(10);
    munmap( p_map, sizeof(people)*10);
    printf( "umap ok \n" );
}