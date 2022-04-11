#pragma once
#include "isoarwnd.h"
#include "Dimensions.h"
class CmyBut :
	public ISoarWnd
{
public:
	CmyBut(void);
public:
	virtual ~CmyBut(void);
	//获取主窗口的大小：即客户区域大小
	virtual LWNDT getType(void) ;
	virtual RECT getClientRect(void) ;
	virtual RECT getWindowRect(void) ;
	virtual void setWindowPos(int x,int y);
	virtual void setArea(RECT& rc);
	virtual void setArea(ComponentArea& area);
	virtual ILeeWnd* GetParent(void);
	virtual void setParent(ILeeWnd * lParent) ;
	virtual INT setZindex(INT dwZindex) ;
	virtual INT getZindex(void) ;
	virtual CLeeString getText(void);
	virtual void setText(CLeeString name);
	virtual void setWndName(CLeeString name) ;
	virtual CLeeString getWndName(void) ;
	virtual void setState(LWNDST state) ;
	virtual LWNDST getState(void) ;
	virtual CLeeString getDreamLook(void) ;
	virtual void setDreamLook(CLeeString name) ;
	virtual void checklookState(POINT &pt);//检测状态，并自动更新
	virtual void attachRoot(HWND root);
	virtual void setWindowHeight(int h) ;
	virtual void setWindowWidth(int w) ;
	virtual void setWindowHeight(Dimension h) ;
	virtual void setWindowWidth(Dimension w) ;
	//////////////////////////////////////////////////////////////////////////
	virtual void DrawSelf(ILeeDrawInterface *DrawFuns) ;
	//消息处理
	virtual LRESULT HandleEvent ( UINT ,WPARAM ,LPARAM ) ;
	virtual ILeeEventHandler* RegisterEventHandler( ILeeEventHandler * pEventHandler){ return 0; }
	//用于获取子窗口:如果只有主窗口则返回
	virtual ILeeWnd* getChildWnd(const CLeeString & name);
	virtual bool addChildWnd(ILeeWnd* pWnd);
protected:
	HWND d_rootWnd;
	LeeWndPtrMap d_WndPtrs;//所有的子窗口
	INT d_zIndex; //Z -order
	RECT rcArea;
	ComponentArea d_area;
	ILeeWnd* d_parent;
	CLeeString d_wndText;
	CLeeString d_wndName;//窗口名字
	CLeeString d_wndlook;//对应皮肤控件
	CLeeString d_wndlookState;//皮肤状态
	LWNDT d_wndtype;
	LWNDST d_wndState;//窗口状态

};
