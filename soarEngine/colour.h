/***********************************************************************
	filename: 	CEGUIcolour.h
	created:	20/8/2004
	author:		Paul D Turner (with code from Jeff Leigh)
	
	purpose:	Defines interface to the colour class used to represent
				colour values within the system
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2006 Paul D Turner & The CEGUI Development Team
 *
 *   Permission is hereby granted, free of charge, to any person obtaining
 *   a copy of this software and associated documentation files (the
 *   "Software"), to deal in the Software without restriction, including
 *   without limitation the rights to use, copy, modify, merge, publish,
 *   distribute, sublicense, and/or sell copies of the Software, and to
 *   permit persons to whom the Software is furnished to do so, subject to
 *   the following conditions:
 *
 *   The above copyright notice and this permission notice shall be
 *   included in all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 *   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 *   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 *   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *   OTHER DEALINGS IN THE SOFTWARE.
 ***************************************************************************/
#ifndef _colour_h_
#define _colour_h_
#include <Windows.h>
typedef unsigned int Argb;    //!< 32 bit ARGB representation of a colour.
typedef unsigned int Rgb;
typedef unsigned int Rgba;
// Shift count and bit mask for A, R, G, B components

enum
{
	AlphaShift  = 24,
	RedShift    = 16,
	GreenShift  = 8,
	BlueShift   = 0
};

enum
{
	AlphaMask   = 0xff000000,
	RedMask     = 0x00ff0000,
	GreenMask   = 0x0000ff00,
	BlueMask    = 0x000000ff
};
class  colour
{
public:
	/*************************************************************************
		Construction & Destruction
	*************************************************************************/
	colour(void);
	colour(const colour& val);
	colour(float red, float green, float blue, float alpha = 1.0f);
	colour(Argb argb);

	/*************************************************************************
		Accessors
	*************************************************************************/
	Argb getARGB(void) const
	{
		if (!d_argbValid)
		{
			d_argb = calculateARGB();
			d_argbValid = true;
		}

		return d_argb;
	}
	Rgb	getRGB(void) const
	{
		if (!d_argbValid)
		{
			d_argb = calculateARGB();
			d_argbValid = true;
		}

		return d_argb&(~AlphaMask);
	}
	Rgba	Rgba(void) const
	{
		DWORD t=d_alpha *255;
		DWORD tmp =((t & 0xff)<<24);
		t =d_red *255;
		tmp=tmp	|((t & 0xff)<<16);
		t=d_green *255;
		tmp=tmp|(( t & 0xff)<<8);
		t= d_blue *255;
		tmp=tmp|(t & 0xff);
		return tmp;
	}

	float	getAlpha(void) const	{return d_alpha;}
	float	getRed(void) const		{return d_red;}
	float	getGreen(void) const	{return d_green;}
	float	getBlue(void) const		{return d_blue;}

	float	getHue(void) const;
	float	getSaturation(void) const;
	float	getLumination(void) const;
    

	/*************************************************************************
		Manipulators
	*************************************************************************/
	void	setARGB(Argb argb);
	inline void setAlpha(float alpha)
    {
        d_argbValid = false;
        d_alpha = alpha;
    }

	inline void setRed(float red)
    {   
        d_argbValid = false;
        d_red = red;
    }

	inline void setGreen(float green)
    {
        d_argbValid = false;
        d_green = green;
    }

	inline void setBlue(float blue)
    {
        d_argbValid = false;
        d_blue = blue;
    }

	inline void set(float red, float green, float blue, float alpha = 1.0f)
    {
        d_argbValid = false;
        d_alpha = alpha;
        d_red = red;
        d_green = green;
        d_blue = blue;
    }

	inline void setRGB(float red, float green, float blue)
    {
        d_argbValid = false;
        d_red = red;
        d_green = green;
        d_blue = blue;
    }

	inline void setRGB(const colour& val)
    {
        d_red = val.d_red;
        d_green = val.d_green;
        d_blue = val.d_blue;
        if (d_argbValid)
        {
            d_argbValid = val.d_argbValid;
            if (d_argbValid)
                d_argb = (d_argb & AlphaMask) | (val.d_argb & (~AlphaMask));
        }
    }

	void	setHSL(float hue, float saturation, float luminance, float alpha = 1.0f);

	void	invertColour(void);
	void	invertColourWithAlpha(void);

	/*************************************************************************
		Operators
	*************************************************************************/
	inline colour& operator=(Argb val)
    {
        setARGB(val);
        return *this;
    }

	inline colour& operator=(const colour& val)
    {
        d_alpha = val.d_alpha;
        d_red   = val.d_red;
        d_green = val.d_green;
        d_blue  = val.d_blue;
        d_argb  = val.d_argb;
        d_argbValid = val.d_argbValid;

        return *this;
    }

	inline colour& operator&=(Argb val)
    {
        setARGB(getARGB() & val);
        return *this;
    }

	inline colour& operator&=(const colour& val)
    {
        setARGB(getARGB() & val.getARGB());
        return *this;
    }

	inline colour& operator|=(Argb val)
    {
        setARGB(getARGB() | val);
        return *this;
    }

	inline colour& operator|=(const colour& val)
    {
        setARGB(getARGB() | val.getARGB());
        return *this;
    }

	inline colour& operator<<=(int val)
    {
        setARGB(getARGB() << val);
        return *this;
    }

	inline colour& operator>>=(int val)
    {
        setARGB(getARGB() >> val);
        return *this;
    }

	inline colour operator+(const colour& val) const
    {
        return colour(
            d_red   + val.d_red, 
            d_green + val.d_green, 
            d_blue  + val.d_blue,
            d_alpha + val.d_alpha
        );
    }

	inline colour operator-(const colour& val) const
    {
        return colour(
            d_red   - val.d_red,
            d_green - val.d_green,
            d_blue  - val.d_blue,
            d_alpha - val.d_alpha
        );
    }

	inline colour operator*(const float val) const
    {       
        return colour(
            d_red   * val, 
            d_green * val, 
            d_blue  * val,
            d_alpha * val 
        );  
    }

    inline colour& operator*=(const colour& val)
    {
        d_red *= val.d_red;
        d_blue *= val.d_blue;
        d_green *= val.d_green;
        d_alpha *= val.d_alpha;

		d_argbValid = false;

        return *this;
    }

	/*************************************************************************
		Compare operators
	*************************************************************************/
	inline bool operator==(const colour& rhs) const
    {
        return d_red   == rhs.d_red   &&
               d_green == rhs.d_green &&
               d_blue  == rhs.d_blue  &&
               d_alpha == rhs.d_alpha;
    }

	inline bool operator!=(const colour& rhs) const
    {
        return !(*this == rhs);
    }

	//
	// Conversion operators
	//
	operator Argb() const		{return getARGB();}
private:
	/*************************************************************************
		Implementation Methods
	*************************************************************************/
	/*!
	\brief
		calculate and return the ARGB value based on the current colour component values.
	*/
	Argb	calculateARGB(void) const;

	/*************************************************************************
		Implementation Data
	*************************************************************************/
	float d_alpha, d_red, d_green, d_blue;		//!< Colour components.
	mutable Argb d_argb;						//!< Colour as ARGB value.
	mutable bool d_argbValid;					//!< True if argb value is valid.
};

#endif	// end of guard _colour_h_
