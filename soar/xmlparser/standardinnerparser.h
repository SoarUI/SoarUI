#pragma once
#include "../../XML/IXMLHandler.h"
/********************************************************************
    Descibe:      LEEDUI 内建XML解析器
    created:      2012/7/20 14:45
    FullName:     
    Path:         
    ClassName:    
    FileType:     h
    Author:       nicklisir@gmail.com ecsuperpay@163.com 
    NeededLib:    
    requires:      
*********************************************************************/
class CInnerParser
{
public:
	CInnerParser(void);
	virtual ~CInnerParser(void);
	typedef std::map<CLeeString,CLeeString,CLeeString::FastLessCompare> NameValuePair;
	bool Parse(IXMLHandler * handler,BYTE* DataPtr,size_t size);
private:
	void fireEndElement(IXMLHandler * handler, TCHAR* element,  NameValuePair& attr);
	void firestartElement(IXMLHandler * handler, TCHAR* element, NameValuePair& attr);
	void fireTextElement(IXMLHandler * handler, TCHAR* element, TCHAR* text);

	//////////////////////////////////////////////////////////////////////////
	void AnalysisLineText(TCHAR** ppDataBuffer,TCHAR* pEND,NameValuePair &Val_nameMap);
	//截取元素名称,<element  xxx> < element xxx xxx> <element>
	bool ParseTagStart(TCHAR* element,TCHAR** ppDataBuffer,TCHAR* lspliter,TCHAR* rSpliter,TCHAR* pEND);
	//</endelement  ></endelement>
	void ParseTagEnd(TCHAR* element,TCHAR** ppDataBuffer);
	//分析名字-值对，形式name="xxx" name2="xxx"  或 name=xxx name2=xxx
	bool ParseNameValuePair(TCHAR* elementName,TCHAR* elementValue,TCHAR** ppDataBuffer,
		TCHAR* equalSpliter,TCHAR* lspliter,TCHAR* rlSpliter,TCHAR* rrSpliter,TCHAR* pEND);
	//截取内容
	bool ParsetxtElement(TCHAR* element,TCHAR** ppDataBuffer,TCHAR* lspliter,TCHAR* rSpliter);
	//清除左边空格
	void TrimLeftSpace(TCHAR* element,size_t count);
	void TrimRightSpace(TCHAR* element,size_t count);
};
