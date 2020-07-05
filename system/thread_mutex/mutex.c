//互斥锁的使用：抢票过程。
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<pthread.h>

int g_tickets =100;

//1.定义互斥锁
pthread_mutex_t mutex;

void *thr_Scalpers(void* arg)
{
  //黄牛的工作就是抢票
  //尽量避免对不需要加锁的操作加锁
  //加锁的越多，意味着需要的时间就愈长
  while(1)
  {
    pthread_mutex_lock(&mutex);//加锁一定在临界资源访问之前加锁。
    if(g_tickets>0){//当g_tickets=1时，进入抢票流程
      usleep(1000);
      printf("I:[%p] got a train ticket:%d\n", pthread_self(), g_tickets);
      g_tickets--;
      pthread_mutex_unlock(&mutex);
      usleep(1000);
    }else{
      //没票的时候记得解锁
      //任意有可推出的地方解锁
      pthread_mutex_unlock(&mutex);
      pthread_exit(NULL);
    }
  }
  return NULL;
}

int main()
{
  int i = 0;
  pthread_t tid[4];
  //2.初始化互斥锁
  pthread_mutex_init(&mutex,NULL);
  for(i=0;i<4;++i){
    int ret = pthread_create(&tid[i],NULL,thr_Scalpers,NULL);
    if(ret!=0){
      printf("thread create failed!\n");
      return -1;
    }
  }
  for(i =0;i<4;++i){
    pthread_join(tid[i],NULL);
  }
  //5.销毁互斥锁
  pthread_mutex_destroy(&mutex);
  return 0;
}
