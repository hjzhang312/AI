#ifndef THREAD_H
#define THREAD_H

/*************************************************************************
    > File Name: Thread.cpp
    > Author: Jan
    > Mail: jan.zhang@hansong-china.com
    > Created Time: 2016年04月18日 星期一
 ************************************************************************/

#include <iostream>
#include <pthread.h>


using namespace std;

class Thread
{
private:
    int ret;

    //线程的状态
    int threadStatus;
    //获取执行方法的指针
    static void* run0(void* pVoid);
    //内部执行方法
    void* run1();

    //线程的状态－新建
    int THREAD_STATUS_NEW;
    //线程的状态－正在运行
    int THREAD_STATUS_RUNNING;
    //线程的状态－运行结束
    int THREAD_STATUS_EXIT;


public:
    Thread();
    ~Thread();

    //当前线程的线程ID
    pthread_t m_ThreadID;
    //线程的运行实体
    virtual void Run() = 0;
    //开始执行线程
    bool Start();
    //获取线程ID
    pthread_t getThreadID();
    //获取线程状态
    //等待线程直至退出
    void Cancel();
    void Join();
    //等待线程退出或者超时
    void Cancel(unsigned long millisTime);
    int distroy();



};



#endif // THREAD_H
