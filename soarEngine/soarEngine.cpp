#include "SoarEngine.h"
#include "Theme\WidgetTheme.h"
#include "Theme\WidgetThemeManager.h"
#include "Mouduls\PluginManager.h"
#include "widgetslook\WidgetLookManager.h"
#include "../SoarHeader/RectF.h"
#include "texture\TextureManager.h"

CSoarEngine::CSoarEngine(HMODULE hmod)
{
	d_reSMgr =new ResourcesManager(hmod);
}

CSoarEngine::~CSoarEngine(void)
{
	delete d_reSMgr;
}
void CSoarEngine::SetWidgetLookDirection(const CLeeString resPath)  
{
	ResourcesManager::getSingletonPtr()->setDirectory(resPath,RT_WIDGETLOOK);
}
void CSoarEngine::SetPluginDirection(const CLeeString plugPath)  
{
	ResourcesManager::getSingletonPtr()->setDirectory(plugPath,RT_PLUGIN);
}
void CSoarEngine::SetImageSetDirection(const CLeeString plugPath)  
{
	ResourcesManager::getSingletonPtr()->setDirectory(plugPath,RT_IMAGESET);
}
void CSoarEngine::SetTextureDirection(const CLeeString plugPath)  
{
	ResourcesManager::getSingletonPtr()->setDirectory(plugPath,RT_TEXTURE);
}
void CSoarEngine::SetResourceDirection(const CLeeString plugPath)  
{
	ResourcesManager::getSingletonPtr()->setDirectory(plugPath,RT_DEFAULT);
}
void CSoarEngine::SetXmlDataProvider(const CLeeString &provider, const CLeeString& Constructor,const CLeeString& Destructor)
{
	CPluginManager::getSingletonPtr()->definepugin(provider,Constructor,Destructor);
	ResourcesManager::getSingletonPtr()->setXmldataProvider(provider);
}
void CSoarEngine::SetDefaultBlender(const CLeeString &blender,const CLeeString& Constructor,const CLeeString& Destructor) 
{
	CPluginManager::getSingletonPtr()->definepugin(blender,Constructor,Destructor);
	ResourcesManager::getSingletonPtr()->setBlender(blender);
}
void CSoarEngine::SetDefaultPaser(ILeeXmlParser * paser)  
{
	ResourcesManager::getSingletonPtr()->setDefaultPaser(paser);
}
bool CSoarEngine::CloneXmlParser(ILeeXmlParser** pRet)
{
	return ResourcesManager::getSingletonPtr()->CloneXmlParser(pRet);
}
void CSoarEngine::SetDefaultImgCodec(const CLeeString&  ImgCodec, const CLeeString& Constructor,const CLeeString& Destructor) 
{
	CPluginManager::getSingletonPtr()->definepugin(ImgCodec,Constructor,Destructor);
	ResourcesManager::getSingletonPtr()->setImageCodec(ImgCodec);
}
void CSoarEngine::SetXmlParser(const CLeeString& parser , const CLeeString& Constructor,const CLeeString& Destructor)
{
	CPluginManager::getSingletonPtr()->definepugin(parser,Constructor,Destructor);
	ResourcesManager::getSingletonPtr()->setXmlParser(parser);
}
//�л����⣬����ΪĬ�ϣ�Ϊ���ؽ��Զ�����
bool CSoarEngine::engine_loadtheme(const CLeeString& themeName) //�������⣬������ʵ����һ��ͼƬ
{
	WidgetTheme* theme =WidgetThemeManager::getSingletonPtr()->getWidgetTheme(themeName);
	if (theme)
	{
		ResourcesManager::getSingletonPtr()->loadResourceTheme(*theme);
		ResourcesManager::getSingletonPtr()->initialiseResourceGroup(theme->getGroup());
		//��Ⱦ��Ԥ�ȼ���TEXTURE���Ա�����
		ILeeRender* blender =ResourcesManager::getSingletonPtr()->getBlender();
		if (blender)
		{   
			CLeeString str =WidgetThemeManager::getSingletonPtr()->getTexturePackage();
			DWORD transparentRGBAColor =TextureManager::getSingletonPtr()->getTransparentRGBA(str);
			CLeeString textures =TextureManager::getSingletonPtr()->getTexture(str);
			bool bUsMark =TextureManager::getSingletonPtr()->getUseMask(str);
			blender->SetTexture(textures,transparentRGBAColor,bUsMark);
		}
		return true;
	}
	return false ;
}
bool CSoarEngine::engine_SetDefaultTheme(const CLeeString& themeFile,const CLeeString& defTheme)//ֻ����һ��
{
	
	ResourcesManager::getSingletonPtr()->setDefaultTheme(defTheme);
	ResourcesManager::getSingletonPtr()->setThemeFile(themeFile);
	ResourcesManager::getSingletonPtr()->autoloadThemeCfg();
	return true ;
}
bool CSoarEngine::engine_defineResources(const CLeeString& resFile) //��Դ����
{
	return ResourcesManager::getSingletonPtr()->autoloadConfig(resFile);
}
bool CSoarEngine::engine_loadDefaultTheme(void)
{
	ResourcesManager::getSingletonPtr()->autoloadTheme();
	//��Ⱦ��Ԥ�ȼ���TEXTURE���Ա�����
	ILeeRender* blender =ResourcesManager::getSingletonPtr()->getBlender();
	if (blender)
	{
		CLeeString str =WidgetThemeManager::getSingletonPtr()->getTexturePackage();
		DWORD transparentRGBAColor =TextureManager::getSingletonPtr()->getTransparentRGBA(str);
		CLeeString textures =TextureManager::getSingletonPtr()->getTexture(str);
		bool bUsMark =TextureManager::getSingletonPtr()->getUseMask(str);
		blender->SetTexture(textures,transparentRGBAColor,bUsMark);
	}
	return true;
}
bool CSoarEngine::engine_destroytheme(const CLeeString& name) 
{
	return true ;
}
bool CSoarEngine::engine_loadGroupRes(const CLeeString& group) 
{
	return true ;
}
bool CSoarEngine::engine_loadResources(const CLeeString& resName)
{
	return true;
}
void CSoarEngine::SetKeyAny(const _ProviderKeyAny& key)
{
	ResourcesManager::getSingletonPtr()->SetKeyAny(key);
}
//////////////////////////////////////////////////////////////////////////
bool  CSoarEngine::hasWidgetlook(const CLeeString& widget)
{
	return WidgetLookManager::getSingletonPtr()->getWidgetLook(widget)!=NULL ;
}
//---ͨ��ģ�飺��һ�������Ʊ��������ڶ������������֣�
void CSoarEngine::DrawWidgetlook(const CLeeString& WidgetName,const CLeeString& elementStatus,const RectF& ControlRect,const CLeeString& text,int vertTextFmt,int horzTextFmt,const RectF* clipper , bool clipToDisplay)
{
	PointF2D pt(0,0);
	WidgetLookManager::getSingletonPtr()->getWidgetLook(WidgetName)->RenderWidget(elementStatus,ControlRect,pt,text,vertTextFmt,horzTextFmt,clipper,clipToDisplay);
}
void CSoarEngine::DrawWidgetlookFrame(const CLeeString& WidgetName,const CLeeString& elementStatus,const RectF& ControlRect,const RectF* clipper , bool clipToDisplay)
{
	PointF2D pt(0,0);
	WidgetLookManager::getSingletonPtr()->getWidgetLook(WidgetName)->RenderWidget_Frame(elementStatus,ControlRect,pt,clipper,clipToDisplay);
}
//--���ڻ���û�����ֵ�widget:ͼ��򱳾���
void CSoarEngine::DrawWidgetlookImage(const CLeeString& WidgetName,const CLeeString& elementStatus,const RectF& ControlRect,const RectF* clipper , bool clipToDisplay)
{
	PointF2D pt(0,0);
	WidgetLookManager::getSingletonPtr()->getWidgetLook(WidgetName)->RenderWidget_Image(elementStatus,ControlRect,pt,clipper,clipToDisplay);
}
void CSoarEngine::clearScene(void)
{
	ResourcesManager::getSingletonPtr()->getBlender()->clearScene();
}
void CSoarEngine::beginScene(void){
	ResourcesManager::getSingletonPtr()->getBlender()->beginScene();
}
void CSoarEngine::endScene(void){
	ResourcesManager::getSingletonPtr()->getBlender()->endScene();
}
void CSoarEngine::beginPatch(void)
{
	ResourcesManager::getSingletonPtr()->getBlender()->beginPatch();
}
void CSoarEngine::endPatch()
{
	ResourcesManager::getSingletonPtr()->getBlender()->endPatch();
}
void CSoarEngine::Present(void) 
{
	ResourcesManager::getSingletonPtr()->getBlender()->Present();
}
void CSoarEngine::InitRender(HWND hWnd)
{
	ResourcesManager::getSingletonPtr()->getBlender()->Initialize(hWnd);
}
void CSoarEngine::InitRenderEx( LPVOID d3dDevice, LPVOID DXGISwapChain)
{
	ResourcesManager::getSingletonPtr()->getBlender()->InitializeEx(d3dDevice ,DXGISwapChain);
}
void CSoarEngine::ShutdownRender(void)
{
	ResourcesManager::getSingletonPtr()->getBlender()->Shutdown();
	ResourcesManager::getSingletonPtr()->getBlender()->Release();
}
bool CSoarEngine::DisplayReset(void)
{
	return ResourcesManager::getSingletonPtr()->getBlender()->DisplayReset();
}
bool CSoarEngine::EnableAlphaBlend(bool bEnableAlphaBlend){
	return ResourcesManager::getSingletonPtr()->getBlender()->EnableAlphaBlend(bEnableAlphaBlend);
}
bool CSoarEngine::GetAlphaBlendState(){
	return ResourcesManager::getSingletonPtr()->getBlender()->GetAlphaBlendState();
}
//�̳߳�֧��
void CSoarEngine::SetThreadPool(ILeeThreadpool * pPool)
{
	ResourcesManager::getSingletonPtr()->SetThreadPool(pPool);
}
void CSoarEngine::Release(void)
{
	delete this;
}