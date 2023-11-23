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
	//�⹦��
	virtual bool engine_loadtheme(const CLeeString& themeName) ;//�������⣬������ʵ����һ��ͼƬ
	virtual bool engine_SetDefaultTheme(const CLeeString& themeFile,const CLeeString& defTheme) ;//ֻ����һ��
	virtual bool engine_defineResources(const CLeeString& resFile) ;//��Դ����
	virtual bool engine_loadDefaultTheme(void) ;
	virtual bool engine_destroytheme(const CLeeString& name) ;
	virtual bool engine_loadGroupRes(const CLeeString& group) ;//��������Դ
	virtual bool engine_loadResources(const CLeeString& resName) ;//������Դ
	virtual void SetKeyAny(const _ProviderKeyAny& key) ;
	//�ӿڶ���
	virtual bool hasWidgetlook(const CLeeString& widget) ;
	//---ͨ��ģ�飺��һ�������Ʊ��������ڶ������������֣�
	virtual void DrawWidgetlook(const CLeeString& elementName,const CLeeString& elementStatus,
		const RectF& ControlRect,const CLeeString& text,int vertTextFmt,int horzTextFmt,const RectF* clipper = 0, bool clipToDisplay = false) ;
	//--���ڻ���û�����ֵ�widget:ͼ��򱳾���
	virtual void DrawWidgetlookImage(const CLeeString& WidgetName,const CLeeString& elementStatus,
		const RectF& ControlRect,const RectF* clipper = 0, bool clipToDisplay = false) ;
	//���ڻ��Ʊ߿�
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
	//�̳߳�֧��
	virtual void SetThreadPool(ILeeThreadpool * pPool);
	virtual void Release(void);
private:
	ResourcesManager * d_reSMgr;//��Դ������
};
