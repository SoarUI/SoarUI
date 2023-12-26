#pragma once
#include <map>
#include "../SoarHeader/ISoarWnd.h"
#include"../SoarHeader/ISoarRoot.h"
#include <winuser.h>
#include "../SoarHeader/CcriticalSection.h"
#include<vector>
/************************************************************************/
/* Soar 消息调度与控制系统:实现消息的全局代理                        */
/************************************************************************/
class CSoarMsgCenterMgr
{
public:
	CSoarMsgCenterMgr(void);
	~CSoarMsgCenterMgr(void);
	bool Register(UINT uMsg,ISoarWnd* handler,ISoarWnd* pOwner);
	bool UnRegister(ISoarWnd* pOwner,UINT uMsg);
	//点对点信息，即时运行同SENDMESSAGE函数
	BOOL ExcuteDirect(MSG & msg,ISoarWnd* pOwner, LRESULT& lr);
	//离线信息
	BOOL excuteoffline(ISoarRoot * root, LRESULT& lr);
	bool addOfflineMsg(SOARMSG &msg);
	//struct
	typedef std::map<int,MSG> MSMSGRECORD;
	typedef std::vector<SOARMSG> FLYDMSGRECORD;
	typedef std::map<ISoarWnd*,MSMSGRECORD> LEEMSGRECORD;
protected:
	LEEMSGRECORD d_registerRecord;
	FLYDMSGRECORD d_offlineMsgqueue;
	CcriticalSection d_QueueLock;//对任务列表保护
	CcriticalSection d_registermsgLock;//对任务列表保护
};
