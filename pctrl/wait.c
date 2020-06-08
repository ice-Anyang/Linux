#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>

int main()
{
  pid_t pid = fork();

  if(pid == 0)
  {
    sleep(5);
    exit(3);
  }
  int status;
  while(waitpid(pid, &status, WNOHANG) == 0) {
    printf("还没放学，打一桌麻将~\n");
    sleep(1);
                        
  }
  if (!(status & 0x7f)) {
    printf("child exit code:%d\n", (status>>8)&0xff);            
  }
  if (WIFEXITED(status)) {
    printf("child exit code:%d\n", WEXITSTATUS(status));
                
  }
  while(1) {
    printf("打一桌麻将~\n");
    sleep(1);                    
  }
  return 0;
}
