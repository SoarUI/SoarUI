#pragma once
#include "../SoarHeader/ISoarRoot.h"
#include "../SoarHeader/singleton_t.h"
#include "SoarBindSheetWnd.h"
#include "SoarMsgCenterMgr.h"
#include "../SoarHeader/CcriticalSection.h"
#include<list>
#include"ISoarSheet.h"
struct WndZindexStruct
{
	INT CurZindex;//0 为特殊窗口占用
	INT PrevZindex;
};
class CSoarRoot :
	public ISoarRoot,public LEE::Singleton_t<CSoarRoot>
{
public:
	CSoarRoot(ISoarEngine* eng);
	typedef	std::map<CLeeString, ISoarWnd*, CLeeString::FastLessCompare>	LeeWndMap;
	typedef	std::map<CLeeString, WndZindexStruct, CLeeString::FastLessCompare>	LeeWndZMap;
	typedef	std::list<ISoarSheet*>	LEESHEETSTACK;
	typedef	std::map<CLeeString, ISoarSheet*, CLeeString::FastLessCompare>	LEESHEETMAP;
public:
	virtual ~CSoarRoot(void);
	//特殊接口
	virtual void endPatch(void);
	virtual void beginPatch(void);
	virtual void clearScene(void);
	virtual void beginScene(void);
	virtual void endScene(void);
	virtual void Present(void) ;
	virtual bool SoarUIRenderLoop(float t) ;
	//Graphic operation
	virtual bool enableAlphaBlend(bool bEnable);
	virtual bool isValidateWnd(ISoarWnd* hWnd);
	virtual void RunSoarMessageSytem(void);
	////Z相关
	/*virtual INT setWndZIndex(ISoarWnd* Wnd,INT dwZindex);
	virtual INT setWndpopupZIndex(ISoarWnd* Wnd,INT dwZindex);*/
	virtual ISoarWnd* setFocusWnd(ISoarWnd* pFocus);
	virtual ISoarWnd* getFocusWnd(void){return d_theActivateSheet->getFocusWnd();}
	virtual void NotifyChildZIndexChanged(ISoarWnd* ChildWnd,int newFocusZpos,int oldZindex);
	//附加到系统窗口；意味着该窗口所有信息将由Soar系统接管
	virtual bool attachMsWindow(HWND hMainWnd) ;
	//创建UI窗口
	virtual bool CreateSoarUI(HWND hMainWnd,Dimension& x,Dimension& y,Dimension& w,Dimension& h);
	virtual bool CreateSoarUI(HWND hMainWnd,int x,int y,int W ,int H);
	virtual void setMainSheetTitle(CLeeString title);
	virtual HWND getMsWindow(void)  ;
	virtual ISoarWnd* getSoarSheet(void){return d_bindingSheet;}
	virtual ISoarSheet* getCurrentActiveSheet(){return d_theActivateSheet;}
	/*Sheet类函数*/
	//获取主窗口的大小：即客户区域大小
	virtual RECT getWindowRect(void)  ;
	//获取主窗口的大小：即客户区域大小
	virtual RECT getClientRect(void)  ;
	virtual ISoarWnd* getWnd(const CLeeString &windowName)  ;
	virtual ISoarWnd* createWnd(const CLeeString & name,LWNDT classType =LWNDT_WND,
		ISoarWnd* Parent=NULL,INT dwZindex =SOAR_WNDINDEX_TOP,LWNDSTY dwstyle=LWNDSTY_CHILD);
	virtual ISoarWnd* createWnd(const CLeeString & name,int x,int y,int w,int h,LWNDT classType =LWNDT_WND,
		ISoarWnd* Parent=NULL,INT dwZindex =SOAR_WNDINDEX_TOP,LWNDSTY dwstyle=LWNDSTY_CHILD) ;
	virtual ISoarWnd* createPopupWnd(const CLeeString & name,LWNDT classType =LWNDT_WND,ISoarWnd* Parent=NULL,
		INT popupZindex =1,LWNDSTY dwstyle=LWNDSTY_CHILD);
	virtual ISoarWnd* createPopupWnd(const CLeeString & name,int x,int y,int w,int h,LWNDT classType =LWNDT_WND,
		ISoarWnd* Parent=NULL,INT popupZindex =SOAR_WNDINDEX_TOP,LWNDSTY dwstyle=LWNDSTY_CHILD) ;
	virtual bool destroyWnd(ISoarWnd * destroyWnd);
	virtual bool eraseWnd(ISoarWnd * eraseWnd) ;
	//统一坐标
	virtual ISoarWnd* createWnd(const CLeeString & name,Dimension& x,Dimension& y,Dimension& w,Dimension& h,LWNDT classType ,
		ISoarWnd* Parent,INT dwZindex,LWNDSTY dwstyle=LWNDSTY_CHILD);
	virtual ISoarWnd* createPopupWnd(const CLeeString & name,Dimension& x,Dimension& y,Dimension& w,Dimension& h,LWNDT classType ,
		ISoarWnd* Parent,INT dwZindex,LWNDSTY dwstyle=LWNDSTY_CHILD);
	//特殊接口：可以让用自定义绘制
	virtual ISoarWnd* attachCustomClassWnd(ISoarWnd* Wnd,ISoarWnd* Parent=NULL,INT dwZindex =SOAR_WNDINDEX_TOP) ;
	/************************************************************************/
	/* 消息系统：                                                            */
	/************************************************************************/
	virtual bool Register(UINT uMsg,ISoarWnd* handler,ISoarWnd* pOwner);
	virtual bool UnRegister(ISoarWnd* pOwner,UINT uMsg);
	virtual bool addOfflineMsg(SOARMSG &msg);
	virtual bool RunSoarMsg(MSG &msg,LRESULT& lr);
	//默认顶层处理(最后处理)
	virtual bool RunTopSheetMsg(MSG &msg,LRESULT& lr);
	//作为UI附件必需调用该函数
	virtual BOOL InJectMsg(UINT uMsg, WPARAM wParam, LPARAM lParam);
	INT translateMouseMessage(MSG &msg,bool bNCMsg);
	/************************************************************************/
	/* UIEngine method                                                      */
	/************************************************************************/
	virtual void ChangTheme(CLeeString & szThemeName);
	/*
	*模态对话框支持
	*/
	virtual void StartModal(ISoarWnd* hModelWnd);
	virtual int EndModal();
	virtual int DoModal(ISoarWnd* hMainWnd);
	virtual int DoModal(ISoarWnd* hMainWnd,ISoarLoopRunner* pGameLoop);
	virtual bool CheckIsSoarMainWnd(ISoarWnd* pChkWnd);//
	//2018-2-20
	virtual LRESULT SoarDefWndProc ( UINT ,WPARAM ,LPARAM ) ;
private:
	virtual LRESULT SoarUIMsgProc ( UINT ,WPARAM ,LPARAM ) ;
	//处理鼠标相关的消息
	virtual bool filter_MouseMessage(UINT uMsg,WPARAM wParam,LPARAM lParam,LRESULT& );
	virtual bool filter_allMessage(UINT uMsg,WPARAM wParam,LPARAM lParam,LRESULT& lr);
	ISoarWnd* createWnd(LWNDT classType,LWNDSTY lstyle);
	
protected:
	//用户类绘制接口
	static void CustomDrawSelfT(const CLeeString& elementName,const CLeeString& elementStatus,const RECT& ControlRect,const CLeeString& text,int vertTextFmt,int horzTextFmt);
	static void CustomDrawSelf(const CLeeString& elementName,const CLeeString& elementStatus,const RECT& ControlRect);
	static bool CustomEnableAlphaBlend(bool bEnable);
	static bool CustomGraphicGetAlphaBlendState();
protected:
	static LeeWndMap d_Wnds;//对象列表
	static LeeWndZMap d_WndZs;//绘制顺序与逻辑顺序
	//新增
	static LeeWndMap d_PopupWnds;//弹出对象列表
	static LeeWndZMap d_PopupWndZs;//弹出绘制顺序与逻辑顺序
    HWND d_hOwner;
	INT d_zIndex;//窗口顺序
	INT d_popupZindex;//弹出窗口索引
	//static ISoarWnd* d_focusWnd;//焦点窗口
	WNDPROC d_oldWndProc;
	static ISoarEngine * d_DrawEngine;//绘制对象
	ISoarWnd *d_bindingSheet;//窗口底板，它是所有窗口的父窗口
	CSoarMsgCenterMgr d_themsgCenter;
	ISoarSheet*       d_theActivateSheet;//
	CcriticalSection d_WndCcSection;
	CcriticalSection d_PopupWndCcSection;
	CcriticalSection d_WndZCcSection;
	CcriticalSection d_PopupWndZCcSection;
	LEESHEETSTACK d_sheetStack; //所有要渲染的sheet
	LEESHEETMAP d_sheetlists;//保存所有的sheet
	bool d_bEnableAlphaBlend;//开启混合
};
