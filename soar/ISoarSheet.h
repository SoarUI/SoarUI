#pragma once
#include"../SoarHeader/ISoarWnd.h"
class ISoarSheet
{
public:
	ISoarSheet(void){}
	virtual ~ISoarSheet(void){}
	virtual ISoarWnd* getPrevFocusWnd(void)=0;
	virtual ISoarWnd* setPrevFocusWnd(ISoarWnd* pFocus)=0;
	virtual ISoarWnd* getFocusWnd(void)=0;
	virtual ISoarWnd* setFocusWnd(ISoarWnd* pFocus)=0;
	virtual ISoarWnd* setMainWnd(ISoarWnd* maidW)=0;
	virtual ISoarWnd* getMainWnd(void) =0;
	virtual void setModalState(bool bModal)=0;
	virtual void addWndRecursive(ISoarWnd* newWnd) =0;
	virtual BOOL seekWnd(ISoarWnd* newWnd) =0;
	virtual bool seekWnd(const CLeeString & name)=0;
	virtual ISoarWnd* getWnd(const CLeeString & name)=0;
	virtual void removeWndRecursive(ISoarWnd* newWnd) =0;
	virtual void eraseWndRecursive(ISoarWnd* pWnd) =0;
	virtual void clearAllLinkWnd() =0;
	//窗口层次改变
	virtual BOOL WndZorderChanged(ISoarWnd* FocusWnd,int newFocusZpos,int oldZindex) =0;
	//窗口样式改变
	virtual BOOL WndStyleChanged(ISoarWnd* newWnd) =0;
	virtual BOOL SoarUIMsgProc ( UINT ,WPARAM ,LPARAM,LRESULT& ) =0 ;
	virtual bool filter_ClickMsg(UINT uMessage,WPARAM wParam,LPARAM lParam,LRESULT&) =0 ;
	virtual bool RenderLoop(ILeeDrawInterface *DrawFuns,float &t) =0;
	//Z管理
	virtual long generateZindex()=0;
	virtual long generatePopupZindex()=0;
	virtual void setStartZindex(long iStart)=0;
	virtual void setStartPopupZindex(long iStart)=0;
	virtual long getMaxZindex()=0;
	virtual long getMaxPopupZindex()=0;
	virtual long bringWndToTopLevel(ISoarWnd* pFocusWnd)=0;
	//Graphic operation
	virtual bool enableAlphaBlend(bool bEnable)=0;
};

