#pragma once
#include "Soarwnd.h"
#include "../SoarHeader/ISoarItemWnd.h"
#include "../SoarHeader/SoarExport.h"
#include"SoarNonClientSegmentBase.h"
class CLeeHeaderWnd :
	public CSoarNonClientSegmentBase, public ISoarBar
{
public:
	
	CLeeHeaderWnd(ISoarWnd *parent,ISoarEngine* Eng,SOARBARALIGN soarAlign);
	~CLeeHeaderWnd(void);
	//
	virtual RECT getBarRect(void) ;
	virtual RECT getBarCliperRect(void) ;
	virtual void setArea(RECT& rc) ;
	virtual void setArea(ComponentArea& area);
	virtual ComponentArea getArea();
	virtual LRESULT HandleEvent( UINT ,WPARAM ,LPARAM ) ;
	virtual BOOL BarHitTest(void);
	virtual LWNDT getType(void) ;
	virtual SOARBARALIGN getBarAlign(void);
	virtual void setTitle(CLeeString name);
	virtual SoarSegmentItem getItem(const DWORD dId);
	virtual bool AddItem(const SoarSegmentItem&);
	virtual bool removeItem(const DWORD dId);
	//头的项数目
	virtual int getItemCount(void);
	virtual int addItem(int nItemWidth,CLeeString str);
	virtual void setItemString(CLeeString str,int islot);
	virtual CLeeString getItemString(int iSlot);
	virtual int getHeight(void);
	virtual int setHeight(int nHeight);
	virtual int getItemWidth(int iSlot);
	virtual SoarSegmentItem getItem(int icol);
	virtual void DrawSelf(ILeeDrawInterface *DrawFuns);
	virtual void setState(LWNDST state) ;
	virtual  void updateStartCol(int iCol);
	virtual void setParent(ISoarSegment*parent){d_parent=parent;}
	virtual ISoarSegment* getParent(){return d_parent;}
	virtual void setMsgReceiver(ISoarWnd* pOwnerWnd){ d_ReceiverWnd=pOwnerWnd;}
	virtual ISoarWnd* getMsgReceiver(){return d_ReceiverWnd;}
	virtual LWNDST getState(void) ;
	
private:
	void setItemPosition(SoarSegmentItem &lpWnd,Dimension& X,Dimension  &Y,Dimension& W,Dimension& H);
	void EnsureColVisible(int iStartCol,int iEndCol);
protected:
	int d_cntItem;
	LeeItemWndMap d_commItems;
	int d_height;
    int d_width;
	int d_curStartColIndex;
	SOARBARALIGN d_barAlign;//摆放
	CLeeString d_wndText;//窗口内容
	ISoarSegment* d_parent;
	ISoarWnd * d_ReceiverWnd;
	LWNDST d_wndState;//窗口状态
};
