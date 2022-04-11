
#ifndef _PropertyDefinition_h_
#define _PropertyDefinition_h_

#include "XmlProperty.h"

    class  PropertyDefinition : public XmlProperty
    {
    public:
        /*!
        \deprecated
            This version of the constructor is deprecated in favour of the
            version taking a help string.
        */
        PropertyDefinition(const CLeeString& name, const CLeeString& initialValue);

        // abstract members from Property
        const CLeeString& get() const;
        void set(const CLeeString& value);

    protected:
        CLeeString d_userString;//
    };

#endif  // end of guard _PropertyDefinition_h_
