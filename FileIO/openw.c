#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

int main()
{
  umask(0);
  int fd = open("myfile",O_WRONLY|O_CREAT,0644);
  if(fd<0){
    perror("open erorr");
    return 1;
  }
  int count = 5;
  const char* msg ="hello world!\n";
  int len = strlen(msg);
  while(count--)
  {
    write(fd,msg,len);

  }
  close(fd);
  return 0;

}

