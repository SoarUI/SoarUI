
#include "PropertyHelper.h"
#include "../ImagesetManager.h"
#include "../Imageset.h"

#include <cstdio>
#include<conio.h>
#include <sstream>

#ifdef _MSC_VER
#define snprintf _sntprintf
#endif

float PropertyHelper::stringToFloat(const CLeeString& str)
{
    using namespace std;

    float val = 0;
    _stscanf(str.c_str(), _T(" %g"), &val);

    return val;
}


unsigned int PropertyHelper::stringToUint(const CLeeString& str)
{
    using namespace std;

    unsigned int val = 0;
    _stscanf(str.c_str(), _T(" %u"), &val);

    return val;
}


int PropertyHelper::stringToInt(const CLeeString& str)
{
    using namespace std;

    int val = 0;
    _stscanf(str.c_str(), _T(" %d"), &val);

    return val;
}


bool PropertyHelper::stringToBool(const CLeeString& str)
{
    if ((str == _T("True")) || (str == _T("true")))
    {
        return true;
    }
    else
    {
        return false;
    }

}


SizeF PropertyHelper::stringToSize(const CLeeString& str)
{
    using namespace std;

    SizeF val(0, 0);
    _stscanf(str.c_str(), _T(" w:%g h:%g"), &val.d_width, &val.d_height);

    return val;
}


PointF2D PropertyHelper::stringToPoint(const CLeeString& str)
{
    using namespace std;

    PointF2D val(0, 0) ;
    _stscanf(str.c_str(), _T(" x:%g y:%g"), &val.d_x, &val.d_y);

    return val;
}


RectF PropertyHelper::stringToRect(const CLeeString& str)
{
    using namespace std;

    RectF val(0, 0, 0, 0);
    _stscanf(str.c_str(), _T(" l:%g t:%g r:%g b:%g"), &val.d_left, &val.d_top, &val.d_right, &val.d_bottom);

    return val;
}


const Image* PropertyHelper::stringToImage(const CLeeString& str)
{
    using namespace std;

    // handle empty string case
    if (str.empty())
        return 0;
   
	TCHAR imageSet[128] ={0,};
	TCHAR imageName[128]={0,};

    int fields=_stscanf(str.c_str(), _T("set:%s image:%s"), imageSet, imageName);
    if (fields<2)
    {
		return NULL;
    }
    
    const Image* image;

   try
    {
		image = ImagesetManager::getSingleton().get(imageSet)->getImage(imageName);
    }
    catch(...)
    {
        image = 0;
    }

    return image;
}


UDim PropertyHelper::stringToUDim(const CLeeString& str)
{
    using namespace std;

    UDim ud;
    _stscanf(str.c_str(), _T(" { %g , %g }"), &ud.d_scale, &ud.d_offset);

    return ud;
}

UPoint PropertyHelper::stringToUPoint(const CLeeString& str)
{
    using namespace std;

    UPoint uv;
    _stscanf(str.c_str(), _T(" { { %g , %g } , { %g , %g } }"),
           &uv.d_x.d_scale, &uv.d_x.d_offset,
           &uv.d_y.d_scale, &uv.d_y.d_offset);

    return uv;
}


URect PropertyHelper::stringToURect(const CLeeString& str)
{
    using namespace std;

    URect ur;
    _stscanf(
        str.c_str(),
        _T(" { { %g , %g } , { %g , %g } , { %g , %g } , { %g , %g } }"),
        &ur.d_min.d_x.d_scale, &ur.d_min.d_x.d_offset,
        &ur.d_min.d_y.d_scale, &ur.d_min.d_y.d_offset,
        &ur.d_max.d_x.d_scale, &ur.d_max.d_x.d_offset,
        &ur.d_max.d_y.d_scale, &ur.d_max.d_y.d_offset
    );

    return ur;
}


CLeeString PropertyHelper::floatToString(float val)
{
    using namespace std;

    TCHAR buff[64];
    snprintf(buff, sizeof(buff), _T("%g"), val);

    return CLeeString(buff);

#if 0 // slower
    std::ostringstream iss;
    iss << val;
    return CLeeString(iss.str());
#endif
}


CLeeString PropertyHelper::uintToString(unsigned int val)
{
    using namespace std;

    TCHAR buff[64];
    snprintf(buff, sizeof(buff), _T("%u"), val);

    return CLeeString(buff);

#if 0 // slower
    std::ostringstream iss;
    iss << val;
    return CLeeString(iss.str());
#endif
}


CLeeString PropertyHelper::intToString(int val)
{
    using namespace std;

    TCHAR buff[64];
    snprintf(buff, sizeof(buff), _T("%d"), val);

    return CLeeString(buff);

#if 0 // slower
    std::ostringstream iss;
    iss << val;
    return CLeeString(iss.str());
#endif
}


CLeeString PropertyHelper::boolToString(bool val)
{
    if (val)
    {
        return CLeeString(_T("True"));
    }
    else
    {
        return CLeeString(_T("False"));
    }

}


CLeeString PropertyHelper::sizeToString(const SizeF& val)
{
    using namespace std;

    TCHAR buff[128];
    snprintf(buff, sizeof(buff), _T("w:%g h:%g"), val.d_width, val.d_height);

    return CLeeString(buff);

#if 0 // slower
    std::ostringstream iss;
    iss << "w:" << val.d_width << " h:" << val.d_height;
    return CLeeString(iss.str());
#endif
}


