
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
		//��ȡ�ÿ���������꣬�ؼ����꣬�Ӵ��������ƫ�ƶ���
        RectF dest_rect(d_area.getRect(ControlRect));
		RectF final_clip_rect =dest_rect;
        //���о�����ʵ����windowsϵͳ�µĿؼ�����
		if (!clipper&& clipToDisplay)
		{
			clipper = &dest_rect;
		}
		if (clipToDisplay)
		{
			final_clip_rect=dest_rect.Intersection(*clipper);
		}
        render_impl(ControlRect, dest_rect/*�ű��������Ⱦ���򣬼�������ڿؼ�����ϵͳ��λ��*/, 
                    &final_clip_rect/*�������е�������Ⱦ����*/, clipToDisplay,PixelOffsetXY);
    }
	void FeelComponentBase::render(const RectF& ControlRect, 
		const RectF& baseRect/*��������µĿ�������*/,const PointF2D& PixelOffsetXY,
		const RectF* clipper, bool clipToDisplay) const
	{
		//��ȡ�ÿ������ı������꣬��������Ҫ�������򣬵�û�������У�
		//����ֻ��¼�˼��㷽����ƫ�ƣ����Ե���Ⱦ�������ؼ�ӳ��
		RectF dest_rect(d_area.getRect(ControlRect, baseRect));
		//���о�����ʵ����windowsϵͳ�µĿؼ�����,��Ϊ���ܻ��ƿͻ�����/�ؼ������������û��ṩ��������
		RectF final_clip_rect =dest_rect;
		if (!clipper&& clipToDisplay)
		{
			clipper = &dest_rect;
		}
		if (clipToDisplay)
		{
			//���յ���Ⱦ��,����˳�����Ҫ
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

