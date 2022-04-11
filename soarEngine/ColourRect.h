
#ifndef _ColourRect_h_
#define _ColourRect_h_

#include "colour.h"

class ColourRect
{
public:
	ColourRect(void);

	ColourRect(const colour& col);

	ColourRect(const colour& top_left, const colour& top_right, const colour& bottom_left, const colour& bottom_right);

	void	setAlpha(float alpha);

	void	setTopAlpha(float alpha);

	void	setBottomAlpha(float alpha);

	void	setLeftAlpha(float alpha);

	void	setRightAlpha(float alpha);
	//单色的
	bool	isMonochromatic() const;
//获取子矩形
	ColourRect getSubRectangle( float left, float right, float top, float bottom ) const;
//获取某点颜色
	colour getColourAtPoint( float x, float y ) const;


	void	setColours(const colour& col);
	void	modulateAlpha(float alpha);
    ColourRect& operator*=(const ColourRect& other);

	inline ColourRect operator*(const float val) const
    {       
        return ColourRect(
            d_top_left * val, 
            d_top_right * val, 
            d_bottom_left * val,
            d_bottom_right * val 
        );  
    }

	inline ColourRect operator+(const ColourRect& val) const
    {       
        return ColourRect(
            d_top_left + val.d_top_left, 
            d_top_right + val.d_top_right, 
            d_bottom_left + val.d_bottom_left,
            d_bottom_right + val.d_bottom_right 
        );  
    }


	colour	d_top_left, 
		d_top_right, 
		d_bottom_left, 
		d_bottom_right;		//<! ColourRect component colours
};

#endif	// end of guard _ColourRect_h_
