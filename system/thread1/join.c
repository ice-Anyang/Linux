//这个用于演示线程等待/分离的接口

#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<pthread.h>
void* thr_start(void* arg)
{
  //分离自己
  pthread_detach(pthread_self());
  printf("休眠三秒\n");
  sleep(1);
  char* buf = "你好呀！\n";
  return buf;
}

int main()
{
  pthread_t tid;
  int ret = pthread_create(&tid,NULL,thr_start,NULL);
  if(ret!=0){
    printf("thread create failed!\n");
    return -1;
  }
  printf("Main thread waiting for normal to exit\n");
  char *rev = NULL;
  //pthread_join(tid,(void**)&rev);
  //ptnread_detch(tid);
  while(1)
  {
    printf("rev:%s\n",rev);
    sleep(1);
  }
  return 0;
}
