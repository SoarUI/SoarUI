
#include "ImagerySection.h"
#include "Propertys/PropertyHelper.h"
#include <iostream>
    ImagerySection::ImagerySection() 
    {}

    ImagerySection::ImagerySection(const CLeeString& name) :
        d_name(name)
    {}
		ImagerySection::~ImagerySection(void)
		{
			clearImageryComponents();
			clearTextComponents();
			clearFrameComponents();
		}
    void ImagerySection::render(const RectF& ControlRect,const PointF2D& PixelOffsetXY,const CLeeString& sztext,int vertTextFmt,int horzTextFmt,
		const RectF* clipper, bool clipToDisplay) 
    {
	
        //œ»”√æ≈π¨ÕºªÊ÷∆øÚº‹
        for(FrameList::const_iterator frame = d_frames.begin(); frame != d_frames.end(); ++frame)
        {
            (*frame).render(ControlRect,  PixelOffsetXY, clipper, clipToDisplay);
        }
		
        //‰÷»æÕºœÒ≤„
        for(ImageryList::const_iterator image = d_images.begin(); image != d_images.end(); ++image)
        {
            (*image).render(ControlRect,  PixelOffsetXY,clipper, clipToDisplay);
        }
        // render all text components in this section
        for(TextList::iterator text = d_texts.begin(); text != d_texts.end(); ++text)
        {
            (*text).renderT(ControlRect,PixelOffsetXY ,sztext,vertTextFmt,horzTextFmt,clipper, clipToDisplay);
        }
    }
	void ImagerySection::render(const RectF& ControlRect,const PointF2D& PixelOffsetXY,
		const RectF* clipper, bool clipToDisplay) 
	{

		//œ»”√æ≈π¨ÕºªÊ÷∆øÚº‹
		for(FrameList::const_iterator frame = d_frames.begin(); frame != d_frames.end(); ++frame)
		{
			(*frame).render(ControlRect,  PixelOffsetXY, clipper, clipToDisplay);
		}

		//‰÷»æÕºœÒ≤„
		for(ImageryList::const_iterator image = d_images.begin(); image != d_images.end(); ++image)
		{
			(*image).render(ControlRect,  PixelOffsetXY,clipper, clipToDisplay);
		}
	}
	void ImagerySection::render(const RectF& ControlRect, const RectF& RelativebaseRect,
		const PointF2D& PixelOffsetXY,
		const RectF* clipper, bool clipToDisplay) 
	{

		// render all frame components in this section
		for(FrameList::const_iterator frame = d_frames.begin(); frame != d_frames.end(); ++frame)
		{
			(*frame).render(ControlRect, RelativebaseRect, PixelOffsetXY, clipper, clipToDisplay);
		}
		// render all image components in this section
		for(ImageryList::const_iterator image = d_images.begin(); image != d_images.end(); ++image)
		{
			(*image).render(ControlRect, RelativebaseRect, PixelOffsetXY,clipper, clipToDisplay);
		}
		// render all text components in this section
		for(TextList::iterator text = d_texts.begin(); text != d_texts.end(); ++text)
		{
		    (*text).render(ControlRect, RelativebaseRect,PixelOffsetXY,  clipper, clipToDisplay);
		}
	}
    void ImagerySection::addImageryComponent( ImageryComponent& img)
    {
        d_images.push_back(img);
    }

    void ImagerySection::clearImageryComponents()
    {
        d_images.clear();
    }

    void ImagerySection::addTextComponent(TextComponent& text)
    {
        d_texts.push_back(text);
    }

    void ImagerySection::clearTextComponents()
    {
        d_texts.clear();
    }

    void ImagerySection::clearFrameComponents()
    {
        d_frames.clear();
    }

    void ImagerySection::addFrameComponent(FrameComponent& frame)
    {
        d_frames.push_back(frame);
    }
    const CLeeString& ImagerySection::getName() const
    {
        return d_name;
    }
    RectF ImagerySection::getBoundingRect(const RectF& ControlRect) const
    {
        RectF compRect;
        RectF bounds(0, 0, 0, 0);

        // measure all frame components
        for(FrameList::const_iterator frame = d_frames.begin(); frame != d_frames.end(); ++frame)
        {
            compRect = (*frame).getComponentArea().getRect(ControlRect);

            bounds.d_left   = min(bounds.d_left, compRect.d_left);
            bounds.d_top    = min(bounds.d_top, compRect.d_top);
			bounds.d_right  = max(bounds.d_right, compRect.d_right);
			bounds.d_bottom = max(bounds.d_bottom, compRect.d_bottom);
        }
        // measure all imagery components
        for(ImageryList::const_iterator image = d_images.begin(); image != d_images.end(); ++image)
        {
            compRect = (*image).getComponentArea().getRect(ControlRect);

            bounds.d_left   =min(bounds.d_left, compRect.d_left);
            bounds.d_top    = min(bounds.d_top, compRect.d_top);
			bounds.d_right  = max(bounds.d_right, compRect.d_right);
			bounds.d_bottom = max(bounds.d_bottom, compRect.d_bottom);
        }
        //// measure all text components
        for(TextList::const_iterator text = d_texts.begin(); text != d_texts.end(); ++text)
        {
            compRect = (*text).getComponentArea().getRect(ControlRect);

            bounds.d_left   = min(bounds.d_left, compRect.d_left);
            bounds.d_top    = min(bounds.d_top, compRect.d_top);
            bounds.d_right  = max(bounds.d_right, compRect.d_right);
            bounds.d_bottom = max(bounds.d_bottom, compRect.d_bottom);
        }

        return bounds;
    }
	 RectF ImagerySection::getBoundingRect( const RectF& ControlRect, const RectF& RelativeRect) const
    {
        RectF compRect;
        RectF bounds(0, 0, 0, 0);

        // measure all frame components
        for(FrameList::const_iterator frame = d_frames.begin(); frame != d_frames.end(); ++frame)
        {
            compRect = (*frame).getComponentArea().getRect(ControlRect, RelativeRect);

            bounds.d_left   = min(bounds.d_left, compRect.d_left);
            bounds.d_top    = min(bounds.d_top, compRect.d_top);
            bounds.d_right  = max(bounds.d_right, compRect.d_right);
            bounds.d_bottom = max(bounds.d_bottom, compRect.d_bottom);
        }
        // measure all imagery components
        for(ImageryList::const_iterator image = d_images.begin(); image != d_images.end(); ++image)
        {
            compRect = (*image).getComponentArea().getRect(ControlRect, RelativeRect);

            bounds.d_left   = min(bounds.d_left, compRect.d_left);
            bounds.d_top    = min(bounds.d_top, compRect.d_top);
			bounds.d_right  = max(bounds.d_right, compRect.d_right);
            bounds.d_bottom = max(bounds.d_bottom, compRect.d_bottom);
        }
        // measure all text components
        for(TextList::const_iterator text = d_texts.begin(); text != d_texts.end(); ++text)
        {
            compRect = (*text).getComponentArea().getRect(ControlRect, RelativeRect);

            bounds.d_left   = min(bounds.d_left, compRect.d_left);
            bounds.d_top    = min(bounds.d_top, compRect.d_top);
            bounds.d_right  = max(bounds.d_right, compRect.d_right);
            bounds.d_bottom = max(bounds.d_bottom, compRect.d_bottom);
        }

        return bounds;
    }