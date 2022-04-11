
#include "NamedArea.h"

    NamedArea::NamedArea(const CLeeString& name) :
        d_name(name)
    {
    }

    const CLeeString& NamedArea::getName() const
    {
        return d_name;
    }

    const ComponentArea& NamedArea::getArea() const
    {
        return d_area;
    }

    void NamedArea::setArea(const ComponentArea& area)
    {
        d_area = area;
    }