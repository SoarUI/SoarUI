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
	//�첽�ύ���������񱻴���ͷ���
	virtual void SubmitTaskAsync(ITaskExecutorBase* pTaskWorker,CLeeCallbackcompiler<void,int> &callee,TaskLevel lv=Normal);
    //ͬ���ύ���ȵ����񱻷���ŷ��أ���HANDLE����ʵ�֣���ȡ�źź󷵻�
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
	//���̲߳���
	void    DeleteIdleThread(int num); 
	void    CreateIdleThread(int num); 
	IThreadBase* GetIdleThread(void);
	IThreadBase* GetLastIdleThread(void);
	bool IsIdleThreadAvaible(void); 
	//ɾ���߳�ռ�õĶ�
	void destroyThread(DWORD threadID);
	void destroyThread(IThreadBase* thread);
	//��task����
	IThreadBase*   DispatchTask(void);
	bool IsTaskAvaible(void);
	_TaskData* PopTask(void);
    void    PushTask(_TaskData * data,TaskLevel lv);
private:
	TaskList d_tasklists;//��ͨ����
	ThreadBusyList d_BusyLists; // ��æ�б�
	ThreadBusyList d_ThreadLists;//�����߳�����
	ThreadList d_IdleLists;//�����б�
	unsigned int d_MaxSize;//���سߴ�
	unsigned int d_AvailLow;//��С�����߳���
	unsigned int d_Availhigh;//�������߳���
	LONG d_currentAvailThreads;//��ǰ�����߳���
	LONG d_currentSize;//��ǰ�سߴ�
	unsigned int d_initSize;//��ʼ�ߴ�
	//
	CcriticalSection d_normalTaskLock;//�������б���
	CcriticalSection d_busyLock;//��Busy�б���б���
	CcriticalSection d_idleLock;//��Idle�б���б���
	CcriticalSection d_threadLock;//���߳�����/���ٱ���
};
#endif