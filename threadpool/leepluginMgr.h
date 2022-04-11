#ifndef LEE_CLeePluginMgr_H__
#define LEE_CLeePluginMgr_H__
/*
负责整个生命周期插件管理，插件管理包括文件注册
*/
#include "../SoarHeader/leeString.h"
#include "leeDynamicLink.h"
#include <map>
struct PluginPacket 
{
	CLeeString sName;
	GUID guID;
};
class CLeePluginMgr
{
public:
	CLeePluginMgr(void);
public:
	virtual ~CLeePluginMgr(void);
	//通过名字获取插件句柄
	CLeeDynamicLink* getPlug(const CLeeString & name);
	void* getFunction(const CLeeString & name,LPCSTR func);
	CLeeDynamicLink* load(const CLeeString & pathname,const CLeeString & name);
	void unload(const CLeeString & name);
	//资源组是否存在
	bool isloaded(const CLeeString& plugName) const;
	void clearAllPlugins(void);
private:
	CLeePluginMgr(const CLeePluginMgr & rhs );
protected:
	//从文件读取插件
	//插件名-Any
	typedef std::map<CLeeString,CLeeDynamicLink*,CLeeString::FastLessCompare> DynamicLinkMap;
	DynamicLinkMap d_plugins;
};
#endif