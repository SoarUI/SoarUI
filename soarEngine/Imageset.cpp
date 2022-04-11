
#include "Imageset.h"
#include "XMLHandler/Imageset_xmlHandler.h"
#include "Propertys/PropertyHelper.h"
#include <iostream>
#include <cmath>
#include "texture/TextureManager.h"
#include "Theme/WidgetThemeManager.h"
#include "ResourcesManager.h"
#include "../SoarHeader/ILeeRender.h"
/*************************************************************************
	Definition of static /const data for Imageset (and sub-classes)
*************************************************************************/
SizeF	Imageset::getImageSize(const CLeeString& name) const			
{
	const Image* img=getImage(name);
	if (img)
	{
		return img->getSize();
	}
	return SizeF(0,0);
}
float Imageset::getImageWidth(const CLeeString& name) const			
{
	const Image* img=getImage(name);
	if (img)
	{
		return img->getWidth();
	}
	return 0.0f;
}
float	Imageset::getImageHeight(const CLeeString& name) const		
{
	const Image* img=getImage(name);
	if (img)
	{
		return img->getHeight();
	}
	return 1.0f;
}
PointF2D	Imageset::getImageOffset(const CLeeString& name) const		
{
	const Image* img=getImage(name);
	if (img)
	{
		return img->getOffsets();
	}
	return PointF2D(0.1f,0.1f);
}
float	Imageset::getImageOffsetX(const CLeeString& name) const		
{
	const Image* img=getImage(name);
	if (img)
	{
		return img->getOffsetX();
	}
	return 0.0f;
}
float	Imageset::getImageOffsetY(const CLeeString& name) const		
{
	const Image* img=getImage(name);
	if (img)
	{
		return img->getOffsetY();
	}
	return 0.0f;
}
/*************************************************************************
	constructor
*************************************************************************/
Imageset::Imageset(void)
{

}

Imageset::Imageset(const CLeeString& name) :
    d_name(name)
{
 
}

	Imageset::Imageset(const Imageset& rhs):d_name(rhs.d_name),
		d_images(rhs.d_images)
{
	;
}
Imageset& Imageset::operator=(const Imageset& other)
{
	if (this==&other)
	{
		return *this ;
	}
	d_images=other.d_images;
	d_name=other.d_name;
	return *this ;
}
/*************************************************************************
	destructor
*************************************************************************/
Imageset::~Imageset(void)
{
	unload();
}
/*************************************************************************
	return the Image object for the named image
*************************************************************************/
 Image* Imageset::getImage( const CLeeString& name) 
{
	ImageRegistry::iterator	pos = d_images.find(name);

	if (pos == d_images.end())
	{
		return NULL;
	}

	return &(pos->second);
}
const Image* Imageset::getImage(const CLeeString& name) const
{
	ImageRegistry::const_iterator	pos = d_images.find(name);

	if (pos == d_images.end())
	{
		return NULL;
	}

	return &(pos->second);
}

/*************************************************************************
	defines a new Image.
*************************************************************************/
void Imageset::defineImage(const CLeeString& name, const RectF& image_rect, const PointF2D& render_offset)
{
	if (isImageDefined(name))
	{
		return ;
	}

	// get scaling factors
	float hscale =  1.0f;
	float vscale =  1.0f;

	// add the Image definition
	d_images[name] = Image(this, name, image_rect, render_offset, hscale, vscale);
}


/*************************************************************************
    Queues an area of the associated Texture the be drawn on the screen.
    Low-level routine not normally used!
*************************************************************************/
void Imageset::draw( const RectF& render_rect,
    const RectF& image_rect, const RectF* clip_rect,const PointF2D& PixelOffsetXY) const
{
    // get the rect area that we will actually draw to (i.e. perform clipping)
    RectF final_rect(clip_rect? render_rect.Intersection(*clip_rect) : render_rect );

    // check if rect was totally clipped
    if ((final_rect.Width() == 0) || (final_rect.Height() == 0))
        return;

   
		//do the drawing here
		ILeeRender * blender =ResourcesManager::getSingletonPtr()->getBlender();
		if (blender)
		{
			blender->Render(final_rect,image_rect,NULL,PixelOffsetXY);
		}
}

/*************************************************************************
	Unload all data, leaving Imageset in a clean (but unusable) state
*************************************************************************/
void Imageset::unload(void)
{
	undefineAllImages();
}

void Imageset::undefineImage(const CLeeString& name)
{
	ImageRegistry::iterator pos=d_images.find(name);
	if (pos != d_images.end())
	{
		d_images.erase(name);
	}
}

void Imageset::undefineAllImages(void)
{
	d_images.clear();
}
