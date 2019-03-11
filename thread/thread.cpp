/**************************************************************************
 *      FileName: thread.cpp
 * 
 *        Author: 
 *   Description: ---
 *  Created Time: 2019年03月09日 星期六 13时08分32秒
 * Last Modified: 2019-03-09 13:08:32
 * 
 **************************************************************************/
#include "thread.h"
#include <sys/syscall.h>
#include <linux/unistd.h>
#include <unistd.h>
#include <assert.h>
namespace Detail
{
__thread int t_cachedTid = 0;
__thread const char* t_threadName = "Main";
__thread char buf[32];

}


namespace
{

int GetTid()
{
  return static_cast<pid_t>(::syscall(SYS_gettid));
}
}

using namespace base;
//得到线程的的真实tid
int CurrentThread::CachedId()
{
  if(Detail::t_cachedTid == 0)
    Detail::t_cachedTid = GetTid();
  return Detail::t_cachedTid;
}

const char* CurrentThread::ThreadName()
{
  return Detail::t_threadName;
}

pid_t CurrentThread::Tid()
{
  return CachedId();
}

Thread::Thread(const ThreadFunc& cb, const std::string& name )
  :started_(false),
  joined_(false),
  tid_(0),
  threadName_(name),
  func_(cb)
{
  SetDefaltName();
}

std::atomic<int> Thread::threadNums_(0);
Thread::Thread(const ThreadFunc&& cb, const std::string& name)
  :started_(false),
  joined_(false),
  tid_(0),
  pthreadId_(0),
  threadName_(name),
  func_(std::move(cb))
{
  SetDefaltName();
}

Thread::~Thread()
{
  if(started_ && !joined_)
    pthread_detach(pthreadId_);
}

void Thread:: SetDefaltName()
{
  int num = ++threadNums_;
  char buf[32];
  if(threadName_.empty())
  {
    snprintf(buf, sizeof buf, "Thread#%d",num);
    threadName_ = buf;
  }
}

void* Thread::StartThread(void* obj)
{
  Thread* thread = static_cast<Thread*>(obj);
  thread->RunInThread();
  
  return NULL; 
}

void Thread::RunInThread()
{
  {
    std::unique_lock<std::mutex> lock(mutex_);
    tid_ = CurrentThread::Tid();
    cond_.notify_all();
  }
   
  Detail::t_threadName = threadName_.empty()? "MainThread" : threadName_.c_str();
  
  func_();
}

void Thread::Start()
{
  assert(!started_);
  started_ = true;

  if(pthread_create(&pthreadId_, NULL, StartThread, this))
  {
    printf("thread create fail\n");
    return;
  }
  {
    std::unique_lock<std::mutex> lock(mutex_);
    while(tid_ == 0)
      cond_.wait(lock);
  }
}

void Thread::Join()
{
  assert(started_);
  assert(!joined_);
  joined_ = true;
  pthread_join(pthreadId_, NULL);
}
