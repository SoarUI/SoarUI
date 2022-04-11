#include "WidgetLookPackage.h"

CWidgetLookPackage::CWidgetLookPackage(void)
{
}
CWidgetLookPackage::CWidgetLookPackage(const CWidgetLookPackage& rhs)
{
	d_widgetLooks = rhs.getWidgetFeelLists();
	d_defaultResourceGroup = rhs.getDefaultResourceGroup();
	d_name =rhs.getName();
}
CWidgetLookPackage::~CWidgetLookPackage(void)
{
	clearAllWidgetLooks();
}
 CWidgetLookPackage::WidgetLookList  CWidgetLookPackage::getWidgetFeelLists(void) const
{
	return d_widgetLooks;
}
bool CWidgetLookPackage::isWidgetLookAvailable(const CLeeString& widget) 
{
	return d_widgetLooks.find(widget) != d_widgetLooks.end();
}
const CLeeString& CWidgetLookPackage::getName(void) const
{
	return d_name;
}
void  CWidgetLookPackage::setName(const CLeeString& name)
{
	d_name =name;
}
CWidgetLook* CWidgetLookPackage::getWidgetLook(const CLeeString& widget) 
{
	if (!isWidgetLookAvailable(widget))
	{
		return NULL;
	}
	WidgetLookList::iterator wlf = d_widgetLooks.find(widget);

	if (wlf != d_widgetLooks.end())
	{
		return&((*wlf).second);
	}
	return    NULL;
}
void CWidgetLookPackage::eraseWidgetLook(const CLeeString& widget)
{
	WidgetLookList::iterator wlf = d_widgetLooks.find(widget);
	if (wlf != d_widgetLooks.end())
	{
		d_widgetLooks.erase(wlf);
	}
}

void CWidgetLookPackage::addWidgetLook(  CWidgetLook& look)
{
	if (isWidgetLookAvailable(look.getName()))
	{
		return ; 
	}
	d_widgetLooks.insert(std::make_pair(look.getName(), look));
}
void CWidgetLookPackage::clearAllWidgetLooks(void)
{
	d_widgetLooks.clear();
}