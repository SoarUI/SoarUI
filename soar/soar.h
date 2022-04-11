#pragma once
#include "../SoarHeader/ISoar.h"
#include <map>
#include "SoarEnv.h"
#include "../xml\ILeeXmlParser.h"
#include "../SoarHeader/ISoarEngine.h"
class CSoar :
	public ISoar
{
public:
	CSoar(HMODULE hModule);
	virtual ~CSoar(void);
public:
	virtual ISoarRoot* createRootWindow(HWND hmainwindow,const CLeeString& cfgfile);
	//通过已有窗口创建SoarUI附件窗口(嵌入)
	virtual ISoarRoot* createSoarUIRootWindow(HWND hmainwindow,const CLeeString& cfgfile,int x,int y,int W ,int H) ;
	virtual ISoarRoot* createRootWindow(const CLeeString& cfgfile,CLeeString szTitle,DWORD style,int x,int y,int W ,int H);
	virtual ISoarRoot* getRootWindow(HWND hmainwindow) ;
	virtual bool InitEnv(void);
	virtual void UnLoadEnv(void);
	virtual int DoMessgeloop(void);
	virtual bool CloneXmlParser(ILeeXmlParser** pRet);
	//作为SOarUI
	virtual int DoMessageUILoop(ISoarLoopRunner* pGameLoop) ;
	virtual void RunSoarMessageSystem(void) ;
	virtual void RunSoarUIRenderSystem(float t) ;
	virtual int RunSoarUIModelDialogSystem(ISoarWnd* hMainWnd,ISoarLoopRunner* pGameLoop);
	//开始屏幕绘制
	virtual void RenderScene_Begin(void) ;
	//结束屏幕绘制
	virtual void RenderScene_End(void) ;
	//开始批量绘制
	virtual void RenderPatch_Begin(void) ;
	//结束批量绘制
	virtual void RenderPatch_End(void) ;
	//清空屏幕
	virtual void RenderScene_Clear(void);
	virtual void RenderScene_Present(void) ;
	virtual void InitRenderEx(LPVOID d3dDevice  );
	virtual void InitRender( HWND hwnd);
	//全引擎开启混合
	virtual bool Render_AlphaBlendEnable(bool bEnable);
	virtual bool Render_GetAlphaBlendState();
public:
	static LRESULT CALLBACK sLeeRootWndProc ( HWND , UINT ,WPARAM ,LPARAM ) ;
protected:
	static CSoarEnv * theSoarEnv;//系统环境
private:
	ISoarEngine * theDrawEngine;
	HANDLE d_handle;
	HMODULE d_hModule;
	ILeeXmlParser* d_parser;
};
