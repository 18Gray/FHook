#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>


typedef struct{
    char name[4];
    int  age;
}people;


//  map_normfile2.c只是简单的映射一个文件，并以people数据结构的格式从mmap()返回的地址处读取10个people结构，并输出读取的值，然后解除映射。
main(int argc, char** argv)  // map a normal file as shared mem:
{
    int fd,i;
    people *p_map;
    fd=open( argv[1],O_CREAT|O_RDWR,00777 );
    p_map = (people*)mmap(NULL,sizeof(people)*10,PROT_READ|PROT_WRITE,
                          MAP_SHARED,fd,0);
    for(i = 0;i<10;i++)
    {
        printf( "name: %s age %d;\n",(*(p_map+i)).name, (*(p_map+i)).age);
    }
    munmap( p_map,sizeof(people)*10);
}