#ifndef GDF_CcriticalSection_SDK_H
#define GDF_CcriticalSection_SDK_H
#define WIN32_LEAN_AND_MEAN //防止windows.h引入winsock.h，导致重定义
#include<windows.h>
//2010-01-27 ThreadSync的核心
/*
关键段的最大好处在于它的易用性，而且内部也使用了Interlocked函数，因此执行速度非常块；
缺点是无法在多个进程之间对线程进行同步
*/
class CcriticalSection
{
public:
	CcriticalSection(void)
	{
		InitializeCriticalSection(&m_Sync);
	}
public:

	~CcriticalSection(void)
	{
		DeleteCriticalSection(&m_Sync);
	}
	inline void Enter(void) 
	{
		EnterCriticalSection(&m_Sync);

	}
	inline void Leave(void) 
	{
		LeaveCriticalSection(&m_Sync);
	}
private:
	CRITICAL_SECTION m_Sync;
};
#endif