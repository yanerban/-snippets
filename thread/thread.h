#ifndef __THREAD_H__
#define __THREAD_H__
#include <pthread.h>
#include <stdint.h>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <string>
#include <atomic>
#include "noncopyable.h"
namespace base
{
class Thread:Noncopyable
{
 public:
   typedef std::function<void()> ThreadFunc;
  Thread(const ThreadFunc& cb, const std::string& name = std::string());

  Thread(const ThreadFunc&& cb, const std::string& name);
  ~Thread();
  void Start();
  void Join();
  
  const std::string& name() const { return threadName_; }
  pid_t Tid() const { return tid_; }

  static int  NumThread()  { return threadNums_.load(); } 
 private:
  void SetDefaltName();
  static void* StartThread(void* obj);
  void RunInThread();
  bool started_;
  bool joined_;
  pid_t tid_;
  pthread_t pthreadId_;
  std::string threadName_;
  ThreadFunc func_;
  std::mutex mutex_;
  std::condition_variable cond_;
  static std::atomic<int> threadNums_;
};

namespace CurrentThread
{
int CachedId();
const char* ThreadName();
pid_t Tid();
}
}

#endif   /* __THREAD_H__ */
