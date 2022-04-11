#pragma once
#include "../../SoarHeader/LeeString.h"
#include "WidgetLook.h"
//#include "../../FeelCommon.h"
#include "../objectReferentBase.h"
/********************************************************************
    Descibe:      ��Feel�ļ�������ڽ���ʹ��
    created:      2012/8/15 9:07
    FullName:     MYFeel\MYFeel\UI\Feel\CWidgetLookPackage.h
    Path:         MYFeel\MYFeel\UI\Feel
    ClassName:    CWidgetLookPackage
    FileType:     h
    Author:       nicklisir@163.com
    NeededLib:    
    requires:      
*********************************************************************/
class CWidgetLookPackage:public objectReferentBase
{
public:
	typedef std::map<CLeeString,  CWidgetLook, CLeeString::FastLessCompare> WidgetLookList;
public:
	CWidgetLookPackage(void);
	CWidgetLookPackage(const CWidgetLookPackage& rhs);
public:
	virtual ~CWidgetLookPackage(void);
	bool isWidgetLookAvailable(const CLeeString& widget) ;
	CWidgetLook* getWidgetLook(const CLeeString& widget) ;
	const CLeeString& getName(void) const;
	void setName(const CLeeString& name);

	//
	void eraseWidgetLook(const CLeeString& widget);
	void clearAllWidgetLooks(void);
	void addWidgetLook( CWidgetLook& look);
	WidgetLookList getWidgetFeelLists(void) const ;
	//
	CLeeString getDefaultResourceGroup(void) const { return d_defaultResourceGroup; }
	void setDefaultResourceGroup(const CLeeString& resourceGroup){ d_defaultResourceGroup = resourceGroup; }
private:
	CWidgetLookPackage& operator=(const CWidgetLookPackage& other);
	
private:
	WidgetLookList  d_widgetLooks;//������Widget��
	CLeeString d_defaultResourceGroup;   //Ĭ����
	CLeeString d_name;//Look������
};
