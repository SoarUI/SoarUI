//#include "StdAfx.h"
#include "threadpool/Thread.h"
#include "threadpool/TaskExecutorBase.h"
#include "threadpool/leeThreadPool.h"
CThread::CThread(void)
{
}

CThread::~CThread(void)
{
}
unsigned int WINAPI CThread::ThreadProc(void* pParams)
{
	CThread * thrd =(CThread*)pParams;
	if (!thrd)
	{
		return -1;
	}
	thrd->Execute();
	return 0;
}
bool CThread::Start(void) 
{
	d_hThread = (HANDLE)_beginthreadex(NULL, 0, CThread::ThreadProc, this,  
		CREATE_SUSPENDED, (UINT*)&d_threadID);
	d_hdestroy =CreateEvent(NULL,false,false,NULL);
	d_hWakeup =CreateEvent(NULL,false,false,NULL);
	return d_hThread !=NULL;
}
bool CThread::Terminate()
{
	bool bret=TerminateThread(d_hThread,-2);
	d_hThread =NULL;
	return bret;
}
bool CThread::Initialise(void)
{
	d_sync.Enter();
	d_threadData.lpWorkerRotine =NULL;
	d_threadData.pData =NULL;
	d_sync.Leave();
	d_hCompeteOneShot =NULL;
	return true;
}
void CThread::Execute(void)
{
	DWORD					dwWait;
	HANDLE					hWaits[2];
	hWaits[0] =d_hdestroy;
	hWaits[1] =d_hWakeup;
	bool					bAutoDelete;
	while(true)
	{
		if (d_shutdown)
		{
			//�Ѿ����
			SetEvent(d_hCompeteOneShot);
			return ;
		}
		dwWait = WaitForMultipleObjects(2, hWaits, FALSE, INFINITE);
		if (dwWait ==WAIT_OBJECT_0)
		{
			//�Ѿ����
			SetEvent(d_hCompeteOneShot);
			return;
		}
		if (dwWait ==WAIT_OBJECT_0 +1)
		{
			//�����ʹ�ö����ǻص�
			d_sync.Enter();
			_TaskData Data =d_threadData;
			d_sync.Leave();
				if (Data.executor)
				{
					bAutoDelete = Data.executor->isAutoDelete();
					//������ʼ�¼�
					Data.executor->start();
					//��ʵ��
					Data.executor->Execute(0);
					//������������¼�
					Data.executor->complete();
					//���ٶ���
					if(bAutoDelete)
					{
						Data.executor->DeleteInstance();
					}
				} 
				else if(Data.lpWorkerRotine!= NULL)
				{
					Data.lpWorkerRotine(Data.pData);
					//���ٶ���pData,Ŀǰδʵ��
				}
				//�Ѿ����
				SetEvent(d_hCompeteOneShot);
			Initialise();
			//֪ͨ�����ߣ�������ɣ���һ������
			d_pool->NotifyTaskFinish(this); // tell the pool, i am now free
			
		}
		
	}
	return ;
}