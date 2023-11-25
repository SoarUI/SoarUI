#pragma once
#include "../SoarHeader/ISoarWnd.h"
#include "../SoarHeader/Dimensions.h"
#include "../SoarHeader/ISoarEngine.h"
/************************************************************************/
/* Soar 系统 窗口基类                                                */
/************************************************************************/
class CSoarWnd :
	public ISoarWnd
{
public:
	CSoarWnd(HWND root,ISoarEngine* Eng);
public:
	virtual ~CSoarWnd(void);
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
	virtual void setText(const CLeeString & name);
	virtual void setTitle(const CLeeString & name);
	virtual void setWndName(const CLeeString & name) ;
	virtual CLeeString getWndName(void) ;
	virtual void setState(LWNDST state) ;
	virtual LWNDST getState(void) ;
	virtual void setVisibleState(LWNDST state) ; //新增 由系统管理
	virtual LWNDST getVisibleState(void);
	virtual CLeeString getDreamLook(void) ;
	virtual void setDreamLook(const CLeeString & name) ;
	virtual void checklookState(POINT &pt);
	virtual void attachRoot(HWND root);
	virtual LWNDSTY getWndStyle(void) ;
	virtual bool  setWndStyle(LWNDSTY lstyle) ;
	virtual bool  isPopupWnd(void) ;
	virtual void  setWndPopup(bool bPopup) ;
	virtual bool  isModalWnd(void) ;
	virtual void  setWndModal(bool bModal) ;
	//////////////////////////////////////////////////////////////////////////
	virtual void DrawSelf(ILeeDrawInterface *DrawFuns) ;
	//Graphic operation
	virtual bool enableAlphaBlend(bool bEnable);
	virtual void setAlphaValue(unsigned char value);
	virtual void scrollwnd(int cx,int cy);
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
	virtual ISoarBar* getScrollSegment(bool vert) ;
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
	virtual LeeWndPtrMap& getChidWndSet(){return d_WndPtrs;}
	//文字渲染相关
	virtual void setVertFormatting(HorizontalTextFormatting hfmt);
    virtual void setHorzFormatting(VerticalTextFormatting vfmt);
private://不导出的函数要定义成private,这样级不会影响虚表的位置
	int CalcClientTOP(int & rcTop) ;
	int CalcClientBOTTOM(int & rcBottom) ;
	int CalcClientLEFT(int & rcLeft) ;
	int CalcClientRIGHT(int & rcRight) ;
	void CalcLeft_Right_Bars() ;
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
	LWNDST d_wndVisibleState;//窗口可视化状态(由系统管理，当它与d_wndState 一致 显示 不一致 隐藏)
	LWNDSTY d_wndStyle;//窗口属性
	bool d_isPopupWnd;//标记为弹出窗口
	bool d_bModalState;//该层为modal层，意味着不处理消息不会传至上层
	bool d_bAutoDestroy;//是否自动销毁
	bool d_bEnableAlphaBlend;//开启混合
	//事件挂接
	ISoarEventHandler* p_EventHandler;
	ISoarBar* p_titlebar;
	ISoarBar* p_menubar;
	ISoarBar* p_HScrollSegment;
	ISoarBar* p_VScrollSegment;
	//重要的不见定义
	int d_scrollSegmentHeight;
	int d_scrollSegmentWidth;
	int d_TitleBarSegmentHeight;
	int d_MenuBarSegmentHeight;
	int d_GroupBoxTitleHeight;
	//文字格式
	int d_horzTextFormat;
	int d_VertTextFormat;
	//
	ANYBARS d_nonclientbars_TOP;
	ANYBARS d_nonclientbars_LEFT;
	ANYBARS d_nonclientbars_RIGHT;
	ANYBARS d_nonclientbars_BOTTOM;

};
