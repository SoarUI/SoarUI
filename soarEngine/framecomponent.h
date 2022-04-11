
#ifndef _FrameComponent_h_
#define _FrameComponent_h_

#include "ComponentBase.h"
class Image;
    class FrameComponent : public FeelComponentBase
    {
    public:
        FrameComponent();
		FrameComponent(const FrameComponent& rhs);
		virtual ~FrameComponent();
        VerticalFormatting getBackgroundVerticalFormatting() const;
        void setBackgroundVerticalFormatting(VerticalFormatting fmt);
        HorizontalFormatting getBackgroundHorizontalFormatting() const;
        void setBackgroundHorizontalFormatting(HorizontalFormatting fmt);
        const ImagePackage* getImagePackage(FrameImageComponent part) const;
        void setImagePackage(FrameImageComponent part,  ImagePackage* image);
        void setImagePackage(FrameImageComponent part, const CLeeString& imageset, const CLeeString& image);

    protected:
		//get the image
		 Image* getImage(FrameImageComponent part)const ;
        // implemets abstract from base
        void render_impl(const RectF& ControlRect, RectF& destRect/*脚本定义的渲染区域*/, 
			const RectF* clipper/*经过剪切的最终渲染区域*/, bool clipToDisplay,const PointF2D& PixelOffsetXY) const;

        // renders the background image (basically a clone of render_impl from ImageryComponent - maybe we need a helper class?)
        void doBackgroundRender(const RectF& ControlRect, RectF& destRect, 
			const RectF* clipper/*经过剪切的最终渲染区域*/, bool clipToDisplay,const PointF2D& PixelOffsetXY) const;

        // formatting options for background
        VerticalFormatting   d_vertFormatting;  //!< Vertical formatting to be applied when rendering the background for the component.
        HorizontalFormatting d_horzFormatting;  //!< Horizontal formatting to be applied when rendering the background for the component.
        // images for the frame
        ImagePackage* d_frameImages[FIC_FRAME_IMAGE_COUNT];  //!< Array that holds the assigned images.
    };

#endif  // end of guard _FrameComponent_h_
