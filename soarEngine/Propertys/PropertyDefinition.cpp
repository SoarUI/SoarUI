
#include "PropertyDefinition.h"
#include <iostream>
    PropertyDefinition::PropertyDefinition(const CLeeString& name,
                                           const CLeeString& initialValue)
      : XmlProperty(name,initialValue),
        d_userString(initialValue )
    {
    }
    // abstract members from Property
     const CLeeString& PropertyDefinition::get() const
    {
		if (!d_userString.empty())
		{
			return d_userString;
		}
		return d_default;
    }

    void PropertyDefinition::set(const CLeeString& value)
    {
      d_userString =value;
    }