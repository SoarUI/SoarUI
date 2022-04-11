#ifndef _PluginAnyManamer_h_
#define _PluginAnyManamer_h_

/********************************************************************
    Descibe:      提供加载过程需要的加密/解密参数
    created:      2012/9/29 8:31
    FullName:     d:\测试项目\MYFeel\MYFeel\PluginAnyManager.h
    Path:         d:\测试项目\MYFeel\MYFeel
    ClassName:    PluginAnyManager
    FileType:     h
    Author:       nicklisir@163.com
    NeededLib:    
    requires:      
*********************************************************************/
#include "ProviderAny.h"
#include "../../SoarHeader/LeeString.h"
#include <map>
#include "../../SoarHeader/Singleton_t.h"
class PluginAnyManager:public LEE::Singleton_t<PluginAnyManager>
{
public:
	PluginAnyManager(void);
	virtual ~PluginAnyManager(void);
	void addAny(const CLeeString& name,const struct _ProviderKeyAny& provider);
	//资源组是否存在
	bool isAnyPresent(const CLeeString& plugName);
	_ProviderKeyAny* getAny(const CLeeString& plugName);
	void removeAny(const CLeeString& plugName);
	void clearAllAny(void);
protected:
	//插件名-Any
	typedef std::map<CLeeString,ProviderAny,CLeeString::FastLessCompare> ProviderAnyMap;
	ProviderAnyMap d_providerAnys;
};
#endif