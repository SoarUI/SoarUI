
#ifndef _XMLAnalyser_h_
#define _XMLAnalyser_h_
#include "../../SoarHeader/LeeString.h"
#include "../../XML\IXMLHandler.h"
/********************************************************************
    Descibe:      XML解析器基类：内容提取，事件触发
    created:      2012/8/21 14:41
    FullName:     \XMLHandler\XMLAnalyser.h
    Path:         XMLHandler
    ClassName:    XMLAnalyser
    FileType:     h
    Author:       nicklisir@163.com
    NeededLib:    
    requires:      
*********************************************************************/
    class IXMLAnalyser 
    {
    public:
        IXMLAnalyser(void){;}
        virtual ~IXMLAnalyser(void){;}
        virtual bool parseXMLFile(IXMLHandler& handler, const CLeeString& filename) = 0;
		//const CLeeString& getIdentifierString() const;
    };

#endif
