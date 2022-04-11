
#include "ImageryComponent.h"
#include "SoarXMLHelper.h"
#include "Image.h"
#include "ImagesetManager.h"
#include "Imageset.h"
#include "Propertys/PropertyHelper.h"
#include "widgetslook/WidgetLook.h"
#include "widgetslook/WidgetLookManager.h"
#include <iostream>
#include <cstdlib>

    ImageryComponent::ImageryComponent() :
        d_imagePack(0),
        d_vertFormatting(VF_TOP_ALIGNED),
        d_horzFormatting(HF_LEFT_ALIGNED)
    {}
	ImageryComponent::~ImageryComponent()
	{
		if (d_imagePack)
		{
			delete d_imagePack ;
		}
	}
	ImageryComponent::ImageryComponent(const ImageryComponent& rhs): d_imagePack(0),
		d_vertFormatting(VF_TOP_ALIGNED),
		d_horzFormatting(HF_LEFT_ALIGNED)
	{
		ImagePackage * pPack=rhs.getImagePackage();
		if (pPack)
		{
			setImagePackage(pPack->d_imagetset,pPack->d_image);
		}
		else
		{
			d_imagePack = 0;
		}
		//area
		setComponentArea(rhs.d_area);
		//propertyname
		setVertFormattingPropertySource(rhs.d_vertFormatPropertyName);
		setHorzFormattingPropertySource(rhs.d_horzFormatPropertyName);
		setOwnerWidget(rhs.d_WidgetLookOwner);
		//property
		setHorizontalFormatting(rhs.d_horzFormatting);
		setVerticalFormatting(rhs.d_vertFormatting);
		//imageproperty
		setImagePropertySource(rhs.getImagePropertySource());
	}
	ImagePackage* ImageryComponent::getImagePackage(void) const
	{
		return d_imagePack;
	}
    const Image* ImageryComponent::getImage() const
    {
		if (!d_imagePack)
		{
			return NULL;
		}
		
		Imageset * imgSet =ImagesetManager::getSingleton().get(d_imagePack->d_imagetset);
		if (imgSet)
		{
			return imgSet->getImage(d_imagePack->d_image);
		} 
		else
		{
			return NULL;
		}
    }

    void ImageryComponent::setImagePackage(ImagePackage* image)
    {
        d_imagePack = image;
    }

    void ImageryComponent::setImagePackage(const CLeeString& imageset, const CLeeString& image)
    {
		if (d_imagePack)
		{
			delete d_imagePack ;
		}
		
        try
        {
            d_imagePack =new ImagePackage ;
			d_imagePack->d_image=image;
			d_imagePack->d_imagetset=imageset;
        }
       catch (...)
        {
            d_imagePack = 0;
        }
    }

    VerticalFormatting ImageryComponent::getVerticalFormatting() const
    {
        return d_vertFormatting;
    }

    void ImageryComponent::setVerticalFormatting(VerticalFormatting fmt)
    {
        d_vertFormatting = fmt;
    }

    HorizontalFormatting ImageryComponent::getHorizontalFormatting() const
    {
        return d_horzFormatting;
    }

    void ImageryComponent::setHorizontalFormatting(HorizontalFormatting fmt)
    {
        d_horzFormatting = fmt;
    }

    void ImageryComponent::render_impl(const RectF& PixelWnd, RectF& destRect, 
		const RectF* clipper, bool /*clipToDisplay*/,const PointF2D& PixelOffsetXY) const
    {
        // get final image to use.
        const Image* img =getImage();
		 HorizontalFormatting horzFormatting = d_horzFormatting;

        VerticalFormatting vertFormatting = d_vertFormatting;
		if (isImageFetchedFromProperty()||
			!d_vertFormatPropertyName.empty()||
			!d_horzFormatPropertyName.empty())
		{
			 CWidgetLook * widgetlook=WidgetLookManager::getSingleton().getWidgetLook(d_WidgetLookOwner);
			 if (widgetlook)
			 {
				CLeeString ff;
				ff=widgetlook->getProperty(d_imagePropertyName).c_str();
				img = isImageFetchedFromProperty()?PropertyHelper::stringToImage(widgetlook->getProperty(d_imagePropertyName)):getImage();
				 horzFormatting = d_horzFormatPropertyName.empty() ? d_horzFormatting :
				CLooksXMLHelper::stringToHorzFormat(widgetlook->getProperty(d_horzFormatPropertyName));
				vertFormatting = d_vertFormatPropertyName.empty() ? d_vertFormatting :
				CLooksXMLHelper::stringToVertFormat(widgetlook->getProperty(d_vertFormatPropertyName));
			 }
		}
        // do not draw anything if image is not set.
        if (!img)
            return;
        unsigned int horzTiles, vertTiles;
        float xpos, ypos;

        SizeF imgSz(img->getOrgSize());
        // calculate initial x co-ordinate and horizontal tile count according to formatting options
        switch (horzFormatting)
        {
            case HF_STRETCHED:
                imgSz.d_width = destRect.Width();
                xpos = destRect.d_left;
                horzTiles = 1;
                break;

            case HF_TILED:
                xpos = destRect.d_left;
                horzTiles = std::abs(static_cast<int>(
                    (destRect.Width() + (imgSz.d_width - 1)) / imgSz.d_width));
                break;

            case HF_LEFT_ALIGNED:
                xpos = destRect.d_left;
                horzTiles = 1;
                break;

            case HF_CENTRE_ALIGNED:
                xpos = destRect.d_left + PixelAligned((destRect.Width() - imgSz.d_width) * 0.5f);
                horzTiles = 1;
                break;

            case HF_RIGHT_ALIGNED:
                xpos = destRect.d_right - imgSz.d_width;
                horzTiles = 1;
                break;

            default:
              ;
        }

        // calculate initial y co-ordinate and vertical tile count according to formatting options
        switch (vertFormatting)
        {
            case VF_STRETCHED:
                imgSz.d_height = destRect.Height();
                ypos = destRect.d_top;
                vertTiles = 1;
                break;

            case VF_TILED:
                ypos = destRect.d_top;
                vertTiles = std::abs(static_cast<int>(
                    (destRect.Height() + (imgSz.d_height - 1)) / imgSz.d_height));
                break;

            case VF_TOP_ALIGNED:
                ypos = destRect.d_top;
                vertTiles = 1;
                break;

            case VF_CENTRE_ALIGNED:
                ypos = destRect.d_top + PixelAligned((destRect.Height() - imgSz.d_height) * 0.5f);
                vertTiles = 1;
                break;

            case VF_BOTTOM_ALIGNED:
                ypos = destRect.d_bottom - imgSz.d_height;
                vertTiles = 1;
                break;

            default:
                ;
        }

        // perform final rendering (actually is now a caching of the images which will be drawn)
        RectF finalRect;
        RectF finalClipper;
        const RectF* clippingRect;
        finalRect.d_top = ypos;
        finalRect.d_bottom = ypos + imgSz.d_height;

        for (unsigned int row = 0; row < vertTiles; ++row)
        {
            finalRect.d_left = xpos;
            finalRect.d_right = xpos + imgSz.d_width;

            for (unsigned int col = 0; col < horzTiles; ++col)
            {
                // use custom clipping for right and bottom edges when tiling the imagery
                if (((vertFormatting == VF_TILED) && row == vertTiles - 1) ||
                    ((horzFormatting == HF_TILED) && col == horzTiles - 1))
                {
                    finalClipper = clipper ? clipper->Intersection(destRect) : destRect;
                    clippingRect = &finalClipper;
                }
                // not tiliing, or not on far edges, just used passed in clipper (if any).
                else
                {
                    clippingRect = clipper;
                }

                // add geometry for image to the target window.
                img->draw( finalRect, clippingRect,PixelOffsetXY);

                finalRect.d_left += imgSz.d_width;
                finalRect.d_right += imgSz.d_width;
            }

            finalRect.d_top += imgSz.d_height;
            finalRect.d_bottom += imgSz.d_height;
        }
    }

    

    bool ImageryComponent::isImageFetchedFromProperty() const
    {
        return !d_imagePropertyName.empty();
    }

    const CLeeString& ImageryComponent::getImagePropertySource() const
    {
        return d_imagePropertyName;
    }

    void ImageryComponent::setImagePropertySource(const CLeeString& property)
    {
        d_imagePropertyName = property;
    }
