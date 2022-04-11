#ifndef _ITaskExecutorBase_h_
#define _ITaskExecutorBase_h_
/********************************************************************
    Descibe:      任务的实质执行者,自包含各种所需的参数
    created:      2012/8/22 22:35
    FullName:     ITaskExecutorBase.h
    Path:        
    ClassName:    ITaskExecutorBase
    FileType:     h
    Author:       nicklisir@163.com
    NeededLib:    
    requires:      
*********************************************************************/
class ITaskExecutorBase
{
public:
	ITaskExecutorBase(void){;}
public:
	virtual ~ITaskExecutorBase(void){}
	virtual void Execute(void* reserved)=0;
	virtual void DeleteInstance() = 0;
	virtual bool isAutoDelete() = 0;
	virtual void start(void)=0;
	virtual void complete(void)=0;
};
#endif