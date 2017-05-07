/**
 * @file     pipeline.c
 *           
 * 流水线模式2 CPU
 * 前一个步骤计算数据后，把数据放在下一个线程的data中，然后通知, 当后面的线程的数据还没
 *  处理时（ready)，就暂停，等待need_data
 */

#include	<stdio.h>
#include <iostream>
#include	<pthread.h>
#include	<unistd.h>
#include  <stdbool.h>
#include <assert.h>
#include	<zhao/tools.h>

struct MutexScopeLock {
  MutexScopeLock(pthread_mutex_t *mtx) : _mtx(mtx)
  {
    pthread_mutex_lock(_mtx);
  }

  ~MutexScopeLock() {
    pthread_mutex_unlock(_mtx);
  }

  pthread_mutex_t *_mtx;
};

struct PipeThread{
    PipeThread(int id, PipeThread *next) {
      _id = id;
      _next = next;
      PCHECK(pthread_mutex_init(&mtx, NULL));
      PCHECK(pthread_cond_init(&is_ready_cond, NULL));
      PCHECK(pthread_cond_init(&need_data_cond, NULL));
      is_ready = false;
    }

    ~PipeThread() {
      PCHECK(pthread_mutex_destroy(&mtx));
      PCHECK(pthread_cond_destroy(&is_ready_cond));
      PCHECK(pthread_cond_destroy(&need_data_cond));
      is_ready = false;
    }

    PipeThread(PipeThread &) = delete;

    void set_new_data(int num) {
      MutexScopeLock lock(&mtx);
      printf("set_new_data to thread[%d]\n", _id);
      while (is_ready) {
        printf("set_new_data to thread[%d] wait need_data_cond\n", _id);
        pthread_cond_wait(&need_data_cond, &mtx);
        printf("set_new_data to thread[%d] wait need_data_cond weekup\n", _id);
      }
      data = num;
      is_ready = true;
      pthread_cond_signal(&is_ready_cond);
    }

    //other thread call this methond
    //bool wait_for_no_ready() {
    //}

    int get_new_data() {
      int num;
      MutexScopeLock lock(&mtx);
      printf("thread[%d] get_new_data\n", _id);
      while (!is_ready) {
        pthread_cond_signal(&need_data_cond);
        printf("thread[%d] get_new_data wait\n", _id);
        pthread_cond_wait(&is_ready_cond, &mtx);
        printf("thread[%d] get_new_data wait weekup\n", _id);
      }
      num = data;
      is_ready = false;
      return num;
    }

    void stop() {
      is_stop = false;
    }


    static void *do_run(void *arg) {
      PipeThread* self = (PipeThread*)arg;
      pthread_detach(pthread_self());
      printf("thread[:%d] running...\n", self->_id);

      while (not self->is_stop) {
        int num = self->get_new_data();

        //simulate work cost time
        sleep(1);
        num += 1;

        if (self->_next)
          self->_next->set_new_data(num);
        else
          printf("pipeline 结果为:%d\n", num);
      }
      return NULL;
    }

    void run() {
      assert(not is_stop);
      pthread_t pth_id;
      PCHECK(pthread_create(&pth_id, NULL, do_run, this));
    }

    int data;
    int is_stop;

    int _id;
    pthread_cond_t is_ready_cond; //有新的数据，每次迭代完检测
    bool is_ready;

    pthread_cond_t need_data_cond;//当前数据处理完了，等待新的数据到来

    pthread_mutex_t mtx;
    PipeThread *_next=nullptr;
};

struct PipeLine {
  PipeLine(int threadNum) 
      :_threadNum(threadNum)
      ,_head(nullptr)
  {
    for (int i=threadNum; i > 0; i--) {
      PipeThread *th = new PipeThread(i, _head);
      _head = th;
    }
  }

  void run() {
    PipeThread *th = _head;
    while (th) {
      th->run();
      th = th->_next;
    }
  }

  void stop() {
    PipeThread *th = _head;
    while (th) {
      th->stop();
      th = th->_next;
    }
  }

  void process(int num) {
    printf("process num:%d\n", num);
    _head->set_new_data(num);
  }

  int _threadNum;
  PipeThread* _head;
};

int main()
{
  PipeLine pipeline(4);
  pipeline.run();

  int num;
  printf("Please Enter number: ");
  while (std::cin >> num) {
    if (num == 0) {
      pipeline.stop();
      break;
    }
    pipeline.process(num);
  }
    
  return 0;
}


