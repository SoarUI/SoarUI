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
		//��ͬ��framecomponent imagecomponent�ĺ���
		void renderT(const RectF& ControlRect,const PointF2D& PixelOffsetXY,const CLeeString& text,int vertTextFmt,int horzTextFmt, 
			const RectF* Pixelclipper = 0, bool clipToDisplay = false) const;
    protected:
		void render_impl(const RectF& ControlRect/*�ؼ�����*/, RectF& destRect/*�ű��������Ⱦ����*/, 
			const RectF* clipper/*�������е�������Ⱦ����*/, bool clipToDisplay,const PointF2D& PixelOffsetXY/*ƫ�Ʋ���*/) const;
    private:
        CLeeString  d_textLogical;//Ҫ��Ⱦ������
        CLeeString  d_font;//����
		int d_size;// �����С
        CLeeString  d_textPropertyName;
        CLeeString  d_fontPropertyName;
		HorizontalTextFormatting d_horzTextFormat;
		VerticalTextFormatting d_VertTextFormat;
    };

#if defined(_MSC_VER)
#  pragma warning(pop)
#endif

#endif  // end of guard _TextComponent_h_
