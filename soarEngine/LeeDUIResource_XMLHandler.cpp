#include "LeeDUIResource_XMLHandler.h"
const CLeeString CLeeDUIResource_XMLHandler::tagResourceDefine =_T("ResourceDefine");
const CLeeString CLeeDUIResource_XMLHandler::tagResourceGroup =_T("ResourceGroup");
const CLeeString CLeeDUIResource_XMLHandler::tagResImageSet =_T("ResImageset");
const CLeeString CLeeDUIResource_XMLHandler::tagWidgetlook =_T("ResWidgetLook");
const CLeeString CLeeDUIResource_XMLHandler::tagResTexture =_T("ResTexture");
const CLeeString CLeeDUIResource_XMLHandler::tagResControl =_T("ResControl");

const CLeeString CLeeDUIResource_XMLHandler::attrName =_T("name");
const CLeeString CLeeDUIResource_XMLHandler::attrType =_T("type");
const CLeeString CLeeDUIResource_XMLHandler::attrfile =_T("file");

//ÊôÐÔ
const CLeeString attrName;
const CLeeString attrType;
CLeeDUIResource_XMLHandler::CLeeDUIResource_XMLHandler(void):d_group(NULL)
{
	registerElementStartHandler(tagResourceDefine, &CLeeDUIResource_XMLHandler::tagResourceDefineStart);
	registerElementStartHandler(tagResourceGroup, &CLeeDUIResource_XMLHandler::tagResourceGroupStart);
	registerElementStartHandler(tagResImageSet, &CLeeDUIResource_XMLHandler::tagResImageSetStart);
	registerElementStartHandler(tagWidgetlook, &CLeeDUIResource_XMLHandler::tagResWidgetlookStart);
	registerElementStartHandler(tagResTexture, &CLeeDUIResource_XMLHandler::tagResTextureStart);
	registerElementStartHandler(tagResControl, &CLeeDUIResource_XMLHandler::tagResControlStart);

	//
	registerElementEndHandler(tagResourceGroup, &CLeeDUIResource_XMLHandler::tagResourceGroupEnd);
}

CLeeDUIResource_XMLHandler::~CLeeDUIResource_XMLHandler(void)
{
	delete d_group;
}
void CLeeDUIResource_XMLHandler::elementStart(const CLeeString& element, const XMLAttributes& attributes)
{
	// find registered handler for this element.
	ElementStartHandlerMap::const_iterator iter = d_startHandlersMap.find(element);

	// if a handler existed
	if (iter != d_startHandlersMap.end())
	{
		// call the handler for this element
		(this->*(iter->second))(attributes);
	}
}
void CLeeDUIResource_XMLHandler::elementEnd(const CLeeString& element)
{
	// find registered handler for this element.
	ElementEndHandlerMap::const_iterator iter = d_endHandlersMap.find(element);

	// if a handler existed
	if (iter != d_endHandlersMap.end())
		// call the handler for this element
		(this->*(iter->second))();
}
void CLeeDUIResource_XMLHandler::registerElementStartHandler(const CLeeString& element, ElementStartHandler handler)
{
	d_startHandlersMap[element] = handler;
}
void CLeeDUIResource_XMLHandler::registerElementEndHandler(const CLeeString& element, ElementEndHandler handler)
{
	d_endHandlersMap[element] = handler;
}
void CLeeDUIResource_XMLHandler::tagResourceDefineStart(const XMLAttributes& attributes)
{
	ResourcesManager::getSingletonPtr()->declareResource(attributes.getValueAsString(attrName),attributes.getValueAsString(attrType),attributes.getValueAsString(attrfile));
}
void CLeeDUIResource_XMLHandler::tagResourceGroupStart(const XMLAttributes& attributes)
{
	d_group =new ResourceGroup;
	d_group->groupname =attributes.getValueAsString(attrName);
}
void CLeeDUIResource_XMLHandler::tagResImageSetStart(const XMLAttributes& attr)
{
	if (d_group)
	{
		d_group->tagetimageset =attr.getValueAsString(attrName);
	}
}
void CLeeDUIResource_XMLHandler::tagResWidgetlookStart(const XMLAttributes& attr)
{
	if (d_group)
	{
		d_group->targetWidgets =attr.getValueAsString(attrName);
	}
}
void CLeeDUIResource_XMLHandler::tagResTextureStart(const XMLAttributes& attrs)
{
	if (d_group)
	{
		d_group->targetTexture =attrs.getValueAsString(attrName);
	}
}
void CLeeDUIResource_XMLHandler::tagResControlStart(const XMLAttributes& attr)
{
	/*if (d_group)
	{
		d_group->targetControls =attr.getValueAsString(attrName);
	}*/
}
void CLeeDUIResource_XMLHandler::tagResourceGroupEnd()
{
	if (d_group)
	{
		ResourcesManager::getSingletonPtr()->addResourceGroup(*d_group);
		delete d_group;
		d_group =NULL;
	}
}