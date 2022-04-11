#ifndef SingletonSync_SDK_H
#define SingletonSync_SDK_H
//�߳�ͬ��ģ��
/*
��ͬһ��Ĳ�ͬ����ʱ�����������������첽������ͬ������ǿ����첽�������������ȱ�㣻
ʹ�÷�������������������Ҫͬ���ĵط���ʹ��CThreadsync ����һ��������ջ������������
����ʹ���ڵ���ͬ����A1,A2[��ͬʱ��C������A1��ɺ�A2���ܿ�ʼ���෴]
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