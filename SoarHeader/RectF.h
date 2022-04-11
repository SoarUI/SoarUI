
#ifndef _RectF_h_
#define _RectF_h_
/********************************************************************
    Descibe:      浮点矩形，包括与windows矩形相转换
    created:      2012/7/25 23:39
    FullName:     RectF.h
    Path:      
    ClassName:    RectF
    FileType:     h
    Author:       nicklisir
    NeededLib:    
    requires:      
*********************************************************************/
#	define PixelAligned(x)	( (float)(int)(( x ) + (( x ) > 0.0f ? 0.5f : -0.5f)) )
#include "VectorF.h"
#include "SizeF.h"
#include<windows.h>
class  RectF
{
public:
	RectF(void) ;
	RectF(float left, float top, float right, float bottom);
    RectF(PointF2D pos, SizeF sz);
	// from two points
	RectF(PointF2D topLeft, PointF2D bottomRight) ;
	//form windows platform
	RectF(LPRECT pWinRect);
	RectF(const RECT& srcRect);
	// from a point and size
	RectF(POINT point, SIZE size);
	// from two points
	RectF(POINT topLeft, POINT bottomRight) ;
	//to windows
	void toWindowRect(RECT &outRect)const;
	// Additional Operations
	void operator=(const RECT& srcRect) ;
	BOOL operator==(const RECT& rect) const ;
	BOOL operator!=(const RECT& rect) const ;
	void operator+=(POINT point) ;
	void operator+=(SIZE size) ;
	void operator+=(LPRECT lpRect) ;
	void operator-=(POINT point) ;
	void operator-=(SIZE size) ;
	void operator-=(LPCRECT lpRect) ;
	void operator&=(const RECT& rect) ;
	void operator|=(const RECT& rect) ;
    
	// Operators returning RectF values
	RectF& operator+(POINT point) ;
	RectF& operator-(POINT point)  ;
	RectF& operator+(LPRECT lpRect) ;
	RectF& operator+(SIZE size)  ;
	RectF& operator-(SIZE size)  ;
	RectF& operator-(LPRECT lpRect)  ;
	RectF& operator|(const RECT& rect2)  ;
	RectF	Intersection(const RECT& rect) ;
	//
	void SetRect(float x1, float y1, float x2, float y2) ;
	void SetRect(PointF2D topLeft, PointF2D bottomRight) ;

	PointF2D	GetTopLeft(void) const		{return PointF2D(d_left, d_top);}
	PointF2D CenterPoint() const throw();
	float	Width(void) const		{return d_right - d_left;}
	float	Height(void) const		{return d_bottom - d_top;}
	SizeF	Size(void) const			{return SizeF(Width(), Height());}
	void	SetTopLeft(const PointF2D& pt);
	void	SetWidth(float width)		{d_right = d_left + width;}
	void	SetHeight(float height)		{d_bottom = d_top + height;}
	void	SetSize(const SizeF& sze)	{SetWidth(sze.d_width); SetHeight(sze.d_height);}
	RectF	Intersection(const RectF& rect) const;
	void	offsetRect(const PointF2D& pt);
	void	offsetRect(const float cx,const float cy);
	void    scale(const float rx,const float ry);
	bool	PtInRect(const PointF2D& pt) const;
	RectF&	constrainSizeMax(const SizeF& sz);
	RectF&	constrainSizeMin(const SizeF& sz);
	RectF&	constrainSize(const SizeF& max_sz, const SizeF& min_sz);


