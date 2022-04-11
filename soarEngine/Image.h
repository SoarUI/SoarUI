
#ifndef _Image_h_
#define _Image_h_
#include "../SoarHeader/leeString.h"
#include "../SoarHeader/RectF.h"
#include <map>
#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4251)
#endif
class Imageset;
class  Image
{
public:
	Image(void) {}
	Image( Imageset* owner, const CLeeString& name, const RectF& area,
		const PointF2D& render_offset, float horzScaling = 1.0f, float vertScaling = 1.0f);
	Image(const Image& image);
	Image& operator=(const Image& other);
	~Image(void);

	const CLeeString&	getName(void) const;
	const CLeeString&	getImagesetName(void) const;
	const class Imageset*	getImageset(void) const {return d_owner;}
	const RectF& getSourceTextureArea(void) const;
	friend class std::map<CLeeString, Image, CLeeString::FastLessCompare>;
	friend struct std::pair<const CLeeString, Image>;
	
	SizeF	getOrgSize(void) const			{ return SizeF(d_area.Width(), d_area.Height()); }
	float	getOrgWidth(void) const		{   return d_area.Width(); }
	float	getOrgHeight(void) const		{ return d_area.Height(); }
	SizeF	getSize(void) const			{ return SizeF(d_scaledWidth, d_scaledHeight); }
	float	getWidth(void) const		{ return d_scaledWidth; }
	float	getHeight(void) const		{ return d_scaledHeight; }
	PointF2D	getOffsets(void) const		{ return d_scaledOffset; }
	float	getOffsetX(void) const		{ return	d_scaledOffset.d_x; }
	float	getOffsetY(void) const		{ return d_scaledOffset.d_y; }
	//更新窗体比例
	void FrameUpdate(const RectF& destRect);
	/*************************************************************************
		Implementation Methods
	*************************************************************************/
	void	setHorzScaling(float factor);
	void	setVertScaling(float factor);

 void draw( const VectorF2& position, const SizeF& size,
              const RectF* clip_rect,const PointF2D& PixelOffsetXY
              ) const
    {
        draw( RectF(position.d_x, position.d_y,
                          position.d_x + size.d_width,
                          position.d_y + size.d_height),
             clip_rect,PixelOffsetXY);
    }
    void draw( const RectF& dest_rect,
              const RectF* clip_rect,const PointF2D& PixelOffsetXY) const;
    void draw( const VectorF2& position,
              const RectF* clip_rect,const PointF2D& PixelOffsetXY) const
    {
        draw( RectF(position.d_x, position.d_y,
                          position.d_x + getWidth(),
                          position.d_y + getHeight()),
             clip_rect, PixelOffsetXY);
    }
   
private:
	/*************************************************************************
		Friends
	*************************************************************************/
	friend class Imageset;
	
	/*************************************************************************
		Implementation Data
	*************************************************************************/
	Imageset*	d_owner;		//!< Link back to Imageset that owns this image
	RectF		d_area;			//图像占据的矩形区域
	PointF2D	d_offset;		//!< Offset to use when rendering
	float	d_orgWidth;		//原始宽度
	float	d_orgdHeight;		//原始高度

	// 自动缩放功能，不要依赖它的值，实时变化的
	float	    d_scaledWidth;		//!< scaled image width.
	float	    d_scaledHeight;		//!< scaled image height.
	PointF2D	d_scaledOffset;		//!< scaled rendering offset.
	CLeeString	d_name;				//!< name of this image.
	
};


#if defined(_MSC_VER)
#	pragma warning(pop)
#endif

#endif	// end of guard _Image_h_
