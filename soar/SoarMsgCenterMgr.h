#pragma once
#include <map>
#include "../SoarHeader/ISoarWnd.h"
#include"../SoarHeader/ISoarRoot.h"
#include <winuser.h>
#include "../SoarHeader/CcriticalSection.h"
#include<vector>
/************************************************************************/
/* Soar ��Ϣ���������ϵͳ:ʵ����Ϣ��ȫ�ִ���                        */
/************************************************************************/
class CSoarMsgCenterMgr
{
public:
	CSoarMsgCenterMgr(void);
	~CSoarMsgCenterMgr(void);
	bool Register(UINT uMsg,ISoarWnd* handler,ISoarWnd* pOwner);
	bool UnRegister(ISoarWnd* pOwner,UINT uMsg);
	//��Ե���Ϣ����ʱ����ͬSENDMESSAGE����
	BOOL ExcuteDirect(MSG & msg,ISoarWnd* pOwner, LRESULT& lr);
	//������Ϣ
	BOOL excuteoffline(ISoarRoot * root, LRESULT& lr);
	bool addOfflineMsg(SOARMSG &msg);
	//struct
	typedef std::map<int,MSG> MSMSGRECORD;
	typedef std::vector<SOARMSG> FLYDMSGRECORD;
	typedef std::map<ISoarWnd*,MSMSGRECORD> LEEMSGRECORD;
protected:
	LEEMSGRECORD d_registerRecord;
	FLYDMSGRECORD d_offlineMsgqueue;
	CcriticalSection d_QueueLock;//�������б���
	CcriticalSection d_registermsgLock;//�������б���
};
