#include<stdio.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>

int main()
{
  close(0);
  //close(2);
  int fd = open("myfile", O_RDONLY);
  if(fd < 0){
  perror("open");
  return 1;
  }
  printf("fd: %d\n", fd);
  close(fd);
  return 0;
}
