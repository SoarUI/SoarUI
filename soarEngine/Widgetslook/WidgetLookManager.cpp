
#include "WidgetLookManager.h"
#include "../Handler/widgetlook_xmlHandler.h"
#include "../XMLHandler/XMLAnalyser.h"
#include "../Theme/WidgetThemeManager.h"
#include "../Propertys/Property.h"
#include "../Propertys/PropertyHelper.h"
#include "../../SoarHeader/leeLog.h"
#include "../Mouduls/parsers/ExpatParser.h"
#include "../ResourcesManager.h"
#include "../threads/WidgetlookThread.h"
/*
2012-10-15 解决锁的问题，仅保留添加，删除锁
*/
    ////////////////////////////////////////////////////////////////////////////////

    WidgetLookManager::WidgetLookManager()
    {
     
    }
    WidgetLookManager::~ WidgetLookManager()
    {
		clearAllWidgetPackages();
		
    }
    bool WidgetLookManager::isWidgetLookAvailable(const CLeeString& widget) 
	{
			 CScopeSync Sync(d_feellock);
			 return d_WidgetLookPackages.find(widget) != d_WidgetLookPackages.end();
    }

    CWidgetLook* WidgetLookManager::getWidgetLook(const CLeeString& lookname,const CLeeString& widget) 
    {
		d_feellock.Enter();
        WidgetFeelList::iterator wlf = d_WidgetLookPackages.find(lookname);

        if (wlf != d_WidgetLookPackages.end())
        {
			d_feellock.Leave();
            return (*wlf).second.getWidgetLook(widget);
        }
		d_feellock.Leave();
        return NULL;
    }
	CWidgetLook* WidgetLookManager::getWidgetLook(const CLeeString& widget) 
	{
		d_feellock.Enter();
		WidgetFeelList::iterator wlf = d_WidgetLookPackages.find(d_thisActiveLook);

		if (wlf != d_WidgetLookPackages.end())
		{
			d_feellock.Leave();
			return (*wlf).second.getWidgetLook(widget);
		}
		d_feellock.Leave();
		return NULL;
	}
    void WidgetLookManager::eraseWidgetPackage(const CLeeString& widget)
    {
	     CScopeSync Sync(d_feellock);
		//检测是否有主题链接它，是的话，返回
        WidgetFeelList::iterator wlf = d_WidgetLookPackages.find(widget);
        if (wlf != d_WidgetLookPackages.end())
        {
			if (wlf->second.getRef()>0)
			{
				return ;
			}
            d_WidgetLookPackages.erase(wlf);
        }
     
    }

    void WidgetLookManager::addWidgetPackage(  CWidgetLookPackage& lookpack)
    {
		d_feellock.Enter();
        if (isWidgetPackagePresent(lookpack.getName()))
        {
			d_feellock.Leave();
           return ;
        }
		d_WidgetLookPackages.insert(std::make_pair(lookpack.getName(), lookpack));
		d_feellock.Leave();
    }
	void WidgetLookManager::clearAllWidgetPackages(void)
	{
		d_feellock.Enter();
		d_WidgetLookPackages.clear();
		d_feellock.Leave();
	}
	bool WidgetLookManager::isWidgetPackagePresent(const CLeeString& widget) 
	{
		CScopeSync Sync(d_feellock);
		return d_WidgetLookPackages.find(widget) != d_WidgetLookPackages.end();
	}
	bool WidgetLookManager::parseFile(const CLeeString& filename )
	{
		if (filename.empty())
		{
			throw(-1);
			return false ;
		}
		//CWidgetlook_xmlHandler hadler;
		try
		{
			//IXMLAnalyser * Analyser =new ExpatParser();
			//if( Analyser->parseXMLFile(hadler,filename) )
			//{
			//	delete Analyser;
			WidgetlookThread lthread;
			lthread.startThread(filename);
			return true ;
			//}
		}
		catch (...)
		{
			throw(-1);
		}
		return false ;
	}