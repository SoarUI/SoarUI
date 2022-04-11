
#include "ImageDim.h"
#include <cassert>
#include "ImagesetManager.h"
 
    ////////////////////////////////////////////////////////////////////////////////

    ImageDim::ImageDim(const CLeeString& imageset, const CLeeString& image, DimensionType dim) :
        d_imageset(imageset),
        d_image(image),
        d_what(dim)
    {}

    void ImageDim::setSourceImage(const CLeeString& imageset, const CLeeString& image)
    {
        d_imageset = imageset;
        d_image = image;
    }

    void ImageDim::setSourceDimension(DimensionType dim)
    {
        d_what = dim;
    }

    float ImageDim::getValue_impl(const RectF&) const
    {
		const Image* img = ImagesetManager::getSingleton().get(d_imageset)->getImage(d_image);

        switch (d_what)
        {
            case DT_WIDTH:
                return img->getWidth();
                break;

            case DT_HEIGHT:
                return img->getHeight();
                break;

            case DT_X_OFFSET:
                return img->getOffsetX();
                break;

            case DT_Y_OFFSET:
                return img->getOffsetY();
                break;

            // these other options will not be particularly useful for most people since they return the edges of the
            // image on the source texture.
            case DT_LEFT_EDGE:
            case DT_X_POSITION:
                return img->getSourceTextureArea().d_left;
                break;

            case DT_TOP_EDGE:
            case DT_Y_POSITION:
                return img->getSourceTextureArea().d_top;
                break;

            case DT_RIGHT_EDGE:
                return img->getSourceTextureArea().d_right;
                break;

            case DT_BOTTOM_EDGE:
                return img->getSourceTextureArea().d_bottom;
                break;

            default:
               throw(_T("ImageDim::getValue - unknown or unsupported DimensionType encountered."));
                break;
        }
    }

    float ImageDim::getValue_impl(const RectF& ControlRect, const RectF&) const
    {
        return getValue_impl(ControlRect);
    }


    BaseDim* ImageDim::clone_impl() const
    {
        ImageDim* ndim = new ImageDim(d_imageset, d_image, d_what);
        return ndim;
    }

  

    ////////////////////////////////////////////////////////////////////////////////

    PropertyDim::PropertyDim(const CLeeString& name, const CLeeString& property,
	    DimensionType type) :
        d_property(property),
        d_childSuffix(name),
		d_type (type)
    {
    }

    float PropertyDim::getValue_impl(const RectF& ControlRect) const
    {
        
        /*UDim d = PropertyHelper::stringToUDim(d_property);

        switch (d_type)
        {
            case DT_WIDTH:
                return d.asAbsolute(ControlRect.Width());

            case DT_HEIGHT:
                return d.asAbsolute(ControlRect.Height());

            default:
				;
        }*/
		return 0.0f;
    }

    float PropertyDim::getValue_impl(const RectF& ControlRect, const RectF&) const
    {
        return getValue_impl(ControlRect);
    }
//ÓÐ´ýÖØÐ´2012-8-26
    BaseDim* PropertyDim::clone_impl() const
    {
        PropertyDim* ndim = new PropertyDim(d_childSuffix, d_property, d_type);
        return ndim;
    }

    ////////////////////////////////////////////////////////////////////////////////

   

  
    

   
  