#include "Property.h"

Property::~Property(void)
{
}
Property::Property(const CLeeString& name,
									   const CLeeString& value)
									   : XmlProperty(name,
									   value)
{
}
Property::Property(const Property& other):XmlProperty(other)
{
	;
}
Property& Property::operator=(const Property& other)
{
	if (this==&other)
	{
		return *this;
	}
	XmlProperty::operator =(other);
	return *this;
}
// abstract members from Property
const CLeeString& Property::get() const
{
	return d_default;
}

void Property::set(const CLeeString& value)
{
	d_default =value;
}