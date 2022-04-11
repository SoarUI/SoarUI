#pragma once
#include "standardinnerParser.h"
#include "../../XML\ILeeXmlParser.h"

class CInnerInterpreter :
	public ILeeXmlParser
{
public:
	CInnerInterpreter(void);
	virtual ~CInnerInterpreter(void);
	bool parseFile(IXMLHandler& handler, const CLeeString& filename,DWORD dwThunkSize) ;
	bool parseData(IXMLHandler& handler, void* pData,size_t dataSize);
	void Release(void);
private:
	CInnerParser d_parser;
};
