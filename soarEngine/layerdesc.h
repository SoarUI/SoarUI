
#ifndef _LayerDesc_h_
#define _LayerDesc_h_
#include <vector>
#include "SectionDesc.h"
#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4251)
#endif
    class  LayerDesc
    {
    public:
		typedef std::vector<SectionDesc> SectionList;
        LayerDesc(unsigned int priority);
		virtual ~LayerDesc();
        void addSectionDesc(const SectionDesc& section);
        void clearSectionDescs();
        unsigned int getLayerPriority() const;
		SectionDesc* getSectionDesc(const CLeeString& sectionName);
		const SectionList& getSectionDescs(void);
        // required to sort layers according to priority
        bool operator<(const LayerDesc& other) const;
        void render(const RectF& ControlRect,const PointF2D& PixelOffsetXY, 
			const RectF* pixelclipper/*���о���*/ = 0, 
			bool clipToDisplay = false) const;
		void render(const RectF& ControlRect,const PointF2D& PixelOffsetXY,const CLeeString& szText,int vertTextFmt,int horzTextFmt,
			const RectF* pixelclipper/*���о���*/ = 0, 
			bool clipToDisplay = false) const;
		void render(const RectF& ControlRect, const RectF& relativebaseRect,
			const PointF2D& PixelOffsetXY,
			const RectF* pixelclipper/*���о���*/ = 0,bool clipclipToDisplay=false) const;
    private:
        SectionList d_sections;         //���е�Section����
        unsigned int  d_layerPriority;    //˳��ֵ
    };

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif

#endif  // end of guard _LayerDesc_h_
