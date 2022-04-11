
#include "ExpatParser.h"
#include "../../../SoarHeader/IResourceProvider.h"
#include "../../Propertys/PropertyHelper.h"
#include "../../DataContainer.h"
#include "../../Mouduls/PluginManager.h"
//ProviderAnyMgr
/*
Update:  2012/08/21 10:29:18
Content: 与主系统分离，优化效率,增加type类型确定资源路径
从最佳配置来说，不应该所有的资源都使用同一中类型粘贴器，使用不同的粘贴器，从而去除type参数并减少依赖度
Person : nicklisir@163.com
*/
#include "../../../SoarHeader/leelog.h"
#include "../../ResourcesManager.h"
#include "../../../XML/ILeeXmlParser.h"
ExpatParser::ExpatParser(void)
{
    // set ID string
//    d_identifierString = _T("LeeDUI::ExpatParser - Official expat based parser module for LeeDUI");
}

ExpatParser::~ExpatParser(void)
{
}

bool ExpatParser::parseXMLFile(IXMLHandler& handler, const CLeeString& filename)
{
    //事件与解析引擎-----------------------------
	/*
	从插件库中获取XML解析器，
	当从组中无法获取解析器，提供默认解析库
	*/
   ILeeXmlParser *parser = NULL ; 
   CLeeString& parsername =ResourcesManager::getSingletonPtr()->getXmlParser();
   CLeeString final_filename =_T("./");
   CLeeString final_filename1 =_T("./");
   final_filename =ResourcesManager::getSingletonPtr()->getDirectory(RT_PLUGIN);
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
   final_filename1 =final_filename ;
   final_filename+=parsername;
    CPluginManager* plugsMgr =CPluginManager::getSingletonPtr();
	if (plugsMgr&& !plugsMgr->isloaded( parsername))//获取设定的XML parser
	{
		plugsMgr->load(final_filename,parsername);
	}
	plugsMgr->CreatePlugin(parsername,(void**)&parser);
	if (parser ==NULL)
	{
		parser = ResourcesManager::getSingletonPtr()->getDefaultPaser() ; 
	}
   //从主入口获取paser
   Dbgc_DumpX(_T("leeduires"),
	   _T("===== loading resources: file:%s ====="),filename.c_str());
 
   if (! parser)//error
   {
	   throw(-1);
	   return false;
   }
   IResourceProvider* lpResourceProvider =NULL;
   CLeeString& szProvider =ResourcesManager::getSingletonPtr()->getXmldataProvider();
   final_filename1 +=szProvider;
   if (plugsMgr&& !plugsMgr->isloaded( szProvider))//获取设定的XML parser
   {
	   plugsMgr->load(final_filename1,szProvider);
   }
   plugsMgr->CreatePlugin(szProvider,(void**)&lpResourceProvider);
   if (lpResourceProvider ==NULL)
   {
	   return false; 
   }
	//事件注册-----------------------------------------------
    RawDataContainer rawXMLData;
	/*
	实现自主加载，RT_IMAGESET RT_FEEL RT_TEXTURE[不用加密]
	在此实现路径获取，组成绝对文件路径
	*/
	//----------资源提供者，可以根据用户提供的提供者调用相应提供器
	/*
	1.从组中获取相应的插件名称
	2.从插件管理器中获取模块
	3.从密钥管理器中获取密钥
	*/
	_ProviderKeyAny keyAny =ResourcesManager::getSingletonPtr()->getKeyAny();
	//加载数据，有可能是加密的
	if(lpResourceProvider)
	{
		//获取文件大小，循环获取
		DWORD dwThunSize=lpResourceProvider->getThunkSize();
		size_t totalsize =0;
		size_t cursize =0;
		rawXMLData.setData(::malloc(dwThunSize));
		rawXMLData.setSize(dwThunSize);
		lpResourceProvider->cleardataState();
		do 
		{
			::ZeroMemory(rawXMLData.getDataPtr(),dwThunSize);
			lpResourceProvider->loadRawDataContainer(filename, rawXMLData,totalsize,&keyAny);
			cursize +=rawXMLData.getSize();
			//加载一段解析一段
			if ( ! parser->parseData(handler,reinterpret_cast<void*>(rawXMLData.getDataPtr()), rawXMLData.getSize()) )
			{
				break;
			}
		} while (cursize< totalsize);
		::free(rawXMLData.getDataPtr());
		lpResourceProvider->Release();
	} 
	//plugsMgr->unload(parsername);
	//plugsMgr->unload(szProvider);
	return true ;
}