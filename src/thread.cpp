#include "thread.h"
#include <istream>
#include <pthread.h>
#include <unistd.h>


Thread::Thread()
{
    m_ThreadID = 0;
    THREAD_STATUS_NEW = 0;
    //线程的状态－正在运行
    THREAD_STATUS_RUNNING = 1;
    //线程的状态－运行结束
    THREAD_STATUS_EXIT = 2;

    threadStatus = THREAD_STATUS_EXIT;

}


Thread::~Thread()
{

}
void* Thread::run0(void* pVoid)
{
    Thread* p = (Thread*) pVoid;
    p->run1();
    return p;
}

void* Thread::run1()
{
    m_ThreadID = pthread_self();
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);           //允许退出线程
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);    //设置立即取消


    threadStatus = THREAD_STATUS_RUNNING;

    Run();
    threadStatus = THREAD_STATUS_EXIT;
    m_ThreadID = 0;

}

bool Thread::Start()
{
    m_ThreadID = 0;
    ret = pthread_create(&m_ThreadID, NULL, run0, this);

    if(ret != 0)
    {
        m_ThreadID = 0;
    }
    return  ret == 0;
}

pthread_t Thread::getThreadID()
{
    return m_ThreadID;
}


void Thread::Cancel()
{

    if (m_ThreadID > 0)
    {
        threadStatus = THREAD_STATUS_EXIT;

        pthread_cancel(m_ThreadID);

        pthread_join(m_ThreadID, NULL);
        m_ThreadID = 0;


    }

    return;

}

void Thread::Join()
{
    if (m_ThreadID > 0)
    {

        pthread_join(m_ThreadID, NULL);
        threadStatus = THREAD_STATUS_EXIT;
        m_ThreadID = 0;

    }
    return;
}

void Thread::Cancel(unsigned long millisTime)
{

    if (m_ThreadID == 0)
    {
        return;
    }
    if (millisTime == 0)
    {
        Cancel();
    }else
    {
        unsigned long k = 0;
        while (threadStatus != THREAD_STATUS_EXIT && k <= millisTime)
        {
            usleep(100);
            k++;
        }
    }
}

int Thread::distroy()
{

    if (m_ThreadID > 0)
    {
        pthread_cancel(m_ThreadID);
       // pthread_join(m_ThreadID, NULL);
        m_ThreadID = 0;
        return 1;
    }
    return -1;
}
