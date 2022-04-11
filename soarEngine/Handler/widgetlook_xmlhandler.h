
#ifndef _Widgetlook_xmlHandler_h_
#define _Widgetlook_xmlHandler_h_

#include "../../SoarHeader/Dimensions.h"
#include <vector>
#include "../Widgetslook/WidgetlookPackage.h"
#include "../../XML/IXMLHandler.h"
    // forward refs
    class WidgetLookManager;
	class CWidgetlookPackage;
    class CWidgetLook;
    class ImagerySection;
    class StateImagery;
    class LayerDesc;
    class SectionDesc;
    class ImageryComponent;
    class ComponentArea;
    class Dimension;
    class NamedArea;
    class FrameComponent;
	class TextComponent;
    class PropertyLinkDefinition;
	class FeelCommon;
    class CWidgetlook_xmlHandler : public IXMLHandler
    {
    public:
        CWidgetlook_xmlHandler();
        ~CWidgetlook_xmlHandler();

    protected:
        /*************************************************************************
            ChainedXMLHandler base class overrides
        *************************************************************************/
        void elementStart(const CLeeString& element,
                               const XMLAttributes& attributes);
        void elementEnd(const CLeeString& element);
        void text(const CLeeString & text){}
    private:
        /*************************************************************************
            Typedefs
        *************************************************************************/
        //! Type for handlers of an opening xml element.
        typedef void (CWidgetlook_xmlHandler::*ElementStartHandler)(const XMLAttributes& attributes);
        //! Type for handlers of a closing xml element.
        typedef void (CWidgetlook_xmlHandler::*ElementEndHandler)();
        //! Map of handlers for opening xml elements.
        typedef std::map<CLeeString, ElementStartHandler, CLeeString::FastLessCompare> ElementStartHandlerMap;
        //! Map of handlers for closing xml elements.
        typedef std::map<CLeeString, ElementEndHandler, CLeeString::FastLessCompare> ElementEndHandlerMap;

        /*************************************************************************
            Implementation Constants
        *************************************************************************/
        // element names
        static const CLeeString tagDreamLook;            //文件标志
        static const CLeeString tagWidgetLook;          //控件
        static const CLeeString tagImagerySection;      //图像集合
        static const CLeeString tagStateImagery;        //状态集合
        static const CLeeString tagLayer;               //状态层
        static const CLeeString tagSection;             //!< Tag name for Section elements.
        static const CLeeString tagImageryComponent;    //!< Tag name for ImageryComponent elements.
        static const CLeeString tagFrameComponent;      //!< Tag name for FrameComponent elements.
		static const CLeeString tagTextComponent;
        static const CLeeString tagArea;                //!< Tag name for Area elements.
        static const CLeeString tagImage;               //!< Tag name for Image elements.
        static const CLeeString tagVertFormat;          //!< Tag name for VertFormat elements.
        static const CLeeString tagHorzFormat;          //!< Tag name for HorzFormat elements.
        static const CLeeString tagVertAlignment;       //!< Tag name for VertAlignment elements.
        static const CLeeString tagHorzAlignment;       //!< Tag name for HorzAlignment elements.
        static const CLeeString tagProperty;            //属性标签
		static const CLeeString tagDim;                 //!< Tag name for dimension container elements.
		static const CLeeString tagUnifiedDim;          //!< Tag name for unified dimension elements.
        static const CLeeString tagAbsoluteDim;         //!< Tag name for absolute dimension elements.
        static const CLeeString tagImageDim;            //!< Tag name for image dimension elements.

		static const CLeeString tagTextVertFormat;;             //!< Tag name for font dimension elements.
		static const CLeeString tagTextHorzFormat;;  
     
        static const CLeeString tagFontDim;             //!< Tag name for font dimension elements.
        static const CLeeString tagPropertyDim;         //!< Tag name for property dimension elements.
        static const CLeeString tagNamedArea;           //!< Tag name for named area elements.
        static const CLeeString tagPropertyDefinition;  //!< Tag name for property definition elements.
        static const CLeeString tagPropertyLinkDefinition;  //!< Tag name for property link elements.
        static const CLeeString tagPropertyLinkTarget;  //!< Tag name for property link target elements.???????
        static const CLeeString tagDimOperator;         //!< Tag name for dimension operator elements.
        static const CLeeString tagVertFormatProperty;  //!< Tag name for element that specifies a vertical formatting property.
        static const CLeeString tagHorzFormatProperty;  //!< Tag name for element that specifies a horizontal formatting property..
        static const CLeeString tagAreaProperty;        //!< Tag name for element that specifies a URect property..
        static const CLeeString tagImageProperty;       //!< Tag name for element that specifies an Image property..
        static const CLeeString tagFontProperty;        //!< Tag name for element that specifies an Font property.
        // attribute names
        static const CLeeString TopLeftAttribute;           //!< Attribute name that stores colour for top-left corner.
        static const CLeeString TopRightAttribute;          //!< Attribute name that stores colour for top-right corner.
        static const CLeeString BottomLeftAttribute;        //!< Attribute name that stores colour for bottom-left corner.
        static const CLeeString BottomRightAttribute;       //!< Attribute name that stores colour for bottom-right corner.
        static const CLeeString ImagesetAttribute;          //!< Attribute name that stores name of an imageset.
        static const CLeeString ImageAttribute;             //!< Attribute name that stores name of an image.
        static const CLeeString TypeAttribute;              //!< Attribute name that stores a type string.
        static const CLeeString NameAttribute;              //!< Attribute name that stores name string
        static const CLeeString PriorityAttribute;          //!< Attribute name that stores an integer priority.
        static const CLeeString SectionNameAttribute;       //!< Attribute name that stores an imagery section name.
        static const CLeeString RendererAttribute;          //!< Attribute name that stores the name of a window renderer factory.
        static const CLeeString ScaleAttribute;             //!< Attribute name that stores a UDim scale value.
        static const CLeeString OffsetAttribute;            //!< Attribute name that stores a UDim offset value.
        static const CLeeString ValueAttribute;             //!< Attribute name that stores a property value string.
        static const CLeeString DimensionAttribute;         //!< Attribute name that stores a dimension type.
        static const CLeeString WidgetAttribute;            //!< Attribute name that stores the name of a widget (suffix).
        static const CLeeString StringAttribute;            //!< Attribute name that stores a string of text.
        static const CLeeString FontAttribute;              //!< Attribute name that stores the name of a font.
        static const CLeeString InitialValueAttribute;      //!< Attribute name that stores the initial default value for a property definition.
        static const CLeeString ClippedAttribute;           //!< Attribute name that stores whether some component will be clipped.
        static const CLeeString OperatorAttribute;          //!< Attribute name that stores the name of an operator.
        static const CLeeString PaddingAttribute;           //!< Attribute name that stores some padding value..
        static const CLeeString TargetPropertyAttribute;    //!< Attribute name that stores a name of a target property.
        static const CLeeString PropertyAttribute;          //!< Attribute name that stores the name of a property.
        //! Attribute name that stores a help string.
        static const CLeeString HelpStringAttribute;


        /*************************************************************************
            implementation methods
        **************************************************************************/
        void assignAreaDimension(Dimension& dim);
     /*   void assignColours(const ColourRect& colours);*/
        void doBaseDimStart(const BaseDim* dim);
        void tagDreamLookStart(const XMLAttributes& attributes);
        void tagWidgetLookStart(const XMLAttributes& attributes);
        void tagImagerySectionStart(const XMLAttributes& attributes);
        void tagStateImageryStart(const XMLAttributes& attributes);
        void tagLayerStart(const XMLAttributes& attributes);
        void tagSectionStart(const XMLAttributes& attributes);
        void tagImageryComponentStart(const XMLAttributes& attributes);
        void tagFrameComponentStart(const XMLAttributes& attributes);
		 void tagTextComponentStart(const XMLAttributes& attributes);
        void tagAreaStart(const XMLAttributes& attributes);
        void tagImageStart(const XMLAttributes& attributes);
        void tagVertFormatStart(const XMLAttributes& attributes);
        void tagHorzFormatStart(const XMLAttributes& attributes);
        void tagVertAlignmentStart(const XMLAttributes& attributes);
        void tagHorzAlignmentStart(const XMLAttributes& attributes);
		//属性定义开始
        void tagPropertyStart(const XMLAttributes& attributes);
        void tagDimStart(const XMLAttributes& attributes);
        void tagUnifiedDimStart(const XMLAttributes& attributes);
        void tagAbsoluteDimStart(const XMLAttributes& attributes);
        void tagImageDimStart(const XMLAttributes& attributes);
        void tagFontDimStart(const XMLAttributes& attributes);
        void tagPropertyDimStart(const XMLAttributes& attributes);
		//命名空间开始
        void tagNamedAreaStart(const XMLAttributes& attributes);
        void tagPropertyDefinitionStart(const XMLAttributes& attributes);
        void tagPropertyLinkDefinitionStart(const XMLAttributes& attributes);
        void tagDimOperatorStart(const XMLAttributes& attributes);
        void tagVertFormatPropertyStart(const XMLAttributes& attributes);
        void tagHorzFormatPropertyStart(const XMLAttributes& attributes);
        void tagAreaPropertyStart(const XMLAttributes& attributes);
        void tagImagePropertyStart(const XMLAttributes& attributes);
        void tagFontPropertyStart(const XMLAttributes& attributes);
		//
		void tagTextVertFormatStart(const XMLAttributes& attributes);
        void tagTextHorzFormatStart(const XMLAttributes& attributes);

        //! Function to handle PropertyLinkTarget elements.
        void tagPropertyLinkTargetStart(const XMLAttributes& attributes);
        
        void tagDreamLookEnd();
        void tagWidgetLookEnd();
        void tagImagerySectionEnd();
        void tagStateImageryEnd();
        void tagLayerEnd();
        void tagSectionEnd();
        void tagImageryComponentEnd();
        void tagFrameComponentEnd();
		void tagTextComponentEnd();
        void tagAreaEnd();
        void tagNamedAreaEnd();
        void tagAnyDimEnd();

        //! Function to handle closing PropertyLinkDefinition XML element.
        void tagPropertyLinkDefinitionEnd();
        void registerElementStartHandler(const CLeeString& element, ElementStartHandler handler);
        void registerElementEndHandler(const CLeeString& element, ElementEndHandler handler);

        /*************************************************************************
            Implementation Data
        *************************************************************************/

        // these are used to implement the handler without using a huge
        // if / else if /else construct, we just register the element name, and
        // handler member function, and everything else is done using those
        // mappings.
        ElementStartHandlerMap  d_startHandlersMap;
        ElementEndHandlerMap    d_endHandlersMap;

        // these hold pointers to various objects under construction.
        CWidgetLook*     d_widgetlook;
		CWidgetLookPackage*  d_widgetPack;
        ImagerySection*     d_imagerysection;
        StateImagery*       d_stateimagery;
        LayerDesc* d_layer;
        SectionDesc* d_section;
        ImageryComponent*   d_imagerycomponent;
        ComponentArea*   d_area;
        Dimension       d_dimension;
        NamedArea*      d_namedArea;
        FrameComponent*  d_framecomponent;
        TextComponent* d_textcomponent;
		//
        std::vector<BaseDim*>    d_dimStack;

        PropertyLinkDefinition* d_propertyLink;
    };

#endif  // end of guard _Skin_xmlHandler_h_

