#pragma once
#include "leestring.h"
#include "../SoarHeader/ILeeDrawInterface.h"
#include <map>
#include<vector>
#include "Dimensions.h"
#include"ISoarEventHandler.h"
/********************************************************************
    created:    2016/01/26
    created:    2016/01/26::   22:38
    filename:   ILeeRoot.h
    file path:  
    file base:  ILeeRoot
    file ext:   h
    author:     nicklisir/lee
    copyright:  nicklisir@gmail.com QQ327681967 ecsuperpay@163.com
    purpose:    腾飞系统父窗口，操作起点
*********************************************************************/
typedef enum SOARWNDTYPE
{
	LWNDT_UNKNOWN              =0,
	LWNDT_SYS_SHEET            =1,//特殊窗口
	LWNDT_SYS_UI_SHEET         =LWNDT_SYS_SHEET +1,
	LWNDT_SYS_SEGMENT          =LWNDT_SYS_UI_SHEET +1,
	LWNDT_SYS_TITLEBAR         =LWNDT_SYS_SEGMENT +1,
	LWNDT_SYS_MENUBAR          =LWNDT_SYS_TITLEBAR +1,
	LWNDT_SYS_SCROLLBAR        =LWNDT_SYS_MENUBAR  +1,
	LWNDT_SYS_HEADERWND        =LWNDT_SYS_SCROLLBAR + 1,
	LWNDT_SYS_TABCTRHEADER     =LWNDT_SYS_HEADERWND +1,
	LWNDT_SYS_SPINBUTTON		=LWNDT_SYS_TABCTRHEADER+1,
	LWNDT_SYS_INFODIALOG        =LWNDT_SYS_SPINBUTTON + 1,
	
	LWNDT_WND                  = LWNDT_SYS_INFODIALOG+1,
	LWNDT_STATIC               = LWNDT_WND+1,
	LWNDT_BUTTON               = LWNDT_STATIC+1,
	LWNDT_RADIOBTN             = LWNDT_BUTTON +1,
	LWNDT_CHECKBTN             =LWNDT_RADIOBTN +1,
	LWNDT_GROUPBOX             =LWNDT_CHECKBTN +1,
	LWNDT_EDIT                 =LWNDT_GROUPBOX +1,
	LWNDT_COMBOBOX             =LWNDT_EDIT +1,
	LWNDT_COMBOEX              =LWNDT_COMBOBOX +1,
	LWNDT_LISTBOX              =LWNDT_COMBOEX  +1,
	LWNDT_TABCTRL              =LWNDT_LISTBOX +1,
	LWNDT_INTERETWND           =LWNDT_TABCTRL +1,
	LWNDT_LISTVIEWWND          =LWNDT_INTERETWND +1,
	LWNDT_TREEVIEWWND          =LWNDT_LISTVIEWWND +1,
	LWNDT_PROGRESSBAR          =LWNDT_TREEVIEWWND +1,
	LWNDT_SPINWND           =LWNDT_PROGRESSBAR +1,
	LWNDT_SCROLLBAR            =LWNDT_SPINWND +1,
	LWNDT_SLIDERWND			=LWNDT_SCROLLBAR +1,
	LWNDT_FRAME                =      35,
	
	LWNDT_POPMENU              =LWNDT_FRAME +1,
	
	LWNDT_POPLISTBOX           =200,
	LWNDT_POPLISTVIEW          =201,
	LWNDT_CMM_FILEDIALOG       =300,
	LWNDT_CMM_FOLDERDIALOG     =301,
	LWNDT_STAICITEMWND         =1001,
	LWNDT_MOVABLEITEMWND       =1002,
	
	LWNDT_STATICPOPMENUITEM    =1004,
	LWNDT_SEGMENT				=2000,
	LWNDT_ITEM_SEGMENT			=LWNDT_SEGMENT+1,
	LWNDT_COLUMNITEM_SEGMENT			=LWNDT_ITEM_SEGMENT+1,
	
}LWNDT,*PLWNDT;
typedef enum SOARBARALIGN
{
	SOARALIGN_LEFT =0,
	SOARALIGN_TOP =1,
	SOARALIGN_BOTTOM =2,
	SOARALIGN_RIGHT =3,
};
typedef enum SOARWNDSTATE
{
	LWNDST_HIDE =0,
	LWNDST_DISABLED =1,
	LWNDST_ENABLE =2,
	LWNDST_MINI =3,
	LWNDST_TItleBarOnly =4,
	LWNDST_SHOW =5,
	LWNDST_MAX =6,
	LWNDST_RESTORE=7,
}LWNDST,*PLWNDST;
typedef enum SOARWNDSTYLE
{
	LWNDSTY_CHILD =0,
	LWNDSTY_HORZWND     =0x1,
	LWNDSTY_VERTWND     =0x2,
	LWNDSTY_HSCROLL     =0x4,
	LWNDSTY_VSCROLL     =0x8,
	LWNDSTY_SYSMENUBAR  =0x10,
	LWNDSTY_SYSTITLEBAR =0x20,
	LWNDSTY_SYSTITLE =0x40,
	LWNDSTY_SYSHEADERCTRL =0x80,
	LWNDSTY_POPUP=0x100,
	LWNDSTY_EXTEND=0x200,
}LWNDSTY,*PLWNDSTY;
#define SOAR_START           WM_USER+1000
#define SOAR_COMMAND         SOAR_START+1
#define SOAR_ITEMSELECTED    SOAR_COMMAND+1
#define SOAR_ITEMCOLUMNSELECTED    SOAR_ITEMSELECTED+1
#define SOAR_SELITEMCOLUMNCHANGED    SOAR_ITEMCOLUMNSELECTED+1
#define SOAR_SELCHANGED      SOAR_SELITEMCOLUMNCHANGED+1
#define SOAR_MENUITEMSELECTED    SOAR_SELCHANGED+1
#define SOAR_SIZE            SOAR_SELCHANGED +1
#define SOAR_HSCROLL         SOAR_SIZE+1
#define SOAR_VSCROLL         SOAR_HSCROLL+1
#define SOAR_MOUSEWHEEL      SOAR_VSCROLL+1
#define SOAR_SETFOCUS        SOAR_MOUSEWHEEL+1
#define SOAR_LOSTFOCUS       SOAR_SETFOCUS +1
#define SOAR_TITLEBARMSG     SOAR_LOSTFOCUS+1
#define SOAR_SYSTEMMSG       SOAR_TITLEBARMSG+1
#define SOAR_GROUPBOXMSG     SOAR_SYSTEMMSG+1
#define SOAR_LBUTTONCLICK    SOAR_GROUPBOXMSG +1
#define SOAR_SLIDERCHANGING  SOAR_LBUTTONCLICK+1
#define SOAR_SLIDERCHANGED  SOAR_LBUTTONCLICK+1

