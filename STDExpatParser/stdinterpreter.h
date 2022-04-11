#pragma once
#include "../XML\ILeeXmlParser.h"
#include "CstdExpatParser.h"

class CSTDInterpreter :
	public ILeeXmlParser
{
public:
	CSTDInterpreter(HMODULE hModule);
	virtual ~CSTDInterpreter(void);
	bool parseFile(IXMLHandler& handler, const CLeeString& filename,DWORD dwThunkSize) ;
	bool parseData(IXMLHandler& handler, void* pData,size_t dataSize);
	virtual void Release(void);
private:
	CstdExpatParser d_parser;
	HMODULE d_hModule;
};
