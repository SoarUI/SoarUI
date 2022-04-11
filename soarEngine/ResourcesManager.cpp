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
//������
void ResourcesManager::addResourceGroup(const ResourceGroup& group)
{
	d_grouplock.Enter();
	d_resourceGroups[group.groupname] =group;
	d_grouplock.Leave();
}
//��Դ���Ƿ����
bool ResourcesManager::resourceGroupExists(const CLeeString& name)
{
	CScopeSync Sync(d_grouplock);
	return d_resourceGroups.find(name) != d_resourceGroups.end();
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
/*�Ƴ���Դ�� ��Ҫ�����Ƿ����Դһ��ж�� */ 
void ResourcesManager::removeResourceGroup(const CLeeString& groupName )
{
	CScopeSync Sync(d_grouplock);
	//�������ǵļ�¼�����Ƿ������������
	ResourceGroupMap::iterator itor=d_resourceGroups.find(groupName);
	if (itor != d_resourceGroups.end())
	{
		//����Ŀǰ�Ƿ�����ʹ�ø���Դ�������������ʹ�ã�֪ͨ��Դ������ж������
		//����������һж��,�ȼ��ټ�������Ϊ0��֪ͨ������ж��
		ResourceDefineMap::iterator it;
		int uRef=0;
		//CLeeString tagetimageset;//������ԴImageset������
		it =d_resImageset.find(itor->second.tagetimageset);
		if (it !=d_resImageset.end())//�Ѿ����ظ���Դ
		{
			uRef =it->second.uRef;
			if (--uRef<=0)
			{
				//֪ͨ������ж��
				//
				d_resImageset.erase(it);
			}
		}
		
		//CLeeString targetTexture;//������ԴTexture������
		it =d_resTexture.find(itor->second.tagetimageset);
		if (it !=d_resTexture.end())//�Ѿ����ظ���Դ
		{
			uRef =it->second.uRef;
			if (--uRef<=0)
			{
				//֪ͨ������ж��
				//
				d_resTexture.erase(it);
			}
		}
		//CLeeString targetWidgets;//������ԴWidget������
		it =d_resWidgetlooks.find(itor->second.tagetimageset);
		if (it !=d_resWidgetlooks.end())//�Ѿ����ظ���Դ
		{
			uRef =it->second.uRef;
			if (--uRef<=0)
			{
				//֪ͨ������ж��
				//
				d_resWidgetlooks.erase(it);
			}
		}
		//��������Դ״̬
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
//���ӳ����״̬
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
//����״̬��������
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
//��Դ����ĳ�ļ��Ƿ����
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
//������Դ
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
//����ģ�������ļ�
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
//��ʼ����Դ�飬������ͳһЭ��������������Ĭ����
void ResourcesManager::initialiseResourceGroup(const CLeeString& groupname)
{
	ResourceGroupMap::iterator it =d_resourceGroups.find(groupname);
	if (it!=d_resourceGroups.end())
	{
		//��ʼ������ļ�
		WidgetThemeManager::getSingleton().setActiveTheme(d_thisActiveTheme);
		//����look
		WidgetLookManager::getSingletonPtr()->setActiveLook(it->second.targetWidgets);//����������ļ�����Ͷ�������ͬ
	}
	
}
//ж����Դ�飬��Ҫ���ǡ�����������
void ResourcesManager::unloadResourceGroup(const CLeeString& name, bool reloadableOnly )
{
	removeResourceGroup(name);
}
//������ʽ������Դ
void ResourcesManager::loadResourceGroup(const CLeeString& group, bool DestroyPrevGroup )
{
	//////////////////////////////////////////////////////////////////////////
	//��ʼ��������
	initialiseResourceGroup(group);
	//��������Դ
	//���¼���XML�ļ�
	ResourceGroupMap::iterator it =d_resourceGroups.find(group);
	if (it!=d_resourceGroups.end())//�����ȶ�����
	{
		int iNt=0;
		//�����ݲ��ܱ仯���ڴ��ڼ�
		d_grouplock.Enter();
		//����ͼ��
		iNt+=loadResImageset(it);
		//����texture
		iNt+=loadResTexture(it);
		//����widgetslook
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

//-----------����ӿ�------------------------------------------//
//������Դ��
void ResourcesManager::loadResourceTheme(const WidgetTheme& theme, bool DestroyPrevGroup )
{
	CLeeString gr =theme.getGroup();
	//����Դ�Ѿ�����,ֱ�����ã�������
	if (IsGroupLoad(gr))
	{
		
		//////////////////////////////////////////////////////////////////////////
		//ɾ����һ����,���Ѽ��������Ƴ�
		if (DestroyPrevGroup)
		{
			;//unloadResourceGroup()
		}
		//////////////////////////////////////////////////////////////////////////
		return ;
	}
	else//���������Դ
	{ 
		
		loadResourceGroup(gr);
	}
	//�鿴�����Texture�Ƿ��Ѿ�����
	if(!IsResTextureLoad(theme.getTexturePackage()))
	{
		ResourceGroupMap::iterator it =d_resourceGroups.find(findGroupContainingResource(theme.getTexturePackage()));
		loadResTexture(it);
	}
	//������ǰ����
	setDefaultTheme(theme.getName());
}
//������Դ����ĳһ�ļ�
//bool ResourcesManager::loadResource(const CLeeString& name, const CLeeString& groupName)
//{
	//////////////////////////////////////////////////////////////////////////
	//�����ļ��Ƿ��Ѿ����ػ����Ѿ�����
	//////////////////////////////////////////////////////////////////////////
	//��ʼ��������
	//initialiseResourceGroup(groupName);
	//����
	//d_grouplock.Enter();
	/*ResourceGroupMap::iterator it =d_resourceGroups.find(groupName);
	if (it!=d_resourceGroups.end())//�����ȶ�����
	{
		ResourceGroupMappingMap::iterator itor=d_resourceMapping.find(groupName);
		if (itor != d_resourceMapping.end())//���б���������Դ
		{
			ResourceDefineMap::iterator beg = itor->second.find(name);

			// loop through the number of entries there are for this author
			ResourceDefineMap::size_type  entries = itor->second.size();
			HANDLE hhand= NULL ;
			if (beg != itor->second.end())
			{
				if (beg->second.type ==RT_IMAGESET)
				{
					//�����Դʵ�������
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
					//�����Դʵ�������
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
					//�����Դʵ�������
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
				{//�����Դʵ�������
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
				//����ĳ�ļ�״̬
				setResourceStatus(RS_LOADED,groupName,name);
			}
		}
	}*/
	//d_grouplock.Leave();
	//return 0;
//}
//-------------------------��Դ���غ���-----------------------------------------------------//
//�첽������Դ����ĳһ�ļ�
//void ResourcesManager::loadResourceAsync(const CLeeString& name, const CLeeString& groupName)
//{
	//////////////////////////////////////////////////////////////////////////
	//�Ƿ��Ѿ������飺
	/*if (IsGroupLoad(groupName))
	{
		return ;
	}*/
	//////////////////////////////////////////////////////////////////////////
	//ResourceMainThread* pThr=new ResourceMainThread;
	//pThr->setParams(groupName,false,false,ResourceType::RT_UNKNOWN);
	//CThreadPoolEx::getSingleton().SubmitTaskAsync(pThr);
//}
//�첽����ĳһ������Դ
bool ResourcesManager::loadResourcebyType(const CLeeString& groupName,ResourceType type)
{
	//////////////////////////////////////////////////////////////////////////
	//�Ƿ��Ѿ����أ�
	if (IsGroupLoad(groupName))
	{
		return true;
	}
	//�п����Ѿ������˸���
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
	if (itor != d_resImageset.end() )//�ҵ�����Դ����
	{
		if(itor->second.status ==RS_LOADED)
		return true ;//֮ǰ�Ѿ�����
		//������Դ�����޸�״̬;�����ļ���
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
		{//������Դ״̬
			itor->second.status =RS_LOADED;
			return true ;
		}
		return true ;
	}
	//û�и���Դ���壬�϶�������
	return false;
}
bool ResourcesManager::loadResTexture(ResourceGroupMap::iterator & it)
{
	ResourceDefineMap::iterator itor =d_resTexture.find(it->second.targetTexture);
	if (itor != d_resTexture.end() )//�ҵ�����Դ����
	{
		if(itor->second.status ==RS_LOADED)
			return true ;//֮ǰ�Ѿ�����
		//������Դ�����޸�״̬;�����ļ���
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
		{//������Դ״̬
			itor->second.status =RS_LOADED;
		    return true ;
		}
	}
	//û�и���Դ���壬�϶�������
	return false;
}
bool ResourcesManager::loadResWidgetlook(ResourceGroupMap::iterator & it)
{
	ResourceDefineMap::iterator itor =d_resWidgetlooks.find(it->second.targetWidgets);
	if (itor != d_resWidgetlooks.end() )//�ҵ�����Դ����
	{
		if(itor->second.status ==RS_LOADED)
			return true ;//֮ǰ�Ѿ�����
		//������Դ�����޸�״̬;�����ļ���
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
		{//������Դ״̬
			itor->second.status =RS_LOADED;
			return true ;
		}
	}
	//û�и���Դ���壬�϶�������
	return false;
}
bool ResourcesManager::IsResImagesetLoad(const CLeeString & name)
{
	ResourceDefineMap::iterator itor =d_resImageset.find(name);
	if (itor != d_resImageset.end() )//�ҵ�����Դ����
	{
		if(itor->second.status ==RS_LOADED)
			return true ;//֮ǰ�Ѿ�����
	}
	return false;
}
bool ResourcesManager::IsResTextureLoad(const CLeeString & name)
{
	ResourceDefineMap::iterator itor =d_resTexture.find(name);
	if (itor != d_resTexture.end() )//�ҵ�����Դ����
	{
		if(itor->second.status ==RS_LOADED)
			return true ;//֮ǰ�Ѿ�����
	}
	return false;
}
bool ResourcesManager::IsResWidgetlookLoad(const CLeeString & name)
{
	ResourceDefineMap::iterator itor =d_resWidgetlooks.find(name);
	if (itor != d_resWidgetlooks.end() )//�ҵ�����Դ����
	{
		if(itor->second.status ==RS_LOADED)
			return true ;//֮ǰ�Ѿ�����
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
	if (plugsMgr&& !plugsMgr->isloaded( d_szblender))//��ȡ�趨��XML parser
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
	
	if (plugsMgr&& !plugsMgr->isloaded( d_szParser))//��ȡ�趨��XML parser
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