#define SOAR_END      SOAR_START+1000
//鼠标定义
#define SOAR_LCLICK_DOWN  1
#define SOAR_LCLICK_UP   SOAR_LCLICK_DOWN+1
#define SOAR_LDBCLICK SOAR_LCLICK_UP+1
#define SOAR_RCLICK_DOWN   SOAR_LDBCLICK+1
#define SOAR_RCLICK_UP  SOAR_RCLICK_DOWN +1
#define SOAR_RDBCLICK SOAR_RCLICK_UP+1
#define SOAR_MCLICK_DOWN SOAR_RDBCLICK+1
#define SOAR_MCLICK_UP SOAR_MCLICK_DOWN+1
#define SOAR_MDBCLICK SOAR_MCLICK_UP+1
#define SOAR_XWHEEL   SOAR_MDBCLICK+1
#define SOAR_MOUSEMOVE SOAR_XWHEEL+1
#define SOAR_NCMOUSEMOVE SOAR_MOUSEMOVE+1
#define SOAR_NCLCLICK_DOWN  SOAR_NCMOUSEMOVE+1
#define SOAR_NCLCLICK_UP   SOAR_NCLCLICK_DOWN+1
#define SOAR_NCLDBCLICK SOAR_NCLCLICK_UP+1
#define SOAR_NCRCLICK_DOWN   SOAR_NCLDBCLICK+1
#define SOAR_NCRCLICK_UP  SOAR_NCRCLICK_DOWN +1
#define SOAR_NCRDBCLICK SOAR_NCRCLICK_UP+1
//titlebar button 定义
#define SOAR_SYSMIN 0x00000001
#define SOAR_SYSCLOSE 0x00000010
#define SOAR_SYSMAX 0x00000100
//消息类型定义
#define SOAR_MSG_ORIG  0
#define SOAR_MSG_TRANSFER 1
#define SOAR_WNDINDEX_TOP -1
class ISoarWnd;
class ISoarEventHandler;
class ISoarSegment;
class ISoarColumnItemBase;
class ISoarObject;
typedef struct SOARMSGCover
{
	ISoarObject* targetWnd;
	ISoarObject* sourceWnd;
	ISoarObject* routeWnd;//中续者
	DWORD message;
	INT msgSourceTag;//原来的标志
	INT mouseEvent;
	INT wParam;
	INT lParam;
	LPVOID Data;
}SOARMSG ,*PSOARMSG;

