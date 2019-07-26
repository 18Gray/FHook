#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>


void* pvtmMmapAlloc (char * mmapFileName, size_t size, char create)
{      
  void * retv = NULL;                                                                                              
  if (create)                                                                                         
  {                                                                                                   
    mode_t origMask = umask(0);                                                                       
    int mmapFd = open(mmapFileName, O_CREAT|O_RDWR, 00666);                                           
    umask(origMask);                                                                                  
    if (mmapFd < 0)                                                                                   
    {                                                                                                 
      perror("open mmapFd failed");                                                                   
      return NULL;                                                                                    
    }                                                                                                 
    if ((ftruncate(mmapFd, size) == 0))               
    {                                                                                                 
      int result = lseek(mmapFd, size - 1, SEEK_SET);               
      if (result == -1)                                                                               
      {                                                                                               
        perror("lseek mmapFd failed");                                                                
        close(mmapFd);                                                                                
        return NULL;                                                                                  
      }                                                                                               

      result = write(mmapFd, "", 1);
      if (result != 1)                                                                                
      {                                                                                               
        perror("write mmapFd failed");                                                                
        close(mmapFd);                                                                                
        return NULL;                                                                                  
      }                                                                                               
      retv  =  mmap(NULL, size,   
                  PROT_READ | PROT_WRITE, MAP_SHARED, mmapFd, 0);                                     

      if (retv == MAP_FAILED || retv == NULL)                                                         
      {                                                                                               
        perror("mmap");                                                                               
        close(mmapFd);                                                                                
        return NULL;                                                                                  
      }                                                                                               
    }                                                                                                 
  }                                                                                                   
  else                                                                                                
  {                                                                                                   
    int mmapFd = open(mmapFileName, O_RDWR, 00666);                                                   
    if (mmapFd < 0)                                                                                   
    {                                                                                                 
      return NULL;                                                                                    
    }                                                                                                 
    int result = lseek(mmapFd, 0, SEEK_END);                                                          
    if (result == -1)                                                                                 
    {                                                                                                 
      perror("lseek mmapFd failed");                  
      close(mmapFd);                                                                                  
      return NULL;                                                                                    
    }                                                                                                 
    if (result == 0)                                                                                  
    {                                                                                                 
      perror("The file has 0 bytes");                           
      close(mmapFd);                                                                                  
      return NULL;                                                                                    
    }                                                                                              
    retv  =  mmap(NULL, size,     
                PROT_READ | PROT_WRITE, MAP_SHARED, mmapFd, 0);                                       

    if (retv == MAP_FAILED || retv == NULL)                                                           
    {                                                                                                 
      perror("mmap");                                                                                 
      close(mmapFd);                                                                                  
      return NULL;                                                                                    
    }                                                                                                 

    close(mmapFd);                                                                                    

  }                                                                                                   
  return retv;                                                                                        
}


