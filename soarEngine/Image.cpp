
#include "Image.h"
#include "Imageset.h"
#include "Propertys/PropertyHelper.h"
#include <cmath>
#include <iostream>

/*************************************************************************
	Constructor
*************************************************************************/
Image::Image( Imageset* owner, const CLeeString& name, 
			 const RectF& area, const PointF2D& render_offset, float horzScaling, float vertScaling) :
	d_owner(owner),
	d_area(area),
	d_offset(render_offset),
	d_name(name)
{
	
	// setup initial image scaling
	setHorzScaling(horzScaling);
	setVertScaling(vertScaling);

}

/*************************************************************************
	Copy constructor
*************************************************************************/
Image::Image(const Image& image) :
	d_owner(image.d_owner),
	d_area(image.d_area),
	d_offset(image.d_offset),
	d_scaledWidth(image.d_scaledWidth),
	d_scaledHeight(image.d_scaledHeight),
	d_scaledOffset(image.d_scaledOffset),
	d_name(image.d_name)
{
}
Image& Image::operator=(const Image& other)
{
	if (this==&other)
	{
		return *this;
	}
	d_name=other.d_name;
	d_offset=other.d_offset;
	d_owner =other.d_owner;
	d_area =other.d_area;
	d_scaledHeight =other.d_scaledHeight;
	d_scaledWidth=other.d_scaledWidth;
	d_scaledOffset=other.d_scaledOffset;
	return *this;
}
/*************************************************************************
	destructor
*************************************************************************/
Image::~Image(void)
{
}


/*************************************************************************
	set the horizontal scaling factor to be applied to this Image
*************************************************************************/
void Image::setHorzScaling(float factor)
{
	d_scaledWidth		= PixelAligned(d_area.Width() * factor);
	d_scaledOffset.d_x	= PixelAligned(d_offset.d_x * factor);
}


/*************************************************************************
	set the vertical scaling factor to be applied to this Image
*************************************************************************/
void Image::setVertScaling(float factor)
{
	d_scaledHeight		= PixelAligned(d_area.Height() * factor);
	d_scaledOffset.d_y	= PixelAligned(d_offset.d_y * factor);
}


/*************************************************************************
    Clip and then queue the image to be rendered.
*************************************************************************/
void Image::draw( const RectF& dest_rect,
    const RectF* clip_rect,const PointF2D& PixelOffsetXY) const
{
    RectF dest(dest_rect);

 

    // draw
    d_owner->draw( dest/*最终要渲染的区域*/,d_area/*图像区域*/, clip_rect/*windows控件区域*/,PixelOffsetXY);


}
/*************************************************************************
	CLeeString object containing the name of this Image	
*************************************************************************/
const CLeeString& Image::getName(void) const
{
	return d_name;
}


/*************************************************************************
	Return the name of the Imageset that contains this Image	
*************************************************************************/
const CLeeString& Image::getImagesetName(void) const
{
	return d_owner->getName();
}

/*************************************************************************
	Return Rect describing the source texture area used by this Image.
*************************************************************************/
const RectF& Image::getSourceTextureArea(void) const
{
    return d_area;
}
/*************************************************************************
	依据控件窗口
*************************************************************************/
void Image::FrameUpdate(const RectF& destRect)
{
	float factorX =destRect.Width()/d_area.Width();
	float factorY =destRect.Height()/d_area.Height();
	setHorzScaling(factorX);
	setVertScaling(factorY);
}