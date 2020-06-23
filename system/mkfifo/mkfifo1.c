#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<error.h>

int main()
{
  mkfifo("tp",0644);
  int infd;
  infd = open("abc",O_RDONLY);
  if(infd==-1){
    perror("open error");
    exit(1);
  }
  int outfd;
  outfd = open("tp",O_WRONLY);
  if(outfd==-1)
  {
    perror("open error");
    exit(1);
  }

  char buf[1024];
  int n;
  while((n=read(infd,buf,1024))>0)
  {
    write(outfd,buf,n);
  }
  close(infd);
  close(outfd);


  return 0;
}