	/*************************************************************************
		Operators
	*************************************************************************/
	void operator&=(const RectF& frect) ;
	void operator|=(const RectF& rect) ;
	bool	operator==(const RectF& rhs) const
	{
		return ((d_left == rhs.d_left) && (d_right == rhs.d_right) && (d_top == rhs.d_top) && (d_bottom == rhs.d_bottom));
	}
	bool	operator!=(const RectF& rhs) const		{return !operator==(rhs);}
	RectF&	operator=(const RectF& rhs);
    RectF operator*(float scalar) const      { return RectF(d_left * scalar, d_top * scalar, d_right * scalar, d_bottom * scalar); }
    const RectF& operator*=(float scalar)    { d_left *= scalar; d_top *= scalar; d_right *= scalar; d_bottom *= scalar; return *this; }
	RectF operator+(const RectF& r) const		{ return RectF(d_left + r.d_left, d_top + r.d_top, d_right + r.d_right, d_bottom + r.d_bottom); }
	//.....
	/*************************************************************************
		Data Fields
	*************************************************************************/
	float	d_left, d_top, d_right, d_bottom;
};
inline RectF::RectF(void):
d_top(0.0f),d_left(0.0f),d_right(1.0f),d_bottom(1.0f)
{

}
inline RectF::RectF(float left, float top, float right, float bottom) :
	d_top(top),
	d_bottom(bottom),
	d_left(left),
	d_right(right)
{
}

inline RectF::RectF(PointF2D pos, SizeF sz) :
    d_top(pos.d_y),
    d_bottom(pos.d_y + sz.d_height),
    d_left(pos.d_x),
    d_right(pos.d_x + sz.d_width)
{
}
inline void RectF::SetRect(float x1, float y1, float x2, float y2)
{
	d_top =y1;
	d_bottom =y2 ;
	d_left =x1;
	d_right =x2;
}
inline void RectF::SetRect(PointF2D topLeft, PointF2D bottomRight)

{
	d_top = topLeft.d_y;
	d_left = topLeft.d_x;
	d_right = bottomRight.d_x;
	d_bottom =bottomRight.d_y;
}

/*************************************************************************
	Return a RectF object that is the intersection of 'this' with 'rect'
*************************************************************************/
inline RectF RectF::Intersection(const RectF& rect) const
{
	// check for total exclusion
	if ((d_right > rect.d_left) &&
		(d_left < rect.d_right) &&
		(d_bottom > rect.d_top) &&
		(d_top < rect.d_bottom))
	{
		RectF temp;

		// fill in temp with the intersection
		temp.d_left = (d_left > rect.d_left) ? d_left : rect.d_left;
		temp.d_right = (d_right < rect.d_right) ? d_right : rect.d_right;
		temp.d_top = (d_top > rect.d_top) ? d_top : rect.d_top;
		temp.d_bottom = (d_bottom < rect.d_bottom) ? d_bottom : rect.d_bottom;

		return temp;
	}
	else
	{
		return RectF(0.0f, 0.0f, 0.0f, 0.0f);
	}

}

/*************************************************************************
	Apply an offset the the RectF
*************************************************************************/
inline void RectF::offsetRect(const PointF2D& pt)
{
	d_left		+= pt.d_x;
	d_right		+= pt.d_x;
	d_top		+= pt.d_y;
	d_bottom	+= pt.d_y;
}
inline void RectF::offsetRect(const float cx,const float cy)
{
	d_left		+= cx;
	d_right		+= cx;
	d_top		+= cy;
	d_bottom	+= cy;
}
inline void RectF::scale(const float rx,const float ry){
	{
	d_left		*= rx;
	d_right		*= rx;
	d_top		*= ry;
	d_bottom	*= ry;
}
}
/*************************************************************************
	Check if a given point is within the RectF
*************************************************************************/
inline bool RectF::PtInRect(const PointF2D& pt) const
{
	if ((d_left > pt.d_x) ||
		(d_right <= pt.d_x) ||
		(d_top > pt.d_y) ||
		(d_bottom <= pt.d_y))
	{
		return false;
	}

	return true;
}

/*************************************************************************
	Set location of rect retaining current size.
*************************************************************************/
inline void RectF::SetTopLeft(const PointF2D& pt)
{
	SizeF sz(Size());

	d_left = pt.d_x;
	d_top  = pt.d_y;
	SetSize(sz);
}


