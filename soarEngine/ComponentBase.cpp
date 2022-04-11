
#include "ComponentBase.h"
#include "Propertys/PropertyHelper.h"
#include <iostream>

    FeelComponentBase::FeelComponentBase()
       
    {}

    FeelComponentBase::~ FeelComponentBase()
    {}
	FeelComponentBase::FeelComponentBase(const FeelComponentBase& obj):
	d_area(obj.d_area),d_horzFormatPropertyName(obj.d_horzFormatPropertyName),
		d_vertFormatPropertyName(obj.d_vertFormatPropertyName),
		d_WidgetLookOwner(obj.d_WidgetLookOwner)
	{

	}
	FeelComponentBase& FeelComponentBase::operator=(const FeelComponentBase& other)
	{
		if (this == &other)
			return *this;
		d_area= other.d_area;
		d_horzFormatPropertyName =other.d_horzFormatPropertyName;
		d_vertFormatPropertyName = other.d_vertFormatPropertyName;
		d_WidgetLookOwner = other.d_WidgetLookOwner;
		return *this;
	}
	const CLeeString& FeelComponentBase::getOwnerWidgetLook() const
	{
		return d_WidgetLookOwner;
	}
	
	void FeelComponentBase::setOwnerWidget(const CLeeString & OwnerName)
	{
		d_WidgetLookOwner = OwnerName;
	}
    void FeelComponentBase::render(const RectF& ControlRect,
		const PointF2D& PixelOffsetXY ,const RectF* clipper,bool clipToDisplay) const
    {
		//获取该框架区域坐标，控件坐标，视传入的坐标偏移而定
        RectF dest_rect(d_area.getRect(ControlRect));
		RectF final_clip_rect =dest_rect;
        //剪切矩形其实就是windows系统下的控件区域
		if (!clipper&& clipToDisplay)
		{
			clipper = &dest_rect;
		}
		if (clipToDisplay)
		{
			final_clip_rect=dest_rect.Intersection(*clipper);
		}
        render_impl(ControlRect, dest_rect/*脚本定义的渲染区域，即该组件在控件坐标系统的位置*/, 
                    &final_clip_rect/*经过剪切的最终渲染区域*/, clipToDisplay,PixelOffsetXY);
    }
	void FeelComponentBase::render(const RectF& ControlRect, 
		const RectF& baseRect/*相对坐标下的控制区域*/,const PointF2D& PixelOffsetXY,
		const RectF* clipper, bool clipToDisplay) const
	{
		//获取该框架区域的本地坐标，它是我们要画的区域，但没经过剪切，
		//类中只记录了计算方法与偏移，所以到渲染才与具体控件映射
		RectF dest_rect(d_area.getRect(ControlRect, baseRect));
		//剪切矩形其实就是windows系统下的控件区域,因为可能绘制客户区域/控件区域，所以让用户提供剪切区域
		RectF final_clip_rect =dest_rect;
		if (!clipper&& clipToDisplay)
		{
			clipper = &dest_rect;
		}
		if (clipToDisplay)
		{
			//最终的渲染区,调用顺序很重要
			final_clip_rect=dest_rect.Intersection(*clipper);
		}
		render_impl(ControlRect, dest_rect,
			&final_clip_rect, clipToDisplay,PixelOffsetXY);
	}
    const ComponentArea& FeelComponentBase::getComponentArea() const
    {
        return d_area;
    }

    void FeelComponentBase::setComponentArea(const ComponentArea& area)
    {
        d_area = area;
    }

    void FeelComponentBase::setVertFormattingPropertySource(const CLeeString& property)
    {
        d_vertFormatPropertyName = property;
    }

    void FeelComponentBase::setHorzFormattingPropertySource(const CLeeString& property)
    {
        d_horzFormatPropertyName = property;
    }

