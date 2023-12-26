#pragma once
#include "../SoarHeader/ISoarItemWnd.h"
#include "SoarNonClientSegmentBase.h"

class CLeeTabCtrlHeader :
	 public ISoarBar,public CSoarNonClientSegmentBase
{
public:
	CLeeTabCtrlHeader(ISoarWnd *parent,ISoarEngine* Eng,SOARBARALIGN soarAlign = SOARALIGN_TOP );
	~CLeeTabCtrlHeader(void);
	///
	virtual RECT getBarRect(void) ;
	virtual RECT getBarCliperRect(void);
	virtual void setArea(RECT& rc) ;
	virtual void setArea(ComponentArea& area);
	virtual ComponentArea getArea();
	virtual BOOL HandleEvent( UINT ,WPARAM ,LPARAM, LRESULT& lr) ;
	virtual BOOL BarHitTest(void);
	virtual LWNDT getType(void) ;
	virtual SOARBARALIGN getBarAlign(void);
	virtual void setTitle(const CLeeString& name);
	virtual SoarSegmentItem getItem(const DWORD dId);
	virtual bool AddItem(const SoarSegmentItem&);
	virtual bool removeItem(const DWORD dId);
	virtual int getItemCount(void);
	virtual void setParent(ISoarSegment*parent){d_parent=parent;}
	virtual ISoarSegment* getParent(){return d_parent;}
	virtual void setMsgReceiver(ISoarWnd* pOwnerWnd){ d_ReceiverWnd=pOwnerWnd;}
	virtual ISoarWnd* getMsgReceiver(){return d_ReceiverWnd;}
	virtual void setState(LWNDST state) ;
	virtual LWNDST getState(void) ;
	//////////////////////////////////////////////////////////////////////////
	virtual void DrawSelf(ILeeDrawInterface *DrawFuns);
	virtual bool addTab(DWORD islot,CLeeString szTab,LPVOID data);
	virtual bool linkTab(DWORD islot,ISoarWnd* dataWnd);
	virtual int setTabHeight(int h);
	virtual int setTabWidth(int w);
	virtual int setButtonsInoneLine(int n);
	virtual int getCurSel(void);
	virtual void setCurSel(int islot);
protected:
	LeeItemWndMap d_TabButtons;
	DWORD d_tabIndex;
	int d_tabHeight;
	int d_tabwidth;
	int d_cntInLine;
	int d_lineIndex;
	int d_curSel;
	SOARBARALIGN d_barAlign;//摆放
	CLeeString d_wndText;//窗口内容
	ISoarSegment* d_parent;
	ISoarWnd * d_ReceiverWnd;
	LWNDST d_wndState;//窗口状态
};
