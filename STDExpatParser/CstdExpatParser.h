#pragma once
#include "../XML\IXMLHandler.h"
/********************************************************************
    Descibe:      使用开源ExpatXML解释器的语法器
    created:      2012/9/24 23:13
    ClassName:    CstdExpatParser
    FileType:     h
    Author:       nicklisir@163.com
    NeededLib:    libexpat.dll libexpat.lib
    requires:      Expat v2.0
*********************************************************************/
struct XML_ParserStruct;
typedef struct XML_ParserStruct *XML_Parser;
typedef void ( *XML_StartElementHandler) (void *userData,
												 const char *name,
												 const char **atts);

typedef void (*XML_EndElementHandler) (void *userData,
											   const char *name);


/* s is not 0 terminated. */
typedef void ( *XML_CharacterDataHandler) (void *userData,
												  const char *s,
												  int len);
typedef XML_Parser (*XML_ParserCreate)(const char*);
typedef void (*XML_SetUserData)(XML_Parser ,void*);
typedef void (*XML_SetElementHandler)(XML_Parser,XML_StartElementHandler,XML_EndElementHandler);
typedef void (*XML_SetCharacterDataHandler)(XML_Parser,XML_CharacterDataHandler);
typedef int (*XML_Parse)(XML_Parser,char*,int,int);
typedef void (*XML_ParserFree)(XML_Parser);
class CstdExpatParser
{
public:
	CstdExpatParser(void);
	virtual ~CstdExpatParser(void);
	virtual bool Parse(IXMLHandler * handler, TCHAR* DataPtr,size_t size);
	virtual void Release(void);
	static void fireEndElement(void * handler, const CHAR* element);
	static void firestartElement(void * handler, const CHAR* element, const CHAR** attr);
	static void fireTextElement(void * handler, const CHAR* text,int len);
	void setmoudle(HMODULE hModule);
private:
	void* d_handle;
   HMODULE d_hModule;
};
