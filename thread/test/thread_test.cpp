/**************************************************************************
 *      FileName: thread_test.cpp
 * 
 *        Author: 
 *   Description: ---
 *  Created Time: 2019年03月10日 星期日 13时33分45秒
 * Last Modified: 2019-03-10 13:33:45
 * 
 **************************************************************************/
#include "../thread.h"
#include <stdio.h>

using namespace base;

void Print()
{
  printf("print:  tid = %d\n %s\n", CurrentThread::Tid(), CurrentThread::ThreadName());
}
int main(int argc, char *argv[])
{
  Thread t1(Print,"test1");
  t1.Start();
  printf("t1 tid = %d, name = %s\n", t1.Tid(), t1.name().c_str());
  
  Thread t2(Print, "test2");
  t2.Start();

  Thread t3(Print);
  t3.Start();

  t1.Join();
  t2.Join();
  t3.Join();
  printf("t2 tid = %d\n", t2.Tid());
  printf("thread nums %d\n", Thread::NumThread());
  return 0;
}
