#ifndef _ThreadPool_h_
#define _ThreadPool_h_

/********************************************************************
    created:    2015/02/10
    created:    2015/2/10::   12:09
    filename:   E:\2015it\LeeSDK\headers\leesdk.ultity\threadpool\LeeThreadPool.h
    file path:  E:\2015it\LeeSDK\headers\leesdk.ultity\threadpool
    file base:  LeeThreadPool
    file ext:   h
    author:     nicklisir/lee
    copyright:  nicklisir@gmail.com QQ327681967
    purpose:    thread Pool
*********************************************************************/
class ITaskExecutorBase;
class IThreadBase;
#include "Singleton_t.h"
#include <list>
#include <map>
#include "threadpool/LeethreadDataVessel.h"
#include "threadpool/ILeeThreadpool.h"
#include "CcriticalSection.h"
class CThreadPoolEx:public ILeeThreadpool,public LEE::Singleton_t<CThreadPoolEx>
{
public:
	typedef std::list<_TaskData*> TaskList;
	typedef std::list<IThreadBase*> ThreadList;
	typedef std::map<unsigned int,IThreadBase*,std::less<unsigned int> >ThreadBusyList;
	friend class IThreadBase;
	friend class CThread;
public:
	CThreadPoolEx(void);
	CThreadPoolEx(unsigned int Init);
public:
	virtual ~CThreadPoolEx(void);
	//异步提交，不等任务被处理就返回
	virtual void SubmitTaskAsync(ITaskExecutorBase* pTaskWorker,CLeeCallbackcompiler<void,int> &callee,TaskLevel lv=Normal);
    //同步提交，等到任务被分配才返回，用HANDLE对象实现，获取信号后返回
	virtual bool SubmitTask(ThreadWorkerFtr pFunc, void* pData);
	virtual bool SubmitTask(ITaskExecutorBase* pTaskWorker);
	//
	virtual void    SetMaxSize(int maxnum){d_MaxSize = maxnum;} 
	virtual int     GetMaxSize(void){return d_MaxSize;} 
	virtual void    SetAvailLow(int minnum){d_AvailLow = minnum;} 
	virtual int     GetAvailLow(void){return d_AvailLow;} 
	virtual void    SetAvailHigh(int highnum){d_Availhigh = highnum;} 
	virtual int     GetAvailHigh(void){return d_Availhigh;} 
	virtual int     GetCurrentAvail(void){return d_currentAvailThreads;} 
	virtual int     GetCurrentThreads(void){return d_currentSize;} 
	virtual int     GetBusyNum(void){return d_BusyLists.size();} 
	virtual void    SetInitSize(int initnum){d_initSize = initnum;} 
	virtual int     GetInitSize(void){return d_initSize;} 
	virtual void    TerminateAll(void); 
protected:  
	void    AppendToIdleList(IThreadBase* jobthread); 
	void    AppendToBusyList(IThreadBase* idlethread); 
	void    RemoveFromBusyList(IThreadBase* idlethread); 
	void    NotifyTaskFinish(IThreadBase* thread);
	//对线程操作
	void    DeleteIdleThread(int num); 
	void    CreateIdleThread(int num); 
	IThreadBase* GetIdleThread(void);
	IThreadBase* GetLastIdleThread(void);
	bool IsIdleThreadAvaible(void); 
	//删除线程占用的堆
	void destroyThread(DWORD threadID);
	void destroyThread(IThreadBase* thread);
	//对task操作
	IThreadBase*   DispatchTask(void);
	bool IsTaskAvaible(void);
	_TaskData* PopTask(void);
    void    PushTask(_TaskData * data,TaskLevel lv);
private:
	TaskList d_tasklists;//普通队列
	ThreadBusyList d_BusyLists; // 繁忙列表
	ThreadBusyList d_ThreadLists;//保存线程数据
	ThreadList d_IdleLists;//空闲列表
	unsigned int d_MaxSize;//最大池尺寸
	unsigned int d_AvailLow;//最小可用线程数
	unsigned int d_Availhigh;//最大可用线程数
	LONG d_currentAvailThreads;//当前可用线程数
	LONG d_currentSize;//当前池尺寸
	unsigned int d_initSize;//初始尺寸
	//
	CcriticalSection d_normalTaskLock;//对任务列表保护
	CcriticalSection d_busyLock;//对Busy列表进行保护
	CcriticalSection d_idleLock;//对Idle列表进行保护
	CcriticalSection d_threadLock;//对线程增加/减少保护
};
#endif