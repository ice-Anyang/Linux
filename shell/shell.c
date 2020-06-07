#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<sys/wait.h>

int main()
{
  while(1){

    printf("[ice@wangji] $");
    //刷新缓冲区
    fflush(stdout);
    char buf[1024]={0};
    //标准输入读取数据
    fgets(buf,1024,stdin);
    buf[strlen(buf)-1]='\0';//将'\n’改为\0'
    printf("cmd:[%s]\n",buf);

    //ls -a  -l  
    char* ptr =buf;
    char* arvg[32]={NULL};
    int argc = 0;

    while(*ptr !='\0')
    {
      if(!isspace(*ptr))
      {
        arvg[argc++] = ptr;
        while(*ptr!='\0'&&!isspace(*ptr))
          ptr++;
        *ptr='\0';
      }
      ptr++;
    }
    arvg[argc]=NULL;
    int i = 0;
    for(i=0;i<argc;++i)
    {
      printf("argv[%d]=[%s]\n",i,arvg[i]);
    }
    pid_t pid = fork();
    if (pid == 0) {  
      execvp(arvg[0], arvg);//子进程进行程序替换
      exit(0);                                 
     }
    waitpid(pid, NULL, 0);//阻塞等待子进程命令执行完毕
  }
  return 0;
}
