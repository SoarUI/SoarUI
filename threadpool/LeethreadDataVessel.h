#ifndef _ThreadPoolStructs_h_
#define _ThreadPoolStructs_h_
#include "leecallback.h"
class ITaskExecutorBase;
typedef unsigned int (*ThreadWorkerFtr)(void* params);
typedef struct TtgTaskData
{
	ThreadWorkerFtr lpWorkerRotine;
	
	union 
	{
		ITaskExecutorBase* executor;
		void* pData;
	};	
	//回调支持
	CLeeCallbackcompiler<void,int> callee;//第二个参数不能为void
	bool bIsVoidFunction;
} _TaskData;
typedef enum EtgTaskLevel
{
	highthigth =0,
	High,
	Normal,
	Low
}TaskLevel;
#endif