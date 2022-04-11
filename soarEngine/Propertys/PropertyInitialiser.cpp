
#include "PropertyInitialiser.h"
#include <iostream>

    PropertyInitialiser::PropertyInitialiser(const CLeeString& property, const CLeeString& value) :
        d_propertyName(property),
        d_propertyValue(value)
    {}
    const CLeeString& PropertyInitialiser::getTargetPropertyName() 
    {
        return d_propertyName;
    }

    const CLeeString& PropertyInitialiser::getInitialiserValue() 
    {
        return d_propertyValue;
    }
