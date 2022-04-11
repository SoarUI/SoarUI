#include "threadpool/leeThreadPool.h"
#include "threadpool/Thread.h"
CThreadPoolEx::CThreadPoolEx(void)
{
	d_initSize =20 ;
	d_MaxSize =40;
	d_AvailLow=3;
	d_Availhigh=5;
	d_currentAvailThreads=0;
	d_currentSize=0;
	CreateIdleThread(20);
}

CThreadPoolEx::~CThreadPoolEx(void)
{
	//��տ����б�
	d_idleLock.Enter();
	d_IdleLists.clear();
	d_idleLock.Leave();
	//��������б�
	_TaskData* pTask =NULL;
	d_normalTaskLock.Enter();
    while ( d_tasklists.size()>0)
    {
		pTask =PopTask();
		if (pTask&&pTask->executor)
		{
			delete pTask->executor;
		} 
		else if(pTask && pTask->pData)
		{
			;//no interface
		}
		delete pTask;
    }
	d_normalTaskLock.Leave();
	//���æ�б�
	TerminateAll();
}
CThreadPoolEx::CThreadPoolEx(unsigned int Init)
{
	d_initSize = Init;
	d_MaxSize =40;
	d_AvailLow=5;
	d_Availhigh=8;
	d_currentAvailThreads=0;
	d_currentSize=0;
	CreateIdleThread(d_initSize);
}
void CThreadPoolEx::TerminateAll(void)
{
	//��ն�
	d_threadLock.Enter();
	ThreadBusyList::iterator itbeg =d_ThreadLists.begin();
	while(itbeg !=d_ThreadLists.end())
	{
		destroyThread(itbeg->second);
		d_ThreadLists.erase(itbeg);
		itbeg =d_ThreadLists.begin();
	}
	d_ThreadLists.clear();
	d_threadLock.Leave();
}
void CThreadPoolEx::CreateIdleThread(int num) 

{ 
	for(int i=0;i<num;i++)
	{ 
		//printf("create idle thread :%d \n",num); 
		IThreadBase* thr = new CThread(); 
		thr->SetThreadPool(this); 
		AppendToIdleList(thr); 
		InterlockedIncrement(&d_currentAvailThreads); 
		thr->Start();
		d_threadLock.Enter();
		d_ThreadLists.insert(std::make_pair(thr->GetThreadID(),thr));
		InterlockedIncrement(&d_currentSize);
		d_threadLock.Leave();
	} 
}
//�ɵ��÷�ȷ�������Ψһ�ԣ�ֻ��һ�������������������������ҵ��ȷ����
void CThreadPoolEx::DeleteIdleThread(int num)
{
	
	ThreadBusyList::iterator itIdle;
	ThreadBusyList::iterator itAl;
	
	for(int i=0;i<num;i++)
	{ 
		
		//��ȡ���е����һ��
		IThreadBase* thr = GetLastIdleThread();
		if (thr && d_IdleLists.size()>d_AvailLow)
		{
			
			//������¼���û�м���������
			itIdle =/*d_IdleLists*/d_ThreadLists.find(thr->GetThreadID());
			d_threadLock.Enter();
			 itAl =d_ThreadLists.find(thr->GetThreadID());
			if (itAl != d_ThreadLists.end())
			{
				itAl->second->Exit();
				itAl->second->Wakeup();
				//WaitForSingleObject(hThread,INFINITE);
				delete itAl->second;
				d_ThreadLists.erase(itAl);
				InterlockedDecrement(&d_currentSize);
			}
			d_threadLock.Leave();
			destroyThread(thr);
		}
	} 
	
}

void CThreadPoolEx::destroyThread(IThreadBase* idlethread)
{
	if (idlethread != NULL)
	{
		HANDLE hThread =idlethread->GetHandle();
		idlethread->Exit();
		idlethread->Wakeup();
		//WaitForSingleObject(hThread,INFINITE);
		delete idlethread;
		InterlockedDecrement(&d_currentSize);
	}
		
}
void CThreadPoolEx::AppendToBusyList(IThreadBase* idlethread)
{
	d_busyLock.Enter();
	d_BusyLists.insert(std::make_pair(idlethread->GetThreadID(),idlethread));
	d_busyLock.Leave();
}
void CThreadPoolEx::RemoveFromBusyList(IThreadBase* idlethread)
{
	d_busyLock.Enter();
	ThreadBusyList::iterator it =d_BusyLists.find(idlethread->GetThreadID());
	if (it != d_BusyLists.end())
	{
		d_BusyLists.erase(it);
	}
	d_busyLock.Leave();
}
void CThreadPoolEx::AppendToIdleList(IThreadBase* idlethread)
{
	d_idleLock.Enter();
	d_IdleLists.push_back(idlethread);
	InterlockedIncrement(&d_currentAvailThreads);
	d_idleLock.Leave();
}

