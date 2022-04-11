#include "PluginManager.h"

CPluginManager::CPluginManager(void)
{
}

CPluginManager::~CPluginManager(void)
{
	crearAllPlugins();
}
void CPluginManager::definepugin(const CLeeString& plugName,const CLeeString& Constructor,const CLeeString& Destructor)
{
	PLUGINDataLIST::iterator it =d_datas.find(plugName);
	if (it!=d_datas.end())
	{
	}
	else
	{
		PluginData tmp;
		tmp.d_name =plugName ;
		tmp.d_constructor =Constructor;
		tmp.d_destructor =Destructor;
		d_datas.insert(std::make_pair(plugName,tmp));
	}
}
void CPluginManager::undefineplugin(const CLeeString& plugName)
{
	PLUGINDataLIST::iterator it =d_datas.find(plugName);
	if (it != d_datas.end())
	{
		d_datas.erase(it);
	}
}
void CPluginManager::unload(const CLeeString& plugName)
{
	CLeePluginMgr::unload(plugName);
}

 void CPluginManager::crearAllPlugins(void)
 {
	 PLUGINDataLIST::iterator it =d_datas.begin();
	 while (it != d_datas.end())
	 {
		 d_datas.erase(it);
		 it =d_datas.begin();
	 }
	 CLeePluginMgr::clearAllPlugins();
 }
 std::string CPluginManager::toAscII(const CLeeString& CLeeString)
 {
#if defined(_UNICODE) || defined(UNICODE)
	 int nlen=WideCharToMultiByte(CP_ACP,0,CLeeString.c_str(),-1,0,0);
	 char *wBuf =new char[(nlen+1)*sizeof(char)];
	 WideCharToMultiByte(CP_ACP,0,CLeeString.c_str(),-1,wBuf,nlen);
	 if (!wBuf)
	 {
		 return std::string("");
	 }
	 std::string tmp =wBuf;
	 delete[] wBuf;
	 return tmp;
#else
	 return CLeeString.c_str();
#endif
 }
 HRESULT CPluginManager::CreatePlugin(const CLeeString& plugin,LPVOID* ppReturn)
 {
	 if (ppReturn ==NULL)
	 {
		 return E_FAIL;
	 }
	 *ppReturn =NULL;
	 CLeeDynamicLink * mod=getPlug(plugin);
	 if (NULL==mod)
	 {
		 return E_FAIL;
	 }
	 PLUGINDataLIST::iterator it =d_datas.find(plugin);
	 if (it != d_datas.end())
	 {
		 // get pointer to parser creation function
		 HRESULT (*createFunc)(void**) =
			 (HRESULT (*)(LPVOID*))mod->getFunction(toAscII(it->second.d_constructor).c_str());
		 if (NULL==createFunc)
		 {
			 return E_FAIL;
		 }
		 // create the parser object
		 return createFunc((void**)ppReturn);
	 }
	return E_FAIL;
 }