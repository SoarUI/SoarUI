#include "widgetlook_xmlHandler.h"
#include "../Widgetslook/WidgetLookManager.h"
#include "../Widgetslook/WidgetLook.h"
#include "../FrameComponent.h"
#include "../TextComponent.h"
#include "../NamedArea.h"
#include "../Propertys/PropertyDefinition.h"
#include "../Propertys/Property.h"
#include "../SoarXMLHelper.h"
#include "../../SoarHeader/leelog.h"
#include <sstream>
#include "../ImageDim.h"
    ////////////////////////////////////////////////////////////////////////////////
    // element names
    const CLeeString CWidgetlook_xmlHandler::tagDreamLook( _T("DreamLook") );
    const CLeeString CWidgetlook_xmlHandler::tagWidgetLook( _T("WidgetLook") );
    const CLeeString CWidgetlook_xmlHandler::tagImagerySection( _T("ImagerySection") );
    const CLeeString CWidgetlook_xmlHandler::tagStateImagery( _T("StateImagery") );
    const CLeeString CWidgetlook_xmlHandler::tagLayer( _T("Layer") );
    const CLeeString CWidgetlook_xmlHandler::tagSection( _T("Section") );
    const CLeeString CWidgetlook_xmlHandler::tagImageryComponent( _T("ImageryComponent") );
    const CLeeString CWidgetlook_xmlHandler::tagFrameComponent( _T("FrameComponent") );
	const CLeeString CWidgetlook_xmlHandler::tagTextComponent( _T("TextComponent"));
    const CLeeString CWidgetlook_xmlHandler::tagArea( _T("Area") );
    const CLeeString CWidgetlook_xmlHandler::tagImage( _T("Image") );
    const CLeeString CWidgetlook_xmlHandler::tagVertFormat( _T("VertFormat") );
    const CLeeString CWidgetlook_xmlHandler::tagHorzFormat( _T("HorzFormat") );
    const CLeeString CWidgetlook_xmlHandler::tagVertAlignment( _T("VertAlignment") );
    const CLeeString CWidgetlook_xmlHandler::tagHorzAlignment( _T("HorzAlignment") );
    const CLeeString CWidgetlook_xmlHandler::tagProperty( _T("Property") );
    const CLeeString CWidgetlook_xmlHandler::tagDim( _T("Dim") );
    const CLeeString CWidgetlook_xmlHandler::tagUnifiedDim( _T("UnifiedDim") );
    const CLeeString CWidgetlook_xmlHandler::tagAbsoluteDim( _T("AbsoluteDim") );
    const CLeeString CWidgetlook_xmlHandler::tagImageDim( _T("ImageDim") );
    const CLeeString CWidgetlook_xmlHandler::tagFontDim( _T("FontDim") );
    const CLeeString CWidgetlook_xmlHandler::tagPropertyDim( _T("PropertyDim") );
    const CLeeString CWidgetlook_xmlHandler::tagNamedArea( _T("NamedArea") );
    const CLeeString CWidgetlook_xmlHandler::tagPropertyDefinition( _T("PropertyDefinition") );
    const CLeeString CWidgetlook_xmlHandler::tagPropertyLinkDefinition( _T("PropertyLinkDefinition") );
    const CLeeString CWidgetlook_xmlHandler::tagPropertyLinkTarget( _T("PropertyLinkTarget") );
    const CLeeString CWidgetlook_xmlHandler::tagDimOperator( _T("DimOperator") );
    const CLeeString CWidgetlook_xmlHandler::tagVertFormatProperty( _T("VertFormatProperty") );
    const CLeeString CWidgetlook_xmlHandler::tagHorzFormatProperty( _T("HorzFormatProperty") );
    const CLeeString CWidgetlook_xmlHandler::tagAreaProperty( _T("AreaProperty") );
    const CLeeString CWidgetlook_xmlHandler::tagImageProperty( _T("ImageProperty") );
    const CLeeString CWidgetlook_xmlHandler::tagFontProperty( _T("FontProperty") );
	const CLeeString  CWidgetlook_xmlHandler::tagTextVertFormat( _T("TextVertFormat") );
	const CLeeString  CWidgetlook_xmlHandler::tagTextHorzFormat( _T("TextHorzFormat") );
    // attribute names
    const CLeeString CWidgetlook_xmlHandler::TopLeftAttribute( _T("topLeft") );
    const CLeeString CWidgetlook_xmlHandler::TopRightAttribute( _T("topRight") );
    const CLeeString CWidgetlook_xmlHandler::BottomLeftAttribute( _T("bottomLeft") );
    const CLeeString CWidgetlook_xmlHandler::BottomRightAttribute( _T("bottomRight") );
    const CLeeString CWidgetlook_xmlHandler::ImagesetAttribute( _T("imageset") );
    const CLeeString CWidgetlook_xmlHandler::ImageAttribute( _T("image") );
    const CLeeString CWidgetlook_xmlHandler::TypeAttribute( _T("type") );
    const CLeeString CWidgetlook_xmlHandler::NameAttribute( _T("name") );
    const CLeeString CWidgetlook_xmlHandler::PriorityAttribute( _T("priority") );
    const CLeeString CWidgetlook_xmlHandler::SectionNameAttribute( _T("section") );
    const CLeeString CWidgetlook_xmlHandler::RendererAttribute( _T("renderer") );
    const CLeeString CWidgetlook_xmlHandler::ScaleAttribute( _T("scale") );
    const CLeeString CWidgetlook_xmlHandler::OffsetAttribute( _T("offset") );
    const CLeeString CWidgetlook_xmlHandler::ValueAttribute( _T("value") );
    const CLeeString CWidgetlook_xmlHandler::DimensionAttribute( _T("dimension") );
    const CLeeString CWidgetlook_xmlHandler::WidgetAttribute( _T("widget") );
    const CLeeString CWidgetlook_xmlHandler::StringAttribute( _T("string") );
    const CLeeString CWidgetlook_xmlHandler::FontAttribute( _T("font") );
    const CLeeString CWidgetlook_xmlHandler::InitialValueAttribute( _T("initialValue") );
    const CLeeString CWidgetlook_xmlHandler::ClippedAttribute( _T("clipped") );
    const CLeeString CWidgetlook_xmlHandler::OperatorAttribute( _T("op") );
    const CLeeString CWidgetlook_xmlHandler::PaddingAttribute( _T("padding") );
    const CLeeString CWidgetlook_xmlHandler::TargetPropertyAttribute( _T("targetProperty") );
    const CLeeString CWidgetlook_xmlHandler::PropertyAttribute( _T("property") );
    const CLeeString CWidgetlook_xmlHandler::HelpStringAttribute( _T("help") );

    ////////////////////////////////////////////////////////////////////////////////


    CWidgetlook_xmlHandler::CWidgetlook_xmlHandler(void) :
        d_widgetlook(0),
        d_imagerysection(0),
        d_stateimagery(0),
        d_layer(0),
        d_section(0),
        d_imagerycomponent(0),
        d_area(0),
        d_namedArea(0),
        d_framecomponent(0),
        d_propertyLink(0),
		d_widgetPack(0),
		d_textcomponent(0)
    {
        // register element start handlers
        registerElementStartHandler(tagDreamLook, &CWidgetlook_xmlHandler::tagDreamLookStart);
        registerElementStartHandler(tagWidgetLook, &CWidgetlook_xmlHandler::tagWidgetLookStart);
        registerElementStartHandler(tagImagerySection, &CWidgetlook_xmlHandler::tagImagerySectionStart);
        registerElementStartHandler(tagStateImagery, &CWidgetlook_xmlHandler::tagStateImageryStart);
        registerElementStartHandler(tagLayer, &CWidgetlook_xmlHandler::tagLayerStart);
        registerElementStartHandler(tagSection, &CWidgetlook_xmlHandler::tagSectionStart);
        registerElementStartHandler(tagImageryComponent, &CWidgetlook_xmlHandler::tagImageryComponentStart);
        registerElementStartHandler(tagFrameComponent, &CWidgetlook_xmlHandler::tagFrameComponentStart);
		registerElementStartHandler(tagTextComponent, &CWidgetlook_xmlHandler::tagTextComponentStart);
        registerElementStartHandler(tagArea, &CWidgetlook_xmlHandler::tagAreaStart);
        registerElementStartHandler(tagImage, &CWidgetlook_xmlHandler::tagImageStart);
        registerElementStartHandler(tagVertFormat, &CWidgetlook_xmlHandler::tagVertFormatStart);
        registerElementStartHandler(tagHorzFormat, &CWidgetlook_xmlHandler::tagHorzFormatStart);
        registerElementStartHandler(tagVertAlignment, &CWidgetlook_xmlHandler::tagVertAlignmentStart);
        registerElementStartHandler(tagHorzAlignment, &CWidgetlook_xmlHandler::tagHorzAlignmentStart);
        registerElementStartHandler(tagProperty, &CWidgetlook_xmlHandler::tagPropertyStart);
        registerElementStartHandler(tagDim, &CWidgetlook_xmlHandler::tagDimStart);
        registerElementStartHandler(tagUnifiedDim, &CWidgetlook_xmlHandler::tagUnifiedDimStart);
        registerElementStartHandler(tagAbsoluteDim, &CWidgetlook_xmlHandler::tagAbsoluteDimStart);
        registerElementStartHandler(tagImageDim, &CWidgetlook_xmlHandler::tagImageDimStart);
 
        registerElementStartHandler(tagFontDim, &CWidgetlook_xmlHandler::tagFontDimStart);
        registerElementStartHandler(tagPropertyDim, &CWidgetlook_xmlHandler::tagPropertyDimStart);
        registerElementStartHandler(tagNamedArea, &CWidgetlook_xmlHandler::tagNamedAreaStart);
        registerElementStartHandler(tagPropertyDefinition, &CWidgetlook_xmlHandler::tagPropertyDefinitionStart);
        registerElementStartHandler(tagPropertyLinkDefinition, &CWidgetlook_xmlHandler::tagPropertyLinkDefinitionStart);
        registerElementStartHandler(tagDimOperator, &CWidgetlook_xmlHandler::tagDimOperatorStart);
        registerElementStartHandler(tagVertFormatProperty, &CWidgetlook_xmlHandler::tagVertFormatPropertyStart);
        registerElementStartHandler(tagHorzFormatProperty, &CWidgetlook_xmlHandler::tagHorzFormatPropertyStart);
        registerElementStartHandler(tagAreaProperty, &CWidgetlook_xmlHandler::tagAreaPropertyStart);
        registerElementStartHandler(tagImageProperty, &CWidgetlook_xmlHandler::tagImagePropertyStart);
        registerElementStartHandler(tagFontProperty, &CWidgetlook_xmlHandler::tagFontPropertyStart);
        registerElementStartHandler(tagPropertyLinkTarget, &CWidgetlook_xmlHandler::tagPropertyLinkTargetStart);
		///
		registerElementStartHandler(tagTextVertFormat, &CWidgetlook_xmlHandler::tagTextVertFormatStart);
        registerElementStartHandler(tagTextHorzFormat, &CWidgetlook_xmlHandler::tagTextHorzFormatStart);
     
        // register element end handlers
        registerElementEndHandler(tagDreamLook, &CWidgetlook_xmlHandler::tagDreamLookEnd);
        registerElementEndHandler(tagWidgetLook, &CWidgetlook_xmlHandler::tagWidgetLookEnd);
        registerElementEndHandler(tagImagerySection, &CWidgetlook_xmlHandler::tagImagerySectionEnd);
        registerElementEndHandler(tagStateImagery, &CWidgetlook_xmlHandler::tagStateImageryEnd);
        registerElementEndHandler(tagLayer, &CWidgetlook_xmlHandler::tagLayerEnd);
        registerElementEndHandler(tagSection, &CWidgetlook_xmlHandler::tagSectionEnd);
        registerElementEndHandler(tagImageryComponent, &CWidgetlook_xmlHandler::tagImageryComponentEnd);
        registerElementEndHandler(tagFrameComponent, &CWidgetlook_xmlHandler::tagFrameComponentEnd);
		registerElementEndHandler(tagTextComponent, &CWidgetlook_xmlHandler::tagTextComponentEnd);
        registerElementEndHandler(tagArea, &CWidgetlook_xmlHandler::tagAreaEnd);
        registerElementEndHandler(tagUnifiedDim, &CWidgetlook_xmlHandler::tagAnyDimEnd);
        registerElementEndHandler(tagAbsoluteDim, &CWidgetlook_xmlHandler::tagAnyDimEnd);
        registerElementEndHandler(tagImageDim, &CWidgetlook_xmlHandler::tagAnyDimEnd);
        registerElementEndHandler(tagFontDim, &CWidgetlook_xmlHandler::tagAnyDimEnd);
        registerElementEndHandler(tagPropertyDim, &CWidgetlook_xmlHandler::tagAnyDimEnd);
        registerElementEndHandler(tagNamedArea, &CWidgetlook_xmlHandler::tagNamedAreaEnd);
        registerElementEndHandler(tagPropertyLinkDefinition, &CWidgetlook_xmlHandler::tagPropertyLinkDefinitionEnd);
    }

    CWidgetlook_xmlHandler::~CWidgetlook_xmlHandler()
    {}

    /*************************************************************************
        Handle an opening XML element tag.
    *************************************************************************/
    void CWidgetlook_xmlHandler::elementStart(const CLeeString& element, const XMLAttributes& attributes)
    {
        // find registered handler for this element.
        ElementStartHandlerMap::const_iterator iter = d_startHandlersMap.find(element);

        // if a handler existed
        if (iter != d_startHandlersMap.end())
        {
            // call the handler for this element
            (this->*(iter->second))(attributes);
        }
        // no handler existed
      
    }

    /*************************************************************************
        Handle a closing XML element tag
    *************************************************************************/
    void CWidgetlook_xmlHandler::elementEnd(const CLeeString& element)
    {
        // find registered handler for this element.
        ElementEndHandlerMap::const_iterator iter = d_endHandlersMap.find(element);

        // if a handler existed
        if (iter != d_endHandlersMap.end())
            // call the handler for this element
            (this->*(iter->second))();
    }

  
    /*************************************************************************
        Method that handles the opening Skin XML tag.
    *************************************************************************/
    void CWidgetlook_xmlHandler::tagDreamLookStart(const XMLAttributes& attr)
    {
		
		assert(d_widgetPack==NULL);
		d_widgetPack =new CWidgetLookPackage;
		d_widgetPack->setName(attr.getValueAsString(NameAttribute,_T("")));
		Dbgc_DumpX(_T("leeduires"),
			_T("===== loading resources Dreamlook:%s========"),d_widgetPack->getName().c_str());
    }

    /*************************************************************************
        Method that handles the opening WidgetLook XML element.
    *************************************************************************/
    void CWidgetlook_xmlHandler::tagWidgetLookStart(const XMLAttributes& attributes)
    {
        assert(d_widgetlook == 0);
        d_widgetlook = new CWidgetLook(attributes.getValueAsString(NameAttribute),
			attributes.getValueAsString(TypeAttribute));
		Dbgc_DumpX(_T("leeduires"),
			_T("===== loading resources Widgetlook:<%s>  start========"),d_widgetlook->getName().c_str());
    }

   
   
    /*************************************************************************
        Method that handles the opening ImagerySection XML element.
    *************************************************************************/
    void CWidgetlook_xmlHandler::tagImagerySectionStart(const XMLAttributes& attributes)
    {
        assert(d_imagerysection == 0);
        d_imagerysection = new ImagerySection(attributes.getValueAsString(NameAttribute));
    }
    /*************************************************************************
        Method that handles the opening StateImagery XML element.
    *************************************************************************/
    void CWidgetlook_xmlHandler::tagStateImageryStart(const XMLAttributes& attributes)
    {
        assert(d_stateimagery == 0);
        d_stateimagery = new StateImagery(attributes.getValueAsString(NameAttribute));
        d_stateimagery->setClippedToDisplay(!attributes.getValueAsBool(ClippedAttribute, true));
    }

    /*************************************************************************
        Method that handles the opening Layer XML element.
    *************************************************************************/
    void CWidgetlook_xmlHandler::tagLayerStart(const XMLAttributes& attributes)
    {
        assert(d_layer == 0);
        d_layer = new LayerDesc(attributes.getValueAsInteger(PriorityAttribute, 0));
    }

    /*************************************************************************
        Method that handles the opening Section XML element.
    *************************************************************************/
    void CWidgetlook_xmlHandler::tagSectionStart(const XMLAttributes& attributes)
    {
        assert(d_section == 0);
        assert(d_widgetlook != 0);
		d_section =
			new SectionDesc(d_widgetlook->getName() ,attributes.getValueAsString(SectionNameAttribute));
    }

    /*************************************************************************
        Method that handles the opening ImageryComponent XML element.
    *************************************************************************/
    void CWidgetlook_xmlHandler::tagImageryComponentStart(const XMLAttributes&)
    {
        assert(d_imagerycomponent == 0);
        d_imagerycomponent = new ImageryComponent();
        //设置宿主信息 
		if (d_widgetlook)
		{
			d_imagerycomponent->setOwnerWidget(d_widgetlook->getName());
		}
    }

    /*************************************************************************
        Method that handles the opening FrameComponent XML element.
    *************************************************************************/
    void CWidgetlook_xmlHandler::tagFrameComponentStart(const XMLAttributes&)
    {
        assert(d_framecomponent == 0);
        d_framecomponent = new FrameComponent();
		//设置宿主信息
		if (d_widgetlook)
		{
			d_framecomponent->setOwnerWidget(d_widgetlook->getName());
		}
    }
    /*
	*/
	void CWidgetlook_xmlHandler::tagTextComponentStart(const XMLAttributes&)
	{
		assert(d_textcomponent == 0);
		d_textcomponent = new TextComponent();
		////设置宿主信息
		if (d_widgetlook)
		{
			d_textcomponent->setOwnerWidget(d_widgetlook->getName());
		}
	}
    /*************************************************************************
        Method that handles the opening Area XML element.
    *************************************************************************/
    void CWidgetlook_xmlHandler::tagAreaStart(const XMLAttributes&)
    {
        assert(d_area == 0);
        d_area = new ComponentArea();
    }

    /*************************************************************************
        Method that handles the opening Image XML element.
    *************************************************************************/
    void CWidgetlook_xmlHandler::tagImageStart(const XMLAttributes& attributes)
    {
        if (d_imagerycomponent)
        {
            d_imagerycomponent->setImagePackage(
				attributes.getValueAsString(ImagesetAttribute), attributes.getValueAsString(ImageAttribute));
        }
        else if (d_framecomponent)
        {
            d_framecomponent->setImagePackage(
                CLooksXMLHelper::stringToFrameImageComponent(attributes.getValueAsString(TypeAttribute)),
                attributes.getValueAsString(ImagesetAttribute),
                attributes.getValueAsString(ImageAttribute));
        }
		
    }

    /*************************************************************************
        Method that handles the opening VertFormat XML element.
    *************************************************************************/
    void CWidgetlook_xmlHandler::tagVertFormatStart(const XMLAttributes& attributes)
    {
        if (d_framecomponent)
        {
            d_framecomponent->setBackgroundVerticalFormatting(
				CLooksXMLHelper::stringToVertFormat(attributes.getValueAsString(TypeAttribute)));
        }
        else if (d_imagerycomponent)
        {
            d_imagerycomponent->setVerticalFormatting(
				CLooksXMLHelper::stringToVertFormat(attributes.getValueAsString(TypeAttribute)));
        }
       
    }

    /*************************************************************************
        Method that handles the opening HorzFormat XML element.
    *************************************************************************/
    void CWidgetlook_xmlHandler::tagHorzFormatStart(const XMLAttributes& attributes)
    {
        if (d_framecomponent)
        {
            d_framecomponent->setBackgroundHorizontalFormatting(
				CLooksXMLHelper::stringToHorzFormat(attributes.getValueAsString(TypeAttribute)));
        }
		
        else if (d_imagerycomponent)
        {
            d_imagerycomponent->setHorizontalFormatting(
				CLooksXMLHelper::stringToHorzFormat(attributes.getValueAsString(TypeAttribute)));
        }
		
    }

    /*************************************************************************
        Method that handles the opening VertAlignment XML element.
    *************************************************************************/
    void CWidgetlook_xmlHandler::tagVertAlignmentStart(const XMLAttributes& attributes)
    {
    }

    /*************************************************************************
        Method that handles the opening HorzAlignment XML element.
    *************************************************************************/
    void CWidgetlook_xmlHandler::tagHorzAlignmentStart(const XMLAttributes& attributes)
    {
    }

    /*************************************************************************
        Method that handles the opening Property XML element.
    *************************************************************************/
    void CWidgetlook_xmlHandler::tagPropertyStart(const XMLAttributes& attributes)
    {
        assert(d_widgetPack != 0);
    }
