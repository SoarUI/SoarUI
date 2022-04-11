#pragma once
#include "TaskExecutorBase.h"
#include "leecallback.h"
#include "LeethreadDataVessel.h"
#include "TaskExecutorBase.h"
#include "../SoarHeader/leemacro.h"
/************************************************************************/
/* LeeSDK �̳߳�                                                        */
/************************************************************************/
class ILeeThreadpool
{
public:

	ILeeThreadpool(void){}
	virtual ~ILeeThreadpool(void){}
	//�첽�ύ���������񱻴���ͷ���
	virtual void SubmitTaskAsync(ITaskExecutorBase* pTaskWorker,CLeeCallbackcompiler<void,int> &callee,TaskLevel lv=Normal) LEEPURE ;
	//ͬ���ύ���ȵ����񱻷���ŷ��أ���HANDLE����ʵ�֣���ȡ�źź󷵻�
	virtual bool SubmitTask(ThreadWorkerFtr pFunc, void* pData) LEEPURE ;
	virtual bool SubmitTask(ITaskExecutorBase* pTaskWorker) LEEPURE ;
	//
	virtual void    SetMaxSize(int maxnum) LEEPURE ; 
	virtual int     GetMaxSize(void) LEEPURE ;
	virtual void    SetAvailLow(int minnum) LEEPURE ;
	virtual int     GetAvailLow(void) LEEPURE ;
	virtual void    SetAvailHigh(int highnum) LEEPURE ; 
	virtual int     GetAvailHigh(void) LEEPURE ;
	virtual int     GetCurrentAvail(void) LEEPURE ;
	virtual int     GetCurrentThreads(void) LEEPURE ;
	virtual int     GetBusyNum(void) LEEPURE ;
	virtual void    SetInitSize(int initnum) LEEPURE ;
	virtual int     GetInitSize(void) LEEPURE ;
	virtual void    TerminateAll(void) LEEPURE ;
};
