
#ifndef _ImagerySection_h_
#define _ImagerySection_h_

#include "ImageryComponent.h"
#include "FrameComponent.h"
#include "TextComponent.h"
//#include "../XMLSerializer.h"
#include <vector>
#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4251)
#endif

    class  ImagerySection
    {
    public:
        ImagerySection();
        ImagerySection(const CLeeString& name);
        void addImageryComponent(ImageryComponent& img);
		virtual ~ImagerySection(void);
        void clearImageryComponents();
        void clearTextComponents();
        void clearFrameComponents();
        void addFrameComponent(FrameComponent& frame);
		void addTextComponent(TextComponent& text);
        const CLeeString& getName() const;
		//获取本地坐标区域
		RectF getBoundingRect(const RectF& ControlRect) const;
		//区域需加上基本坐标【与区域相同坐标系】，剪切到控件区域内
		RectF getBoundingRect(const RectF& ControlRect, const RectF& RelativeRect) const;
		//获取文字区间
        void render(const RectF& ControlRect,const PointF2D& PixelOffsetXY, const CLeeString& text,int vertTextFmt,int horzTextFmt,
			const RectF* clipper = 0, bool clipToDisplay = false);
		void render(const RectF& ControlRect,const PointF2D& PixelOffsetXY,
			const RectF* clipper = 0, bool clipToDisplay = false);
		void render(const RectF& ControlRect, const RectF& pixelbaseRect,
			const PointF2D& PixelOffsetXY,
			const RectF* clipper  = 0 , bool clipToDisplay = false );

    private:
        typedef std::vector<ImageryComponent> ImageryList;
        typedef std::vector<FrameComponent> FrameList;
		typedef std::vector<TextComponent> TextList;

        CLeeString       d_name;             //ImagerySection 名字
        FrameList    d_frames;           //九宫图块渲染模块
        ImageryList  d_images;           //图像块渲染模块
		TextList     d_texts;            //文字渲染模块
    };

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif

#endif  // end of guard _ImagerySection_h_
