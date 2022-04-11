#ifndef _CScopeSync_H_
#define _CScopeSync_H_
//线程同步模块
/*
Update:  2012/10/28 20:32:41
栈同步，利用栈特性，自动离开临界区 原名：CStackSync
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