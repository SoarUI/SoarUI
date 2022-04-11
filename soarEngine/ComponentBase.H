
#ifndef _ComponentBase_h_
#define _ComponentBase_h_

#include "../SoarHeader/Dimensions.h"
    class FeelComponentBase
    {
    public:
        FeelComponentBase();
        virtual ~FeelComponentBase();
		FeelComponentBase(const FeelComponentBase& obj);
		FeelComponentBase& operator=(const FeelComponentBase& other);
		/**************
		新增加获取属性函数，记录宿主信息
		****************/
		const CLeeString& getOwnerWidgetLook() const;
		void setOwnerWidget(const CLeeString & OwnerName);
		//---------------------------------------------
        const ComponentArea& getComponentArea() const;
        void setComponentArea(const ComponentArea& area);
        void setColoursPropertySource(const CLeeString& property);
        void setVertFormattingPropertySource(const CLeeString& property);
        void setHorzFormattingPropertySource(const CLeeString& property);
	
		   void render(const RectF& ControlRect,const PointF2D& PixelOffsetXY, 
			   const RectF* Pixelclipper = 0, bool clipToDisplay = false) const;
		   void render(const RectF& ControlRect, const RectF& baseRect,const PointF2D& PixelOffsetXY,const RectF* Pixelclipper  = 0 , bool clipToDisplay  = false ) const;
    protected:
         virtual void render_impl(const RectF& ControlRect, RectF& destRect, 
			 const RectF* clipper, bool clipToDisplay,const PointF2D& PixelOffsetXY) const = 0;

        // data fields
        ComponentArea   d_area;                 //目标区域
        CLeeString      d_vertFormatPropertyName;   //垂直属性
        CLeeString      d_horzFormatPropertyName;   //水平属性
		//增加一个WidgetLook name用于查找属性值等
		CLeeString d_WidgetLookOwner;
    };
 struct ImagePackage 
 {
	 CLeeString d_imagetset;
	 CLeeString d_image;
 };
 
#endif  // end of guard _ComponentBase_h_
