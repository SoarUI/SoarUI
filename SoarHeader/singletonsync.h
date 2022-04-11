#ifndef SingletonSync_SDK_H
#define SingletonSync_SDK_H
//线程同步模块
/*
当同一类的不同对象时，将会柱塞，不能异步；当不同类对象是可以异步，这就是这个类的缺点；
使用方法：从它派生，在需要同步的地方，使用CThreadsync 定义一对象，则由栈负责生命周期
该类使用于单件同步，A1,A2[，同时对C操作，A1完成后A2才能开始或相反]
*/
#include "CcriticalSection.h"
template <class T>
class CSingletonSync
{
	friend class CThreadSync;
public:
class CThreadSync
{
public:
	CThreadSync(void)
	{
		T::m_Sync.Enter();
	}
	~CThreadSync(void)
	{
		T::m_Sync.Leave();
	}
};
protected:
	static CcriticalSection m_Sync;	
};
template<class T>
CcriticalSection CSingletonSync<T>::m_Sync;
#endif