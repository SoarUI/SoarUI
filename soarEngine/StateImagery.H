
#ifndef _StateImagery_h_
#define _StateImagery_h_

#include "LayerDesc.h"
#include "../SoarHeader/RectF.h"
#include <set>

#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4251)
#endif

    class  StateImagery
    {
    public:
        StateImagery() {}
		virtual ~StateImagery(void);
        StateImagery(const CLeeString& name);
        void addLayer(const LayerDesc& layer);
        void clearLayers();
        const CLeeString& getName() const;
        bool isClippedToDisplay() const;
        void setClippedToDisplay(bool setting);
		 void render(const RectF& ControlRect,const PointF2D& PixelOffsetXY,const CLeeString& szText,int vertTextFmt,int horzTextFmt, const RectF* pixelclipper/*剪切矩形*/ = 0) ;
		 void render(const RectF& ControlRect,const PointF2D& PixelOffsetXY, const RectF* pixelclipper/*剪切矩形*/ = 0) ;
		 void render(const RectF& ControlRect, const RectF& relativebaseRect,const PointF2D& PixelOffsetXY,const RectF* pixelclipper/*剪切矩形*/ = 0) ;
    private:
        typedef std::multiset<LayerDesc> LayersList;

        CLeeString         d_stateName;    //状态名字
        LayersList      d_layers;       //包含所使用的层信息
        bool            d_clipToDisplay; //!< true if Imagery for this state should be clipped to the display instead of winodw (effectively, not clipped).
    };

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif

#endif  // end of guard _StateImagery_h_
