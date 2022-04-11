#ifndef GDF_CcriticalSection_SDK_H
#define GDF_CcriticalSection_SDK_H
#define WIN32_LEAN_AND_MEAN //��ֹwindows.h����winsock.h�������ض���
#include<windows.h>
//2010-01-27 ThreadSync�ĺ���
/*
�ؼ��ε����ô��������������ԣ������ڲ�Ҳʹ����Interlocked���������ִ���ٶȷǳ��飻
ȱ�����޷��ڶ������֮����߳̽���ͬ��
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