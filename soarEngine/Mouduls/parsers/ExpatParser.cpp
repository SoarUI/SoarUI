
#include "ExpatParser.h"
#include "../../../SoarHeader/IResourceProvider.h"
#include "../../Propertys/PropertyHelper.h"
#include "../../DataContainer.h"
#include "../../Mouduls/PluginManager.h"
//ProviderAnyMgr
/*
Update:  2012/08/21 10:29:18
Content: ����ϵͳ���룬�Ż�Ч��,����type����ȷ����Դ·��
�����������˵����Ӧ�����е���Դ��ʹ��ͬһ������ճ������ʹ�ò�ͬ��ճ�������Ӷ�ȥ��type����������������
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
    //�¼����������-----------------------------
	/*
	�Ӳ�����л�ȡXML��������
	���������޷���ȡ���������ṩĬ�Ͻ�����
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
	if (plugsMgr&& !plugsMgr->isloaded( parsername))//��ȡ�趨��XML parser
	{
		plugsMgr->load(final_filename,parsername);
	}
	plugsMgr->CreatePlugin(parsername,(void**)&parser);
	if (parser ==NULL)
	{
		parser = ResourcesManager::getSingletonPtr()->getDefaultPaser() ; 
	}
   //������ڻ�ȡpaser
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
   if (plugsMgr&& !plugsMgr->isloaded( szProvider))//��ȡ�趨��XML parser
   {
	   plugsMgr->load(final_filename1,szProvider);
   }
   plugsMgr->CreatePlugin(szProvider,(void**)&lpResourceProvider);
   if (lpResourceProvider ==NULL)
   {
	   return false; 
   }
	//�¼�ע��-----------------------------------------------
    RawDataContainer rawXMLData;
	/*
	ʵ���������أ�RT_IMAGESET RT_FEEL RT_TEXTURE[���ü���]
	�ڴ�ʵ��·����ȡ����ɾ����ļ�·��
	*/
	//----------��Դ�ṩ�ߣ����Ը����û��ṩ���ṩ�ߵ�����Ӧ�ṩ��
	/*
	1.�����л�ȡ��Ӧ�Ĳ������
	2.�Ӳ���������л�ȡģ��
	3.����Կ�������л�ȡ��Կ
	*/
	_ProviderKeyAny keyAny =ResourcesManager::getSingletonPtr()->getKeyAny();
	//�������ݣ��п����Ǽ��ܵ�
	if(lpResourceProvider)
	{
		//��ȡ�ļ���С��ѭ����ȡ
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
			//����һ�ν���һ��
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