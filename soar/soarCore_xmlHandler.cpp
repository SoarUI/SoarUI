#include "SoarCore_xmlHandler.h"
#include "../SoarHeader/leeLog.h"
const CLeeString CSoarCore_xmlHandler::tagCorel =_T("Corel");
const CLeeString CSoarCore_xmlHandler::tagResPaser =_T("ResourceXMLParser");
const CLeeString CSoarCore_xmlHandler::tagDefaultBlender =_T("DefaultBlender");
const CLeeString CSoarCore_xmlHandler::tagXMLDataProvider =_T("XmlDataProvider");
const CLeeString CSoarCore_xmlHandler::tagPlugDirectory =_T("PlugsDirectory");
const CLeeString CSoarCore_xmlHandler::tagImageDirectory =_T("ImagesetDirectory");
const CLeeString CSoarCore_xmlHandler::tagWidgetlookDirectory =_T("WidgetLookDirectory");
const CLeeString CSoarCore_xmlHandler::tagTextureDirectory =_T("TextureDirectory");
const CLeeString CSoarCore_xmlHandler::tagControlDirectory =_T("ControlDirectory");
const CLeeString CSoarCore_xmlHandler::tagResource =_T("Resource");
const CLeeString CSoarCore_xmlHandler::tagResTheme =_T("ResourceTheme");
const CLeeString CSoarCore_xmlHandler::tagResDirectory =_T("ResourceDirectory");
const CLeeString CSoarCore_xmlHandler::tagImageCodec = _T("ImageCodec") ;
//属性
const CLeeString CSoarCore_xmlHandler::attrName =_T("name");
const CLeeString CSoarCore_xmlHandler::attrConstructor =_T("constructor");
const CLeeString CSoarCore_xmlHandler::attrDestructor =_T("destructor");
const CLeeString CSoarCore_xmlHandler::attrDirectory =_T("directory");
const CLeeString CSoarCore_xmlHandler::attrfile =_T("file");
const CLeeString CSoarCore_xmlHandler::attrDefaultTheme =_T("DefaultTheme");
typedef HRESULT (*DLLCREATE)(LPVOID *ppReturn);
CSoarCore_xmlHandler::CSoarCore_xmlHandler(ISoarEngine* lookene)
{
	theLookEng = lookene;
	registerElementStartHandler(tagResPaser, &CSoarCore_xmlHandler::tagResPaserStart);
	registerElementStartHandler(tagXMLDataProvider, &CSoarCore_xmlHandler::tagXMLDataProviderStart);
	registerElementStartHandler(tagDefaultBlender, &CSoarCore_xmlHandler::tagDefaultBlenderStart);
	registerElementStartHandler(tagPlugDirectory, &CSoarCore_xmlHandler::tagPlugDirectoryStart);
	registerElementStartHandler(tagImageDirectory, &CSoarCore_xmlHandler::tagImageDirectoryStart);
	registerElementStartHandler(tagWidgetlookDirectory, &CSoarCore_xmlHandler::tagWidgetlookDirectoryStart);
	registerElementStartHandler(tagTextureDirectory, &CSoarCore_xmlHandler::tagTextureDirectoryStart);
	registerElementStartHandler(tagControlDirectory, &CSoarCore_xmlHandler::tagControlDirectoryStart);
	registerElementStartHandler(tagResTheme, &CSoarCore_xmlHandler::tagResThemeStart);
	registerElementStartHandler(tagResource, &CSoarCore_xmlHandler::tagResourceStart);
	registerElementStartHandler(tagResDirectory,&CSoarCore_xmlHandler::tagResDirectoryStart);
	registerElementStartHandler(tagImageCodec,&CSoarCore_xmlHandler::tagImageCodecStart);
	//
	 registerElementEndHandler(tagResPaser, &CSoarCore_xmlHandler::tagResPaserEnd);
     registerElementEndHandler(tagXMLDataProvider, &CSoarCore_xmlHandler::tagXMLDataProviderEnd);
	 registerElementEndHandler(tagDefaultBlender, &CSoarCore_xmlHandler::tagDefaultBlenderEnd);
	 registerElementEndHandler(tagPlugDirectory, &CSoarCore_xmlHandler::tagPlugDirectoryEnd);
	 registerElementEndHandler(tagImageDirectory, &CSoarCore_xmlHandler::tagImageDirectoryEnd);
     registerElementEndHandler(tagWidgetlookDirectory, &CSoarCore_xmlHandler::tagWidgetlookDirectoryEnd);
	 registerElementEndHandler(tagTextureDirectory, &CSoarCore_xmlHandler::tagTextureDirectoryEnd);
	 registerElementEndHandler(tagControlDirectory, &CSoarCore_xmlHandler::tagControlDirectoryEnd);
	 registerElementEndHandler(tagResource, &CSoarCore_xmlHandler::tagResourceEnd);
	 registerElementEndHandler(tagResTheme, &CSoarCore_xmlHandler::tagResThemeEnd);
	 registerElementEndHandler(tagCorel,&CSoarCore_xmlHandler::tagCorelEnd);
}

