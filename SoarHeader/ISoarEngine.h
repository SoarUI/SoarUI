#pragma once
#include "leemacro.h"
#include "leestring.h"
#include "../XML\ILeeXmlParser.h"
#include "ProviderAny.h"
#include "../threadpool\ILeeThreadpool.h"
#include "RectF.h"
class IControlEngine;
/*
LooksEngine ��Ҫ����Ui��Դ�ļ�����ж��,����SOARϵͳ����Ŀؼ���״̬�ļ���
*/
class ISoarEngine
{
public:

	ISoarEngine(void){}
	virtual ~ISoarEngine(void){}
public:
	//����widgetlook�ļ�·��
	virtual void SetWidgetLookDirection(const CLeeString resPath) LEEPURE ;
	//���ò��·��
	virtual void SetPluginDirection(const CLeeString plugPath) LEEPURE ;
	//ͼ��·��
	virtual void SetImageSetDirection(const CLeeString plugPath)  LEEPURE ;
	//����ʵ��ͼ���ļ�·��
	virtual void SetTextureDirection(const CLeeString resPath)  LEEPURE ;
	//����Ĭ����Դ·��
	virtual void SetResourceDirection(const CLeeString resPath)  LEEPURE ;
	///�������
	//XML������
	virtual void SetXmlDataProvider(const CLeeString& provider , const CLeeString& Constructor,const CLeeString& Destructor) LEEPURE ;
	//XML������
	virtual void SetXmlParser(const CLeeString & parser, const CLeeString& Constructor,const CLeeString& Destructor) LEEPURE ;
	//��Ⱦ��
	virtual void SetDefaultBlender(const CLeeString& blender , const CLeeString& Constructor,const CLeeString& Destructor) LEEPURE ;
	//ͼ�������
	virtual void SetDefaultImgCodec(const CLeeString & ImgCodec , const CLeeString& Constructor,const CLeeString& Destructor) LEEPURE ;
	//�ⲿ������
	virtual void SetDefaultPaser(ILeeXmlParser * paser) LEEPURE ;
	//����XML������
	virtual bool CloneXmlParser(ILeeXmlParser** pRet) LEEPURE;
	//�⵼������
	virtual bool engine_loadtheme(const CLeeString& themeName) LEEPURE ;//�������⣬������ʵ����һ��ͼƬ
	virtual bool engine_SetDefaultTheme(const CLeeString& themeFile,const CLeeString& defTheme) LEEPURE ;//ֻ����һ��
	virtual bool engine_defineResources(const CLeeString& resFile) LEEPURE ;//��Դ����
	//����Ĭ����Դ
	virtual bool engine_loadDefaultTheme(void) LEEPURE ;
	//ж����Դ
	virtual bool engine_destroytheme(const CLeeString& name) LEEPURE ;
	//��������Դ
	virtual bool engine_loadGroupRes(const CLeeString& group) LEEPURE ;
	////������Դ
	virtual bool engine_loadResources(const CLeeString& resName) LEEPURE ;
	//�������ݽӿ�
	virtual void SetKeyAny(const _ProviderKeyAny& key) LEEPURE ;
	//�ӿڶ���
	virtual bool hasWidgetlook(const CLeeString& widget) LEEPURE ;
	//---ͨ��ģ�飺��һ�������Ʊ��������ڶ������������֣�
	virtual void DrawWidgetlook(const CLeeString& elementName,const CLeeString& elementStatus,
		const RectF& ControlRect,const CLeeString& text,int vertTextFmt,int horzTextFmt,const RectF* clipper = 0, bool clipToDisplay = false)LEEPURE;
	//--���ڻ���û�����ֵ�widget:ͼ��򱳾���
	virtual void DrawWidgetlookImage(const CLeeString& WidgetName,const CLeeString& elementStatus,
		const RectF& ControlRect,const RectF* clipper = 0, bool clipToDisplay = false)LEEPURE;
	//���ڻ��Ʊ߿�
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
	//�̳߳�֧��
	virtual void SetThreadPool(ILeeThreadpool * pPool) LEEPURE ;
};
