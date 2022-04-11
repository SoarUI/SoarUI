#ifndef _PropertyHelper_h_
#define _PropertyHelper_h_
#include "../../SoarHeader/RectF.h"
#include "../../SoarHeader/UDim.h"
#include"../colour.h"
#include"../ColourRect.h"
#include <Windows.h>
#include "../../SoarHeader/LeeString.h"
class Image;
class PropertyHelper
{
public:
	static	float	stringToFloat(const CLeeString& str);
	static	unsigned int	stringToUint(const CLeeString& str);
	static	int		stringToInt(const CLeeString& str);
	static	bool	stringToBool(const CLeeString& str);
	static	SizeF	stringToSize(const CLeeString& str);
	static	PointF2D	stringToPoint(const CLeeString& str);
    static  PointF3D stringToVector3(const CLeeString& str);
	static	RectF	stringToRect(const CLeeString& str);
	static const Image*	stringToImage(const CLeeString& str);
	static	colour	stringToColour(const CLeeString& str);
	static	ColourRect	stringToColourRect(const CLeeString& str);
	static	UDim	stringToUDim(const CLeeString& str);
	static	UPoint	stringToUPoint(const CLeeString& str);
	static	URect	stringToURect(const CLeeString& str);
	//static	UBox	stringToUBox(const CLeeString& str);

	static CLeeString	floatToString(float val);
	static CLeeString	uintToString(unsigned int val);
	static CLeeString	intToString(int val);
	static CLeeString	boolToString(bool val);
	static CLeeString	sizeToString(const SizeF& val);
	static CLeeString	pointToString(const PointF2D& val);
    static CLeeString   vector3ToString(const PointF3D& val);
	static CLeeString	rectToString(const RectF& val);
	static CLeeString	imageToString(const Image* const val);
	static CLeeString	colourToString(const colour& val);
	static CLeeString	colourRectToString(const ColourRect& val);
	static CLeeString	udimToString(const UDim& val);
	static CLeeString	upointToString(const UPoint& val);
	static CLeeString	urectToString(const URect& val);
	//static CLeeString	uboxToString(const UBox& val);

};
#endif	// end of guard _PropertyHelper_h_