CSoarCore_xmlHandler::~CSoarCore_xmlHandler(void)
{
	d_startHandlersMap.clear();
	d_endHandlersMap.clear();
}
void CSoarCore_xmlHandler::elementStart(const CLeeString& element, const XMLAttributes& attributes)
{
	// find registered handler for this element.
	ElementStartHandlerMap::const_iterator iter = d_startHandlersMap.find(element);

	// if a handler existed
	if (iter != d_startHandlersMap.end())
	{
		// call the handler for this element
		(this->*(iter->second))(attributes);
	}
	Dbgc_DumpX(_T("leeduires"),
		_T("===== loading resources corel: element:<%s> start ====="),element.c_str());
}
void CSoarCore_xmlHandler::elementEnd(const CLeeString& element)
{
	// find registered handler for this element.
	ElementEndHandlerMap::const_iterator iter = d_endHandlersMap.find(element);

	// if a handler existed
	if (iter != d_endHandlersMap.end())
		// call the handler for this element
		(this->*(iter->second))();
}
void CSoarCore_xmlHandler::registerElementStartHandler(const CLeeString& element, ElementStartHandler handler)
{
	d_startHandlersMap[element] = handler;
}
void CSoarCore_xmlHandler::registerElementEndHandler(const CLeeString& element, ElementEndHandler handler)
{
	d_endHandlersMap[element] = handler;
}
void CSoarCore_xmlHandler::tagResPaserStart(const XMLAttributes& attr)
{
	CLeeString name =attr.getValueAsString(attrName);
	CLeeString Constructor =attr.getValueAsString(attrConstructor);
	CLeeString Destructor =attr.getValueAsString(attrDestructor);
	theLookEng->SetXmlParser(name,Constructor,Destructor);
	
}
void CSoarCore_xmlHandler::tagDefaultBlenderStart(const XMLAttributes& attrs)
{
	CLeeString name =attrs.getValueAsString(attrName);
	CLeeString Constructor =attrs.getValueAsString(attrConstructor);
	CLeeString Destructor =attrs.getValueAsString(attrDestructor);
	theLookEng->SetDefaultBlender(name,Constructor,Destructor);
	Dbgc_DumpX(_T("leeduires"),
		_T("===== Engine setting default Blender<%s> mothod<%s> mothod<%s>====="),Dbgc_Param3(name.c_str(),Constructor.c_str(),Destructor.c_str() ) );
}
void CSoarCore_xmlHandler::tagXMLDataProviderStart(const XMLAttributes& attr)
{
	CLeeString name =attr.getValueAsString(attrName);
	CLeeString Constructor =attr.getValueAsString(attrConstructor);
	CLeeString Destructor =attr.getValueAsString(attrDestructor);
	theLookEng->SetXmlDataProvider(name,Constructor,Destructor);
	Dbgc_DumpX(_T("leeduires"),
		_T("===== Engine setting default Data provider<%s> mothod<%s> mothod<%s>====="),Dbgc_Param3(name.c_str(),Constructor.c_str(),Destructor.c_str()) );
}
void CSoarCore_xmlHandler::tagImageCodecStart(const XMLAttributes& attr)
{
	CLeeString name =attr.getValueAsString(attrName);
	CLeeString Constructor =attr.getValueAsString(attrConstructor);
	CLeeString Destructor =attr.getValueAsString(attrDestructor);
	theLookEng->SetDefaultImgCodec(name,Constructor,Destructor);
	Dbgc_DumpX(_T("leeduires"),
		_T("===== Engine setting default ImgCodec<%s> mothod<%s> mothod<%s>====="),Dbgc_Param3(name.c_str(),Constructor.c_str(),Destructor.c_str() ));
}
void CSoarCore_xmlHandler::tagPlugDirectoryStart(const XMLAttributes& attr)
{
	theLookEng->SetPluginDirection(attr.getValueAsString(attrDirectory));
	Dbgc_DumpX(_T("leeduires"),
		_T("===== Engine setting Plugin direction<%s> ====="),attr.getValueAsString(attrDirectory).c_str());
}
void CSoarCore_xmlHandler::tagImageDirectoryStart(const XMLAttributes& attr)
{
	theLookEng->SetImageSetDirection(attr.getValueAsString(attrDirectory));
	Dbgc_DumpX(_T("leeduires"),
		_T("===== Engine setting ImageSet direction<%s> ====="),attr.getValueAsString(attrDirectory).c_str());
}
void CSoarCore_xmlHandler::tagWidgetlookDirectoryStart(const XMLAttributes& attr)
{
	theLookEng->SetWidgetLookDirection(attr.getValueAsString(attrDirectory));
	Dbgc_DumpX(_T("leeduires"),
		_T("===== Engine setting Widgetlook direction<%s> ====="),attr.getValueAsString(attrDirectory).c_str());
}
void CSoarCore_xmlHandler::tagTextureDirectoryStart(const XMLAttributes& attr)
{
	theLookEng->SetTextureDirection(attr.getValueAsString(attrDirectory));
	Dbgc_DumpX(_T("leeduires"),
		_T("===== Engine setting Texture direction<%s> ====="),attr.getValueAsString(attrDirectory).c_str());
}
void CSoarCore_xmlHandler::tagControlDirectoryStart(const XMLAttributes& attr)
{
}
void CSoarCore_xmlHandler::tagResDirectoryStart(const XMLAttributes& attr)
{
	theLookEng->SetResourceDirection(attr.getValueAsString(attrDirectory));
	Dbgc_DumpX(_T("leeduires"),
		_T("===== Engine setting resource direction<%s> ====="),attr.getValueAsString(attrDirectory).c_str());
}
void CSoarCore_xmlHandler::tagResourceStart(const XMLAttributes& attr)
{
	d_resourceFile =attr.getValueAsString(attrfile);
}
void CSoarCore_xmlHandler::tagResThemeStart(const XMLAttributes& attr)
{
	d_themeFile =attr.getValueAsString(attrfile);
	d_defaultTheme =attr.getValueAsString(attrDefaultTheme);
	theLookEng->engine_SetDefaultTheme(d_themeFile,d_defaultTheme);
	Dbgc_DumpX(_T("leeduires"),
		_T("===== Engine setting default theme:<%s> theme file<%s> ====="),Dbgc_Param2(d_defaultTheme.c_str(),d_themeFile.c_str() ));
}
//
void CSoarCore_xmlHandler::tagResPaserEnd(){}
void CSoarCore_xmlHandler::tagDefaultBlenderEnd(){}
void CSoarCore_xmlHandler::tagXMLDataProviderEnd(){}
void CSoarCore_xmlHandler::tagPlugDirectoryEnd(){}
void CSoarCore_xmlHandler::tagImageDirectoryEnd(){}
void CSoarCore_xmlHandler::tagWidgetlookDirectoryEnd(){}
void CSoarCore_xmlHandler::tagTextureDirectoryEnd(){}
void CSoarCore_xmlHandler::tagControlDirectoryEnd(){}
void CSoarCore_xmlHandler::tagResourceEnd(){}
void CSoarCore_xmlHandler::tagResThemeEnd(){}
void CSoarCore_xmlHandler::tagCorelEnd()
{
	Dbgc_DumpX(_T("leeduires"),
		_T("===== Engine load resource and define the resources define file<%s> ====="),d_resourceFile.c_str());
	//先加载资源
	theLookEng->engine_defineResources(d_resourceFile);
}