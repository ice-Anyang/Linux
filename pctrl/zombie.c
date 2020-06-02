#include<stdio.h>
#include<unistd.h>

int val = 2;
int main()
{
  pid_t pid = getgid();

  pid_t child_pid = fork();
  //子进程进入
  while(child_pid == 0)
  {
    val = 10;
    printf("this is child:%d---%d-%p\n", getpid(), val, &val); 
  }
  //父进程进入
  while(child_pid > 0)
  {
    val = 20;
    sleep(1);
    printf("--------%d---ppid:%d\n", getpid(), pid);
  }
  
  //父子进程共享代码
  while(1)
  {
    printf("--------%d---ppid:%d\n", getpid(), pid);
    sleep(100);
  }

  return 0;

}
