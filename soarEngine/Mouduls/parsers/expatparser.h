
#ifndef _ExpatParser_h_
#define _ExpatParser_h_
#include "../../XMLHandler/XMLAnalyser.h"
/********************************************************************
    Descibe:      MYFeel�ṩ��Ĭ��XML��������װ�����ӽ�����������
    created:      2012/8/21 14:44
    FullName:     d:\������Ŀ\MYFeel\MYFeel\UI\XMLHandler\ExpatParser.h
    Path:         d:\������Ŀ\MYFeel\MYFeel\UI\XMLHandler
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
    //����DLL�������XML����������Դ�ṩ�ߡ�
    bool parseXMLFile(IXMLHandler& handler, const CLeeString& filename);
};

#endif 
