#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<pthread.h>

void test()
{
  pthread_exit(NULL);
  return;
}

void *thread_start(void* arg)
{
  printf("the parameter passed are :%s",(char*)arg);
  //sleep(3);
  //pthread_exit(NULL);
  //return NULL;
  while(1){
    printf("this is a normal thread:%p\n",pthread_self());
    sleep(1);
  }
  return NULL;
}

//程序运行之后，操作系统会自动创建一个主线程pcb调度main函数
int main()
{
  pthread_t tid;
  char* buf="jintiantianqizhenhaoa~~\n";
  int ret = pthread_create(&tid,NULL,thread_start,(void*)buf);
  if(ret!=0)
  {
    printf("Thread cereation failed\n");
    return -1;
  }
  printf("normal thread id:%p\n",tid);
  //sleep(3);
  //pthread_cancel(tid);
  pthread_exit(NULL);
  while(1)
  {
    printf("this is main thread:%p\n",pthread_self());
    sleep(1);
  }
  return 0;
}
