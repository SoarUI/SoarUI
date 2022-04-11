#include "WidgetThemeManager.h"
#include "../Handler/Theme_xmlHandler.h"
#include "../Mouduls/parsers/ExpatParser.h"
WidgetThemeManager::WidgetThemeManager(void)
{

}
WidgetThemeManager::~WidgetThemeManager(void)
{
	clearAllTheme();
}
void WidgetThemeManager::parseFile(const CLeeString& filename )
{
	if (filename.empty())
	{
		throw(-1);
		return ;
	}
	Theme_xmlHandler hadler;
	try
	{
		IXMLAnalyser * Analyser =new ExpatParser();
		Analyser->parseXMLFile(hadler,filename);
		delete Analyser;
	}
	catch (...)
	{
		throw(-1);
	}
	
}
void WidgetThemeManager::clearAllTheme(void)
{
	CScopeSync Sync(d_themelock);
	d_widgetThemes.clear();
}
bool WidgetThemeManager::isThemeAvailable(const CLeeString& Theme) 
{
	CScopeSync Sync(d_themelock);
	 return d_widgetThemes.find(Theme) != d_widgetThemes.end();
}
WidgetTheme* WidgetThemeManager::getWidgetTheme(const CLeeString& Theme) 
{
	CScopeSync Sync(d_themelock);
	WidgetThemeList::iterator wlf = d_widgetThemes.find(Theme);
	if (wlf != d_widgetThemes.end())
	{
		return &(wlf->second);
	}
	else
	{
	  return NULL;
	}

}
void WidgetThemeManager::eraseWidgetTheme(const CLeeString& Theme)
{
	CScopeSync Sync(d_themelock);
	WidgetThemeList::iterator wlf = d_widgetThemes.find(Theme);
	if (wlf != d_widgetThemes.end())
	{
		d_widgetThemes.erase(wlf);
	}
}
void WidgetThemeManager::addWidgetTheme(const WidgetTheme& Theme)
{
	if (isThemeAvailable(Theme.getName()))
	{
		return ; 
	}
	d_themelock.Enter();
	d_widgetThemes[Theme.getName()] =Theme;
	d_themelock.Leave();
	
}
WidgetThemeManager::WidgetThemeList WidgetThemeManager::getWidgetThemes(void )
{
	CScopeSync Sync(d_themelock);
	return d_widgetThemes;
}
void WidgetThemeManager::setActiveTheme(const CLeeString& theme)
{ 
	d_activeTheme = theme; 
}
CLeeString& WidgetThemeManager::getActiveTheme(void)
{
	return d_activeTheme;
}

//允许混合模式图片不存在，但主题必须存在
CLeeString WidgetThemeManager::getTexturePackage(void)
{
	return getWidgetTheme(d_activeTheme)->getTexturePackage();
}