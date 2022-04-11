#ifndef _Thread_h_
#define _Thread_h_
/********************************************************************
    Descibe:      对线程函数的抽象封装目前只实现Windows平台
    created:      2012/8/22 22:02
    FullName:     d:\测试项目\Threadpooltest\Thread.h
    Path:         d:\测试项目\Threadpooltest
    ClassName:    Thread
    FileType:     h
    Author:       nicklisir@163.com
    NeededLib:    
    requires:      
*********************************************************************/
#include <process.h>
#include <windows.h>
#include "threadpool/ILeeThreadObject.h"
#include "ccriticalSection.h"
class CThread:public IThreadBase
{
public:
	CThread(void);
public:
	virtual ~CThread(void);
	static unsigned int WINAPI ThreadProc(void* pParams);//创建线程使用的回调
	virtual void Execute(void);//实质执行
	virtual bool Start(void) ;
	virtual bool Terminate();
	virtual bool     Initialise(void);
};
#endif