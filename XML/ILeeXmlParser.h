#pragma once
#include "IXMLHandler.h"
class ILeeXmlParser
{
public:
	ILeeXmlParser(void){}
	virtual ~ILeeXmlParser(void){}
public:
	virtual bool parseFile(IXMLHandler& handler, const CLeeString& filename,DWORD dwThunkSize) LEEPURE;
	virtual bool parseData(IXMLHandler& handler, void* pData,size_t dataSize) LEEPURE;
	virtual void Release(void) LEEPURE;
};
