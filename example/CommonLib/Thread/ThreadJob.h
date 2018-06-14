/********************************************************************
//	File Name	:	ThreadJob.h
//	Version		:	1.0
//	Datetime	:	2009-8-12  19:39	--	Created.
//	Author		:
//	Comment		:	线程函数接口，需要创建线程的类继承自这个即可
*********************************************************************/
#pragma once
#include <Windows.h>
#include <queue>

namespace CommonUtils
{

    /////////////////////////////CLock///////////////////////////////////
    class CLock
    {
    public:
        CLock();
        ~CLock();

        class CAutoLock
        {
        public:
            CAutoLock(CLock & lock) :m_lock(lock){ m_lock.Locked(); };
            ~CAutoLock(){ m_lock.UnLock(); };

        private:
            CLock & m_lock;
        };

    public:
        void Wait(DWORD t = INFINITE);
        void Notify();
        void Locked();
        void UnLock();
        BOOL TryLock();

    private:
        CRITICAL_SECTION	m_csLock;
        HANDLE				m_hEvent;
    };


    template<class T>
    class CQueue
    {
    public:
        CQueue(){};
        ~CQueue(){};

    public:
        size_t size()
        {
            CLock::CAutoLock lock(m_lock);
            return m_queue.size();
        }

        void push(const T & t)
        {
            CLock::CAutoLock lock(m_lock);
            m_queue.push(t);
        }

        void pop()
        {
            CLock::CAutoLock lock(m_lock);
            m_queue.pop();
        }

        T& front()
        {
            CLock::CAutoLock lock(m_lock);
            return m_queue.front();
        }

    private:
        std::queue<T> m_queue;
        CLock  m_lock;
    };

    //////////////////////////////CThreadJob//////////////////////////////////

    class CThreadJob
    {
    public:
        CThreadJob();
        virtual ~CThreadJob();

    public:
        virtual void Lock();					// get thread-object lock
        virtual void Unlock();					// release thread-object lock
        virtual void Wait(DWORD t = INFINITE);	// thread suspend
        virtual void Notify();					// thread resume
        virtual BOOL Start(BOOL bJoin = TRUE);										// start work
        virtual void Join();

    protected:
        virtual UINT Run() = 0;										// you must implement to do the work
        virtual void Stop();			// stop work

        UINT	m_ucbMsg;											// callback message for OnProgress
        UINT	m_uErr;												// thread error code
        BOOL	m_bAbort;											// whether to abort: DoWork must check this
        HANDLE	m_hThread;
        DWORD	m_nThreadId;
        CLock	m_Lock;												// object lock

    private:
        static	UINT WINAPI ThreadProc(LPVOID pObj);
    };

}
