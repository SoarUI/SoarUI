#ifndef _CScopeSync_H_
#define _CScopeSync_H_
//�߳�ͬ��ģ��
/*
Update:  2012/10/28 20:32:41
ջͬ��������ջ���ԣ��Զ��뿪�ٽ��� ԭ����CStackSync
*/
#include "CcriticalSection.h"
   class CScopeSync
	{
	 public:
		 CScopeSync( CcriticalSection &cSync) : VarSync(cSync)
		{
			VarSync.Enter();
		}
		~CScopeSync(void)
		{
			VarSync.Leave();
		}
	private:
	 CcriticalSection & VarSync;
	};
#endif