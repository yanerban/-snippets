#ifndef __THREAD_H__
#define __THREAD_H__
#include <pthread.h>
#include <stdint.h>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <string>
#include <atomic>
namespace base
{
class Thread
{
 public:
   typedef std::function<void()> ThreadFunc;
  Thread(const ThreadFunc& cb, const std::string& name );

  Thread(const ThreadFunc&& cb, const std::string& name);
  ~Thread();
  void Start();
  void Join();
  static void* ThreadStart(void* obj);
  std::string name() { return threadName_; }
  pid_t Tid() { return tid_; }
 private:
  bool started_;
  bool joined_;
  pid_t tid_;
  std::string threadName_;
  std::mutex mutex_;
  std::condition_variable cond_;
  std::atomic<int> ThreadNums_;
};
}



#endif   /* __THREAD_H__ */
