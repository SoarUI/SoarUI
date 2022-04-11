#pragma once
/********************************************************************
    Descibe:      FlyDream资源管理器
    created:      2012/8/6 14:59
    FullName:     ResourcesManager.h
    Path:         
    ClassName:    ResourcesManager
    FileType:     h
    Author:       nicklisir
    NeededLib:    
    requires:      
*********************************************************************/
#include "../SoarHeader/Singleton_t.h"
#include "../SoarHeader/scopeSync.h"
//#include "../datacontainer.h"
#include "ResourceDirectory.h"
#include "Theme/WidgetTheme.h"
#include <map>
#include<vector>
#include <set>
#include "../SoarHeader/CcriticalSection.h"
#include "../SoarHeader/ProviderAny.h"
#include "../SoarHeader/IResourceProvider.h"
#include "../XML/ILeeXmlParser.h"
#include "../threadpool/ILeeThreadpool.h"
#include "../SoarHeader/ILeeRender.h"
#include "Theme/WidgetThemeManager.h"
#include "ImagesetManager.h"
#include "texture/TextureManager.h"
#include "widgetslook/WidgetLookManager.h"
#include "Mouduls/PluginManager.h"
/********************************************************************
    Descibe:      组是资源的基本单位，组中包含所有相关资源
    created:      2012/10/16 12:18
    FullName:     \ResourcesManager.h
    Path:         
    ClassName:    ResourcesManager
    FileType:     h
    Author:       
    NeededLib:    
    requires:      
*********************************************************************/
enum ResourceType
{
	RT_UNKNOWN=-1,
	RT_IMAGESET,
	RT_WIDGETLOOK,
	RT_SCRIPT,
	RT_THEME,
	RT_MAPP,
	RT_TEXTURE,
	RT_CONTROL,
	RT_DEFAULT,
	RT_PLUGIN,
	RT_BLENDER
};
enum ProviderType
{
	PVRT_UNKNOWN=-1,
	PVRT_CODEC,
	PVRT_TEXTURE
};
enum ResourceStatus
{
	RS_UNINITIALSED = 0,
	RS_INITIALISING = 1,
	RS_INITIALISED = 2,
	RS_LOADING = 3,
	RS_LOADED = 4,
	RS_UNLOAD =5,
	RS_USING =6,
};
//资源定义
struct ResourceGroup
{
	CLeeString groupname;//组名
	CLeeString tagetimageset;//引用资源Imageset的名称
	CLeeString targetTexture;//引用资源Texture的名称
	CLeeString targetWidgets;//引用资源Widget的名称
};
//
struct ResourceDefine
{
	CLeeString name;//名称
	CLeeString filename;//相关文件名
	ResourceStatus status;//资源状态
	int uRef;//引用计数
};
//资源状态
struct ResourceGroupStatus 
{
	CLeeString groupname;//组名
	ResourceStatus status;//组资源状态
};
class ResourcesManager:public LEE::Singleton_t<ResourcesManager>
{
public:
	ResourcesManager(HMODULE hmod);
	
