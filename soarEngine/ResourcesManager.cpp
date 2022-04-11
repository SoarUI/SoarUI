#include "ResourcesManager.h"
#include "Handler/Theme_xmlHandler.h"
#include "Handler/Texture_xmlHandler.h"
#include "Handler/widgetlook_xmlHandler.h"
#include "Theme/WidgetThemeManager.h"
#include "widgetslook/WidgetLookManager.h"
#include "ImagesetManager.h"
#include "Texture/TextureManager.h"
//#include "../DataDesc.h"
#include "Mouduls/parsers/ExpatParser.h"
#include "../SoarHeader/leelog.h"
#include "Mouduls/PluginManager.h"
#include "LeeDUIResource_XMLHandler.h"
ResourcesManager::ResourcesManager(HMODULE hmod):d_bLockForLoading(false),
d_paser(NULL),
d_blender(NULL),
thisThreadPool(NULL)
{
	d_dirs.setMoudle(hmod);
	d_WidgetThemeMgr =new WidgetThemeManager();
	d_ImagesetMgr= new ImagesetManager();
	d_TextureMgr=new TextureManager();
	d_WidgetLookMgr =new WidgetLookManager();
	d_PluginMgr =new CPluginManager();
}
ResourcesManager::~ResourcesManager(void)
{
	d_resourceGroups.clear();
	d_resImageset.clear();
	//texture
	d_resTexture.clear();
	//control
	d_resWidgetlooks.clear();
	///
	delete d_WidgetThemeMgr;
	delete d_ImagesetMgr;
	delete d_TextureMgr;
	delete d_WidgetLookMgr;
	delete d_PluginMgr;
	
}
ResourceType ResourcesManager::stringToResourceType(
	const CLeeString& type) const
{
	if (type == _T("Imageset") )
		return RT_IMAGESET;
	else if (type == _T("WidgetLook") )
		return RT_WIDGETLOOK;
	else if (type == _T("Theme") )
		return RT_THEME;
	else if (type == _T("Script"))
		return RT_SCRIPT;
	else if (type == _T("Mapp") )
		return RT_MAPP;
	else if(type == _T("Texture"))
		return RT_TEXTURE;
	else if(type == _T("Control"))
		return RT_CONTROL;
	else
		return RT_UNKNOWN;
}
//增加组
void ResourcesManager::addResourceGroup(const ResourceGroup& group)
{
	d_grouplock.Enter();
	d_resourceGroups[group.groupname] =group;
	d_grouplock.Leave();
}
//资源组是否存在
bool ResourcesManager::resourceGroupExists(const CLeeString& name)
{
	CScopeSync Sync(d_grouplock);
	return d_resourceGroups.find(name) != d_resourceGroups.end();
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
/*移除资源组 需要考虑是否把资源一起卸载 */ 
void ResourcesManager::removeResourceGroup(const CLeeString& groupName )
{
	CScopeSync Sync(d_grouplock);
	//查找我们的记录，看是否存在这样的组
	ResourceGroupMap::iterator itor=d_resourceGroups.find(groupName);
	if (itor != d_resourceGroups.end())
	{
		//查找目前是否正在使用该资源，如果不是正在使用，通知资源管理者卸载它：
		//根据类型逐一卸载,先减少计数，当为0，通知管理器卸载
		ResourceDefineMap::iterator it;
		int uRef=0;
		//CLeeString tagetimageset;//引用资源Imageset的名称
		it =d_resImageset.find(itor->second.tagetimageset);
		if (it !=d_resImageset.end())//已经加载该资源
		{
			uRef =it->second.uRef;
			if (--uRef<=0)
			{
				//通知管理器卸载
				//
				d_resImageset.erase(it);
			}
		}
		
		//CLeeString targetTexture;//引用资源Texture的名称
		it =d_resTexture.find(itor->second.tagetimageset);
		if (it !=d_resTexture.end())//已经加载该资源
		{
			uRef =it->second.uRef;
			if (--uRef<=0)
			{
				//通知管理器卸载
				//
				d_resTexture.erase(it);
			}
		}
		//CLeeString targetWidgets;//引用资源Widget的名称
		it =d_resWidgetlooks.find(itor->second.tagetimageset);
		if (it !=d_resWidgetlooks.end())//已经加载该资源
		{
			uRef =it->second.uRef;
			if (--uRef<=0)
			{
				//通知管理器卸载
				//
				d_resWidgetlooks.erase(it);
			}
		}
		//设置组资源状态
		ResourceGroupStatusMap::iterator itt =d_resGroupStatus.find(groupName);
		if (itt !=d_resGroupStatus.end())
		{
			itt->second.status =RS_UNLOAD;
		}
	}
}
/** Verify if a resource location exists for the given group. */ 
bool ResourcesManager::resourceLocationExists(const CLeeString& name, 
												  const CLeeString& resGroup,ResourceType restype)
{
	CScopeSync Sync(d_resourcemappinglock);
	
	return false;
}
bool ResourcesManager::resourceExistsInGroup(const CLeeString& name,const CLeeString &group,ResourceType restype)
{
	CScopeSync Sync(d_resourcemappinglock);
	ResourceGroupMap::iterator it =d_resourceGroups.find(group);
	if (it !=d_resourceGroups.end())
	{
		ResourceDefineMap::iterator itor ;
		switch(restype)
		{
		case RT_IMAGESET:
			{
				itor =d_resImageset.find(name);
				return itor !=d_resImageset.end();
			}
		case RT_TEXTURE:
			{
				itor =d_resTexture.find(name);
				return itor !=d_resTexture.end();
			}
		case RT_WIDGETLOOK:
			{
				itor =d_resWidgetlooks.find(name);
				return itor !=d_resWidgetlooks.end();
			}
		default:
			return false;
		}
	}
	return false;
}
CLeeString ResourcesManager::findGroupContainingResource(const CLeeString& name)
{
	CScopeSync Sync(d_resourcemappinglock);
	ResourceGroupMap::iterator it =d_resourceGroups.begin();
	while (it !=d_resourceGroups.end())
	{
		if (it->second.tagetimageset == name ||
			it->second.targetTexture==name ||
			it->second.targetWidgets == name)
		{
			return it->second.groupname;
		}
		++it;
	}
	return CLeeString(_T(""));
}

ResourcesManager::ResourceGroupMap ResourcesManager::getResourceGroups(void) 
{
	CScopeSync Sync(d_grouplock);
	return d_resourceGroups;
}
//////////////////////////////////////////////////////////////////////////
//检测映射组状态
bool ResourcesManager::IsGroupLoad(const CLeeString& group)
{
	CScopeSync Sync(d_resourcemappinglock);
	ResourceGroupStatusMap::iterator it =d_resGroupStatus.find(group);
	if (it !=d_resGroupStatus.end())
	{
		if (it->second.status ==ResourceStatus::RS_LOADED)
		{
			return true ;
		}
	}
	return false ;
}
//对组状态进行设置
void ResourcesManager::setResourceGroupStatus(ResourceStatus status,const CLeeString& stringGroup)
{
	d_grouplock.Enter();
	ResourcesManager::ResourceGroupStatusMap::iterator itor =d_resGroupStatus.find(stringGroup);
	if (itor == d_resGroupStatus.end())
	{
		return ;
	}
	d_grouplock.Leave();
	d_resourcemappinglock.Enter();
	itor->second.status =status ;
	d_resourcemappinglock.Leave();
}
void ResourcesManager::setResourceStatus(ResourceStatus status,const CLeeString& name,ResourceType restype)
{
	ResourceDefineMap::iterator itor ;
	switch(restype)
	{
	case RT_IMAGESET:
		{
			itor =d_resImageset.find(name);
			if(itor !=d_resImageset.end())
			{
				itor->second.status =status;
			}
			return;
		}
	case RT_TEXTURE:
		{
			itor =d_resTexture.find(name);
			if(itor !=d_resTexture.end())
			{
				itor->second.status =status;
			}
			return;
		}
	case RT_WIDGETLOOK:
		{
			itor =d_resWidgetlooks.find(name);
			if(itor !=d_resWidgetlooks.end())
			{
				itor->second.status =status;
			}
			return;
		}
	default:
		return ;
	}
}
//资源组中某文件是否加载
bool ResourcesManager::resourceIsLoaded(const CLeeString& resinGroup,ResourceType restype)
{
	switch(restype)
	{
	case RT_IMAGESET:
		{
			return IsResImagesetLoad(resinGroup) ;//
		}
	case RT_TEXTURE:
		{
			return IsResTextureLoad(resinGroup) ;//
		}
	case RT_WIDGETLOOK:
		{
			return IsResWidgetlookLoad(resinGroup) ;//
		}
	}
	return false ;
}
//////////////////////////////////////////////////////////////////////////
//声明资源
void ResourcesManager::declareResource(const CLeeString& name,const CLeeString& resourceType,
					 const CLeeString & filename)
{
	d_resourcemappinglock.Enter();
	ResourceDefine obj;
	obj.filename =filename;
	obj.name =name;
	obj.status=RS_UNINITIALSED;
	obj.uRef =1;
	ResourceType type =stringToResourceType(resourceType);
	ResourceDefineMap::iterator it ;
	switch(type)
	{
	case RT_IMAGESET:
		{
			it =d_resImageset.find(name);
			if (it != d_resImageset.end() )
			{
				it->second.uRef += 1;
				return ;
			}
			else
			{
				d_resImageset.insert(std::make_pair(name,obj));
				return ;
			}
		}
	case RT_TEXTURE:
		{
			it =d_resTexture.find(name);
			if (it != d_resTexture.end() )
			{
				it->second.uRef += 1;
				return ;
			}
			else
			{
				d_resTexture.insert(std::make_pair(name,obj));
				return ;
			}
		}
	case RT_WIDGETLOOK:
		{
			it =d_resWidgetlooks.find(name);
			if (it != d_resWidgetlooks.end() )
			{
				it->second.uRef += 1;
				return ;
			}
			else
			{
				d_resWidgetlooks.insert(std::make_pair(name,obj));
				return ;
			}
		}
	}
}
//

//
void ResourcesManager::setDefaultTheme(const CLeeString &thmedef)
{
	d_thisActiveTheme = thmedef;
}
void ResourcesManager::setThemeFile(const CLeeString &thmedef)
{
	d_themeFile =thmedef;
}
void ResourcesManager::autoloadThemeCfg(void)
{
	CLeeString final_filename =_T("./");
	final_filename =getDirectory(RT_DEFAULT);
	if (final_filename.substr(final_filename.length()-1,1)==_T("/"))
	{
		;
	}
	else if (final_filename.substr(final_filename.length()-1,1)==_T("\\"))
	{
		;
	}
	else
	{
		final_filename +=_T("/");
	}
	final_filename+=d_themeFile;
	WidgetThemeManager::getSingleton().parseFile(final_filename);
	
}
//加载模块配置文件
bool ResourcesManager::autoloadConfig(const CLeeString& cfgFile)
{
	CLeeDUIResource_XMLHandler handler;
	ILeeXmlParser* paser =ResourcesManager::getSingletonPtr()->getDefaultPaser();
	CLeeString final_filename =_T("./");
	final_filename =getDirectory(RT_DEFAULT);
	if (final_filename.substr(final_filename.length()-1,1)==_T("/"))
	{
		;
	}
	else if (final_filename.substr(final_filename.length()-1,1)==_T("\\"))
	{
		;
	}
	else
	{
		final_filename +=_T("/");
	}
	final_filename+=cfgFile;
	Dbgc_DumpX(_T("leeduires"),
		_T("===== loading resources cfg: start file %s ====="),final_filename.c_str());

	return paser->parseFile(handler,final_filename,0);
}
void ResourcesManager::autoloadTheme(void)
{
	WidgetTheme* theme =WidgetThemeManager::getSingletonPtr()->getWidgetTheme(d_thisActiveTheme);
	if (theme)
	{
		loadResourceTheme(*theme);
	}

}
//初始化资源组，在这里统一协调各个管理器的默认名
void ResourcesManager::initialiseResourceGroup(const CLeeString& groupname)
{
	ResourceGroupMap::iterator it =d_resourceGroups.find(groupname);
	if (it!=d_resourceGroups.end())
	{
		//初始化外观文件
		WidgetThemeManager::getSingleton().setActiveTheme(d_thisActiveTheme);
		//激活look
		WidgetLookManager::getSingletonPtr()->setActiveLook(it->second.targetWidgets);//这里决定了文件必须和定义名相同
	}
	
}
//卸载资源组，需要考虑。。。。。。
void ResourcesManager::unloadResourceGroup(const CLeeString& name, bool reloadableOnly )
{
	removeResourceGroup(name);
}
//以组形式加载资源
void ResourcesManager::loadResourceGroup(const CLeeString& group, bool DestroyPrevGroup )
{
	//////////////////////////////////////////////////////////////////////////
	//初始化该组先
	initialiseResourceGroup(group);
	//加载组资源
	//以下加载XML文件
	ResourceGroupMap::iterator it =d_resourceGroups.find(group);
	if (it!=d_resourceGroups.end())//必须先定义组
	{
		int iNt=0;
		//组内容不能变化，在此期间
		d_grouplock.Enter();
		//加载图像集
		iNt+=loadResImageset(it);
		//加载texture
		iNt+=loadResTexture(it);
		//加载widgetslook
		iNt+=loadResWidgetlook(it);
		d_grouplock.Leave();
		if (iNt ==3)
		{
			ResourceGroupStatusMap::iterator itor =d_resGroupStatus.find(group);
			if (itor != d_resGroupStatus.end())
			{
				itor->second.status =RS_LOADED;
			}
		}
	}
}

//------------------------------------------------------//

//-----------主题接口------------------------------------------//
//加载资源组
void ResourcesManager::loadResourceTheme(const WidgetTheme& theme, bool DestroyPrevGroup )
{
	CLeeString gr =theme.getGroup();
	//组资源已经加载,直接设置，并更新
	if (IsGroupLoad(gr))
	{
		
		//////////////////////////////////////////////////////////////////////////
		//删除上一主题,从已加载组中移除
		if (DestroyPrevGroup)
		{
			;//unloadResourceGroup()
		}
		//////////////////////////////////////////////////////////////////////////
		return ;
	}
	else//按组加载资源
	{ 
		
		loadResourceGroup(gr);
	}
	//查看所需的Texture是否已经加载
	if(!IsResTextureLoad(theme.getTexturePackage()))
	{
		ResourceGroupMap::iterator it =d_resourceGroups.find(findGroupContainingResource(theme.getTexturePackage()));
		loadResTexture(it);
	}
	//保存先前主题
	setDefaultTheme(theme.getName());
}
//加载资源组中某一文件
//bool ResourcesManager::loadResource(const CLeeString& name, const CLeeString& groupName)
//{
	//////////////////////////////////////////////////////////////////////////
	//检测该文件是否已经加载或组已经加载
	//////////////////////////////////////////////////////////////////////////
	//初始化该组先
	//initialiseResourceGroup(groupName);
	//加载
	//d_grouplock.Enter();
	/*ResourceGroupMap::iterator it =d_resourceGroups.find(groupName);
	if (it!=d_resourceGroups.end())//必须先定义组
	{
		ResourceGroupMappingMap::iterator itor=d_resourceMapping.find(groupName);
		if (itor != d_resourceMapping.end())//组中必须声明资源
		{
			ResourceDefineMap::iterator beg = itor->second.find(name);

			// loop through the number of entries there are for this author
			ResourceDefineMap::size_type  entries = itor->second.size();
			HANDLE hhand= NULL ;
			if (beg != itor->second.end())
			{
				if (beg->second.type ==RT_IMAGESET)
				{
					//检测资源实体存在性
					if (!resourceIsLoaded(groupName,beg->second.name))
					{
						ImagesetThread * pThread =new ImagesetThread;
						pThread->setParams(beg->second.filename, groupName,d_DefaultTheme,beg->second.name);
						hhand=CreateEvent(NULL,TRUE,false,NULL);
						pThread->setNotifyCompletEvent(hhand);
						CThreadPoolEx::getSingleton().SubmitTaskAsync(pThread);
					}
					
				}
				if (beg->second.type==RT_WIDGETLOOK)
				{
					//检测资源实体存在性
					if (!resourceIsLoaded(groupName,beg->second.name))
					{
						LookNFeelThread * pThread =new LookNFeelThread;
						pThread->setParams(beg->second.filename, groupName,d_DefaultTheme,beg->second.name);
						hhand=CreateEvent(NULL,TRUE,false,NULL);
						pThread->setNotifyCompletEvent(hhand);
						CThreadPoolEx::getSingleton().SubmitTaskAsync(pThread);
					}
					
				}
				if (beg->second.type==RT_CONTROL)
				{
					//检测资源实体存在性
					if (!resourceIsLoaded(groupName,beg->second.name))
					{
						ControlThread * pThread =new ControlThread;
						pThread->setParams(beg->second.filename, groupName,d_DefaultTheme,beg->second.name);
						hhand=CreateEvent(NULL,TRUE,false,NULL);
						pThread->setNotifyCompletEvent(hhand);
						CThreadPoolEx::getSingleton().SubmitTaskAsync(pThread);
					}
					
				}
				if (beg->second.type==RT_TEXTURE)
				{//检测资源实体存在性
					if (!resourceIsLoaded(groupName,beg->second.name))
					{
						TextureThread * pThread =new TextureThread;
						pThread->setParams(beg->second.filename, groupName,d_DefaultTheme,beg->second.name);
						hhand=CreateEvent(NULL,TRUE,false,NULL);
						pThread->setNotifyCompletEvent(hhand);
						CThreadPoolEx::getSingleton().SubmitTaskAsync(pThread);
					}
					
				}
				++beg;
			}
			if (hhand)
			{
				WaitForSingleObject(hhand,INFINITE);
				CloseHandle(hhand);
				//设置某文件状态
				setResourceStatus(RS_LOADED,groupName,name);
			}
		}
	}*/
	//d_grouplock.Leave();
	//return 0;
//}
//-------------------------资源加载函数-----------------------------------------------------//
//异步加载资源组中某一文件
//void ResourcesManager::loadResourceAsync(const CLeeString& name, const CLeeString& groupName)
//{
	//////////////////////////////////////////////////////////////////////////
	//是否已经加载组：
	/*if (IsGroupLoad(groupName))
	{
		return ;
	}*/
	//////////////////////////////////////////////////////////////////////////
	//ResourceMainThread* pThr=new ResourceMainThread;
	//pThr->setParams(groupName,false,false,ResourceType::RT_UNKNOWN);
	//CThreadPoolEx::getSingleton().SubmitTaskAsync(pThr);
//}
//异步加载某一类型资源
bool ResourcesManager::loadResourcebyType(const CLeeString& groupName,ResourceType type)
{
	//////////////////////////////////////////////////////////////////////////
	//是否已经加载：
	if (IsGroupLoad(groupName))
	{
		return true;
	}
	//有可能已经加载了个别
	ResourceGroupMap::iterator itor = d_resourceGroups.find(groupName);
	if (itor != d_resourceGroups.end())
	{
		switch(type)
		{
		case RT_IMAGESET:
			{
				return loadResImageset(itor) ;//
			}
		case RT_TEXTURE:
			{
				return loadResTexture(itor) ;//
			}
		case RT_WIDGETLOOK:
			{
				return loadResWidgetlook(itor) ;//
			}
		}
	}
	
	/////////////////////////////////////
	return false;
}
//----------------------------------------------------------------------------//

//texture

//----------------------------------------------------------------------------//
bool ResourcesManager::loadResImageset(ResourceGroupMap::iterator & it)
{
	ResourceDefineMap::iterator itor =d_resImageset.find(it->second.tagetimageset);
	if (itor != d_resImageset.end() )//找到该资源定义
	{
		if(itor->second.status ==RS_LOADED)
		return true ;//之前已经加载
		//加载资源，并修改状态;查找文件名
		CLeeString filename =itor->second.filename;
		CLeeString final_filename =_T("./");
		final_filename =getDirectory(RT_IMAGESET);
		if (final_filename.substr(final_filename.length()-1,1)==_T("/"))
		{
			;
		}
		else if (final_filename.substr(final_filename.length()-1,1)==_T("\\"))
		{
			;
		}
		else
		{
			final_filename +=_T("/");
		}
		
		final_filename+=filename;
		if( ImagesetManager::getSingletonPtr()->parseFile(final_filename) )
		{//更新资源状态
			itor->second.status =RS_LOADED;
			return true ;
		}
		return true ;
	}
	//没有该资源定义，肯定出错了
	return false;
}
bool ResourcesManager::loadResTexture(ResourceGroupMap::iterator & it)
{
	ResourceDefineMap::iterator itor =d_resTexture.find(it->second.targetTexture);
	if (itor != d_resTexture.end() )//找到该资源定义
	{
		if(itor->second.status ==RS_LOADED)
			return true ;//之前已经加载
		//加载资源，并修改状态;查找文件名
		CLeeString filename =itor->second.filename;
		CLeeString final_filename =_T("./");
		final_filename =getDirectory(RT_TEXTURE);
		if (final_filename.substr(final_filename.length()-1,1)==_T("/"))
		{
			;
		}
		else if (final_filename.substr(final_filename.length()-1,1)==_T("\\"))
		{
			;
		}
		else
		{
			final_filename +=_T("/");
		}

		final_filename+=filename;
		if (TextureManager::getSingletonPtr()->parseFile(final_filename) )
		{//更新资源状态
			itor->second.status =RS_LOADED;
		    return true ;
		}
	}
	//没有该资源定义，肯定出错了
	return false;
}
bool ResourcesManager::loadResWidgetlook(ResourceGroupMap::iterator & it)
{
	ResourceDefineMap::iterator itor =d_resWidgetlooks.find(it->second.targetWidgets);
	if (itor != d_resWidgetlooks.end() )//找到该资源定义
	{
		if(itor->second.status ==RS_LOADED)
			return true ;//之前已经加载
		//加载资源，并修改状态;查找文件名
		CLeeString filename =itor->second.filename;
		CLeeString final_filename =_T("./");
		final_filename =getDirectory(RT_WIDGETLOOK);
		if (final_filename.substr(final_filename.length()-1,1)==_T("/"))
		{
			;
		}
		else if (final_filename.substr(final_filename.length()-1,1)==_T("\\"))
		{
			;
		}
		else
		{
			final_filename +=_T("/");
		}

		final_filename+=filename;
		if( WidgetLookManager::getSingletonPtr()->parseFile(final_filename) )
		{//更新资源状态
			itor->second.status =RS_LOADED;
			return true ;
		}
	}
	//没有该资源定义，肯定出错了
	return false;
}
bool ResourcesManager::IsResImagesetLoad(const CLeeString & name)
{
	ResourceDefineMap::iterator itor =d_resImageset.find(name);
	if (itor != d_resImageset.end() )//找到该资源定义
	{
		if(itor->second.status ==RS_LOADED)
			return true ;//之前已经加载
	}
	return false;
}
bool ResourcesManager::IsResTextureLoad(const CLeeString & name)
{
	ResourceDefineMap::iterator itor =d_resTexture.find(name);
	if (itor != d_resTexture.end() )//找到该资源定义
	{
		if(itor->second.status ==RS_LOADED)
			return true ;//之前已经加载
	}
	return false;
}
bool ResourcesManager::IsResWidgetlookLoad(const CLeeString & name)
{
	ResourceDefineMap::iterator itor =d_resWidgetlooks.find(name);
	if (itor != d_resWidgetlooks.end() )//找到该资源定义
	{
		if(itor->second.status ==RS_LOADED)
			return true ;//之前已经加载
	}
	return false;
}

CLeeString ResourcesManager::getDirectory(ResourceType type)
{
		if (type==ResourceType::RT_IMAGESET)
		{
			return d_dirs.getImagesetDirectory();
		} 
		else if (type == ResourceType::RT_WIDGETLOOK)
		{
			return d_dirs.getWidgetlookDirectory();
		}
		else if (type ==ResourceType::RT_TEXTURE)
		{
			return d_dirs.getTextureDirectory();
		}
		else if (type ==ResourceType::RT_PLUGIN)
		{
			return d_dirs.getPluginDirectory();
		}
		else if (type==ResourceType::RT_CONTROL)
		{
			return d_dirs.getControlDirectory();
		}
		return d_dirs.getResourceDirectory();
}
void ResourcesManager::setDirectory(const CLeeString& dir,ResourceType type)
{
	if (type==ResourceType::RT_IMAGESET)
	{
		d_dirs.setImagesetDirectory(dir);
	} 
	else if (type == ResourceType::RT_WIDGETLOOK)
	{
		d_dirs.setWidgetlookDirectory(dir);
	}
	else if (type ==ResourceType::RT_TEXTURE)
	{
		d_dirs.setTextureDirectory(dir);
	}
	else if (type ==ResourceType::RT_PLUGIN)
	{
		d_dirs.setPluginDirectory(dir);
	}
	else if (type== ResourceType::RT_CONTROL)
	{
		d_dirs.setControlDirectory(dir);
	}
	else if (type == ResourceType::RT_DEFAULT)
	{
		d_dirs.setResourceDirectory(dir);
	}
}
//----------------------------------------
CLeeString& ResourcesManager::getXmldataProvider(void)
{
	return d_szdataProvider;
}
_ProviderKeyAny  ResourcesManager::getKeyAny(void)
{
	return d_keyAny ;
}
void ResourcesManager::SetKeyAny(const _ProviderKeyAny& key)
{
	d_keyAny =key;
}
ILeeXmlParser* ResourcesManager::getDefaultPaser(void)
{
	return d_paser;
}
ILeeRender* ResourcesManager::getBlender(void)
{
	CPluginManager* plugsMgr =CPluginManager::getSingletonPtr();
	if (d_blender !=NULL)
	{
		return d_blender;
	}
	if (plugsMgr&& !plugsMgr->isloaded( d_szblender))//获取设定的XML parser
	{
		CLeeString final_filename =_T("./");
		final_filename =getDirectory(RT_PLUGIN);
		if (final_filename.substr(final_filename.length()-1,1)==_T("/"))
		{
			;
		}
		else if (final_filename.substr(final_filename.length()-1,1)==_T("\\"))
		{
			;
		}
		else
		{
			final_filename +=_T("/");
		}
		final_filename+=d_szblender;
		plugsMgr->load(final_filename,d_szblender);
	}
	plugsMgr->CreatePlugin(d_szblender,(void**)&d_blender);
	return d_blender;
}
bool ResourcesManager::CloneXmlParser(ILeeXmlParser** pRet)
{
	CPluginManager* plugsMgr =CPluginManager::getSingletonPtr();
	
	if (plugsMgr&& !plugsMgr->isloaded( d_szParser))//获取设定的XML parser
	{
		CLeeString final_filename =_T("./");
		final_filename =getDirectory(RT_PLUGIN);
		if (final_filename.substr(final_filename.length()-1,1)==_T("/"))
		{
			;
		}
		else if (final_filename.substr(final_filename.length()-1,1)==_T("\\"))
		{
			;
		}
		else
		{
			final_filename +=_T("/");
		}
		final_filename+=d_szParser;
		plugsMgr->load(final_filename,d_szParser);
	}
	HRESULT hr=plugsMgr->CreatePlugin(d_szParser,(void**)pRet);
	return SUCCEEDED(hr);
}
void ResourcesManager::setXmldataProvider(const CLeeString& providername)
{
	d_szdataProvider = providername ;
}
void ResourcesManager::setImageCodec(const CLeeString& providername)
{
	d_szImgcodec = providername ;
}
void ResourcesManager::setDefaultPaser(ILeeXmlParser* parser)
{
	d_paser =parser;
}
void ResourcesManager::setBlender(const CLeeString& blender)
{
	d_szblender =blender;
}
void ResourcesManager::setXmlParser(const CLeeString& parser)
{
	d_szParser = parser;
}
CLeeString & ResourcesManager::getXmlParser(void)
{
	return d_szParser;
}
void ResourcesManager::SetThreadPool(ILeeThreadpool * pPool)
{
	thisThreadPool = pPool;
}
//----------------------------------------