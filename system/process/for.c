#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main()
{

  pid_t pi = fork();

  if(pi < 0)
  {
    perror("fork is error!\n");
    return  1;
  }
  else if(pi == 0)
  {
    printf("This is child fork,pid = %d\n",getpid());
    sleep(5);
  }
  else 
  {
    printf("This is parent fork,pid = %d\n",getpid());
    sleep(3);
    exit(0);
  }
  return 0;
}
