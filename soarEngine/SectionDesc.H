
#ifndef _FalSectionDesc_h_
#define _FalSectionDesc_h_
#include <Windows.h>
#include"../SoarHeader/leeString.h"
#include "../SoarHeader/RectF.h"
    // forward refs
    class WidgetLookFeel;
    class ImagerySection;
	struct TextContext;
    class  SectionDesc
    {
    public:
        SectionDesc(const CLeeString& owner, const CLeeString& sectionName);
		virtual ~SectionDesc(void);
        const CLeeString& getOwnerWidgetLook() const;
		const CLeeString& getOwnerSuffix() const;
		void setOwnerSuffix(const CLeeString & componentName);
        const CLeeString& getSectionName() const;

		 void render(const RectF& ControlRect,const PointF2D& PixelOffsetXY,
			 const RectF* clipper = 0, bool clipToDisplay = false) const ;
		 void render(const RectF& ControlRect,const PointF2D& PixelOffsetXY,const CLeeString &szText,int vertTextFmt,int horzTextFmt,
			 const RectF* clipper = 0, bool clipToDisplay = false) const;
		void render(const RectF& ControlRect, const RectF& relativebaseRect,const PointF2D& PixelOffsetXY,
			const RectF* clipper  = 0 , bool clipToDisplay  = false )const;
    private:
        CLeeString          d_owner;                //包含该section 的WidgetLook名称
        CLeeString          d_sectionName;          //imagerysection的名称
		//以下为高级特效，待开发
    };

#endif  // end of guard _SectionDesc_h_
