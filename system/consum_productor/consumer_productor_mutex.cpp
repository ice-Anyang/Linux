// 实现消费者与生产者模型（mutex与cond）

#include<iostream>
#include<cstdio>
#include<queue>
#include<pthread.h>
//using namespace std;
#define MAX_QUEUE 5

class BlockQueue
{
  public:
    BlockQueue(int max = MAX_QUEUE):
      _capacity(max)
  {
    pthread_mutex_init(&_mutex,NULL);
    pthread_cond_init(&_cond_con,NULL);
    pthread_cond_init(&_cond_pro,NULL);
  }
    ~BlockQueue()
    {
      pthread_mutex_destroy(&_mutex);
      pthread_cond_destroy(&_cond_con);
      pthread_cond_destroy(&_cond_pro);
    }
  public:
    bool Push (int data)
    {
      //入队
      pthread_mutex_lock(&_mutex);
      //1.加锁
      //2.判断是否满
      while(_queue.size()==_capacity)
      {
        pthread_cond_wait(&_cond_con,&_mutex);
      }
      //3.添加新节点
      _queue.push(data);
      //4.唤醒消费者
      pthread_cond_signal(&_cond_con);
      //5.解锁
      pthread_mutex_unlock(&_mutex);
    }
    bool Pop(int *data)
    {
      // 出队操作
      // 1.加锁
      pthread_mutex_lock(&_mutex);
      // 2.判断是否满
      while(_queue.empty()){
        pthread_cond_wait(&_cond_con,&_mutex);
      }
      *data = _queue.front();
      _queue.pop();
      //唤醒生产者
      pthread_cond_signal(&_cond_pro);
      // 解锁操作
    }
  private:
    std::queue<int> _queue;
    int _capacity;
    pthread_mutex_t _mutex;
    pthread_cond_t _cond_con;
    pthread_cond_t _cond_pro;
};

#define MAX_THREAD 5

void *thr_consumer(void* arg)
{
  BlockQueue *q = (BlockQueue*)arg;
  while(1)
  {
    int data;
    q->Pop(&data);
    printf("消费者:%p 出队数据:%d\n", pthread_self(), data);
  }
}

void *thr_produtor(void* arg)
{
  BlockQueue* q = (BlockQueue*)arg;
  int i = 0;
  while(1){
    q->Push(i);
    printf("生产者：%p 入队数据：%d\n", pthread_self(), i++);
  }
}

int main()
{
  pthread_t ctid[MAX_THREAD];
  pthread_t ptid[MAX_THREAD];

  int i,ret;
  BlockQueue q;

  //消费者线程
  for(i = 0; i<MAX_THREAD;++i)
  {
    ret = pthread_create(&ctid[i],NULL,thr_consumer,(void*)&q);
    if(ret!=0)
      return -1;
    }
  }

  for(i=0;i<MAX_THREAD;i++)
  {
    ret = pthread_create(&ptid[i],NULL,thr_produtor,(void*)&q);
    if(ret!=0)
    {
      return -1;
    }
  }
  //等待所有线程退出
  for (i = 0; i < MAX_THREAD; i++) {
    pthread_join(ctid[i], NULL);
    pthread_join(ptid[i], NULL);
  }
  return 0;
}