	//资源组,以组作为索引，方便按组加载
	typedef std::map<CLeeString,ResourceGroup,CLeeString::FastLessCompare> ResourceGroupMap;
	//name-define 资源定义《在FlyDream.cfg中定义》
	typedef std::map<CLeeString,ResourceDefine,CLeeString::FastLessCompare> ResourceDefineMap;
	//group-define 组-》相关资源定义【资源映射】
	typedef std::map<CLeeString,ResourceGroupStatus,CLeeString::FastLessCompare > ResourceGroupStatusMap;
public:
	virtual ~ResourcesManager(void);
	//初始化资源组
	void initialiseResourceGroup(const CLeeString& groupname);
	//加载资源组
	void loadResourceGroup(const CLeeString& group, bool DestroyPrevGroup = true);
	//加载资源组
	void loadResourceTheme(const WidgetTheme& theme, bool DestroyPrevGroup = true);
	//卸载资源组
	void unloadResourceGroup(const CLeeString& name, bool reloadableOnly = true);
	//加载资源组中某一类型
	bool loadResource(const CLeeString& name, ResourceType type);
	//异步加载某一类型文件
	void loadResourceAsync(const CLeeString& name, ResourceType type);
	//异步加载某组某一类型资源
	bool loadResourcebyType(const CLeeString& group,ResourceType type);
	//卸载未被使用的资源
	void unloadUnreferencedResourcesInGroup(const CLeeString& name, 
		bool reloadableOnly = true);
	//清理资源组
	void clearResourceGroup(const CLeeString& name);
	//销毁资源组
	void destroyResourceGroup(const CLeeString& name);
	//资源组是否已经初始化
	bool isResourceGroupInitialised(const CLeeString& name);
	//资源组是否已经加载
	bool isResourceGroupLoaded(const CLeeString& name);
	//资源组是否存在
	bool resourceGroupExists(const CLeeString& name);
	//资源组中某文件是否加载
	bool resourceIsLoaded(const CLeeString& resinGroup,ResourceType restype);
	//增加路径资源组
	void addResourceGroup(const ResourceGroup& group);
	/** Removes a resource location from the search path. */ 
	void removeResourceGroup(const CLeeString& name );
	/** Verify if a resource location exists for the given group. */ 
	bool resourceLocationExists(const CLeeString& name, 
		const CLeeString& resGroup,ResourceType restype);
    //声明资源
	void declareResource(const CLeeString& name,const CLeeString& resourceType,
		const CLeeString & filename);
	void undeclareResource(const CLeeString& name, const CLeeString& groupName);
	//某类资源名称是否存在于某组中
	bool resourceExistsInGroup(const CLeeString& resname,const CLeeString &group,ResourceType restype);
	//查找某组是否包含有该名字的资源
    CLeeString findGroupContainingResource(const CLeeString& resname);
	//2012-8-15
	ResourcesManager::ResourceGroupMap getResourceGroups(void) ;
	//-----资源状态---------------------
	//对组进行设置
	void setResourceGroupStatus(ResourceStatus status,const CLeeString& stringGroup);
	void setResourceStatus(ResourceStatus status,const CLeeString& name,ResourceType restype);
	//用于外部调用
	bool IsGroupLoad(const CLeeString& group);
//-----------------------------资源路径-----------------------------------------------------------
	CLeeString getDirectory(ResourceType type);
	void setDirectory(const CLeeString& dir,ResourceType type);
	//-----------------------------------------------------------------------------------//
	//加载主题文件
	void autoloadTheme(void);
	//
	void setDefaultTheme(const CLeeString &thmedef);
	void setThemeFile(const CLeeString &thmedef);
	//加载模块配置文件
	void autoloadThemeCfg(void);
	bool autoloadConfig(const CLeeString& cfgFile);
	//----------------------------------------
	void setXmldataProvider(const CLeeString &providername);
	void setImageCodec(const CLeeString& providername);
	CLeeString& getXmldataProvider(void);
	_ProviderKeyAny  getKeyAny(void);
	void SetKeyAny(const _ProviderKeyAny& key);
	ILeeXmlParser* getDefaultPaser(void);
	void setDefaultPaser(ILeeXmlParser* parser);
	void setBlender(const CLeeString& blender);
	//
	void setXmlParser(const CLeeString& parser);
	CLeeString & getXmlParser(void);
	ILeeRender* getBlender(void);
	bool CloneXmlParser(ILeeXmlParser** pRet);
	void SetThreadPool(ILeeThreadpool * pPool);
	ILeeThreadpool* GetThreadPool(void) {return thisThreadPool ;}
	//----------------------------------------
	//字符串到资源类型转换.
	ResourceType stringToResourceType(const CLeeString& type) const;
protected:
	bool loadResImageset(ResourceGroupMap::iterator & it);
	bool loadResTexture(ResourceGroupMap::iterator & it);
	bool loadResWidgetlook(ResourceGroupMap::iterator & it);
	
	//
	bool IsResImagesetLoad(const CLeeString & name);
	bool IsResTextureLoad(const CLeeString & name);
	bool IsResWidgetlookLoad(const CLeeString & name);

	
protected:
	//资源组,
	ResourceGroupMap  d_resourceGroups;//flydream所定义的组
	ResourceGroupStatusMap d_resGroupStatus;//组状态
	//文件加载
	//////////////////////////////////////////////////////////////////////////
	//主题
	CLeeString d_thisActiveTheme;
	CLeeString d_thisActiveTexture;
	CLeeString d_themeFile;
	bool    d_bLockForLoading;
	CcriticalSection d_grouplock;//组锁
	CcriticalSection d_resourcemappinglock;//资源锁
	CcriticalSection d_grouploadinglock;//组跟踪锁
	CcriticalSection d_grouploadedlock;//组加载完成所
	//分类资源记录
	//主题
	//图像集
	ResourceDefineMap d_resImageset;
	//texture
    ResourceDefineMap d_resTexture;
	//widgetlook
	ResourceDefineMap d_resWidgetlooks;
	
	////////////////////////////////////////////////////////////////////////
	//资源路径
	CResourceDirectory d_dirs;//
	//资源文件数据提供者
	CLeeString d_szdataProvider;
	CLeeString d_szImgcodec;
	CLeeString d_szblender;
	CLeeString d_szParser;
	////--------------?|
	ILeeXmlParser* d_paser;//XML解析器
	_ProviderKeyAny d_keyAny ;
	ILeeRender * d_blender;//渲染器
	//线程池
	ILeeThreadpool *thisThreadPool;
	HMODULE d_hmod;
	//////////////////////////////////////////////////////////////////////////
	WidgetThemeManager* d_WidgetThemeMgr;
	ImagesetManager  * d_ImagesetMgr;
	TextureManager  * d_TextureMgr;
	WidgetLookManager * d_WidgetLookMgr;
	CPluginManager * d_PluginMgr;
};
