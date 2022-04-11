#ifndef _PluginManager_h_
#define _PluginManager_h_
/********************************************************************
    Descibe:      插件管理器
    created:      2012/9/21 10:38
    FullName:     d:\测试项目\MYFeel\MYFeel\PluginManager.h
    Path:         d:\测试项目\MYFeel\MYFeel
    ClassName:    PluginManager
    FileType:     h
    Author:       nicklisir@163.com
    NeededLib:    
    requires:      
*********************************************************************/
#include <map>
#include "../../SoarHeader/Singleton_t.h"
#include "../../SoarHeader/SingletonSync.h"
#include "../../threadpool/leePluginMgr.h"
#include "PluginData.h"
class CPluginManager: public LEE::Singleton_t<CPluginManager>,public CLeePluginMgr
{
public:
	typedef std::map<CLeeString,PluginData,CLeeString::FastLessCompare> PLUGINDataLIST;
public:
	CPluginManager(void);
public:
	virtual ~CPluginManager(void);
	void definepugin(const CLeeString& plugName,const CLeeString& Constructor,const CLeeString& Destructor);
	void unload(const CLeeString& plugName) ;
	void undefineplugin(const CLeeString& plugName) ;
    void crearAllPlugins(void);
	//根据协议创建插件
	HRESULT CreatePlugin(const CLeeString& plugin,LPVOID* ppReturn);
	//方便函数
	std::string toAscII(const CLeeString& CLeeString);
private:
	PLUGINDataLIST d_datas;
};
#endif