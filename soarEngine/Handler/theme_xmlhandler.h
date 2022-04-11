#pragma once
#include "../Theme/WidgetTheme.h"
#include "../../XML/XMLAttributes.h"
#include "../../XML/IXMLHandler.h"
class Theme_xmlHandler :
	public IXMLHandler
{
public:
	Theme_xmlHandler(void);
	// xml tag names
	static const CLeeString UIThemeElement;
	static const CLeeString ThemeElement;
	static const CLeeString ResourceGroupElement;
	static const CLeeString SmallLookElement;
	static const CLeeString HelpElement;
	static const CLeeString TextureElement;

	// xml attribute names
	static const CLeeString FilenameAttribute;
	static const CLeeString NameAttribute;
	static const CLeeString TextureAttribute;
	static const CLeeString StringAttribute;
	static const CLeeString GroupAttribute;
	static const CLeeString ThemeAttribute;
	static const CLeeString TypeAttribute;

	// functions to handle the various elements
	void handleUIThemeElement(const XMLAttributes& attr);
	void handleThemeElement(const XMLAttributes& attr);
	void handleTextureElement(const XMLAttributes& attr);
	void handleResourceGroupElement(const XMLAttributes& attr);
	void handleSmalllookElement(const XMLAttributes& attr);
	void handleHelpstringElement(const XMLAttributes& attr);
	void handleUIThemeElementEnd(void);
	void handleThemeElementEnd(void);
public:
	virtual ~Theme_xmlHandler(void);
	void elementStart(const CLeeString& element, const XMLAttributes& attributes);
	void elementEnd(const CLeeString& element);
	void text(const CLeeString & text){}
private:
	CLeeString d_ThemeString;
	CLeeString d_ImagesetFilename;
	CLeeString d_Groupname;
	CLeeString d_SmallFileName;
	CLeeString d_Helpstring;
	CLeeString d_name;
	CLeeString d_Tex;
	CLeeString d_type;
};
