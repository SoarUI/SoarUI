
#include "LayerDesc.h"
#include <iostream>
#include "Propertys/PropertyHelper.h"

    LayerDesc::LayerDesc(unsigned int priority) :
        d_layerPriority(priority)
    {}
		LayerDesc::~LayerDesc(void)
		{
			clearSectionDescs();
		}
    void LayerDesc::render(const RectF& ControlRect,const PointF2D& PixelOffsetXY, 
		const RectF* clipper/*¼ôÇÐ¾ØÐÎ*/, bool clipToDisplay) const
    {
        // render all sections in this layer
        for(SectionList::const_iterator curr = d_sections.begin(); curr != d_sections.end(); ++curr)
        {
            (*curr).render(ControlRect, PixelOffsetXY,clipper, clipToDisplay);
        }
    }
	void LayerDesc::render(const RectF& ControlRect,const PointF2D& PixelOffsetXY,const  CLeeString& szText,int vertTextFmt,int horzTextFmt,
		const RectF* clipper/*¼ôÇÐ¾ØÐÎ*/, bool clipToDisplay)const 
	{
		// render all sections in this layer
		for(SectionList::const_iterator curr = d_sections.begin(); curr != d_sections.end(); ++curr)
		{
			(*curr).render(ControlRect, PixelOffsetXY,szText,vertTextFmt,horzTextFmt,clipper, clipToDisplay);
		}
	}
	void LayerDesc::render(const RectF& ControlRect, const RectF& relativebaseRect,const PointF2D& PixelOffsetXY,
		const RectF* pixelclipper/*¼ôÇÐ¾ØÐÎ*/,bool clipToDisplay) const
	{
		// render all sections in this layer
		for(SectionList::const_iterator curr = d_sections.begin(); curr != d_sections.end(); ++curr)
		{
			(*curr).render(ControlRect, relativebaseRect, PixelOffsetXY,pixelclipper,clipToDisplay);
		}
	}
    void LayerDesc::addSectionDesc(const SectionDesc& section)
    {
        d_sections.push_back(section);
    }

    void LayerDesc::clearSectionDescs()
    {
        d_sections.clear();
    }

    unsigned int LayerDesc::getLayerPriority() const
    {
        return d_layerPriority;
    }

    bool LayerDesc::operator<(const LayerDesc& other) const
    {
        return d_layerPriority < other.d_layerPriority;
    }
