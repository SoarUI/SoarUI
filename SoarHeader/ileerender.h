#pragma once
#include "ILeeTexture.h"
#include "RectF.h"
/*
*��ϵͳרΪFlydream��������Ϊ�Ժ󿪷�LeeDxϵͳ��Ϊ�ȷ����ã����Դ�Ϊ���������µ�3D����
*����ʱ�� 2016.9.26 0��50   D3D11
*��ϵͳĿǰΪ��С������Ԫ;������ϵͳ������¼��Ǽ���һ̨���ýϺõ�̨ʽʹ�õ�WIN7 64λ��
*����Flydream�Ļ��ƴ������Ծ���дһ������������
*�Ӷ�������win�����ƣ�ʵ�ָ�������ܺͼ�ʵ�Ļ��档



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
	//����
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
