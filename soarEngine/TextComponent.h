#ifndef _TextComponent_h_
#define _TextComponent_h_

#include "ComponentBase.h"
#include "../SoarHeader/leetextcontext.h"
#if defined(_MSC_VER)
#  pragma warning(push)
#  pragma warning(disable : 4251)
#endif

    class  TextComponent : public FeelComponentBase
    {
    public:
        TextComponent();
        ~TextComponent();
        TextComponent(const TextComponent& obj);
        TextComponent& operator=(const TextComponent& other);
        const CLeeString& getText() const;
        void setTextFMT(TextContext& text);
        const CLeeString& getFont() const;
        void setFont(const CLeeString& font);
        bool isTextFetchedFromProperty() const;
        const CLeeString& getTextPropertySource() const;
        void setTextPropertySource(const CLeeString& property);
        bool isFontFetchedFromProperty() const;
        const CLeeString& getFontPropertySource() const;
        void setFontPropertySource(const CLeeString& property);
         void setVertFormattingPropertySource(const CLeeString& property);
        void setHorzFormattingPropertySource(const CLeeString& property);
		//不同于framecomponent imagecomponent的函数
		void renderT(const RectF& ControlRect,const PointF2D& PixelOffsetXY,const CLeeString& text,int vertTextFmt,int horzTextFmt, 
			const RectF* Pixelclipper = 0, bool clipToDisplay = false) const;
    protected:
		void render_impl(const RectF& ControlRect/*控件区域*/, RectF& destRect/*脚本定义的渲染区域*/, 
			const RectF* clipper/*经过剪切的最终渲染区域*/, bool clipToDisplay,const PointF2D& PixelOffsetXY/*偏移参数*/) const;
    private:
        CLeeString  d_textLogical;//要渲染的文字
        CLeeString  d_font;//字体
		int d_size;// 字体大小
        CLeeString  d_textPropertyName;
        CLeeString  d_fontPropertyName;
		HorizontalTextFormatting d_horzTextFormat;
		VerticalTextFormatting d_VertTextFormat;
    };

#if defined(_MSC_VER)
#  pragma warning(pop)
#endif

#endif  // end of guard _TextComponent_h_
