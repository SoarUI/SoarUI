#ifndef _IThreadBase_h_
#define _IThreadBase_h_

/********************************************************************
    Descibe:      线程基类，对线程抽象，实现可移植性
    created:      2012/8/23 22:50
    FullName:     d:\测试项目\Threadpooltest\IThreadBase.h
    Path:         d:\测试项目\Threadpooltest
    ClassName:    IThreadBase
    FileType:     h
    Author:       nicklisir@163.com
    NeededLib:    
    requires:      
*********************************************************************/
#include "leethreadDatavessel.h"
#include "ccriticalSection.h"
class CThreadPoolEx;
class ITaskExecutorBase;
class IThreadBase
{
public:
	IThreadBase(void):
	d_shutdown(false),
	d_pool(NULL),
	d_hThread(NULL),d_hWakeup(NULL),d_hdestroy(NULL),d_hCompeteOneShot(NULL){}
public:
	virtual ~IThreadBase(void){}
	virtual void Execute(void) =0;//实质执行
	virtual bool     Start(void)=0;        //Start to execute the thread 
	virtual bool     Terminate(void)=0;    //Terminate the threa 
	virtual bool     Initialise(void)=0;
	virtual void     Exit(void){d_shutdown =true;} 
	virtual bool     Wakeup(void){return SetEvent(d_hWakeup);}
	virtual bool     Resume(void){return ResumeThread(d_hThread);}
	virtual bool     Sleep(void){return SuspendThread(d_hThread);}
	int      GetThreadID(void){return d_threadID;} 
	HANDLE   GetHandle(void){return d_hThread;}
	bool     SetPriority(int priority); 
	int      GetPriority(void); 
	void SetThreadPool(CThreadPoolEx* pool){d_sync.Enter();d_pool =pool;d_sync.Leave();}
	CThreadPoolEx* GetThreadPool(void){return d_pool;}
	virtual void SetTask(_TaskData * data)
	{d_sync.Enter();
	d_threadData.lpWorkerRotine =data->lpWorkerRotine;
	d_threadData.executor =data->executor;
	d_threadData.pData =data->pData;
	d_sync.Leave();}
	HANDLE GetWakeup(void)
	{
		return d_hWakeup;
	}
	HANDLE GetOneShotComplete(void)
	{
		return d_hCompeteOneShot;
	}
	void SetOneShotComplete(HANDLE hOneShot)
	{
		d_hCompeteOneShot =hOneShot;
	}
	HANDLE GetDestroy(void)
	{
		return d_hdestroy;
	}
protected:
	DWORD d_threadID;
	HANDLE d_hThread;
	HANDLE d_hWakeup;
	HANDLE d_hCompeteOneShot;//完成一次
	HANDLE d_hdestroy;
	bool d_shutdown;
	_TaskData d_threadData;
	CThreadPoolEx * d_pool;
	CcriticalSection d_sync;
};
#endif