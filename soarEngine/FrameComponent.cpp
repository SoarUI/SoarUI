
#include "FrameComponent.h"
#include "SoarXMLHelper.h"
#include "Image.h"
#include "ImagesetManager.h"
#include "Imageset.h"
#include "widgetslook/WidgetLook.h"
#include "widgetslook/WidgetLookManager.h"
#include <iostream>
#include <cstdlib>

    FrameComponent::FrameComponent() :
        d_vertFormatting(VF_STRETCHED),
        d_horzFormatting(HF_STRETCHED)
    {
        for (int i = 0; i < FIC_FRAME_IMAGE_COUNT; ++i)
            d_frameImages[i] = 0;
    }
	FrameComponent::~FrameComponent()
	{
		for (int i = 0; i < FIC_FRAME_IMAGE_COUNT; i++)
		{
			ImagePackage * pPack=d_frameImages[i];
			delete  pPack;
			d_frameImages[i] = 0;
		}
	}
	FrameComponent::FrameComponent(const FrameComponent& rhs)
	{
		for (int i = FIC_BACKGROUND; i < FIC_FRAME_IMAGE_COUNT; i++)
		{
			const ImagePackage * pPack=rhs.getImagePackage((FrameImageComponent)i);
			if (pPack)
			{
				setImagePackage((FrameImageComponent)i,pPack->d_imagetset,pPack->d_image);
			}
			else
			{
				d_frameImages[i] = 0;
			}
		}
		//area
		setComponentArea(rhs.d_area);
		//propertyname
		setVertFormattingPropertySource(rhs.d_vertFormatPropertyName);
		setHorzFormattingPropertySource(rhs.d_horzFormatPropertyName);
		setOwnerWidget(rhs.d_WidgetLookOwner);
		//property
		setBackgroundHorizontalFormatting(rhs.d_horzFormatting);
		setBackgroundVerticalFormatting(rhs.d_vertFormatting);
	}
    VerticalFormatting FrameComponent::getBackgroundVerticalFormatting() const
    {
        return d_vertFormatting;
    }

    void FrameComponent::setBackgroundVerticalFormatting(VerticalFormatting fmt)
    {
        d_vertFormatting = fmt;
    }

    HorizontalFormatting FrameComponent::getBackgroundHorizontalFormatting() const
    {
        return d_horzFormatting;
    }

    void FrameComponent::setBackgroundHorizontalFormatting(HorizontalFormatting fmt)
    {
        d_horzFormatting = fmt;
    }

    const ImagePackage* FrameComponent::getImagePackage(FrameImageComponent part) const
    {
        assert(part < FIC_FRAME_IMAGE_COUNT);

        return d_frameImages[part];
    }

    void FrameComponent::setImagePackage(FrameImageComponent part,  ImagePackage* image)
    {
        assert(part < FIC_FRAME_IMAGE_COUNT);

        d_frameImages[part] = image;
    }

    void FrameComponent::setImagePackage(FrameImageComponent part, const CLeeString& imageset, const CLeeString& image)
    {
        assert(part < FIC_FRAME_IMAGE_COUNT);

        try
        {
			ImagePackage * pPackage=new ImagePackage();
			pPackage->d_image=image;
			pPackage->d_imagetset=imageset;
            d_frameImages[part] = pPackage;
        }
        catch(...)
        {
            d_frameImages[part] = 0;
        }
    }
	Image* FrameComponent::getImage(FrameImageComponent part) const
	{
		assert(part < FIC_FRAME_IMAGE_COUNT);
		ImagePackage * package =d_frameImages[part];
		if (package)
		{
			Imageset * imgSet =ImagesetManager::getSingleton().get(package->d_imagetset);
			if (imgSet)
			{
				return imgSet->getImage(package->d_image);
			} 
			else
			{
				return NULL;
			}
		} 
		else
		{
			return NULL;
		}
	}
    void FrameComponent::render_impl(const RectF& ControlRect, RectF& destRect,  
		const RectF* clipper/*经过剪切的最终渲染区域*/, bool clipToDisplay,const PointF2D& PixelOffsetXY) const
    {
		/*
		     *     *       *      *
              t-l-c   t      t-r-c
			 *     *       *      *
               l              r
			 *     *       *      *
               b-l-c   b      b-r-c
			 *     *       *      *
		*/
        RectF backgroundRect(destRect);
        RectF finalRect;
        SizeF imageSize;
        VectorF2 imageOffsets;
        float leftfactor, rightfactor, topfactor, bottomfactor;
      
        // vars we use to track what to do with the side pieces.
        float topOffset/*X偏移*/ = 0, bottomOffset/*底部X偏移*/ = 0,
			leftOffset/*Y偏移*/ = 0, rightOffset/*Y偏移*/ = 0;
        float topWidth, bottomWidth, leftHeight, rightHeight;
        topWidth/*顶宽度【X】用于计算因子*/ = bottomWidth = destRect.Width();
        leftHeight = rightHeight = destRect.Height();
        //中间变量
		Image * lpImage =NULL ;
        // top-left image左上角
		lpImage = getImage(FIC_TOP_LEFT_CORNER);
        if (d_frameImages[FIC_TOP_LEFT_CORNER]&&
			lpImage)
		{
            // calculate final destination area
            imageSize = lpImage->getOrgSize();
            imageOffsets = lpImage->getOffsets();
            finalRect.d_left = destRect.d_left;
            finalRect.d_top  = destRect.d_top;
            finalRect.SetSize(imageSize);
            finalRect = destRect.Intersection (finalRect);//求出左上角区块

            // update adjustments required to edges do to presence of this element.
            topOffset  += imageSize.d_width + imageOffsets.d_x;
            leftOffset += imageSize.d_height + imageOffsets.d_y;
            topWidth   -= topOffset;
            leftHeight -= leftOffset;

            // draw this element.
            lpImage->draw( finalRect, clipper,PixelOffsetXY);
        }

        // top-right image
		lpImage = getImage(FIC_TOP_RIGHT_CORNER);
        if (d_frameImages[FIC_TOP_RIGHT_CORNER]&&lpImage)
        {
            // calculate final destination area
            imageSize = lpImage->getOrgSize();
            imageOffsets = lpImage->getOffsets();
            finalRect.d_left = destRect.d_right - imageSize.d_width;
            finalRect.d_top  = destRect.d_top;
            finalRect.SetSize(imageSize);
            finalRect = destRect.Intersection (finalRect);

            // update adjustments required to edges do to presence of this element.
            rightOffset += imageSize.d_height + imageOffsets.d_y;
            topWidth    -= imageSize.d_width - imageOffsets.d_x;
            rightHeight -= rightOffset;

            // draw this element.
            lpImage->draw(finalRect, clipper,PixelOffsetXY);
        }

        // bottom-left image
		lpImage = getImage(FIC_BOTTOM_LEFT_CORNER);
        if (d_frameImages[FIC_BOTTOM_LEFT_CORNER]&&lpImage)
        {
            // calculate final destination area
            imageSize = lpImage->getOrgSize();
            imageOffsets = lpImage->getOffsets();
            finalRect.d_left = destRect.d_left;
            finalRect.d_top  = destRect.d_bottom - imageSize.d_height;
            finalRect.SetSize(imageSize);
            finalRect = destRect.Intersection (finalRect);

            // update adjustments required to edges do to presence of this element.
            bottomOffset += imageSize.d_width + imageOffsets.d_x;
            bottomWidth  -= bottomOffset;
            leftHeight   -= imageSize.d_height - imageOffsets.d_y;

           

            // draw this element.
            lpImage->draw( finalRect, clipper,PixelOffsetXY);
        }

        // bottom-right image
		lpImage = getImage(FIC_BOTTOM_RIGHT_CORNER);
        if (d_frameImages[FIC_BOTTOM_RIGHT_CORNER]&&lpImage)
        {
            // calculate final destination area
            imageSize = lpImage->getOrgSize();
            imageOffsets = lpImage->getOffsets();
            finalRect.d_left = destRect.d_right - imageSize.d_width;
            finalRect.d_top  = destRect.d_bottom - imageSize.d_height;
            finalRect.SetSize(imageSize);
            finalRect = destRect.Intersection (finalRect);

            // update adjustments required to edges do to presence of this element.
            bottomWidth -= imageSize.d_width - imageOffsets.d_x;
            rightHeight -= imageSize.d_height - imageOffsets.d_y;

            // draw this element.
            lpImage->draw( finalRect, clipper,PixelOffsetXY);
        }
		/*****************************************************************
		******************************************************************/
        // top image
		lpImage = getImage(FIC_TOP_EDGE);
        if (d_frameImages[FIC_TOP_EDGE] && lpImage)
        {
			//update the scales
			topfactor = topWidth /lpImage->getOrgWidth();
			lpImage->setVertScaling(1.0f);
			lpImage->setHorzScaling(topfactor);
            // calculate final destination area
            imageSize = lpImage->getSize();
            finalRect.d_left   = destRect.d_left + topOffset;
            finalRect.d_right  = finalRect.d_left + topWidth;
            finalRect.d_top    = destRect.d_top;
            finalRect.d_bottom = finalRect.d_top + imageSize.d_height;
            finalRect = destRect.Intersection (finalRect);

            // adjust background area to miss this edge
            backgroundRect.d_top += imageSize.d_height + lpImage->getOffsetY();

            // draw this element.
            lpImage->draw( finalRect, clipper,PixelOffsetXY);
        }

        // bottom image
		lpImage = getImage(FIC_BOTTOM_EDGE);
        if (d_frameImages[FIC_BOTTOM_EDGE] && lpImage)
        {
			//Update the scales h-v
			bottomfactor = bottomWidth /lpImage->getOrgWidth();
			lpImage->setVertScaling(1.0f);
			lpImage->setHorzScaling(bottomfactor);
            // calculate final destination area
            imageSize = lpImage->getSize();
            finalRect.d_left   = destRect.d_left + bottomOffset;
            finalRect.d_right  = finalRect.d_left + bottomWidth;
            finalRect.d_bottom = destRect.d_bottom;
            finalRect.d_top    = finalRect.d_bottom - imageSize.d_height;
            finalRect = destRect.Intersection (finalRect);

            // adjust background area to miss this edge
            backgroundRect.d_bottom -= imageSize.d_height - lpImage->getOffsetY();;

            
            // draw this element.
           lpImage->draw( finalRect, clipper,PixelOffsetXY);
        }

        // left image
		lpImage = getImage(FIC_LEFT_EDGE);
        if (d_frameImages[FIC_LEFT_EDGE] && lpImage)
        {
			//Update the scales h-v
			leftfactor = leftHeight /lpImage->getOrgHeight();
			lpImage->setVertScaling(leftfactor);
			lpImage->setHorzScaling(1.0f);
            // calculate final destination area
            imageSize = lpImage->getSize();
            finalRect.d_left   = destRect.d_left;
            finalRect.d_right  = finalRect.d_left + imageSize.d_width;
            finalRect.d_top    = destRect.d_top + leftOffset;
            finalRect.d_bottom = finalRect.d_top + leftHeight;
            finalRect = destRect.Intersection (finalRect);

            // adjust background area to miss this edge
            backgroundRect.d_left += imageSize.d_width + lpImage->getOffsetX();

            // draw this element.
            lpImage->draw( finalRect, clipper,PixelOffsetXY);
        }

        // right image
		lpImage =getImage(FIC_RIGHT_EDGE);
        if (d_frameImages[FIC_RIGHT_EDGE]&& lpImage)
        {
			//Update the scales h-v
			rightfactor = rightHeight /lpImage->getOrgHeight();
			lpImage->setVertScaling(rightfactor);
			lpImage->setHorzScaling(1.0f);
            // calculate final destination area
            imageSize = lpImage->getSize();
            finalRect.d_top    = destRect.d_top + rightOffset;
            finalRect.d_bottom = finalRect.d_top + rightHeight;
            finalRect.d_right  = destRect.d_right;
            finalRect.d_left   = finalRect.d_right - imageSize.d_width;
            finalRect = destRect.Intersection (finalRect);

            // adjust background area to miss this edge
            backgroundRect.d_right -= imageSize.d_width - lpImage->getOffsetX();

           
            // draw this element.
            lpImage->draw( finalRect, clipper,PixelOffsetXY);
        }

		if (d_frameImages[FIC_BACKGROUND])
		{
			// render background image.
			doBackgroundRender(ControlRect, backgroundRect,  clipper, clipToDisplay,PixelOffsetXY);
		}
    }

    void FrameComponent::doBackgroundRender(const RectF& ControlRect, RectF& destRect, 
		const RectF* clipper, bool clipToDisplay,const PointF2D& PixelOffsetXY) const
    {
        HorizontalFormatting horzFormatting = d_horzFormatting;
        VerticalFormatting vertFormatting = d_vertFormatting; 
        if (!d_vertFormatPropertyName.empty()||
			!d_horzFormatPropertyName.empty())
        {
			CWidgetLook * widgetlook=WidgetLookManager::getSingleton().getWidgetLook(d_WidgetLookOwner);
			if (widgetlook)
			{
				horzFormatting = d_horzFormatPropertyName.empty() ? d_horzFormatting :
							CLooksXMLHelper::stringToHorzFormat(widgetlook->getProperty(d_horzFormatPropertyName));
					vertFormatting = d_vertFormatPropertyName.empty() ? d_vertFormatting :
							CLooksXMLHelper::stringToVertFormat(widgetlook->getProperty(d_vertFormatPropertyName));
			}
        }
        
        unsigned int horzTiles, vertTiles;
        float xpos, ypos;
        const Image * lpImage =getImage(FIC_BACKGROUND);
		if (!lpImage)
		{
			return ;
		}
		
        SizeF imgSz(lpImage->getOrgSize());

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
                throw(_T("FrameComponent::doBackgroundRender - An unknown VerticalFormatting value was specified."));
        }

        // perform final rendering (actually is now a caching of the images which will be drawn)
        RectF finalRect;
        RectF finalClipper;
        const RectF* clippingRect;
        finalRect.d_top = ypos;
        finalRect.d_bottom = ypos + imgSz.d_height;

        for (unsigned int  row = 0; row < vertTiles; ++row)
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

                // add image to the rendering cache for the target window.
                lpImage->draw( finalRect, clippingRect,PixelOffsetXY);

                finalRect.d_left += imgSz.d_width;
                finalRect.d_right += imgSz.d_width;
            }

            finalRect.d_top += imgSz.d_height;
            finalRect.d_bottom += imgSz.d_height;
        }
    }