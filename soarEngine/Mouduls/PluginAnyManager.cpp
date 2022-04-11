#include "PluginAnyManager.h"

PluginAnyManager::PluginAnyManager(void)
{
}

PluginAnyManager::~PluginAnyManager(void)
{
	clearAllAny();
}
void PluginAnyManager::addAny(const CLeeString& name,const struct _ProviderKeyAny& provider)
{
	if (name.empty())
	{
		return;
	}
	if (!isAnyPresent(name))
	{
		d_providerAnys[name] =provider;
	}
}
//资源组是否存在
bool PluginAnyManager::isAnyPresent(const CLeeString& plugName)
{
	return d_providerAnys.find(plugName)!=d_providerAnys.end();
}
_ProviderKeyAny* PluginAnyManager::getAny(const CLeeString& plugName)
{
	ProviderAnyMap::iterator it=d_providerAnys.find(plugName);
	if (it!=d_providerAnys.end())
	{
		return &(it->second);
	}
	return NULL;
}
void PluginAnyManager::removeAny(const CLeeString& plugName)
{
	ProviderAnyMap::iterator it=d_providerAnys.find(plugName);
	if (it!=d_providerAnys.end())
	{
		d_providerAnys.erase(it);
	}
}
void PluginAnyManager::clearAllAny(void)
{
	d_providerAnys.clear();
}