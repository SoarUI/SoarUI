#include "Theme_xmlHandler.h"
#include "../Theme/WidgetThemeManager.h"
#include "../../SoarHeader/leeLog.h"

// xml tag names
const CLeeString Theme_xmlHandler::UIThemeElement = _T("UITheme");
const CLeeString Theme_xmlHandler::ThemeElement = _T("Theme");
const CLeeString Theme_xmlHandler::ResourceGroupElement = _T("resourceGroup");
const CLeeString Theme_xmlHandler::SmallLookElement = _T("smalllook");
const CLeeString Theme_xmlHandler::HelpElement = _T("helpstring");
const CLeeString Theme_xmlHandler::TextureElement = _T("Texture");

// xml attribute names
const CLeeString Theme_xmlHandler::FilenameAttribute = _T("filename");
const CLeeString Theme_xmlHandler::NameAttribute = _T("name");
const CLeeString Theme_xmlHandler::TextureAttribute = _T("texture");
const CLeeString Theme_xmlHandler::StringAttribute = _T("string");
const CLeeString Theme_xmlHandler::GroupAttribute = _T("group");
const CLeeString Theme_xmlHandler::ThemeAttribute = _T("theme");
const CLeeString Theme_xmlHandler::TypeAttribute = _T("type") ;
Theme_xmlHandler::Theme_xmlHandler(void)
{
	;
}
Theme_xmlHandler::~Theme_xmlHandler(void)
{
	;
}
void Theme_xmlHandler::elementStart(const CLeeString& element,
                                     const XMLAttributes& attributes)
{
    if (element == UIThemeElement)
        handleUIThemeElement(attributes);
    else if (element == ThemeElement)
        handleThemeElement(attributes);
    else if (element == ResourceGroupElement)
        handleResourceGroupElement(attributes);
    else if (element == SmallLookElement)
        handleSmalllookElement(attributes);
    else if (element == HelpElement)
        handleHelpstringElement(attributes);
	else if(element == TextureElement)
		handleTextureElement(attributes);
	Dbgc_DumpX(_T("leeduires"),
		_T("===== loading resources theme: tag:%s ====="),element.c_str());
   
}

//----------------------------------------------------------------------------//
void Theme_xmlHandler::elementEnd(const CLeeString& element)
{
    if (element == UIThemeElement)
		handleUIThemeElementEnd();
	else if (element == ThemeElement)
	{
		handleThemeElementEnd();
	}
	
       
}
void Theme_xmlHandler::handleUIThemeElement(const XMLAttributes& attr)
{
	;
}
void Theme_xmlHandler::handleThemeElement(const XMLAttributes& attr)
{
  d_ThemeString=attr.getValueAsString(NameAttribute, _T("") );
}
void Theme_xmlHandler::handleResourceGroupElement(const XMLAttributes& attr)
{
 d_Groupname=attr.getValueAsString(GroupAttribute, _T("") );
}
void Theme_xmlHandler::handleSmalllookElement(const XMLAttributes& attr)
{
	d_SmallFileName=attr.getValueAsString(TextureAttribute, _T("") );
}
void Theme_xmlHandler::handleHelpstringElement(const XMLAttributes& attr)
{
	d_Helpstring=attr.getValueAsString(StringAttribute, _T("") );
}
void Theme_xmlHandler::handleThemeElementEnd(void)
{
  WidgetTheme theme =WidgetTheme(d_ThemeString,d_Groupname,d_SmallFileName,d_Tex,d_Helpstring);
  WidgetThemeManager * ThemeMgr =WidgetThemeManager::getSingletonPtr();
  if (ThemeMgr)
  {
	  Dbgc_DumpX(_T("leeduires"),
		  _T("===== loading resources theme: add theme:<%s> ====="),theme.getName().c_str());
	  ThemeMgr->addWidgetTheme(theme);
  }

}
void Theme_xmlHandler::handleUIThemeElementEnd(void)
{
	;
}
void Theme_xmlHandler::handleTextureElement(const XMLAttributes & attrs)
{
	d_Tex =attrs.getValueAsString(NameAttribute,_T(""));
}
