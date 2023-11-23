
#ifndef _SizeF_h_
#define _SizeF_h_
class  SizeF
{
public:
	SizeF(void) {}
	SizeF(float width, float height) : d_width(width), d_height(height) {}
	SizeF(const SizeF& v): d_width(v.d_width), d_height(v.d_height) {}

	float d_width, d_height;

	SizeF operator*(float c) const
	{
		return SizeF(d_width * c, d_height * c);
	}

	SizeF operator+(const SizeF& s) const
	{
		return SizeF(d_width + s.d_width, d_height + s.d_height);
	}
	bool operator==(const SizeF& other) const
	{
		return d_width == other.d_width && d_height == other.d_height;
	}


	bool operator!=(const SizeF& other) const
	{
		return !operator==(other);
	}
};

#endif	// end of guard _SizeF_h_

