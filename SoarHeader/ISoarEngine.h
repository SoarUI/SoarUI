#pragma once
#include "leemacro.h"
#include "leestring.h"
#include "../XML\ILeeXmlParser.h"
#include "ProviderAny.h"
#include "../threadpool\ILeeThreadpool.h"
#include "RectF.h"
class IControlEngine;
/*
LooksEngine 主要负责Ui资源的加载与卸载,各种SOAR系统定义的控件，状态的加载
*/
class ISoarEngine
{
public:

	ISoarEngine(void){}
	virtual ~ISoarEngine(void){}
public:
	//设置widgetlook文件路径
	virtual void SetWidgetLookDirection(const CLeeString resPath) LEEPURE ;
	//设置插件路径
	virtual void SetPluginDirection(const CLeeString plugPath) LEEPURE ;
	//图像集路径
	virtual void SetImageSetDirection(const CLeeString plugPath)  LEEPURE ;
	//设置实际图像文件路径
	virtual void SetTextureDirection(const CLeeString resPath)  LEEPURE ;
	//设置默认资源路径
	virtual void SetResourceDirection(const CLeeString resPath)  LEEPURE ;
	///插件功能
	//XML处理器
	virtual void SetXmlDataProvider(const CLeeString& provider , const CLeeString& Constructor,const CLeeString& Destructor) LEEPURE ;
	//XML解析器
	virtual void SetXmlParser(const CLeeString & parser, const CLeeString& Constructor,const CLeeString& Destructor) LEEPURE ;
	//渲染器
	virtual void SetDefaultBlender(const CLeeString& blender , const CLeeString& Constructor,const CLeeString& Destructor) LEEPURE ;
	//图像解码器
	virtual void SetDefaultImgCodec(const CLeeString & ImgCodec , const CLeeString& Constructor,const CLeeString& Destructor) LEEPURE ;
	//外部解析器
	virtual void SetDefaultPaser(ILeeXmlParser * paser) LEEPURE ;
	//创建XML解释器
	virtual bool CloneXmlParser(ILeeXmlParser** pRet) LEEPURE;
	//库导出功能
	virtual bool engine_loadtheme(const CLeeString& themeName) LEEPURE ;//加载主题，主题其实就是一张图片
	virtual bool engine_SetDefaultTheme(const CLeeString& themeFile,const CLeeString& defTheme) LEEPURE ;//只加载一次
	virtual bool engine_defineResources(const CLeeString& resFile) LEEPURE ;//资源定义
	//加载默认资源
	virtual bool engine_loadDefaultTheme(void) LEEPURE ;
	//卸载资源
	virtual bool engine_destroytheme(const CLeeString& name) LEEPURE ;
	//加载组资源
	virtual bool engine_loadGroupRes(const CLeeString& group) LEEPURE ;
	////加载资源
	virtual bool engine_loadResources(const CLeeString& resName) LEEPURE ;
	//加密数据接口
	virtual void SetKeyAny(const _ProviderKeyAny& key) LEEPURE ;
	//接口定义
	virtual bool hasWidgetlook(const CLeeString& widget) LEEPURE ;
	//---通用模块：第一步（绘制背景），第二步（绘制文字）
	virtual void DrawWidgetlook(const CLeeString& elementName,const CLeeString& elementStatus,
		const RectF& ControlRect,const CLeeString& text,int vertTextFmt,int horzTextFmt,const RectF* clipper = 0, bool clipToDisplay = false)LEEPURE;
	//--用于绘制没有文字的widget:图像或背景等
	virtual void DrawWidgetlookImage(const CLeeString& WidgetName,const CLeeString& elementStatus,
		const RectF& ControlRect,const RectF* clipper = 0, bool clipToDisplay = false)LEEPURE;
	//用于绘制边框
	virtual void DrawWidgetlookFrame(const CLeeString& WidgetName,const CLeeString& elementStatus,
		const RectF& ControlRect,const RectF* clipper, bool clipToDisplay )LEEPURE;
	virtual void endPatch(void) LEEPURE;
	virtual void beginPatch(void) LEEPURE;
	virtual void clearScene(void) LEEPURE;
	virtual void beginScene(void) LEEPURE;
	virtual void endScene(void) LEEPURE;
	virtual void Present(void) LEEPURE;
	virtual void InitRender(HWND hWnd) LEEPURE;
	virtual void InitRenderEx(LPVOID d3dDevice  ) LEEPURE;
	virtual void ShutdownRender(void) LEEPURE;
	virtual bool DisplayReset(void) LEEPURE ;
	virtual bool EnableAlphaBlend(bool bEnableAlphaBlend)LEEPURE;
	virtual bool GetAlphaBlendState()LEEPURE;
	//线程池支持
	virtual void SetThreadPool(ILeeThreadpool * pPool) LEEPURE ;
};
