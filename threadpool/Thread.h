#ifndef _Thread_h_
#define _Thread_h_
/********************************************************************
    Descibe:      ���̺߳����ĳ����װĿǰֻʵ��Windowsƽ̨
    created:      2012/8/22 22:02
    FullName:     d:\������Ŀ\Threadpooltest\Thread.h
    Path:         d:\������Ŀ\Threadpooltest
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
	static unsigned int WINAPI ThreadProc(void* pParams);//�����߳�ʹ�õĻص�
	virtual void Execute(void);//ʵ��ִ��
	virtual bool Start(void) ;
	virtual bool Terminate();
	virtual bool     Initialise(void);
};
#endif