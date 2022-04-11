#pragma once
#include "../XML\ixmlhandler.h"
#include "ResourcesManager.h"

class CLeeDUIResource_XMLHandler :
	public IXMLHandler
{
public:
	CLeeDUIResource_XMLHandler(void);
public:
	virtual ~CLeeDUIResource_XMLHandler(void);
	void elementStart(const CLeeString& element, const XMLAttributes& attributes);
	void elementEnd(const CLeeString& element);
	void text(const CLeeString& text){}
	//
	typedef void (CLeeDUIResource_XMLHandler::*ElementStartHandler)(const XMLAttributes& attributes);
	typedef void (CLeeDUIResource_XMLHandler::*ElementEndHandler)();
	typedef std::map<CLeeString, ElementStartHandler, CLeeString::FastLessCompare> ElementStartHandlerMap;
	typedef std::map<CLeeString, ElementEndHandler, CLeeString::FastLessCompare> ElementEndHandlerMap;
private:
	void registerElementStartHandler(const CLeeString& element, ElementStartHandler handler);
	void registerElementEndHandler(const CLeeString& element, ElementEndHandler handler);
	//
	void tagResourceDefineStart(const XMLAttributes& attributes);
	void tagResourceGroupStart(const XMLAttributes& attributes);
	void tagResImageSetStart(const XMLAttributes& attr);
	void tagResWidgetlookStart(const XMLAttributes& attr);
	void tagResTextureStart(const XMLAttributes& attrs);
	void tagResControlStart(const XMLAttributes& attr);
	void tagResourceGroupEnd();
private:
	static const CLeeString tagResourceDefine;
	static const CLeeString tagResourceGroup;
	static const CLeeString tagResImageSet;
	static const CLeeString tagWidgetlook;
	static const CLeeString tagResTexture;
	static const CLeeString tagResControl;

	// Ù–‘
	static const CLeeString attrName;
	static const CLeeString attrType;
	static const CLeeString attrfile;
	ElementStartHandlerMap  d_startHandlersMap;
	ElementEndHandlerMap    d_endHandlersMap;
	ResourceGroup * d_group;
};
