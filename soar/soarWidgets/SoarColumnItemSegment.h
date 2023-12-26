#pragma once
#include "../../SoarHeader/ISoarWnd.h"
#include "../SoarWnd.h"
class CSoarColumnItemSegment :
	public ISoarColumnItemBase
{
public:
	CSoarColumnItemSegment(HWND root,ISoarEngine* Eng);
	~CSoarColumnItemSegment(void);
	
	virtual DWORD getIndex(void) ;
	virtual DWORD getsubIndex(void) ;
	virtual DWORD getID(void) ;
	virtual LPCTSTR getTitle(void) ;
	virtual LPVOID getData(void) ;
	virtual int getHeight(void) ;
	virtual int getWidght(void) ;
//
	virtual void setIndex(DWORD dwIndex) ;
	virtual void setsubIndex(DWORD dwIndex) ;
	virtual void setID(DWORD dwId) ;
	virtual void setTitle(const CLeeString& name) ;
	virtual void setData(LPVOID data) ;
	virtual void setHeight(int h) ;
	virtual void setWidght(int w) ;
	virtual bool getCheck(void);
	virtual void setcheck(bool bcheck);
	virtual void DrawSelf(ILeeDrawInterface *DrawFuns);
	virtual void setOwnerWnd(ISoarWnd* pOwnerWnd){ d_OwnerWnd = pOwnerWnd;}
	virtual ISoarWnd* getOwnerWnd(){ return d_OwnerWnd;}
	virtual BOOL HandleEvent( UINT ,WPARAM ,LPARAM, LRESULT& lr) ;

	virtual RECT getBarRect(void);
	virtual RECT getBarCliperRect(void);
	virtual void setArea(RECT& rc);
	virtual void setArea(ComponentArea& area) ;
	virtual ComponentArea getArea();
	virtual BOOL BarHitTest(void);
	virtual LWNDT getType(void) ;
	virtual SOARBARALIGN getBarAlign(void) ;
	//
	virtual SoarSegmentItem getItem(const DWORD dId);
	virtual bool AddItem(const SoarSegmentItem&);
	virtual bool removeItem(const DWORD dId);
	virtual int getItemCount(void);
	virtual void setState(LWNDST state) ;
	virtual LWNDST getState(void) ;
	virtual void setParent(ISoarSegment*parent){d_parent=parent;}
	virtual ISoarSegment* getParent(){return d_parent;}
	virtual void setMsgReceiver(ISoarWnd* pOwnerWnd){ d_ReceiverWnd=pOwnerWnd;}
	virtual ISoarWnd* getMsgReceiver(){return d_ReceiverWnd;}
protected:
	DWORD d_ID ;
	CLeeString d_string;
	LPVOID d_data;
	DWORD d_iIndex;
	DWORD d_subindex;
	int d_width;
	int d_height;
	bool d_checkState;
	ISoarWnd * d_OwnerWnd;
	ISoarWnd * d_ReceiverWnd;
	ISoarSegment* d_parent;
	CLeeString d_wndlook;//对应皮肤控件
	CLeeString d_wndlookState;//皮肤状态
	LWNDT d_wndtype;//窗口类型
	LWNDST d_wndState;//窗口状态
	ComponentArea d_area;//保存统一坐标信息
	SEGMENTITEMS d_items;
	SOARBARALIGN d_barAlign;//摆放
	ISoarEngine *d_DrawEng;//作图组件
};
