#include "SoarMsgCenterMgr.h"

CSoarMsgCenterMgr::CSoarMsgCenterMgr(void)
{
}

CSoarMsgCenterMgr::~CSoarMsgCenterMgr(void)
{
}
bool CSoarMsgCenterMgr::Register(UINT uMsg,ISoarWnd* handler,ISoarWnd* pOwner)
{
	//
	d_registermsgLock.Enter();
	LEEMSGRECORD::iterator it =d_registerRecord.find(pOwner);
		if (it != d_registerRecord.end())
		{
			//������Ϣ
			MSMSGRECORD::iterator ia =(it->second).find(uMsg);
			if (ia != (it->second).end())
			{
				if ((ia->second).hwnd !=(HWND)handler)
				{
					(ia->second).hwnd =(HWND)handler;
				}
				d_registermsgLock.Leave();
				return true ;
			}
			MSG msg;
			msg.hwnd =(HWND)handler;
			msg.message =uMsg;
			(it->second)[uMsg] =msg;
			d_registermsgLock.Leave();
			return true;
		}
		MSG msg;
		msg.hwnd =(HWND)handler;
		msg.message =uMsg;
		MSMSGRECORD msrd;
		msrd[uMsg]=msg;
		d_registerRecord[pOwner]=msrd;
		d_registermsgLock.Leave();
		return true;
}
bool CSoarMsgCenterMgr::UnRegister(ISoarWnd* pOwner,UINT uMsg)
{
	d_registermsgLock.Enter();
	LEEMSGRECORD::iterator it =d_registerRecord.find(pOwner);
		if (it != d_registerRecord.end())
		{
			//������Ϣ
			MSMSGRECORD::iterator ia =(it->second).find(uMsg);
			if (ia != (it->second).end())
			{
				(it->second).erase(ia);
				d_registermsgLock.Leave();
				return true ;
			}
			d_registermsgLock.Leave();
			return true;
		}
		d_registermsgLock.Leave();
		return true;
}
LRESULT CSoarMsgCenterMgr::ExcuteDirect(MSG & msg,ISoarWnd* pOwner)
{
	LRESULT lr =0;
	////////�ػ��������Ϣ
	     if( (pOwner->getWndStyle() & LWNDSTY_SYSTITLEBAR) && pOwner->getTitleBar()!=NULL  )
		 {
			 
			 if(pOwner->getTitleBar()->BarHitTest() )
			 { 
				 lr= pOwner->getTitleBar()->HandleEvent(msg.message,msg.wParam,msg.lParam) ;
				 return lr;
			 }
		 }
		 if( (pOwner->getWndStyle() & LWNDSTY_SYSMENUBAR) && pOwner->getMenuBar()!=NULL  )
		 {
			 
			 if(pOwner->getMenuBar()->BarHitTest() )
			 { 
				 lr= pOwner->getMenuBar()->HandleEvent(msg.message,msg.wParam,msg.lParam) ;
				 return lr;
			 }
		 }
		  if( (pOwner->getWndStyle() & LWNDSTY_HSCROLL) && pOwner->getScrollSegment(false)!=NULL  )
		 {
			 
			 if(pOwner->getScrollSegment(false)->BarHitTest() )
			 { 
				 lr= pOwner->getScrollSegment(false)->HandleEvent(msg.message,msg.wParam,msg.lParam) ;
				 return lr;
			 }
		 }
		   if( (pOwner->getWndStyle() & LWNDSTY_VSCROLL) && pOwner->getScrollSegment(true)!=NULL  )
		 {
			 
			 if(pOwner->getScrollSegment(true)->BarHitTest() )
			 { 
				 lr= pOwner->getScrollSegment(true)->HandleEvent(msg.message,msg.wParam,msg.lParam) ;
				 return lr;
			 }
		 }
		   ///�ǿͻ���������
		 if(pOwner->HandleNonClientBarsEvent(msg.message,msg.wParam,msg.lParam,lr) )
			 return lr;
		 //��ͨ����
		 lr= pOwner->HandleEvent(pOwner,msg.message,msg.wParam,msg.lParam) ;
		 return lr;
}
LRESULT CSoarMsgCenterMgr::excuteoffline(ISoarRoot * root)
{
	//������Ϣ
	d_QueueLock.Enter();
	if (d_offlineMsgqueue.empty())
	{
		d_QueueLock.Leave();
		return 0;
	}
	FLYDMSGRECORD::iterator ia =d_offlineMsgqueue.begin();
	FLYDMSGRECORD::iterator ib =d_offlineMsgqueue.end();
	if(!d_offlineMsgqueue.empty() )
	{
		ia =d_offlineMsgqueue.begin();
		SOARMSG msg =*ia;
		ISoarObject* thisWnd=msg.targetWnd;
		//Segment ��Ϣ���������
	    d_offlineMsgqueue.erase(ia);
		//��֤�����Ƿ���Ч
		if(thisWnd&&
			root->isValidateWnd((ISoarWnd*)thisWnd) &&
			thisWnd->getType()<LWNDT_SEGMENT 
			 )
		{
			((ISoarWnd*)thisWnd)->HandleEvent((ISoarWnd*)thisWnd,msg.message,(WPARAM)&msg,msg.lParam) ;
		}
		else if(thisWnd &&
			root->isValidateWnd((ISoarWnd*)thisWnd) &&
			thisWnd->getType()>LWNDT_SEGMENT)
		{//segmentӵ����Ϣ����������
			ISoarSegment* thisSegment=(ISoarSegment*)thisWnd;
			thisSegment->HandleEvent(msg.message,(WPARAM)&msg,msg.lParam) ;
		}
	}
	d_QueueLock.Leave();
	return false;
}
bool CSoarMsgCenterMgr::addOfflineMsg(SOARMSG &msg)
{
	ISoarObject* thisWnd=msg.routeWnd;
	d_QueueLock.Enter();
	d_offlineMsgqueue.push_back(msg);
	d_QueueLock.Leave();
	return true;
}