IThreadBase* CThreadPoolEx::GetLastIdleThread(void) 

{ 
	d_idleLock.Enter();
	if(d_IdleLists.size() > 0 ) 
	{ 
		IThreadBase* thr = (IThreadBase*)d_IdleLists.back(); 
		d_IdleLists.pop_back();
		InterlockedDecrement(&d_currentAvailThreads);
		d_idleLock.Leave();
		return thr; 
	}
	d_idleLock.Leave();
	return NULL; 
} 
IThreadBase* CThreadPoolEx::GetIdleThread(void) 

{ 
	d_idleLock.Enter();
	if(d_IdleLists.size() > 0 ) 
	{ 
		IThreadBase* thr = (IThreadBase*)d_IdleLists.front(); 
		d_IdleLists.pop_front();
		InterlockedDecrement(&d_currentAvailThreads);
		d_idleLock.Leave();
		return thr; 
	}
	d_idleLock.Leave();
	return NULL; 
}
bool CThreadPoolEx::IsIdleThreadAvaible(void)
{
	bool isavaible=false;
	d_idleLock.Enter();
	isavaible =d_IdleLists.size()>0;
	d_idleLock.Leave();
	return isavaible;
}
bool CThreadPoolEx::IsTaskAvaible(void)
{
	bool isAval =false;
	d_normalTaskLock.Enter();
	//���ṩ���ȼ��������ṩ��ͨ��������
	if (d_tasklists.size()>0)
	{
		isAval =true;
	}
	d_normalTaskLock.Leave();
	return isAval;
}
_TaskData* CThreadPoolEx::PopTask(void)
{
	_TaskData* pTask=NULL;
	d_normalTaskLock.Enter();
	//���ṩ���ȼ��������ṩ��ͨ��������
	 if (d_tasklists.size()>0)
	{
		pTask =d_tasklists.front();
		d_tasklists.pop_front();
	}
	d_normalTaskLock.Leave();
	return pTask;
}
IThreadBase* CThreadPoolEx::DispatchTask(void)
{
	_TaskData* data =PopTask();
	if (!data)
	{
		return NULL;
	}
	IThreadBase*  idlethr = GetIdleThread(); 
	if(idlethr !=NULL) 
	{ 
		HANDLE hWakeup =idlethr->GetWakeup();
		AppendToBusyList(idlethr); 
		idlethr->SetThreadPool(this);  
		idlethr->SetTask(data); 
		ResumeThread(idlethr->GetHandle());
		SetEvent(hWakeup);
	} 
	//��������Ѿ�������֪ͨ�¼���������.�̳߳�ֻ֧���޲����ص�
	data->callee();
	//ɾ��Data�Ķ��ڴ�
	delete data;
	return idlethr ;
}
void CThreadPoolEx::PushTask(_TaskData * data,TaskLevel lv)
{
	d_normalTaskLock.Enter();
	switch(lv)
	{
	case TaskLevel::Normal:
		{
			d_tasklists.push_front(data);
		}
		break;
	case TaskLevel::Low:
		{
			d_tasklists.push_back(data);	
		}
		break;
	}
	d_normalTaskLock.Leave();
}
//���������Σ������سߴ磬�ȷ����ٵ���
void CThreadPoolEx::NotifyTaskFinish(IThreadBase* thread)
{
	//����������л�ȡ�����ٷ��䣬���Ž�Idlelist�У����迼����������
	if (thread && IsTaskAvaible()>0)
	{
		//��������
		_TaskData* Data=NULL ;
		Data=PopTask();
		if (Data)
		{   
			HANDLE hWakeup =thread->GetWakeup();
		thread->SetThreadPool(this);  
		thread->SetTask(Data);
		//ɾ��Data�Ķ��ڴ�
		delete Data;
		//�ָ̻߳�״̬
		ResumeThread(thread->GetHandle());
		//
		SetEvent(hWakeup);
	//	printf("assign thread task\n");
		}
		//printf("assign thread task >>>>>>>>>>>>>>=====\n");
	}
	else// �Żس���,���Ի��չ������̣߳�ʹ������ƽ����
	{
		//�ӷ�æ�б���ɾ��
		RemoveFromBusyList(thread);
		AppendToIdleList(thread);
		//printf("put thread in idle<<<<<<<<<<<<<<<<<\n");
		//�����ش�С
		d_idleLock.Enter();
		//���߳���û�г���Ҳ��Ϊû�г���
		if(d_currentSize>d_initSize &&d_IdleLists.size()>d_Availhigh) 
		{ 
			if (d_IdleLists.size()>d_Availhigh)//������������������
			{
				DeleteIdleThread(d_IdleLists.size()-d_Availhigh);
			} 
		} 
		d_idleLock.Leave();
	}

}
//�첽�ύ����Ƚ�
void CThreadPoolEx::SubmitTaskAsync(ITaskExecutorBase* pTaskWorker,CLeeCallbackcompiler<void,int> &callee, TaskLevel lv)
{
	//assert(pTaskWorker!=NULL); 
	//����Ƿ��п����̣߳��еĻ�ֱ�ӷ�������
	_TaskData *Data =new _TaskData;
	Data->executor=pTaskWorker;
	Data->lpWorkerRotine =NULL;
	Data->callee = callee;
	//������б�
	PushTask(Data,lv);
	//����Ƿ�Ҫ�����߳� 
	d_idleLock.Enter();
	if(d_IdleLists.size()<d_AvailLow && d_currentSize<d_MaxSize) 
	{ 
		if(d_currentSize+d_AvailLow < d_MaxSize ) //������������Ϳ�������ζ��Ҫ���ӱ���
			CreateIdleThread(d_AvailLow-d_IdleLists.size()); 
	} 
	////������п����̣߳���������
	if (d_IdleLists.size()>0)
	{
		DispatchTask();
	}
	d_idleLock.Leave();
}
//ͬ���ύ
bool CThreadPoolEx::SubmitTask(ITaskExecutorBase* pTaskWorker)
{
	_TaskData *data =new _TaskData;
	data->executor=pTaskWorker;
	data->lpWorkerRotine =NULL;
	IThreadBase*  idlethr = GetIdleThread(); 
	if(idlethr ==NULL) 
	{ 
		CreateIdleThread(1);
		idlethr = GetIdleThread();
	}
	if (idlethr)
	{
		HANDLE hWakeup =idlethr->GetWakeup();
		AppendToBusyList(idlethr); 
		idlethr->SetThreadPool(this);  
		idlethr->SetTask(data); 
		HANDLE hComplete =CreateEvent(NULL,false,false,NULL);
		idlethr->SetOneShotComplete(hComplete);
		ResumeThread(idlethr->GetHandle());
		SetEvent(hWakeup);
		WaitForSingleObject(hComplete,INFINITE);
		CloseHandle(hComplete);
		//ɾ��Data�Ķ��ڴ�
		delete data;
		return true;
	}
	//
	//��������Ѿ�������֪ͨ�¼���������.�̳߳�ֻ֧���޲����ص�
	//ɾ��Data�Ķ��ڴ�
	delete data;
	return false;
}
bool CThreadPoolEx::SubmitTask(ThreadWorkerFtr pFunc, void* pData)
{
	_TaskData *data =new _TaskData;
	data->pData=pData;
	data->lpWorkerRotine =pFunc;
	IThreadBase*  idlethr = GetIdleThread(); 
	if(idlethr ==NULL) 
	{ 
		CreateIdleThread(1);
		idlethr = GetIdleThread();
	}
	if (idlethr)
	{
		HANDLE hWakeup =idlethr->GetWakeup();
		AppendToBusyList(idlethr); 
		idlethr->SetThreadPool(this);  
		idlethr->SetTask(data); 
		HANDLE hComplete =CreateEvent(NULL,false,false,NULL);
		idlethr->SetOneShotComplete(hComplete);
		ResumeThread(idlethr->GetHandle());
		SetEvent(hWakeup);
		WaitForSingleObject(hComplete,INFINITE);
		CloseHandle(hComplete);
		//ɾ��Data�Ķ��ڴ�
		delete data;
		return true;
	}
	//
	//ɾ��Data�Ķ��ڴ�
	delete data;
	return false;
}