#ifndef _PluginManager_h_
#define _PluginManager_h_
/********************************************************************
    Descibe:      ���������
    created:      2012/9/21 10:38
    FullName:     d:\������Ŀ\MYFeel\MYFeel\PluginManager.h
    Path:         d:\������Ŀ\MYFeel\MYFeel
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
	//����Э�鴴�����
	HRESULT CreatePlugin(const CLeeString& plugin,LPVOID* ppReturn);
	//���㺯��
	std::string toAscII(const CLeeString& CLeeString);
private:
	PLUGINDataLIST d_datas;
};
#endif