// 描    述：在这个demo中实现一个线程池类供外部使用 
// *       实现一个线程池任务类 
// *       实现一个线程池类
// *
// *       通过让用户传入要处理的数据以及方法的方式完全与外部解耦和

#include <cstdio>
#include <iostream>
#include <queue>
#include <stdlib.h>
#include <pthread.h>

#include <unistd.h>
#include <time.h>

typedef void (*task_handler_t) (int arg);
class ThreadTask
{
  public:
    ThreadTask() {}
    ThreadTask(const int &data, task_handler_t handler):
      _data(data), _handler(handler){   }
    void SetTask(const int &data, task_handler_t handler) {
      _data = data;
      _handler = handler;

    }
    void Run() {
      return _handler(_data);

    }
  private:
    int  _data;
    task_handler_t _handler;
};

#define THREAD_COUNT 5
class ThreadPool
{
  public:
    ThreadPool(int thr_count = THREAD_COUNT):_max_thr(thr_count){
      pthread_mutex_init(&_mutex, NULL);
      pthread_cond_init(&_con_pool, NULL);
      for (int i = 0; i < _max_thr; i++) {
        pthread_t tid;
        //void *thread_routine(void *arg);
        int ret = pthread_create(&tid, NULL, thr_routine, (void*)this);
        if (ret != 0) {
          std::cerr << "thread create error\n";
          exit(0);

        }
        pthread_detach(tid);//不需要知道返回值的内容，分离线程
      }
    }
    ~ThreadPool(){
      pthread_mutex_destroy(&_mutex);
      pthread_cond_destroy(&_con_pool);

    }
    bool TashPush(const ThreadTask &task) {
      //外部的生产者实现任务入队
      //这里线程池任务队列并没有设置最大节点上限,因此不需要判断阻塞
      pthread_mutex_lock(&_mutex);
      _queue.push(task);
      pthread_mutex_unlock(&_mutex);
      pthread_cond_signal(&_con_pool);
      return true;
    }

    void QueueLock(){
      pthread_mutex_lock(&_mutex);
    }
    bool QueueIsEmpty(){
      return _queue.empty();
    }
    void ThreadWait(){
      pthread_cond_wait(&_con_pool,&_mutex);
    }
    bool TaskPop(){
      *task = _queue.front();
      _queue.pop();
      return true;
    }
    void QueueUnlock(){
      pthread_mutex_unlock(&_mutex);
    }
    // 定义静态函数 ，需要无隐形指针
  private:
    //这个函数一个类成员函数，有一个隐藏参数this指针，因此需要定义成为静态函数
    //这是一个静态函数,没有this指针，导致无法直接访问类的内部成员变量
    //通过参数传入当前的对象this指针，进而访问对象的公有成员变量以及成员函数
    static void *thr_routine(void *arg) {
      ThreadPool *pool = (ThreadPool*)arg;
      while(1) {
        //循环从队列获取任务进行处理
        pool->QueueLock();//类外无法直接访问类的私有成员，因此需要通过公有接口实现操作
        while(pool->QueueIsEmpty()) {//访问_queue是否为NULL
          pool->ThreadWait(); //访问_con_pool以及_mutex
        }
        ThreadTask task;
        pool->TaskPop(&task); // 访问_queue
        pool->QueueUnlock();  // 访问_mutex
        //任务的处理一定要放在解锁之                                                                                                                                                                                                           //否则，如果任务处理时间过长会导致其它线程获取不到任务
        task.Run();//通过run接口直接通过用户传入的方法完成数据处理
      }
    }
  private:
    int _max_thr;//线程的数量
    std::queue<ThreadTask> _queue;//任务队列
    pthread_mutex_t _mutex; //实现队列的互斥操作
    pthread_cond_t _con_pool;//线程池中的线程等待队列

};
void test(int data)
{
  srand(time(NULL));
  int sec = rand()%5;
  printf("thread:%p:%d sleep %d sec\n", pthread_self(), data, sec);
  sleep(sec);

}
int main()
{
  ThreadPool pool;
  for (int i = 0; i < 10; i++) {
    ThreadTask task;
    task.SetTask(i, test);// Run() {test(i)}
  pool.TashPush(task);

  }
  while(1) {
    sleep(1);

  }
  return 0;

}



