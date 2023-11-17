#include "StdAfx.h"
#include "CstdExpatParser.h"

#include<conio.h>
#include "../SoarHeader/leelog.h"
//#include "Expat.h"
#ifdef _WIN64
//#pragma comment ( lib, "libexpat64.lib")
#else
//#pragma comment ( lib, "libexpat.lib")
#endif
CstdExpatParser::CstdExpatParser(void)
{
	;//d_identityString =_T("standarXmlExpat");
}

CstdExpatParser::~CstdExpatParser(void)
{
	::FreeLibrary((HMODULE)d_handle);
}
void CstdExpatParser::Release()
{
	delete this;
}
bool CstdExpatParser::Parse(IXMLHandler * handler,TCHAR* DataPtr,size_t size)
{
	char	szLibPath[MAX_PATH];	// Full DLL path
	// Get full path of the DLL not the exe path
	if (!GetModuleFileName( d_hModule , szLibPath, MAX_PATH))
		return 0 ;
	char* p=_tcsrchr(szLibPath,TEXT('\\') ) + 1 ;

	CLeeString szFilename;
	CLeeString szFullName;
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];

	_splitpath( szLibPath, drive, dir, fname, ext );
	szFullName.Format("%s%s\\%s",drive,dir,"libexpat.dll");
	d_handle=::LoadLibrary(szFullName.c_str());
	XML_ParserCreate XMLCreater =(XML_ParserCreate)::GetProcAddress((HMODULE)d_handle,"XML_ParserCreate");
	XML_Parser parser =XMLCreater(NULL);
	if (!parser)
	{
		return false;
	}
	XML_SetUserData XMLSetUserData =(XML_SetUserData)::GetProcAddress((HMODULE)d_handle,"XML_SetUserData");
	XMLSetUserData(parser,(void*)handler);
	XML_SetElementHandler XMLSetHandler =(XML_SetElementHandler)::GetProcAddress((HMODULE)d_handle,"XML_SetElementHandler");
	XMLSetHandler(parser,firestartElement,fireEndElement);
	XML_SetCharacterDataHandler XMLTextHandler =(XML_SetCharacterDataHandler)::GetProcAddress((HMODULE)d_handle,"XML_SetCharacterDataHandler");
	XMLTextHandler(parser,fireTextElement);
    XML_Parse XMLParse =(XML_Parse)::GetProcAddress((HMODULE)d_handle,"XML_Parse");
	XML_ParserFree XMLParserFree =(XML_ParserFree)::GetProcAddress((HMODULE)d_handle,"XML_ParserFree");
	if (!XMLParse(parser,DataPtr,size,true))
	{
		XMLParserFree(parser);
		return false;
	}
	XMLParserFree(parser);
	return true;
}
void CstdExpatParser::fireEndElement(void * handler, const CHAR* element)
{
	IXMLHandler * handlor =static_cast<IXMLHandler*>(handler);
	handlor->elementEnd(element);
}
void CstdExpatParser::firestartElement(void * handler,  const CHAR* element,const CHAR** attr)
{
	IXMLHandler * handlor =static_cast<IXMLHandler*>(handler);
	XMLAttributes attrs;
	for(size_t i = 0 ; attr[i] ; i += 2)
		attrs.add((const char*)attr[i], (const char*)attr[i+1]);
	handlor->elementStart(element, attrs);
}
void CstdExpatParser::fireTextElement(void * handler,const CHAR* text,int len)
{
	IXMLHandler * handlor =static_cast<IXMLHandler*>(handler);
	handlor->text(text);
}
void CstdExpatParser::setmoudle(HMODULE hModule)
{
	d_hModule =hModule;
}