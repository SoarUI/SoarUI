#pragma once
/********************************************************************
    created:    2017/11/30
    created:    2017/11/30::   01:05
    file base:  ISoar
    file ext:   h
    author:     nicklisir/Soar/flydream
    copyright:  QQ327681967 ecsuperpay@163.com
    purpose:    腾飞系统显示核心模块
*********************************************************************/
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "ISoarRoot.h"
#include "ISoarWnd.h"
#include "leestring.h"
#include "../XML/ILeeXmlParser.h"
#include "ISoarLoopRunner.h"
class ISoar
{
public:

	ISoar(void){}
	virtual ~ISoar(void){}
public:
	//通过已有的窗口创建Soar系统
	virtual ISoarRoot* createRootWindow(HWND hmainwindow,const CLeeString& cfgfile) =0;
	//通过已有窗口创建SoarUI附件窗口(嵌入)
	virtual ISoarRoot* createSoarUIRootWindow(HWND hmainwindow,const CLeeString& cfgfile,int x,int y,int W ,int H) =0;
	//快速创建Soar系统
	virtual ISoarRoot* createRootWindow(const CLeeString& cfgfile, const CLeeString &szTitle,DWORD style,int x,int y,int W ,int H) =0;
	virtual ISoarRoot* getRootWindow(HWND hmainwindow) =0;
	virtual bool InitEnv(void) =0;
	virtual void UnLoadEnv(void) =0;
	virtual int DoMessgeloop(void) =0;
	virtual int DoMessageUILoop(ISoarLoopRunner* pGameLoop) =0;
	virtual bool CloneXmlParser(ILeeXmlParser** pRet)=0;
	//SoarUI支持
	virtual void RunSoarMessageSystem(void) =0;
	virtual void RunSoarUIRenderSystem(float t) =0;
	virtual int RunSoarUIModelDialogSystem(ISoarWnd* hMainWnd,ISoarLoopRunner* pGameLoop) =0;
	//开始屏幕绘制
	virtual void RenderScene_Begin(void) =0;
	//结束屏幕绘制
	virtual void RenderScene_End(void) =0;
	//开始批量绘制
	virtual void RenderPatch_Begin(void)=0 ;
	//结束批量绘制
	virtual void RenderPatch_End(void)=0 ;
	virtual bool Render_AlphaBlendEnable(bool bEnable)=0;
	virtual bool Render_GetAlphaBlendState()=0;
	//清空屏幕
	virtual void RenderScene_Clear(void) =0;
	virtual void RenderScene_Present(void) =0;
	virtual void InitRenderEx( LPVOID d3dDevice  )=0;
	virtual void InitRender( HWND hwnd)=0;
};
