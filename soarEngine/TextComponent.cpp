#include "TextComponent.h"
#include "soarXMLHelper.h"
#include "Propertys/PropertyHelper.h"
#include <iostream>
#include "ResourcesManager.h"
#include "../SoarHeader/ILeeRender.h"

    TextComponent::TextComponent() 
    {
		d_VertTextFormat=VTF_CENTRE_ALIGNED;
		d_horzTextFormat=HTF_LEFT_ALIGNED;
	}

    TextComponent::~TextComponent()
    {
    }

    TextComponent::TextComponent(const TextComponent& obj) :
        FeelComponentBase(obj),
        d_textLogical(obj.d_textLogical),
        d_font(obj.d_font),
        d_textPropertyName(obj.d_textPropertyName),
        d_fontPropertyName(obj.d_fontPropertyName),
		d_VertTextFormat(obj.d_VertTextFormat),
		d_horzTextFormat(obj.d_horzTextFormat)
    {
    }

    TextComponent& TextComponent::operator=(const TextComponent& other)
    {
        if (this == &other)
            return *this;
        FeelComponentBase::operator=(other);
        d_textLogical = other.d_textLogical;
        d_font = other.d_font;
        d_textPropertyName = other.d_textPropertyName;
        d_fontPropertyName = other.d_fontPropertyName;
		d_VertTextFormat=other.d_VertTextFormat;
		d_horzTextFormat=other.d_horzTextFormat;
        return *this;
    }

    const CLeeString& TextComponent::getText() const
    {
        return d_textLogical;
    }
    const CLeeString& TextComponent::getFont() const
    {
        return d_font;
    }

    void TextComponent::setFont(const CLeeString& font)
    {
        d_font = font;
    }

	void TextComponent::render_impl(const RectF& ControlRect, RectF& destRect/*�ű��������Ⱦ����*/, 
		const RectF* clipper/*�������е�������Ⱦ����*/, bool clipToDisplay,const PointF2D& PixelOffsetXY) const
	{ 
		// get the rect area that we will actually draw to (i.e. perform clipping)
		RectF final_rect(clipper? destRect.Intersection(*clipper) : destRect );
        // calculate final colours to be used
       // ColourRect finalColours;
      //  initColoursRect(srcWindow, modColours, finalColours);
		final_rect.d_left	= PixelAligned(final_rect.d_left);
		final_rect.d_right	= PixelAligned(final_rect.d_right);
		final_rect.d_top	= PixelAligned(final_rect.d_top);
		final_rect.d_bottom	= PixelAligned(final_rect.d_bottom);
		//���պϳ�Ϊ�������
		final_rect.offsetRect(PixelOffsetXY.d_x,PixelOffsetXY.d_y);
		//do the drawing here
		/*ILeeRender * blender =ResourcesManager::getSingletonPtr()->getBlender();
		if (blender)
		{
			blender->RenderText(final_rect,PixelOffsetXY,text);
		}*/
    }
	void TextComponent::renderT(const RectF& ControlRect,const PointF2D& PixelOffsetXY,const CLeeString& text,int vertTextFmt,int horzTextFmt, 
		const RectF* clipper, bool clipToDisplay) const
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
		//////////////////////////////////////////////////////////////////////////(������render��ʵ�֣��ڸú����б��ϲ���һ��)
		// get the rect area that we will actually draw to (i.e. perform clipping)
		RectF final_rect(clipper? dest_rect.Intersection(*clipper) : dest_rect );
		// calculate final colours to be used
		// ColourRect finalColours;
		//  initColoursRect(srcWindow, modColours, finalColours);
		final_rect.d_left	= PixelAligned(final_rect.d_left);
		final_rect.d_right	= PixelAligned(final_rect.d_right);
		final_rect.d_top	= PixelAligned(final_rect.d_top);
		final_rect.d_bottom	= PixelAligned(final_rect.d_bottom);
		//���պϳ�Ϊ�������-----
		final_rect.offsetRect(PixelOffsetXY.d_x,PixelOffsetXY.d_y);
		//���ָ�ʽ
		int _VertTextFormat =0;
		int _horzTextFormat=0;
		if(vertTextFmt != -1){
			_VertTextFormat =vertTextFmt;
		}
		else{
			_VertTextFormat =d_VertTextFormat;
		}
		if(horzTextFmt!=-1){
			_horzTextFormat=horzTextFmt;
		}
		else{
			_horzTextFormat=d_horzTextFormat;
		}
		//do the drawing here
		ILeeRender * blender =ResourcesManager::getSingletonPtr()->getBlender();
		if (blender)
		{
			blender->RenderText(final_rect,PixelOffsetXY,text,_VertTextFormat,_horzTextFormat);
		}
	}
    bool TextComponent::isTextFetchedFromProperty() const
    {
        return !d_textPropertyName.empty();
    }

    const CLeeString& TextComponent::getTextPropertySource() const
    {
        return d_textPropertyName;
    }

    void TextComponent::setTextPropertySource(const CLeeString& property)
    {
        d_textPropertyName = property;
    }

    bool TextComponent::isFontFetchedFromProperty() const
    {
        return !d_fontPropertyName.empty();
    }

    const CLeeString& TextComponent::getFontPropertySource() const
    {
        return d_fontPropertyName;
    }

    void TextComponent::setFontPropertySource(const CLeeString& property)
    {
        d_fontPropertyName = property;
    }
	void TextComponent::setVertFormattingPropertySource(const CLeeString& property)
    {
        d_vertFormatPropertyName = property;
		d_VertTextFormat=CLooksXMLHelper::stringToVertTextFormat(property);
    }

    void TextComponent::setHorzFormattingPropertySource(const CLeeString& property)
    {
        d_horzFormatPropertyName = property;
		d_horzTextFormat=CLooksXMLHelper::stringToHorzTextFormat(property);
    }