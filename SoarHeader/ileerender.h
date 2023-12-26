#pragma once
#include "ILeeTexture.h"
#include "RectF.h"
/*
*该系统专为Flydream开发，并为以后开发LeeDx系统作为先锋作用，并以此为基础开发新的3D引擎
*开发时间 2016.9.26 0：50   D3D11
*该系统目前为最小基本单元;开发该系统的起点事件是家里一台配置较好的台式使用的WIN7 64位，
*导致Flydream的绘制错误，所以决定写一个绘制驱动，
*从而不用受win的限制，实现更多的性能和坚实的画面。



*/
class ILeeRender
{
public:
	ILeeRender(void){}
	virtual ~ILeeRender(void){}
	//
	virtual LPCTSTR   getIdentityString(void) const =0;
	//
	virtual bool Initialize( HWND hwnd )=0;
	virtual bool InitializeEx(LPVOID d3dDevice,LPVOID DXGISwapChain)=0;
	virtual void Shutdown( void )=0;
	virtual void Release(void) =0;
	//批次
	virtual bool beginPatch(void)=0;
	virtual bool endPatch(void)=0;
	virtual bool clearScene(void)=0;
	virtual void beginScene(void)=0;
	virtual void endScene(void)=0;
	virtual void Present(void)=0;
	virtual bool DisplayReset(void)=0;
	virtual bool EnableAlphaBlend(bool bEnableAlphaBlend)=0;
	virtual bool GetAlphaBlendState()=0;
	//
	virtual void Render(const RectF & destRect,const RectF&texture_rect,
		ILeeTexture* texture,const PointF2D& PixelOffsetXY/*,IRenderCache*chache*/)=0;
	virtual void RenderText(const RectF & destRect,const PointF2D& PixelOffsetXY,
		const CLeeString& szText,int vertTextFmt,int horzTextFmt)=0;
	//virtual void DecodeCImg(IImgCodec* Coder,ILeeTexture** pTexture,const IRawDataContainer& data)=0;
	virtual bool SetTexture(CLeeString & textures,DWORD dwMaskARGB, DWORD dwFontARGB, bool usTransparent) =0;
	
};
