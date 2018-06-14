#include "stdafx.h"
#include "ThreadJob.h"
#include <process.h>


namespace CommonUtils
{

    CLock::CLock()
    {
        ::InitializeCriticalSection(&m_csLock);
        m_hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    }

    CLock::~CLock()
    {
        ::DeleteCriticalSection(&m_csLock);
        if (NULL != m_hEvent)
        {
            ::CloseHandle(m_hEvent);
            m_hEvent = NULL;
        }
    }

    void CLock::Wait(DWORD t)
    {
        if (NULL != m_hEvent)
        {
            ::WaitForSingleObject(m_hEvent, t);
        }
    }
    void CLock::Notify()
    {
        if (NULL != m_hEvent)
        {
            ::SetEvent(m_hEvent);
        }
    }
    void CLock::Locked()
    {
        ::EnterCriticalSection(&m_csLock);
    }
    void CLock::UnLock()
    {
        ::LeaveCriticalSection(&m_csLock);
    }

    BOOL CLock::TryLock()
    {
        return ::TryEnterCriticalSection(&m_csLock);
    }

    CThreadJob::CThreadJob()
        : m_hThread(NULL)
    {

    }

    CThreadJob::~CThreadJob()
    {
        if (NULL != m_hThread)
        {
            ::CloseHandle(m_hThread);
            m_hThread = NULL;
        }
    }

    UINT WINAPI CThreadJob::ThreadProc(LPVOID pObj)
    {
        CThreadJob * pJob = (CThreadJob *)pObj;
        pJob->m_uErr = pJob->Run();							// call virt fn to do the work
        return pJob->m_uErr;								// ..and return error code to Windows
    }

    void CThreadJob::Lock()
    {
        m_Lock.Locked();
    }
    void CThreadJob::Unlock()
    {
        m_Lock.UnLock();
    }

    void CThreadJob::Wait(DWORD t)
    {
        m_Lock.Wait(t);
    }
    void CThreadJob::Notify()
    {
        m_Lock.Notify();
    }

    BOOL CThreadJob::Start(BOOL bJoin)
    {
        if (bJoin)
        {
            Join();
        }
        m_bAbort = FALSE;
        m_uErr = 0;

        m_hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadProc, (LPVOID)this, 0, NULL);

        return m_hThread != NULL;
    }

    void CThreadJob::Stop()
    {
        m_bAbort = TRUE;
        Notify();
    }

    void CThreadJob::Join()
    {
        Stop();
        if (NULL != m_hThread)
        {
            WaitForSingleObject(m_hThread, INFINITE);
            m_hThread = NULL;
        }
    }
}
