#include "leePluginMgr.h"

CLeePluginMgr::CLeePluginMgr(void)
{
}

CLeePluginMgr::~CLeePluginMgr(void)
{
	clearAllPlugins();
}
CLeeDynamicLink* CLeePluginMgr::getPlug(const CLeeString & plugName)
{
	DynamicLinkMap::const_iterator it =d_plugins.find(plugName);
	if (it!=d_plugins.end())
	{
		return it->second;
	}
	return NULL;
}
void* CLeePluginMgr::getFunction(const CLeeString & name,LPCSTR func)
{
	CLeeDynamicLink* tmp =getPlug(name);
	if (NULL==tmp)
	{
		return NULL ;
	}
	return tmp->getFunction(func);
}
CLeeDynamicLink* CLeePluginMgr::load(const CLeeString & pathname,const CLeeString & plugName)
{
	if (plugName.empty())
	{
		return NULL;
	}
	//已经加载，直接返回
	if (isloaded(plugName))
	{
		return getPlug(plugName);
	}
	CLeeDynamicLink* tmp=new CLeeDynamicLink();
	void* h=tmp->load(pathname,plugName);
	if (h != NULL)
	{
		d_plugins.insert(std::make_pair(plugName,tmp));
		return tmp;
	}
	delete tmp;
	tmp =NULL;
	return tmp;
}
void CLeePluginMgr::unload(const CLeeString & plugName)
{
	DynamicLinkMap::iterator it =d_plugins.find(plugName);
	if (it!=d_plugins.end())
	{
		(it->second)->unload();
		delete it->second;
		d_plugins.erase(it);
	}
}
bool CLeePluginMgr::isloaded(const CLeeString& plugName) const
{
	return d_plugins.find(plugName)!=d_plugins.end();
}
void CLeePluginMgr::clearAllPlugins(void)
{
	DynamicLinkMap::iterator it =d_plugins.begin();
	while ( it !=d_plugins.end())
	{
		(it->second)->unload();
		delete it->second;
		d_plugins.erase(it);
		it =d_plugins.begin();
	}
}