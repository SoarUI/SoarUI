#ifndef TSOARWND_IMPL_H
#define TSOARWND_IMPL_H
#include "../SoarHeader/ISoarWnd.h"
#include "../SoarHeader/Dimensions.h"
#include "../SoarHeader/ISoarEngine.h"
template<typename T>
class TSoarWndImpl
{
public:
	TSoarWndImpl(HWND root,ISoarEngine* Eng);
	virtual ~TSoarWndImpl(void);
	///
	virtual void InitializeSegments(void);
	virtual HWND getRootWnd(void) ;
	//获取主窗口的大小：即客户区域大小
	virtual LWNDT getType(void) ;
	virtual RECT getClientRect(void) ;//获取相对于父窗口的矩形
	virtual RECT getWindowRect(void) ;
	virtual void setArea(RECT& rc);
	virtual void setArea(ComponentArea& area);
	virtual void getArea(ComponentArea& area);
	virtual void setWindowPos(int x,int y);
	virtual void setWindowHeight(int h) ;
	virtual void setWindowWidth(int w) ;
	virtual void setWindowHeight(Dimension h) ;
	virtual void setWindowWidth(Dimension w) ;
	virtual ISoarWnd* GetParent(void);
	virtual RECT getCliperRect(void);
	virtual void setParent(ISoarWnd * lParent) ;
	virtual INT setZindex(INT dwZindex) ;
	virtual INT getZindex(void) ;
	virtual INT setPopupZindex(INT dwZindex) ;
	virtual INT getPopupZindex(void) ;
	virtual CLeeString getText(void);
	virtual void setText(CLeeString name);
	virtual void setTitle(CLeeString name);
	virtual void setWndName(CLeeString name) ;
	virtual CLeeString getWndName(void) ;
	virtual void setState(LWNDST state) ;
	virtual LWNDST getState(void) ;
	virtual CLeeString getDreamLook(void) ;
	virtual void setDreamLook(CLeeString name) ;
	virtual void checklookState(POINT &pt);
	virtual void attachRoot(HWND root);
	virtual LWNDSTY getWndStyle(void) ;
	virtual bool  setWndStyle(LWNDSTY lstyle) ;
	virtual bool  isPopupWnd(void) ;
	virtual void  setWndPopup(bool bPopup) ;
	//////////////////////////////////////////////////////////////////////////
	virtual void DrawSelf(ILeeDrawInterface *DrawFuns) ;
	//
	virtual LRESULT HandleEvent( UINT ,WPARAM ,LPARAM ) ;
	virtual LRESULT HandleEvent ( ISoarWnd* pOwner,UINT ,WPARAM ,LPARAM );
	virtual void HandleMenuEvent(int nPos,int nID,SOARMSG * pMsgInfo);
	virtual ISoarEventHandler* RegisterEventHandler( ISoarEventHandler * pEventHandler) ;
	//用于获取子窗口:如果只有主窗口则返回
	virtual ISoarWnd* getChildWnd(const CLeeString & name);
	virtual bool addChildWnd(ISoarWnd* pWnd);
	virtual bool eraseChildWnd(ISoarWnd* pWnd);
	//////////////////////////////////////////////////////////////////////////
	virtual ISoarBar* getMenuBar(void) ;
	virtual ISoarBar* getTitleBar(void) ;
	virtual ISoarScroll* getScrollSegment(bool vert) ;
	virtual int setScrollInfo(int width,int height);
	virtual int getScrollInfo(bool bHeight);
	virtual int setTitleBarSegment(int nHeight);
	virtual int getTitleBarSegment(void);
	virtual int setMenuBarSegment(int nHeight);
	virtual int getMenuBarSegment(void);
	//////////////////////////////////////////////////
	//计算函数
	virtual RECT CalcClientRec(RECT & rcWnd) ;
	virtual ISoarBar* AddNonClientBar(const CLeeString& elementName,
		SOARBARALIGN align,LWNDT classType,int nHeight_or_Width);
	virtual void DrawNonClientBars(ILeeDrawInterface *DrawFuns) ;
	virtual BOOL HandleNonClientBarsEvent( UINT umsg ,WPARAM wp ,LPARAM lp ,LRESULT &lr);
	protected:
    HWND d_rootWnd;//原始窗口
	LeeWndPtrMap d_WndPtrs;//所有的子窗口
	ISoarEngine *d_DrawEng;//作图组件
	INT d_zIndex; //Z -order
	INT d_zPopupIndex;//弹出索引
	//统一坐标
	ComponentArea d_area;//保存统一坐标信息
	ISoarWnd* d_parent;//父窗口
	CLeeString d_wndText;//窗口内容
	CLeeString d_wndName;//窗口名字
	CLeeString d_wndlook;//对应皮肤控件
	CLeeString d_wndlookState;//皮肤状态
	LWNDT d_wndtype;//窗口类型
	LWNDST d_wndState;//窗口状态
	LWNDSTY d_wndStyle;//窗口属性
	bool d_isPopupWnd;//标记为弹出窗口
	//事件挂接
	ISoarEventHandler* p_EventHandler;
	ISoarBar* p_titlebar;
	ISoarBar* p_menubar;
	ISoarScroll* p_HScrollSegment;
	ISoarScroll* p_VScrollSegment;
	//重要的不见定义
	int d_scrollSegmentHeight;
	int d_scrollSegmentWidth;
	int d_TitleBarSegmentHeight;
	int d_MenuBarSegmentHeight;
	int d_GroupBoxTitleHeight;
	//
	ANYBARS d_nonclientbars;
};

#endif