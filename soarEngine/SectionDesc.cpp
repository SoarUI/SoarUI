
#include "SectionDesc.h"
#include "ImagerySection.h"
#include "widgetslook/WidgetLook.h"
#include "widgetslook/WidgetLookManager.h"
#include "Propertys/PropertyHelper.h"
#include <iostream>

    SectionDesc::SectionDesc(const CLeeString& owner,
                                               const CLeeString& sectionName) :
        d_owner(owner),
        d_sectionName(sectionName)
    {}
		SectionDesc::~SectionDesc(void)
		{
		}
    const CLeeString& SectionDesc::getOwnerWidgetLook() const
    {
        return d_owner;
    }
    const CLeeString& SectionDesc::getSectionName() const
    {
        return d_sectionName;
    }
	
   
//----------------------------------------------------------------------------//
void SectionDesc::render(const RectF& ControlRect,const PointF2D& PixelOffsetXY,
								  const RectF* clipper,
								  bool clipToDisplay) const
{
	try
	{
		ImagerySection* sect = NULL;
		// get the imagery section object with the name we're set up to use
		 CWidgetLook * widgetlook=WidgetLookManager::getSingleton().getWidgetLook(d_owner);
		 //从宿主即窗口中获取ImagerySection 获取相关图像，位置等
		 if (widgetlook)
		 {
				 sect = widgetlook->getImagerySection(d_sectionName); 
		 }
		if (sect)
		{
			// render the imagery section
			sect->render(ControlRect,PixelOffsetXY ,clipper, clipToDisplay);
		}
	}
	// do nothing here, errors are non-faltal and are logged for debugging purposes.
	catch (...)
	{
		throw("invalid codec");
	}
}
void SectionDesc::render(const RectF& ControlRect,const PointF2D& PixelOffsetXY,const CLeeString& szText,int vertTextFmt,int horzTextFmt,
						 const RectF* clipper,
						 bool clipToDisplay) const
{
	try
	{
		ImagerySection* sect = NULL;
		// get the imagery section object with the name we're set up to use
		CWidgetLook * widgetlook=WidgetLookManager::getSingleton().getWidgetLook(d_owner);
		//从宿主即窗口中获取ImagerySection 获取相关图像，位置等
		if (widgetlook)
		{
			sect = widgetlook->getImagerySection(d_sectionName); 
		}
		if (sect)
		{
			// render the imagery section
			sect->render(ControlRect,PixelOffsetXY,szText ,vertTextFmt,horzTextFmt,clipper, clipToDisplay);
		}
	}
	// do nothing here, errors are non-faltal and are logged for debugging purposes.
	catch (...)
	{
		throw("invalid codec");
	}
}
void SectionDesc::render(const RectF& ControlRect, const RectF& RelativebaseRect,const PointF2D& PixelOffsetXY,
						 const RectF* clipper,
						 bool clipToDisplay) const
{

	try
	{
		// get the imagery section object with the name we're set up to use
		CWidgetLook * widgetlook=WidgetLookManager::getSingleton().getWidgetLook(d_owner);
		ImagerySection* sect = NULL;
		if (widgetlook)
		{
				sect = widgetlook->getImagerySection(d_sectionName);
		}
		if (sect)
		{
			// render the imagery section
			sect->render(ControlRect, RelativebaseRect, PixelOffsetXY,clipper, clipToDisplay);
		}


	}
	// do nothing here, errors are non-faltal and are logged for debugging purposes.
	catch (...)
	{}
}