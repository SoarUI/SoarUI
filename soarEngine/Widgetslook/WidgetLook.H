
#ifndef _CWidgetLook_h_
#define _CWidgetLook_h_

#include "../Propertys/propertyinitialiser.h"
#include "../Propertys/PropertyDefinition.h"
#include "IWidgetLook.h"
#include "../../SoarHeader/leetextcontext.h"
#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4251)
#endif
	 class  CWidgetLook:public IWidgetLook
    {
	public:
		/** Typedefs for property related lists. */
        typedef std::vector<PropertyDefinition>   vDefinitionList;//Feel文件中定义的属性
		typedef std::vector<CLeeString> AnimationList;
		public:
		typedef std::map<CLeeString,  StateImagery, CLeeString::FastLessCompare>    StateList;//状态集
		typedef std::map<CLeeString,  ImagerySection, CLeeString::FastLessCompare>  ImageryList;//图像集
		typedef std::map<CLeeString, NamedArea, CLeeString::FastLessCompare>       NamedAreaList;//命名区域
		typedef std::map<CLeeString, PropertyDefinition, CLeeString::FastLessCompare>     XmlPropertyList;//属性定义
    public:
        CWidgetLook(const CLeeString& name,const CLeeString & windowtype);
		CWidgetLook(void);
        CWidgetLook(const CWidgetLook& rhs);
		CWidgetLook& operator=(const CWidgetLook& other);
		virtual ~CWidgetLook(void);

		//
		const CLeeString& getName(void) const;
		const CLeeString& getWindowType(void) const;
		//图像集合----------------------------------------//
		//获取图像描述
		ImagerySection* getImagerySection(const CLeeString& section);
		void addImagerySection(ImagerySection& section);
		void clearImagerySections(void);
		RectF getImagerySectionBoundingRect(const CLeeString& section,const RectF& ControlRect);
		bool isImagerySectionPresent(const CLeeString& section) const;
		//状态集合---------------------------------------.//
		//该控件是否定义了该状态
		bool isStateImageryPresent(const CLeeString& state) const;
		StateImagery* getStateImagery(const CLeeString& state);
		void addStateImagery( StateImagery& state);
		//清除所有状态定义
		void clearStateImagerys(void);
		//本地坐标系下区域
		virtual RectF getBoundingRect(const RectF& ControlRect) const ;
		//添加命名空间
		void addNamedArea(const NamedArea& area);
		//清除所有命名控空间
		void clearNamedAreas();
		//获取命名空间
		const NamedArea* getNamedArea(const CLeeString& name) const;
		//是否已经定义了命名空间
		bool isNamedAreaDefined(const CLeeString& name) const;

		////////////////////////////////////////////////////
		 RectF getcomponentImagerySectionBoundingRect(const CLeeString& component,const CLeeString& section,const RectF& ControlRect);
		//获取该控件定义的属性
		const CLeeString &getProperty(const CLeeString& Propertyname)const;
		//查找属性定义
		const PropertyDefinition* findPropertyDefinition(const CLeeString& propertyName) const;
        void  addPropertyDefine(const PropertyDefinition& propDef);
		void  removePropertyDefine(const CLeeString& PropName);
		void  clearPropertyDefines(void);
		 bool isPropDefinitionPresent(const CLeeString& name) const;
		const vDefinitionList& getPropertyDefinitions() const { return d_Definitions; } 
		//渲染接口
		void RenderWidget(const CLeeString& stringStatus,const RectF& ControlRect,const PointF2D & PixelOffsetXY,const CLeeString& szText,int vertTextFmt,int horzTextFmt,const RectF* clipper = 0, bool clipToDisplay = false);
		void RenderWidget_Frame(const CLeeString& stringStatus,const RectF& ControlRect,const PointF2D & PixelOffsetXY,const RectF* clipper = 0, bool clipToDisplay = false);
		void RenderWidget_Image(const CLeeString& stringStatus,const RectF& ControlRect,const PointF2D & PixelOffsetXY,const RectF* clipper = 0, bool clipToDisplay = false);
		
		//////////////////////////////////////////////////////////////////////////
		//typedef std::vector<PropertyLinkDefinition> vLinkPropertyList;//属性连接，利于内存利用
    private:
        mutable vDefinitionList  d_Definitions;  //属性定义
		CLeeString         d_name;         //控件外观名称
		CLeeString         d_windowType;   //分类
		ImageryList     d_imagerySections;  //图像集合
		StateList       d_stateImagery;     //状态集合
		NamedAreaList   d_namedAreas;       //命名区域集合
		XmlPropertyList d_Propertyset;//属性定义
    };


#if defined(_MSC_VER)
#	pragma warning(pop)
#endif

#endif  // end of guard _CWidgetLook_h_
