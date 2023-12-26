#pragma once
#include "soarwnd.h"
#include<vector>
struct CSoarTitleBar :
	public ISoarBar
{
public:
	CSoarTitleBar(ISoarWnd *parent,ISoarEngine* Eng);
	~CSoarTitleBar(void);
	virtual SoarSegmentItem getItem(const DWORD dId);
	virtual bool AddItem(const SoarSegmentItem&);
	virtual int getItemCount(void);
	virtual bool removeItem(const DWORD dId);
	virtual RECT getBarRect(void) ;
	virtual RECT getBarCliperRect(void);
	virtual void setArea(RECT& rc) ;
	virtual void setArea(ComponentArea& area);
	virtual ComponentArea getArea();
	virtual void DrawSelf(ILeeDrawInterface *DrawFuns) ;
	virtual BOOL HandleEvent( UINT ,WPARAM ,LPARAM, LRESULT& lr) ;
	virtual BOOL BarHitTest(void);
	virtual LWNDT getType(void) ;
	virtual SOARBARALIGN getBarAlign(void);
	virtual void setTitle(const CLeeString& name);
	virtual void setParent(ISoarSegment*parent){d_parent=parent;}
	virtual ISoarSegment* getParent(){return d_parent;}
	virtual void setMsgReceiver(ISoarWnd* pOwnerWnd){ d_ReceiverWnd=pOwnerWnd;}
	virtual ISoarWnd* getMsgReceiver(){return d_ReceiverWnd;}
	virtual void setState(LWNDST state) ;
	virtual LWNDST getState(void) ;
	
protected:
	BOOL HandSheetEvent(UINT uMsg ,WPARAM wParam ,LPARAM lParam, LRESULT& lr);
	BOOL HandSimpleEvent(UINT uMsg ,WPARAM wParam ,LPARAM lParam, LRESULT& lr);
protected:
	bool d_lbuttondown;
	ISoarWnd* d_OwnerWnd;
	CLeeString d_wndlook;//对应皮肤控件
	CLeeString d_wndlookState;//皮肤状态
	ComponentArea d_area;//保存统一坐标信息
	ISoarEngine *d_DrawEng;//作图组件
	SEGMENTITEMS d_items;
	LWNDT d_wndtype;//窗口类型
	SOARBARALIGN d_barAlign;//摆放
	CLeeString d_wndText;//窗口内容
	ISoarSegment* d_parent;
	ISoarWnd * d_ReceiverWnd;
	LWNDST d_wndState;//窗口状态
};
