#pragma once
#include "../SoarHeader/ISoarEngine.h"
#include "ResourcesManager.h"

class CSoarEngine :
	public ISoarEngine
{
public:
	CSoarEngine(HMODULE hmod);
public:
	virtual ~CSoarEngine(void);
	virtual void SetWidgetLookDirection(const CLeeString resPath)  ;
	virtual void SetPluginDirection(const CLeeString plugPath)  ;
	virtual void SetImageSetDirection(const CLeeString plugPath)  ;
	virtual void SetTextureDirection(const CLeeString resPath)  ;
	virtual void SetResourceDirection(const CLeeString resPath) ;
	virtual void SetXmlDataProvider(const CLeeString& provider, const CLeeString& Constructor,const CLeeString& Destructor)  ;
	virtual void SetXmlParser(const CLeeString& parser, const CLeeString& Constructor,const CLeeString& Destructor) ;
	virtual void SetDefaultBlender(const CLeeString& render, const CLeeString& Constructor,const CLeeString& Destructor) ;
	virtual void SetDefaultImgCodec(const CLeeString&  ImgCodec, const CLeeString& Constructor,const CLeeString& Destructor) ;
	virtual void SetDefaultPaser(ILeeXmlParser * paser)  ;
    virtual bool CloneXmlParser(ILeeXmlParser** pRet);
	//库功能
	virtual bool engine_loadtheme(const CLeeString& themeName) ;//加载主题，主题其实就是一张图片
	virtual bool engine_SetDefaultTheme(const CLeeString& themeFile,const CLeeString& defTheme) ;//只加载一次
	virtual bool engine_defineResources(const CLeeString& resFile) ;//资源定义
	virtual bool engine_loadDefaultTheme(void) ;
	virtual bool engine_destroytheme(const CLeeString& name) ;
	virtual bool engine_loadGroupRes(const CLeeString& group) ;//加载组资源
	virtual bool engine_loadResources(const CLeeString& resName) ;//加载资源
	virtual void SetKeyAny(const _ProviderKeyAny& key) ;
	//接口定义
	virtual bool hasWidgetlook(const CLeeString& widget) ;
	//---通用模块：第一步（绘制背景），第二步（绘制文字）
	virtual void DrawWidgetlook(const CLeeString& elementName,const CLeeString& elementStatus,
		const RectF& ControlRect,const CLeeString& text,int vertTextFmt,int horzTextFmt,const RectF* clipper = 0, bool clipToDisplay = false) ;
	//--用于绘制没有文字的widget:图像或背景等
	virtual void DrawWidgetlookImage(const CLeeString& WidgetName,const CLeeString& elementStatus,
		const RectF& ControlRect,const RectF* clipper = 0, bool clipToDisplay = false) ;
	//用于绘制边框
	virtual void DrawWidgetlookFrame(const CLeeString& WidgetName,const CLeeString& elementStatus,
		const RectF& ControlRect,const RectF* clipper = 0, bool clipToDisplay = false);
	virtual void endPatch(void);
	virtual void beginPatch(void);
	virtual void clearScene(void);
	virtual void beginScene(void);
	virtual void endScene(void);
	virtual void Present(void) ;
	virtual void InitRender(HWND hWnd) ;
	virtual void InitRenderEx( LPVOID d3dDevice, LPVOID DXGISwapChain);
	virtual void ShutdownRender(void) ;
	virtual bool DisplayReset(void);
	virtual bool EnableAlphaBlend(bool bEnableAlphaBlend);
	virtual bool GetAlphaBlendState();
	//线程池支持
	virtual void SetThreadPool(ILeeThreadpool * pPool);
	virtual void Release(void);
private:
	ResourcesManager * d_reSMgr;//资源管理器
};
