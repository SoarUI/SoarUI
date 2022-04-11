
#include "StateImagery.h"
#include "../SoarHeader/RectF.h"
#include <iostream>

    StateImagery::StateImagery(const CLeeString& name) :
        d_stateName(name),
        d_clipToDisplay(false)
    {}
		StateImagery::~StateImagery(void)
		{
			d_layers.clear();
		}
    void StateImagery::render(const RectF& ControlRect,const PointF2D& PixelOffsetXY,
		const RectF* pixelclipper/*剪切矩形*/)
    {
        // TODO: Fix layer priority handling
        // 渲染相应状态定义的所有层，只负责调用，计算由底层计算
        for(LayersList::iterator curr = d_layers.begin(); curr != d_layers.end(); ++curr)
            (*curr).render(ControlRect,PixelOffsetXY,pixelclipper, d_clipToDisplay);
    }
	void StateImagery::render(const RectF& ControlRect,const PointF2D& PixelOffsetXY,const CLeeString& szText,int vertTextFmt,int horzTextFmt,
		const RectF* pixelclipper/*剪切矩形*/)
	{
		// TODO: Fix layer priority handling
		// 渲染相应状态定义的所有层，只负责调用，计算由底层计算
		for(LayersList::iterator curr = d_layers.begin(); curr != d_layers.end(); ++curr)
			(*curr).render(ControlRect,PixelOffsetXY,szText,vertTextFmt,horzTextFmt,pixelclipper, d_clipToDisplay);
	}
    void StateImagery::render(const RectF& ControlRect, const RectF& relativebaseRect,const PointF2D& PixelOffsetXY,
		const RectF* pixelclipper/*剪切矩形*/) 
	{
		// render all layers defined for this state
		for(LayersList::iterator curr = d_layers.begin(); curr != d_layers.end(); ++curr)
			(*curr).render(ControlRect, relativebaseRect, PixelOffsetXY,pixelclipper, d_clipToDisplay);
	}
    void StateImagery::addLayer(const LayerDesc& layer)
    {
        d_layers.insert(layer);
    }

    void StateImagery::clearLayers()
    {
        d_layers.clear();
    }

    const CLeeString& StateImagery::getName() const
    {
        return d_stateName;
    }

    bool StateImagery::isClippedToDisplay() const
    {
        return d_clipToDisplay;
    }

    void StateImagery::setClippedToDisplay(bool setting)
    {
        d_clipToDisplay = setting;
    }
