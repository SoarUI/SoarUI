
#ifndef _NamedArea_h_
#define _NamedArea_h_

#include "../SoarHeader/Dimensions.h"

    class  NamedArea
    {
    public:
        NamedArea() {}
        NamedArea(const CLeeString& name);
        const CLeeString& getName() const;
        const ComponentArea& getArea() const;
        void setArea(const ComponentArea& area);
       // void writeXMLToStream(XMLSerializer& xml_stream) const;
    private:
        CLeeString d_name;
        ComponentArea d_area;
    };
#endif  // end of guard _NamedArea_h_
