#ifndef _PluginAnyManamer_h_
#define _PluginAnyManamer_h_

/********************************************************************
    Descibe:      �ṩ���ع�����Ҫ�ļ���/���ܲ���
    created:      2012/9/29 8:31
    FullName:     d:\������Ŀ\MYFeel\MYFeel\PluginAnyManager.h
    Path:         d:\������Ŀ\MYFeel\MYFeel
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
	//��Դ���Ƿ����
	bool isAnyPresent(const CLeeString& plugName);
	_ProviderKeyAny* getAny(const CLeeString& plugName);
	void removeAny(const CLeeString& plugName);
	void clearAllAny(void);
protected:
	//�����-Any
	typedef std::map<CLeeString,ProviderAny,CLeeString::FastLessCompare> ProviderAnyMap;
	ProviderAnyMap d_providerAnys;
};
#endif