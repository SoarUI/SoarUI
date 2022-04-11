
#include "WidgetLook.h"
#include <iostream>
#include <algorithm>
#include "WidgetLookManager.h"
#include "../colour.h"
#include "../Propertys/Property.h"
#include "../Propertys/PropertyHelper.h"

	CWidgetLook::CWidgetLook(const CLeeString& name,const CLeeString & windowtype):
	d_name(name),d_windowType(windowtype)
    {
	}
	CWidgetLook::CWidgetLook( void ) 
	{}
	CWidgetLook::CWidgetLook(const CWidgetLook& rhs):
		d_name(rhs.d_name),
	d_windowType(rhs.d_windowType),
	d_namedAreas(rhs.d_namedAreas),
	d_stateImagery(rhs.d_stateImagery),
	d_imagerySections(rhs.d_imagerySections),d_Definitions(rhs.d_Definitions)
	{
	}
	CWidgetLook& CWidgetLook::operator=(const CWidgetLook& other)
	{
		if (this == &other)
			return *this;
		d_Definitions =other.d_Definitions;
		d_name =other.d_name;
		d_windowType =other.d_windowType;
		d_namedAreas =other.d_namedAreas;
		d_stateImagery =other.d_stateImagery;
		d_imagerySections =other.d_imagerySections;
		return *this;
	}
	CWidgetLook::~CWidgetLook(void)
	{
		clearNamedAreas();
		clearStateImagerys();
		clearImagerySections();
		clearPropertyDefines();
	}
	RectF CWidgetLook::getBoundingRect(const RectF& ControlRect) const
	{
		return ControlRect;
	}
	//-----------------------------------------------------------------------

    void CWidgetLook::addPropertyDefine(const PropertyDefinition& propdef)
    {
		if (!isPropDefinitionPresent(propdef.getName()))
		{
			d_Definitions.push_back(propdef);
		}
    }
    bool CWidgetLook::isPropDefinitionPresent(const CLeeString& name) const
	{
		vDefinitionList::const_reverse_iterator i = d_Definitions.rbegin();
		while (i != d_Definitions.rend())
		{
			if ((*i).getName() == name)
				return TRUE;
			++i;
		}
		return FALSE;
	}
    void CWidgetLook::clearPropertyDefines()
    {
        d_Definitions.clear();
    }

 //   
 // 
	////查找属性定义
	const PropertyDefinition* CWidgetLook::findPropertyDefinition(const CLeeString& propertyName) const
	{
		vDefinitionList::const_reverse_iterator i = d_Definitions.rbegin();
		while (i != d_Definitions.rend())
		{
			if ((*i).getName() == propertyName)
				return &(*i);
			++i;
		}
		return 0;
	}
 //  
	////获取该控件定义的属性
	const CLeeString &CWidgetLook::getProperty(const CLeeString& Propertyname)const
	{
       const PropertyDefinition *pPorty=findPropertyDefinition(Propertyname);
	   if (pPorty)
	   {
		  return  pPorty->get();
	   }
	   else
	   {
		   throw(_T("invalid Propertyname or forget to definite it"));
	   }
	   
	}

//渲染接口
	void CWidgetLook::RenderWidget(const CLeeString& stringStatus,const RectF& ControlRect,const PointF2D & PixelOffsetXY,const CLeeString& szText,int vertTextFmt,int horzTextFmt,const RectF* clipper , bool clipToDisplay )
	{
		//check the params put in
		WidgetStatus status =en_NORMAL;
		CLeeString stringColour =_T("NormalTextColor");
		RectF rcLocal =getBoundingRect(ControlRect);
		//step0 render the control itself
		if (isStateImageryPresent(stringStatus))
		{
			getStateImagery(stringStatus)->setClippedToDisplay(clipToDisplay);
			getStateImagery(stringStatus)->render(ControlRect,PixelOffsetXY,szText,vertTextFmt,horzTextFmt,clipper);
		}
		else if (isStateImageryPresent(_T("Normal")) )
		{
			getStateImagery(_T("Normal"))->setClippedToDisplay(clipToDisplay);
			getStateImagery(_T("Normal"))->render(ControlRect,PixelOffsetXY,szText,vertTextFmt,horzTextFmt,clipper);
		}
		//step 1 render the text
		
	}
	void CWidgetLook::RenderWidget_Frame(const CLeeString& stringStatus,const RectF& ControlRect,const PointF2D & PixelOffsetXY,const RectF* clipper , bool clipToDisplay )
	{
		//check the params put in
		WidgetStatus status =en_NORMAL;
		CLeeString stringColour =_T("NormalTextColor");
		RectF rcLocal =getBoundingRect(ControlRect);
		//step0 render the control itself
		if (isStateImageryPresent(stringStatus))
		{
			getStateImagery(stringStatus)->setClippedToDisplay(clipToDisplay);
			getStateImagery(stringStatus)->render(ControlRect,PixelOffsetXY,clipper);
		}
		else if (isStateImageryPresent(_T("Normal")) )
		{
			getStateImagery(_T("Normal"))->setClippedToDisplay(clipToDisplay);
			getStateImagery(_T("Normal"))->render(ControlRect,PixelOffsetXY,clipper);
		}
		
	}
	void CWidgetLook::RenderWidget_Image(const CLeeString& stringStatus,const RectF& ControlRect,const PointF2D & PixelOffsetXY,const RectF* clipper, bool clipToDisplay )
	{
		//check the params put in
		WidgetStatus status =en_NORMAL;
		CLeeString stringColour =_T("NormalTextColor");
		RectF rcLocal =getBoundingRect(ControlRect);
		
		//step0 render the control itself
		if (isStateImageryPresent(stringStatus))
		{
			getStateImagery(stringStatus)->setClippedToDisplay(clipToDisplay);
			getStateImagery(stringStatus)->render(ControlRect,PixelOffsetXY,clipper);
		}
		else if (isStateImageryPresent(_T("Normal")) )
		{
			getStateImagery(_T("Normal"))->setClippedToDisplay(clipToDisplay);
			getStateImagery(_T("Normal"))->render(ControlRect,PixelOffsetXY,clipper);
		}
	}
	
	
	//////////////////////////////////////////////////////////////////////////
	//
	//直接提供状态
	