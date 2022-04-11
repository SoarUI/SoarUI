
#ifndef _PropertyInitialiser_h_
#define _PropertyInitialiser_h_
#include "XmlProperty.h"

    class  PropertyInitialiser
    {
    public:
		PropertyInitialiser(){}//
        PropertyInitialiser(const CLeeString& property, const CLeeString& value);
        const CLeeString& getTargetPropertyName();
        const CLeeString& getInitialiserValue();
    private:
       CLeeString   d_propertyName;     //!< Name of a property to be set.
       CLeeString   d_propertyValue;    //!< Value string to be set on the property.
    };

#endif  // end of guard _PropertyInitialiser_h_