struct SoarSegmentItem
{
	CLeeString d_wndlook;//对应皮肤控件
	CLeeString d_wndlookState;//皮肤状态
	CLeeString d_wndText;//显示地文字
	ComponentArea d_area;//保存统一坐标信息
	ISoarWnd* d_pMsgReceiver;//接收消息地窗口
	ISoarWnd* d_pNextWnd;//二级窗口(菜单项的二级窗口)
	DWORD d_nId;//唯一ID，在一个窗口中/Segment时唯一
	DWORD d_iIndex;
	DWORD d_subindex;
	DWORD d_parentSegmentID;//它的父segmentID
	void* udata;
};	
typedef std::vector<SoarSegmentItem> SEGMENTITEMS;
class ISoarObject{
public:
	virtual LWNDT getType(void) =0;
	//virtual void setParent(ISoarObject*parent)=0;
	//virtual ISoarObject* getParent()=0;
};
/*
Segment :非Wnd的都叫Segment，它依赖Wnd而存在
*/
class ISoarSegment: public ISoarObject
{
public:
	virtual RECT getBarRect(void) =0;
	virtual RECT getBarCliperRect(void) =0;
	virtual void setArea(RECT& rc) =0;
	virtual void setArea(ComponentArea& area) =0;
	virtual ComponentArea getArea()=0;
	virtual void DrawSelf(ILeeDrawInterface *DrawFuns)=0 ;
	virtual BOOL HandleEvent( UINT ,WPARAM ,LPARAM, LRESULT&) =0;
	virtual BOOL BarHitTest(void) =0;
	virtual SOARBARALIGN getBarAlign(void) =0;
	virtual void setTitle(const CLeeString& name)=0;
	virtual void setParent(ISoarSegment*parent)=0;
	virtual ISoarSegment* getParent()=0;
	virtual void setState(LWNDST state) =0;
	virtual void setMsgReceiver(ISoarWnd* pOwnerWnd)=0;
	virtual ISoarWnd* getMsgReceiver()=0;
	//Graphic operation not enable yet 
	//virtual bool enableAlphaBlend(bool bEnable)=0;
	//文字渲染相关
	//virtual void setVertFormatting(HorizontalTextFormatting hfmt)=0;
    //virtual void setHorzFormatting(VerticalTextFormatting vfmt)=0;
};
class ISoarBar: public ISoarSegment
{
public:
	virtual SoarSegmentItem getItem(const DWORD dId)=0;
	virtual bool AddItem(const SoarSegmentItem&)=0;
	virtual bool removeItem(const DWORD dId)=0;
	virtual int getItemCount(void)=0;
};
typedef std::vector<ISoarBar*> ANYBARS;
class ISoarScroll: public ISoarSegment
{
public:
	virtual BOOL HandleEvent( UINT ,WPARAM ,LPARAM, LRESULT&) =0;
};
class ISoarItemBase: public ISoarSegment
{
public:
	virtual LPCTSTR getTitle(void)=0 ;
	virtual DWORD getIndex(void)=0 ;
	virtual DWORD getID(void)=0 ;
	virtual LPVOID getData(void)=0 ;
	virtual void setData(LPVOID data)=0 ;
	virtual void setID(DWORD dwId) =0;
	virtual void setIndex(DWORD dwIndex)=0 ;
	virtual void setOwnerWnd(ISoarWnd* pOwnerWnd)=0;
	virtual ISoarWnd* getOwnerWnd()=0;
	virtual void setHeight(int h) =0;
	virtual void setWidght(int w) =0;
	virtual bool getCheck(void)=0;
	virtual void setcheck(bool bcheck)=0;
	virtual ISoarColumnItemBase* getItem(int nCol)=0;
	virtual int AddItem(ISoarColumnItemBase*col)=0;
	virtual bool removeItem(const int dId)=0;
	virtual int getItemCount(void)=0;
	virtual void destroyAllItems()=0;
	
};
class ISoarColumnItemBase: public ISoarSegment
{
public:
	virtual LPCTSTR getTitle(void)=0 ;
	virtual DWORD getIndex(void)=0 ;
	virtual DWORD getsubIndex(void)=0 ;
	virtual DWORD getID(void)=0 ;
	virtual LPVOID getData(void)=0 ;
	virtual void setData(LPVOID data)=0 ;
	virtual void setID(DWORD dwId) =0;
	virtual void setIndex(DWORD dwIndex)=0 ;
	virtual void setsubIndex(DWORD dwIndex)=0 ;
	virtual void setOwnerWnd(ISoarWnd* pOwnerWnd)=0;
	virtual ISoarWnd* getOwnerWnd()=0;
	
