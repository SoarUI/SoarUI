
#ifndef _ExpatParser_h_
#define _ExpatParser_h_
#include "../../XMLHandler/XMLAnalyser.h"
/********************************************************************
    Descibe:      MYFeel提供的默认XML解析器包装，连接解析器的桥梁
    created:      2012/8/21 14:44
    FullName:     d:\测试项目\MYFeel\MYFeel\UI\XMLHandler\ExpatParser.h
    Path:         d:\测试项目\MYFeel\MYFeel\UI\XMLHandler
    ClassName:    ExpatParser
    FileType:     h
    Author:       nicklisir@163.com
    NeededLib:    
    requires:      
*********************************************************************/
class  ExpatParser : public IXMLAnalyser
{
public:
    ExpatParser(void);
    ~ExpatParser(void);
    //连接DLL【插件：XML解析器，资源提供者】
    bool parseXMLFile(IXMLHandler& handler, const CLeeString& filename);
};

#endif 
