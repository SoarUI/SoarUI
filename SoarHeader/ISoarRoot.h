#pragma once
#include "leestring.h"
/********************************************************************
    created:    2016/01/26
    created:    2016/01/26::   22:38
    filename:   ISoarRoot.h
    file path:  
    file base:  ISoarRoot
    file ext:   h
    author:     nicklisir/Soar
    copyright:  nicklisir@gmail.com QQ327681967 ecsuperpay@163.com
    purpose:    腾飞系统父窗口，操作起点
*********************************************************************/
#include "ISoarWnd.h"
#include <map>
#include "Dimensions.h"
#include "ISoarLoopRunner.h"
class ISoarRoot
{
public:

	ISoarRoot(void){}
	virtual ~ISoarRoot(void){}
	
public:
	//附加到系统窗口；意味着该窗口所有信息将由Soar系统接管
	virtual bool attachMsWindow(HWND hMainWnd) = 0;
	virtual bool CreateSoarUI(HWND hMainWnd,int x,int y,int W ,int H) =0;
	virtual bool CreateSoarUI(HWND hMainWnd,Dimension& x,Dimension& y,Dimension& w,Dimension& h)=0;
	virtual void setMainSheetTitle(CLeeString title) =0;
	virtual HWND getMsWindow(void) =0 ;
	virtual ISoarWnd* getSoarSheet(void) = 0;
	//获取窗口的大小：非客户区与客户区
	virtual RECT getWindowRect(void) =0 ;
	//获取窗口客户区的大小
	virtual RECT getClientRect(void) =0 ;
	virtual ISoarWnd* getWnd(const CLeeString &windowName) =0 ;
	virtual ISoarWnd* createWnd(const CLeeString & name,LWNDT classType ,ISoarWnd* Parent,INT dwZindex,LWNDSTY dwstyle) =0;
	virtual ISoarWnd* createWnd(const CLeeString & name,int x,int y,int w,int h,LWNDT classType ,
		ISoarWnd* Parent,INT dwZindex,LWNDSTY dwstyle)=0;
	//产生窗口
	virtual ISoarWnd* createWnd(const CLeeString & name,Dimension& x,Dimension& y,Dimension& w,Dimension& h,
		LWNDT classType ,ISoarWnd* Parent,INT dwZindex,LWNDSTY dwstyle)=0;
	virtual ISoarWnd* createPopupWnd(const CLeeString & name,LWNDT classType ,ISoarWnd* Parent,
		INT popupZindex,LWNDSTY dwstyle)=0;
	virtual ISoarWnd* createPopupWnd(const CLeeString & name,int x,int y,int w,int h,LWNDT classType ,
		ISoarWnd* Parent,INT popupZindex ,LWNDSTY dwstyle) =0 ;
	virtual ISoarWnd* createPopupWnd(const CLeeString & name,Dimension& x,Dimension& y,Dimension& w,Dimension& h,
		LWNDT classType ,ISoarWnd* Parent,INT dwZindex,LWNDSTY dwstyle) =0;
	//销毁与移除功能
	virtual bool destroyWnd(ISoarWnd * destroyWnd) =0 ;
	virtual bool eraseWnd(ISoarWnd * eraseWnd) =0 ;
	//特殊接口：可以让用自定义绘制
	virtual ISoarWnd* attachCustomClassWnd(ISoarWnd* Wnd,ISoarWnd* Parent ,INT dwZindex )=0 ;
	
	//----Engine_UI---start---
	virtual void ChangTheme(CLeeString & szThemeName)= 0;
	//UI更新
	virtual void endPatch(void)=0;
	virtual void beginPatch(void)=0;
	virtual void clearScene(void)=0;
	virtual void beginScene(void)=0;
	virtual void endScene(void)=0;
	virtual void Present(void)=0 ;
	virtual bool SoarUIRenderLoop(float t) =0;
	//Graphic operation
	virtual bool enableAlphaBlend(bool bEnable)=0;
	virtual bool isValidateWnd(ISoarWnd* hWnd) =0;
	//执行消息
	virtual void RunSoarMessageSytem(void) =0;
	//初始化弹出窗口状态
	virtual void StartModal(ISoarWnd* hModelWnd) =0;
	virtual int EndModal()=0;
	//显示模态窗口--考虑废除2021-12-11 4：28
	virtual int DoModal(ISoarWnd* hMainWnd) =0;
	virtual int DoModal(ISoarWnd* hMainWnd,ISoarLoopRunner* pGameLoop) =0;
	virtual bool CheckIsSoarMainWnd(ISoarWnd* pChkWnd)=0;//
	//----Engine_UI---End------
	/************************************************************************/
	/* 消息系统：                                                            */
	/************************************************************************/
	virtual LRESULT SoarUIMsgProc ( UINT ,WPARAM ,LPARAM )=0 ;
	virtual bool Register(UINT uMsg,ISoarWnd* handler,ISoarWnd* pOwner) =0 ;
	virtual bool UnRegister(ISoarWnd* pOwner,UINT uMsg) =0;
	//添加离线消息相当于MS的postMessage
	virtual bool addOfflineMsg(SOARMSG &msg) =0;
	//----2018-3-24
	///注入消息到Soar系统：鼠标消息和键盘信息
	virtual BOOL InJectMsg(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

};