//---------------------------------------------------------统一坐标开始---------------------------------
	 /*************************************************************************
        Method that handles the opening DimOperator XML element.
    *************************************************************************/
    void CWidgetlook_xmlHandler::tagDimOperatorStart(const XMLAttributes& attributes)
    {
        if (!d_dimStack.empty())
        {
            d_dimStack.back()->setDimensionOperator(
				CLooksXMLHelper::stringToDimensionOperator(attributes.getValueAsString(OperatorAttribute)));
        }
    }

    /*************************************************************************
        Assign a dimension to a ComponentArea depending upon the dimension's
        type.
    *************************************************************************/
    void CWidgetlook_xmlHandler::assignAreaDimension(Dimension& dim)
    {
        if (d_area)
        {
            switch (dim.getDimensionType())
            {
            case DT_LEFT_EDGE:
            case DT_X_POSITION:
                d_area->d_left = dim;
                break;
            case DT_TOP_EDGE:
            case DT_Y_POSITION:
                d_area->d_top = dim;
                break;
            case DT_RIGHT_EDGE:
            case DT_WIDTH:
                d_area->d_right_or_width = dim;
                break;
            case DT_BOTTOM_EDGE:
            case DT_HEIGHT:
                d_area->d_bottom_or_height = dim;
                break;
            default:
			;
            }
        }
    }
    /*************************************************************************
        Method that performs common handling for all *Dim elements.
    *************************************************************************/
    void CWidgetlook_xmlHandler::doBaseDimStart(const BaseDim* dim)
    {
        BaseDim* cloned = dim->clone();
        d_dimStack.push_back(cloned);
    }

    /*************************************************************************
        Method that handles the opening Dim XML element.
    *************************************************************************/
    void CWidgetlook_xmlHandler::tagDimStart(const XMLAttributes& attributes)
    {
        d_dimension.setDimensionType(CLooksXMLHelper::stringToDimensionType(attributes.getValueAsString(TypeAttribute)));
    }

    /*************************************************************************
        Method that handles the opening UnifiedDim XML element.
    *************************************************************************/
    void CWidgetlook_xmlHandler::tagUnifiedDimStart(const XMLAttributes& attributes)
    {
        UnifiedDim base(
            UDim(attributes.getValueAsFloat(ScaleAttribute, 0.0f),
                 attributes.getValueAsFloat(OffsetAttribute, 0.0f)),
            CLooksXMLHelper::stringToDimensionType(attributes.getValueAsString(TypeAttribute)));

        doBaseDimStart(&base);
    }

    /*************************************************************************
        Method that handles the opening AbsoluteDim XML element.
    *************************************************************************/
    void CWidgetlook_xmlHandler::tagAbsoluteDimStart(const XMLAttributes& attributes)
    {
        AbsoluteDim base(attributes.getValueAsFloat(ValueAttribute, 0.0f));
        doBaseDimStart(&base);
    }

    /*************************************************************************
        Method that handles the opening ImageDim XML element.
    *************************************************************************/
    void CWidgetlook_xmlHandler::tagImageDimStart(const XMLAttributes& attributes)
    {
        ImageDim base(attributes.getValueAsString(ImagesetAttribute),
                      attributes.getValueAsString(ImageAttribute),
                      CLooksXMLHelper::stringToDimensionType(attributes.getValueAsString(DimensionAttribute)));

        doBaseDimStart(&base);
    }
