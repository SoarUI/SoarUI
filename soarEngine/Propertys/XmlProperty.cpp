
#include "XmlProperty.h"
#include <iostream>

	/*************************************************************************
		Returns whether the property is at it's default value.
	*************************************************************************/
	bool XmlProperty::isDefault(void) const
	{
		return (get() == d_default);
	}

	/*************************************************************************
		Returns the default value of the XmlProperty as a String.
	*************************************************************************/
	const CLeeString& XmlProperty::getDefault(void) const
	{
		return d_default;
	}
	XmlProperty::XmlProperty(const XmlProperty& rhs):
	d_default(rhs.d_default),d_name(rhs.d_name)
	{
		;
	}
	XmlProperty& XmlProperty::operator=(const XmlProperty& other)
	{
		if (this==&other)
		{
			return *this;
		}
		d_default=other.d_default;
		d_name=other.d_name;
		return *this;
	}