/*************************************************************************
	check the size of the RectF object and if it is bigger than 'sz', 
	resize it so it isn't.	
*************************************************************************/
inline RectF& RectF::constrainSizeMax(const SizeF& sz)
{
	if (Width() > sz.d_width)
	{
		SetWidth(sz.d_width);
	}

	if (Height() > sz.d_height)
	{
		SetHeight(sz.d_height);
	}

	return *this;
}


/*************************************************************************
	check the size of the RectF object and if it is smaller than 'sz',
	resize it so it isn't.
*************************************************************************/
inline RectF& RectF::constrainSizeMin(const SizeF& sz)
{
	if (Width() < sz.d_width)
	{
		SetWidth(sz.d_width);
	}

	if (Height() < sz.d_height)
	{
		SetHeight(sz.d_height);
	}

	return *this;
}


/*************************************************************************
	check the size of the RectF object and if it is bigger than 'max_sz'
	or smaller than 'min_sz', resize it so it isn't.
*************************************************************************/
inline RectF& RectF::constrainSize(const SizeF& max_sz, const SizeF& min_sz)
{
	SizeF curr_sz(Size());

	if (curr_sz.d_width > max_sz.d_width)
	{
		SetWidth(max_sz.d_width);
	}
	else if (curr_sz.d_width < min_sz.d_width)
	{
		SetWidth(min_sz.d_width);
	}

	if (curr_sz.d_height > max_sz.d_height)
	{
		SetHeight(max_sz.d_height);
	}
	else if (curr_sz.d_height < min_sz.d_height)
	{
		SetHeight(min_sz.d_height);
	}

	return *this;
}