CLeeString PropertyHelper::pointToString(const PointF2D& val)
{
    using namespace std;

    TCHAR buff[128];
    snprintf(buff, sizeof(buff), _T("x:%g y:%g"), val.d_x, val.d_y);

    return CLeeString(buff);

#if 0 // slower
    std::ostringstream iss;
    iss << "x:" << val.d_x << " y:" << val.d_y;
    return CLeeString(iss.str());
#endif
}


CLeeString PropertyHelper::rectToString(const RectF& val)
{
    using namespace std;

    TCHAR buff[256];
    snprintf(buff, sizeof(buff), _T("l:%g t:%g r:%g b:%g"),
             val.d_left, val.d_top, val.d_right, val.d_bottom);

    return CLeeString(buff);

#if 0 // slower
    std::ostringstream iss;
    iss << "l:" << val.d_left << " t:" << val.d_top << " r:" << val.d_right << " b:" << val.d_bottom;
    return CLeeString(iss.str());
#endif
}


CLeeString PropertyHelper::imageToString(const Image* const val)
{
    if (val)
    {
        return CLeeString(_T("set:")) + val->getImagesetName() + _T(" image:") + val->getName();
    }

    return CLeeString(_T(""));
}


CLeeString PropertyHelper::udimToString(const UDim& val)
{
    using namespace std;

    TCHAR buff[128];
    snprintf(buff, sizeof(buff), _T("{%g,%g}"), val.d_scale, val.d_offset);

    return CLeeString(buff);

#if 0
    std::ostringstream iss;
    iss << '{' << val.d_scale << ',' << val.d_offset << '}';
    return CLeeString(iss.str());
#endif
}


CLeeString PropertyHelper::upointToString(const UPoint& val)
{
    using namespace std;

    TCHAR buff[256];
    snprintf(buff, sizeof(buff), _T("{{%g,%g},{%g,%g}}"),
             val.d_x.d_scale, val.d_x.d_offset, val.d_y.d_scale, val.d_y.d_offset);

    return CLeeString(buff);

#if 0
    std::ostringstream iss;

    iss << "{{"
    << val.d_x.d_scale << ',' << val.d_x.d_offset << "},{"
    << val.d_y.d_scale << ',' << val.d_y.d_offset << "}}";

    return CLeeString(iss.str());
#endif
}


CLeeString PropertyHelper::urectToString(const URect& val)
{
    using namespace std;

    TCHAR buff[512];
    snprintf(buff, sizeof(buff), _T("{{%g,%g},{%g,%g},{%g,%g},{%g,%g}}"),
             val.d_min.d_x.d_scale, val.d_min.d_x.d_offset,
             val.d_min.d_y.d_scale, val.d_min.d_y.d_offset,
             val.d_max.d_x.d_scale, val.d_max.d_x.d_offset,
             val.d_max.d_y.d_scale, val.d_max.d_y.d_offset);

    return CLeeString(buff);

#if 0
    std::ostringstream iss;

    iss << "{{"
    << val.d_min.d_x.d_scale << ',' << val.d_min.d_x.d_offset << "},{"
    << val.d_min.d_y.d_scale << ',' << val.d_min.d_y.d_offset << "},{"
    << val.d_max.d_x.d_scale << ',' << val.d_max.d_x.d_offset << "},{"
    << val.d_max.d_y.d_scale << ',' << val.d_max.d_y.d_offset << "}}";

    return CLeeString(iss.str());
#endif
}

CLeeString PropertyHelper::colourToString(const colour& val)
{
    using namespace std;

    TCHAR buff[16];
    snprintf(buff, sizeof(buff),_T("%.8X"), val.getARGB());

    return CLeeString(buff);
}


colour PropertyHelper::stringToColour(const CLeeString& str)
{
    using namespace std;

    Argb val = 0xFF000000;
    _stscanf(str.c_str(), _T(" %8X"), &val);

    return colour(val);

}


CLeeString PropertyHelper::colourRectToString(const ColourRect& val)
{
    using namespace std;

    TCHAR buff[64];
    sprintf(buff, _T("tl:%.8X tr:%.8X bl:%.8X br:%.8X"), val.d_top_left.getARGB(), val.d_top_right.getARGB(), val.d_bottom_left.getARGB(), val.d_bottom_right.getARGB());

    return CLeeString(buff);
}


ColourRect PropertyHelper::stringToColourRect(const CLeeString& str)
{
    using namespace std;

    if (str.length() == 8)
    {
        Argb all = 0xFF000000;
        _stscanf(str.c_str(), "%8X", &all);
        return ColourRect(all);
    }

    Argb topLeft = 0xFF000000, topRight = 0xFF000000, bottomLeft = 0xFF000000, bottomRight = 0xFF000000;
    _stscanf(str.c_str(), _T("tl:%8X tr:%8X bl:%8X br:%8X"), &topLeft, &topRight, &bottomLeft, &bottomRight);

    return ColourRect(topLeft, topRight, bottomLeft, bottomRight);
}

//----------------------------------------------------------------------------//
PointF3D PropertyHelper::stringToVector3(const CLeeString& str)
{
    using namespace std;

    PointF3D val(0, 0, 0);
    _stscanf(str.c_str(), _T(" x:%g y:%g z:%g"), &val.d_x, &val.d_y, &val.d_z);

    return val;
}

//----------------------------------------------------------------------------//
CLeeString PropertyHelper::vector3ToString(const PointF3D& val)
{
    using namespace std;

    TCHAR buff[128];
    snprintf(buff, sizeof(buff), _T("x:%g y:%g z:%g"), val.d_x, val.d_y, val.d_z);

    return CLeeString(buff);
}
