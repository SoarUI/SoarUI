#pragma once
#include "../XML/IXMLHandler.h"
#include "../SoarHeader/ISoarEngine.h"

class CSoarCore_xmlHandler :
	public IXMLHandler
{
public:
	CSoarCore_xmlHandler(ISoarEngine* lookene);
public:
	virtual ~CSoarCore_xmlHandler(void);
	void elementStart(const CLeeString& element, const XMLAttributes& attributes);
	void elementEnd(const CLeeString& element);
	void text(const CLeeString& text){}
	//
	 typedef void (CSoarCore_xmlHandler::*ElementStartHandler)(const XMLAttributes& attributes);
	 typedef void (CSoarCore_xmlHandler::*ElementEndHandler)();
	  typedef std::map<CLeeString, ElementStartHandler, CLeeString::FastLessCompare> ElementStartHandlerMap;
	  typedef std::map<CLeeString, ElementEndHandler, CLeeString::FastLessCompare> ElementEndHandlerMap;
private:
	void registerElementStartHandler(const CLeeString& element, ElementStartHandler handler);
		void registerElementEndHandler(const CLeeString& element, ElementEndHandler handler);
		//
		 void tagResPaserStart(const XMLAttributes& attr);
		 void tagDefaultBlenderStart(const XMLAttributes& attrs);
		 void tagXMLDataProviderStart(const XMLAttributes& attr);
		 void tagPlugDirectoryStart(const XMLAttributes& attr);
		 void tagImageDirectoryStart(const XMLAttributes& attr);
		 void tagWidgetlookDirectoryStart(const XMLAttributes& attr);
		 void tagTextureDirectoryStart(const XMLAttributes& attr);
		 void tagControlDirectoryStart(const XMLAttributes& attr);
		 void tagResourceStart(const XMLAttributes& attr);
		 void tagResThemeStart(const XMLAttributes& attr);
		 void tagResDirectoryStart(const XMLAttributes& attr);
		 void tagImageCodecStart(const XMLAttributes& attr);
		 //
		 void tagResPaserEnd();
		 void tagDefaultBlenderEnd();
		 void tagXMLDataProviderEnd();
		 void tagPlugDirectoryEnd();
		 void tagImageDirectoryEnd();
		 void tagWidgetlookDirectoryEnd();
		 void tagTextureDirectoryEnd();
		 void tagControlDirectoryEnd();
		 void tagResourceEnd();
		 void tagResThemeEnd();
		 void tagCorelEnd();
private:
	static const CLeeString tagCorel;
	static const CLeeString tagResPaser;
	static const CLeeString tagDefaultBlender;
	static const CLeeString tagXMLDataProvider;
	static const CLeeString tagPlugDirectory;
	static const CLeeString tagImageDirectory;
	static const CLeeString tagWidgetlookDirectory;
	static const CLeeString tagTextureDirectory;
	static const CLeeString tagControlDirectory;
	static const CLeeString tagResource;
	static const CLeeString tagResTheme;
	static const CLeeString tagResDirectory;
	static const CLeeString tagImageCodec;
	// Ù–‘
	static const CLeeString attrName;
	static const CLeeString attrConstructor;
	static const CLeeString attrDestructor;
	static const CLeeString attrDirectory;
	static const CLeeString attrfile;
	static const CLeeString attrDefaultTheme;
	ElementStartHandlerMap  d_startHandlersMap;
	ElementEndHandlerMap    d_endHandlersMap;
	ISoarEngine* theLookEng;
	//CLeeUIplugMgr *theplugs;
	CLeeString d_themeFile;
	CLeeString d_defaultTheme;
	CLeeString d_resourceFile;
};