inline RectF& RectF::operator=(const RectF& rhs)
{
	d_left = rhs.d_left;
	d_top = rhs.d_top;
	d_right = rhs.d_right;
	d_bottom = rhs.d_bottom;

	return *this;
}
inline void RectF::operator&=(const RectF& frect) 
{
 *this =Intersection(frect);
}
inline void RectF::operator|=(const RectF& rect) 
{ 
	if (d_left >rect.d_left)
	{d_left=rect.d_left;}
	if (d_top >rect.d_top)
	{d_top=rect.d_top;}
	if (d_right <rect.d_right)
	{d_right=rect.d_right;}
	if (d_bottom <rect.d_bottom)
	{d_bottom=rect.d_bottom;}

}
inline PointF2D RectF::CenterPoint() const throw()
{
	return PointF2D((d_left + d_right)/2, (d_top + d_bottom )/2);
}
/******************************************************************
between windows rect and rectf
*********************************************************************/
inline RectF::RectF(const RECT& srcRect) 
{ 
	d_left =srcRect.left;
	d_right=srcRect.right;
	d_top=srcRect.top;
	d_bottom =srcRect.bottom;
}
inline RectF::RectF(LPRECT lpSrcRect) 
{
	if (!lpSrcRect)
	{
		return;
	}
	d_left =lpSrcRect->left;
	d_right=lpSrcRect->right;
	d_top=lpSrcRect->top;
	d_bottom =lpSrcRect->bottom;
}
inline RectF::RectF(POINT point, SIZE size) 
{ 
	d_right = (d_left = point.x) + size.cx;
	d_bottom = (d_top = point.y) + size.cy; 
}
inline RectF::RectF(POINT topLeft, POINT bottomRight) 
{ 
	d_left = topLeft.x; 
	d_top = topLeft.y;
	d_right = bottomRight.x; 
	d_bottom = bottomRight.y; 
}
inline RectF RectF::Intersection(const RECT& rc) 
{
	RectF temp(rc);
	return Intersection(temp);

}
inline void RectF::toWindowRect(RECT &outRect)const
{ 
	outRect.left=PixelAligned(d_left);
	outRect.top=PixelAligned(d_top);
	outRect.right=PixelAligned(d_right);
	outRect.bottom=PixelAligned(d_bottom);
}
inline void RectF::operator=(const RECT& srcRect)
{ 
	d_left =srcRect.left;
	d_right=srcRect.right;
	d_top=srcRect.top;
	d_bottom =srcRect.bottom;
}
inline BOOL RectF::operator==(const RECT& rect) const 
{ 
	if (PixelAligned(d_left) == rect.left&&
		PixelAligned(d_right) == rect.right&&
		PixelAligned(d_top) == rect.top &&
		PixelAligned(d_bottom) ==rect.bottom)
	{
		return true;
	}
	return false; 
}
inline BOOL RectF::operator!=(const RECT& rect) const 
{ 
	if (PixelAligned(d_left) != rect.left||
		PixelAligned(d_right) != rect.right||
		PixelAligned(d_top) != rect.top ||
		PixelAligned(d_bottom) !=rect.bottom)
	{
		return true;
	}
	return false;
}
inline void RectF::operator+=(POINT point) 
{ 
	d_left +=point.x;
	d_right+=point.x;
	d_top+=point.y;
	d_bottom += point.y; 
}
inline void RectF::operator+=(SIZE size) 
{ 
	d_left +=size.cx;
	d_right+=size.cx;
	d_top+=size.cy;
	d_bottom += size.cy; 
}
inline void RectF::operator+=(LPRECT lpRect) 
{ 
	if (PixelAligned(d_left) >lpRect->left)
	{d_left=lpRect->left;}
	if (PixelAligned(d_top) >lpRect->top)
	{d_top=lpRect->top;}
	if (PixelAligned(d_right) <lpRect->right)
	{d_right=lpRect->right;}
	if (PixelAligned(d_bottom) <lpRect->bottom)
	{d_bottom=lpRect->bottom;}
}
inline void RectF::operator-=(POINT point) 
{ 
	offsetRect( -point.x, -point.y); 
}
inline void RectF::operator-=(SIZE size) 
{ 
	offsetRect( -size.cx, -size.cy); 
}
inline void RectF::operator-=(LPCRECT lpRect)
{ 
	d_left -=lpRect->left;
	d_right -= lpRect->right;
	d_top -=lpRect->top;
	d_bottom -=lpRect->bottom;
}
inline void RectF::operator&=(const RECT& rect) 
{ 
	RectF temp(rect);
	*this=Intersection(rect); 
}
inline void RectF::operator|=(const RECT& rect) 
{ 
	if (PixelAligned(d_left) >rect.left)
	{d_left=rect.left;}
	if (PixelAligned(d_top) >rect.top)
	{d_top=rect.top;}
	if (PixelAligned(d_right) <rect.right)
	{d_right=rect.right;}
	if (PixelAligned(d_bottom) <rect.bottom)
	{d_bottom=rect.bottom;}
	 
}
inline RectF& RectF::operator+(POINT pt) 
{ 
	offsetRect( pt.x, pt.y);
	return *this;
}
inline RectF& RectF::operator-(POINT pt) 
{ 
	offsetRect( -pt.x, -pt.y);
	return *this;
}
inline RectF& RectF::operator+(SIZE size) 
{ 
	offsetRect( size.cx, size.cy);
	return *this;
}
inline RectF& RectF::operator-(SIZE size)
{ 
	offsetRect( -size.cx, -size.cy); 
	return *this;
}
inline RectF& RectF::operator+(LPRECT lpRect)
{ 
	d_left +=lpRect->left;
	d_right += lpRect->right;
	d_top +=lpRect->top;
	d_bottom +=lpRect->bottom;
	return *this;
}
inline RectF& RectF::operator-(LPRECT lpRect) 
{ d_left -=lpRect->left;
d_right -= lpRect->right;
d_top -=lpRect->top;
d_bottom -=lpRect->bottom;
return *this;
}
inline RectF& RectF::operator|(const RECT& rect2)
{ 
	if (PixelAligned(d_left) >rect2.left)
	{d_left=rect2.left;}
	if (PixelAligned(d_top) >rect2.top)
	{d_top=rect2.top;}
	if (PixelAligned(d_right) <rect2.right)
	{d_right=rect2.right;}
	if (PixelAligned(d_bottom) <rect2.bottom)
	{d_bottom=rect2.bottom;}
	return *this;
}
#endif	// end of guard _RectF_h_