//---------------------------------------------------统一坐标结束------------------------
    /*************************************************************************
        Method that handles the opening FontDim XML element.
    *************************************************************************/
    void CWidgetlook_xmlHandler::tagFontDimStart(const XMLAttributes& attributes)
    {
     
    }
 /*************************************************************************
        Method that handles the closing of all *Dim XML elements.
    *************************************************************************/
    void CWidgetlook_xmlHandler::tagAnyDimEnd()
    {
        if (!d_dimStack.empty())
        {
            BaseDim* currDim = d_dimStack.back();
            d_dimStack.pop_back();

            if (!d_dimStack.empty())
            {
                d_dimStack.back()->setOperand(*currDim);
            }
            else
            {
                d_dimension.setBaseDimension(*currDim);
                assignAreaDimension(d_dimension);
            }

            // release the dim we popped.
            delete currDim;
        }
    }
    /*************************************************************************
        Method that handles the closing Area XML element.
    *************************************************************************/
    void CWidgetlook_xmlHandler::tagAreaEnd()
    {
        assert( 
			   (d_imagerycomponent != 0)|| 
			   (d_textcomponent != 0)   ||
			    d_namedArea != 0 || 
				d_framecomponent != 0);
        assert(d_area != 0);
        if (d_framecomponent)
        {
            d_framecomponent->setComponentArea(*d_area);
        }
        else if (d_imagerycomponent)
        {
            d_imagerycomponent->setComponentArea(*d_area);
        }
		else if (d_textcomponent)
		{
			d_textcomponent->setComponentArea(*d_area);
		}
        else if (d_namedArea)
        {
            d_namedArea->setArea(*d_area);
		}
		
        delete d_area;
        d_area = 0;
    }

    /*************************************************************************
        Method that handles the closing NamedArea XML element.
    *************************************************************************/
    void CWidgetlook_xmlHandler::tagNamedAreaEnd()
    {
        assert(d_widgetlook != 0);

        if (d_namedArea)
        {
            d_widgetlook->addNamedArea(*d_namedArea);
			delete d_namedArea;
            d_namedArea = 0;
        }
    }
    /*************************************************************************
        Method that handles the opening PropertyDim XML element.
    *************************************************************************/
    void CWidgetlook_xmlHandler::tagPropertyDimStart(const XMLAttributes& attributes)
    {
        CLeeString str_type = attributes.getValueAsString(TypeAttribute);
        DimensionType type = DT_INVALID;
        if (!str_type.empty())
            type = CLooksXMLHelper::stringToDimensionType(str_type);

        PropertyDim base(attributes.getValueAsString(WidgetAttribute),
                         attributes.getValueAsString(NameAttribute),
                         type);

        doBaseDimStart(&base);
    }

    /*************************************************************************
        Method that handles the opening NamedArea XML element.
    *************************************************************************/
    void CWidgetlook_xmlHandler::tagNamedAreaStart(const XMLAttributes& attributes)
    {
        assert(d_namedArea == 0);
        d_namedArea = new NamedArea(attributes.getValueAsString(NameAttribute));
    }

    /*************************************************************************
        Method that handles the opening PropertyDefinition XML element.
    *************************************************************************/
    void CWidgetlook_xmlHandler::tagPropertyDefinitionStart(const XMLAttributes& attributes)
    {
        assert(d_widgetlook);

        PropertyDefinition prop(
            attributes.getValueAsString(NameAttribute),
            attributes.getValueAsString(InitialValueAttribute)
        );
        d_widgetlook->addPropertyDefine(prop);
    }

    /*************************************************************************
        Method that handles the opening PropertyLinkDefinition XML element.
    *************************************************************************/
    void CWidgetlook_xmlHandler::tagPropertyLinkDefinitionStart(const XMLAttributes& attributes)
    {
        
    }

   
    /*************************************************************************
        Method that handles the opening VertFormatProperty XML element.
    *************************************************************************/
    void CWidgetlook_xmlHandler::tagVertFormatPropertyStart(const XMLAttributes& attributes)
    {
        if (d_framecomponent)
            d_framecomponent->setVertFormattingPropertySource(attributes.getValueAsString(NameAttribute));
        else if (d_imagerycomponent)
            d_imagerycomponent->setVertFormattingPropertySource(attributes.getValueAsString(NameAttribute));
		 else if (d_textcomponent)
            d_textcomponent->setVertFormattingPropertySource(attributes.getValueAsString(NameAttribute));

    }

    /*************************************************************************
        Method that handles the opening HorzFormatProperty XML element.
    *************************************************************************/
    void CWidgetlook_xmlHandler::tagHorzFormatPropertyStart(const XMLAttributes& attributes)
    {
        if (d_framecomponent)
            d_framecomponent->setHorzFormattingPropertySource(attributes.getValueAsString(NameAttribute));
        else if (d_imagerycomponent)
            d_imagerycomponent->setHorzFormattingPropertySource(attributes.getValueAsString(NameAttribute));
		else if (d_textcomponent)
            d_textcomponent->setHorzFormattingPropertySource(attributes.getValueAsString(NameAttribute));
    }
	void CWidgetlook_xmlHandler::tagTextVertFormatStart(const XMLAttributes& attributes){
		 if (d_textcomponent)
            d_textcomponent->setHorzFormattingPropertySource(attributes.getValueAsString(ValueAttribute));
	}
    void CWidgetlook_xmlHandler::tagTextHorzFormatStart(const XMLAttributes& attributes){
		 if (d_textcomponent)
            d_textcomponent->setHorzFormattingPropertySource(attributes.getValueAsString(ValueAttribute));
	}
    /*************************************************************************
        Method that handles the opening AreaProperty XML element.
    *************************************************************************/
    void CWidgetlook_xmlHandler::tagAreaPropertyStart(const XMLAttributes& attributes)
    {
        assert (d_area != 0);

        d_area->setAreaPropertySource(attributes.getValueAsString(NameAttribute));
    }

    /*************************************************************************
        Method that handles the opening ImageProperty XML element.
    *************************************************************************/
    void CWidgetlook_xmlHandler::tagImagePropertyStart(const XMLAttributes& attributes)
    {
        assert(d_imagerycomponent != 0);

        d_imagerycomponent->setImagePropertySource(attributes.getValueAsString(NameAttribute));
    }


    /*************************************************************************
        Method that handles the opening FontProperty XML element.
    *************************************************************************/
    void CWidgetlook_xmlHandler::tagFontPropertyStart(const XMLAttributes& attributes)
    {
        assert(d_textcomponent != 0);

        d_textcomponent->setFontPropertySource(attributes.getValueAsString(NameAttribute));
    }
    /*************************************************************************
        Method that handles the closing Falagard XML element.
    *************************************************************************/
    void CWidgetlook_xmlHandler::tagDreamLookEnd()
    {
		Dbgc_DumpX(_T("leeduires"),
			_T("===== loading resources Dreamlook:<%s>  end========"),d_widgetPack->getName().c_str());
		WidgetLookManager* wlf=WidgetLookManager::getSingletonPtr();
		if (wlf)
		{
			wlf->addWidgetPackage(*d_widgetPack);
			delete d_widgetPack;
			d_widgetPack =NULL ;
		}
		
    }

    /*************************************************************************
        Method that handles the closing WidgetLook XML element.
    *************************************************************************/
    void CWidgetlook_xmlHandler::tagWidgetLookEnd()
    {
		assert(d_widgetPack);

        if (d_widgetPack)
        {
			Dbgc_DumpX(_T("leeduires"),
				_T("===== loading resources Widgetlook:<%s>  end========"),d_widgetlook->getName().c_str());
            d_widgetPack->addWidgetLook(*d_widgetlook);
			delete d_widgetlook;
            d_widgetlook = 0;
        }
    }

    /*************************************************************************
        Method that handles the closing ImagerySection XML element.
    *************************************************************************/
    void CWidgetlook_xmlHandler::tagImagerySectionEnd()
    {
        assert(d_widgetlook != 0);

        if (d_imagerysection)
        {
			d_widgetlook->addImagerySection(*d_imagerysection);		
            delete d_imagerysection;
            d_imagerysection = 0;
        }
    }

    /*************************************************************************
        Method that handles the closing StateImagery XML element.
    *************************************************************************/
    void CWidgetlook_xmlHandler::tagStateImageryEnd()
    {
        assert(d_widgetlook != 0);

        if (d_stateimagery)
        {
			d_widgetlook->addStateImagery(*d_stateimagery);
            delete d_stateimagery;
            d_stateimagery = 0;
        }
    }

    /*************************************************************************
        Method that handles the closing Layer XML element.
    *************************************************************************/
    void CWidgetlook_xmlHandler::tagLayerEnd()
    {
        assert(d_stateimagery != 0);

        if (d_layer)
        {
            d_stateimagery->addLayer(*d_layer);
			delete d_layer;
            d_layer = 0;
        }
    }

    /*************************************************************************
        Method that handles the closing Section XML element.
    *************************************************************************/
    void CWidgetlook_xmlHandler::tagSectionEnd()
    {
        assert(d_layer != 0);

        if (d_section)
        {
            d_layer->addSectionDesc(*d_section);
			delete d_section;
            d_section = 0;
        }
    }

    /*************************************************************************
        Method that handles the closing ImageryComponent XML element.
    *************************************************************************/
    void CWidgetlook_xmlHandler::tagImageryComponentEnd()
    {
        assert(d_imagerysection != 0);

        if (d_imagerycomponent)
        {
            d_imagerysection->addImageryComponent(*d_imagerycomponent);
			delete d_imagerycomponent;
            d_imagerycomponent = 0;
        }
    }

    /*************************************************************************
        Method that handles the closing FrameComponent XML element.
    *************************************************************************/
    void CWidgetlook_xmlHandler::tagFrameComponentEnd()
    {
        assert(d_imagerysection != 0);

        if (d_framecomponent)
        {
            d_imagerysection->addFrameComponent(*d_framecomponent);
			delete d_framecomponent;
            d_framecomponent = 0;
        }
    }
	void CWidgetlook_xmlHandler::tagTextComponentEnd()
	{
		assert(d_imagerysection != 0);

		if (d_textcomponent)
		{
			d_imagerysection->addTextComponent(*d_textcomponent);
			delete d_textcomponent;
			d_textcomponent = 0;
		}
	}
	
    void CWidgetlook_xmlHandler::tagPropertyLinkDefinitionEnd()
    {
       
        d_propertyLink = 0;
    }

    void CWidgetlook_xmlHandler::tagPropertyLinkTargetStart(const XMLAttributes& attributes)
    {
   
    }

    /*************************************************************************
        register a handler for the opening tag of an XML element
    *************************************************************************/
    void CWidgetlook_xmlHandler::registerElementStartHandler(const CLeeString& element, ElementStartHandler handler)
    {
        d_startHandlersMap[element] = handler;
    }

    /*************************************************************************
        register a handler for the closing tag of an XML element
    *************************************************************************/
    void CWidgetlook_xmlHandler::registerElementEndHandler(const CLeeString& element, ElementEndHandler handler)
    {
        d_endHandlersMap[element] = handler;
    }

