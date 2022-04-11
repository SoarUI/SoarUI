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

	void TextComponent::render_impl(const RectF& ControlRect, RectF& destRect/*脚本定义的渲染区域*/, 
		const RectF* clipper/*经过剪切的最终渲染区域*/, bool clipToDisplay,const PointF2D& PixelOffsetXY) const
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
		//最终合成为相关坐标
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
		//////////////////////////////////////////////////////////////////////////(以上是render的实现，在该函数中被合并在一起)
		// get the rect area that we will actually draw to (i.e. perform clipping)
		RectF final_rect(clipper? dest_rect.Intersection(*clipper) : dest_rect );
		// calculate final colours to be used
		// ColourRect finalColours;
		//  initColoursRect(srcWindow, modColours, finalColours);
		final_rect.d_left	= PixelAligned(final_rect.d_left);
		final_rect.d_right	= PixelAligned(final_rect.d_right);
		final_rect.d_top	= PixelAligned(final_rect.d_top);
		final_rect.d_bottom	= PixelAligned(final_rect.d_bottom);
		//最终合成为相关坐标-----
		final_rect.offsetRect(PixelOffsetXY.d_x,PixelOffsetXY.d_y);
		//文字格式
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