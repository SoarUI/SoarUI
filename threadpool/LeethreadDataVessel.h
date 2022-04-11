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
	//�ص�֧��
	CLeeCallbackcompiler<void,int> callee;//�ڶ�����������Ϊvoid
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