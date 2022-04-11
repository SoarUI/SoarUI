#ifndef _VectorF_H_
#define _VectorF_H_
#include "SizeF.h"
class  VectorF2
{
public:
    VectorF2(void) {}
    VectorF2(float x, float y) : d_x(x), d_y(y) {}
    VectorF2(const VectorF2& v) : d_x(v.d_x), d_y(v.d_y) {}

    VectorF2& operator*=(const VectorF2& vec)
    {
        d_x *= vec.d_x;
        d_y *= vec.d_y;

        return *this;
    }

    VectorF2& operator/=(const VectorF2& vec)
    {
        d_x /= vec.d_x;
        d_y /= vec.d_y;

        return *this;
    }

    VectorF2& operator+=(const VectorF2& vec)
    {
        d_x += vec.d_x;
        d_y += vec.d_y;

        return *this;
    }

    VectorF2& operator-=(const VectorF2& vec)
    {
        d_x -= vec.d_x;
        d_y -= vec.d_y;

        return *this;
    }

    VectorF2 operator+(const VectorF2& vec) const
    {
        return VectorF2(d_x + vec.d_x, d_y + vec.d_y);
    }

    VectorF2 operator-(const VectorF2& vec) const
    {
        return VectorF2(d_x - vec.d_x, d_y - vec.d_y);
    }

    VectorF2 operator*(const VectorF2& vec) const
    {
        return VectorF2(d_x * vec.d_x, d_y * vec.d_y);
    }

	VectorF2 operator*(float c) const
    {
        return VectorF2(d_x * c, d_y * c);
    }

    bool operator==(const VectorF2& vec) const
    {
        return ((d_x == vec.d_x) && (d_y == vec.d_y));
    }

    bool operator!=(const VectorF2& vec) const
    {
        return !(operator==(vec));
    }

    SizeF asSize() const     { return SizeF(d_x, d_y); }

    float d_x, d_y;
};

/*!
\brief
 Point2D class
*/
typedef VectorF2  PointF2D;



class VectorF3
{
public:
    VectorF3(void) {}
    VectorF3(float x, float y, float z) : d_x(x), d_y(y), d_z(z) {}
    VectorF3(const VectorF3& v) : d_x(v.d_x), d_y(v.d_y), d_z(v.d_z) {}

    bool operator==(const VectorF3& vec) const
    {
        return ((d_x == vec.d_x) && (d_y == vec.d_y) && (d_z == vec.d_z));
    }

    bool operator!=(const VectorF3& vec) const
    {
        return !(operator==(vec));
    }

	VectorF3 operator*(float c) const
	{
		return VectorF3(d_x * c, d_y * c, d_z * c);
	}

	VectorF3 operator+(const VectorF3& v) const
	{
		return VectorF3(d_x + v.d_x, d_y + v.d_y, d_z + v.d_z);
	}

    float d_x, d_y, d_z;
};
/*!
\brief
 Point3D class
*/
typedef VectorF3  PointF3D;
#endif