	virtual void setHeight(int h) =0;
	virtual void setWidght(int w) =0;
	virtual bool getCheck(void)=0;
	virtual void setcheck(bool bcheck)=0;
	virtual void setState(LWNDST state) =0;
	virtual LWNDST getState(void) =0;
	virtual SoarSegmentItem getItem(const DWORD dId) =0;
	virtual bool AddItem(const SoarSegmentItem&) =0;
	virtual bool removeItem(const DWORD dId) =0;
	virtual int getItemCount(void) =0;
	
};

typedef	std::map<CLeeString, ISoarWnd*, CLeeString::FastLessCompare>	LeeWndPtrMap;
class ISoarWnd: public ISoarObject
{
public:
    
	ISoarWnd(void){}
	virtual ~ISoarWnd(void){}

public:
	virtual void InitializeSegments(void) =0;
	virtual HWND getRootWnd(void) =0;
	//获取主窗口的大小：即客户区域大小
	virtual LWNDT getType(void) =0;
	//获取相对于父窗口的矩形
	virtual RECT getClientRect(void) =0 ;
	//获取相对于Root窗口的坐标
	virtual RECT getWindowRect(void) =0 ;
	virtual RECT getCliperRect(void) =0;
	virtual void setArea(RECT& rc) =0 ;
	virtual void setArea(ComponentArea& area)=0;
	virtual void getArea(ComponentArea& area)=0;
	virtual void setWindowPos(int x,int y) =0;
	virtual void setWindowHeight(int h)=0 ;
	virtual void setWindowWidth(int w)=0 ;
	virtual void setWindowHeight(Dimension h)=0 ;
	virtual void setWindowWidth(Dimension w)=0 ;
	virtual ISoarWnd* GetParent(void) = 0;
	virtual void setParent(ISoarWnd * lParent) = 0;
	virtual INT setZindex(INT dwZindex)=0 ;
	virtual INT getZindex(void)=0 ;
	virtual INT setPopupZindex(INT dwZindex)=0 ;
	virtual INT getPopupZindex(void)=0 ;
	virtual LPCTSTR getText(void) =0;
	virtual void setWndName(const CLeeString &) =0;
	virtual LPCTSTR getWndName(void) =0;
	virtual void setText(const CLeeString &name) =0;
	virtual void setTitle(const CLeeString &name)=0;
	virtual void setState(LWNDST state) =0;
	virtual LWNDST getState(void) =0;
	virtual void setVisibleState(LWNDST state) = 0; //新增 由系统管理
	virtual LWNDST getVisibleState(void) = 0;
	virtual LPCTSTR getDreamLook(void) =0;
	virtual void setDreamLook(const CLeeString &) =0;
	virtual void checklookState(POINT &pt) =0;//检测状态，并自动更新
	virtual void attachRoot(HWND root) = 0;
	virtual LWNDSTY getWndStyle(void) =0;
	virtual bool  setWndStyle(LWNDSTY lstyle) =0;
	virtual bool  isPopupWnd(void) =0 ;
	virtual void  setWndPopup(bool bPopup) =0;
	virtual bool  isModalWnd(void) =0;
	virtual void  setWndModal(bool bModal) =0;
	//////////////////////////////////////////////////////////////////////////
	virtual void DrawSelf(ILeeDrawInterface *DrawFuns) =0;
	//消息处理
	virtual BOOL HandleEvent ( UINT ,WPARAM ,LPARAM, LRESULT&)=0 ;
	virtual BOOL HandleEvent ( ISoarWnd* pOwner,UINT ,WPARAM ,LPARAM, LRESULT&) =0;
	//挂钩处理类
	virtual ISoarEventHandler* RegisterEventHandler( ISoarEventHandler * pEventHandler) = 0;
	//用于获取子窗口:如果只有主窗口则返回
	virtual ISoarWnd* getChildWnd(const CLeeString & name) =0;
	virtual bool addChildWnd(ISoarWnd* pWnd) =0;
	virtual bool eraseChildWnd(ISoarWnd* pWnd) =0 ;
	//////////////////////////////////////////////////////////////////////////
	virtual ISoarBar* getMenuBar(void) =0;
	virtual ISoarBar* getTitleBar(void) =0;
	virtual ISoarBar* getScrollSegment(bool vert) =0;
	virtual int setScrollInfo(int width,int height) =0;
	virtual int getScrollInfo(bool bHeight) =0;
	virtual int setTitleBarSegment(int nHeight) =0;
	virtual int getTitleBarSegment(void) =0;
	virtual int setMenuBarSegment(int nHeight) =0;
	virtual int getMenuBarSegment(void) =0;
	//计算函数
	virtual RECT CalcClientRec(RECT & rcWnd) =0;
	virtual ISoarBar* AddNonClientBar(const CLeeString& elementName,SOARBARALIGN align,
		LWNDT classType,int nHeight_or_Width) = 0 ;
	 virtual void DrawNonClientBars(ILeeDrawInterface *DrawFuns) = 0;
	 virtual BOOL HandleNonClientBarsEvent( UINT umsg ,WPARAM wp ,LPARAM lp ,LRESULT &lr) =0 ;
	//////////////////////////////////////////////////////////////////////////
	//处理函数
	//menu
	virtual void HandleMenuEvent(int nPos,int nID,SOARMSG * pMsgInfo) =0 ;
	//子窗口集
	virtual LeeWndPtrMap& getChidWndSet()=0;
	//文字渲染相关
	virtual void setVertFormatting(HorizontalTextFormatting hfmt)=0;
    virtual void setHorzFormatting(VerticalTextFormatting vfmt)=0;
	//Graphic operation
	virtual bool enableAlphaBlend(bool bEnable)=0;
	virtual void setAlphaValue(unsigned char value)=0;
	virtual void scrollwnd(int cx,int cy)=0;
};