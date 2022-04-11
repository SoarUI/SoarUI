
#ifndef _ImageryComponent_h_
#define _ImageryComponent_h_

#include "ComponentBase.h"

class Image;
    class  ImageryComponent : public FeelComponentBase
    {
    public:
        ImageryComponent();
		ImageryComponent(const ImageryComponent& rhs);
		virtual ~ImageryComponent();
        const Image* getImage() const;
        void setImagePackage(ImagePackage* image);
		ImagePackage* getImagePackage(void) const;
        void setImagePackage(const CLeeString& imageset, const CLeeString& image);
        VerticalFormatting getVerticalFormatting() const;
        void setVerticalFormatting(VerticalFormatting fmt);
        HorizontalFormatting getHorizontalFormatting() const;
        void setHorizontalFormatting(HorizontalFormatting fmt);
        bool isImageFetchedFromProperty() const;
        const CLeeString& getImagePropertySource() const;
        void setImagePropertySource(const CLeeString& property);

    protected:
		void render_impl(const RectF& PixelWnd, RectF& destRect, 
			const RectF* clipper, bool clipToDisplay,const PointF2D& PixelOffsetXY) const;
        ImagePackage*         d_imagePack;           //!< Image to be drawn by this image component.
        VerticalFormatting   d_vertFormatting;  //!< Vertical formatting to be applied when rendering the image component.
        HorizontalFormatting d_horzFormatting;  //!< Horizontal formatting to be applied when rendering the image component.
       CLeeString d_imagePropertyName;            //!< Name of the property to access to obtain the image to be used.
	   
    };
#endif  // end of guard _ImageryComponent_h_
