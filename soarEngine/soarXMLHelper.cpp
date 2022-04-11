
#include "soarXMLHelper.h"

    VerticalFormatting CLooksXMLHelper::stringToVertFormat(const CLeeString& str)
    {
        if (str == _T("CentreAligned"))
        {
            return VF_CENTRE_ALIGNED;
        }
        else if (str == _T("BottomAligned") )
        {
            return VF_BOTTOM_ALIGNED;
        }
        else if (str == _T("Tiled"))
        {
            return VF_TILED;
        }
        else if (str == _T("Stretched"))
        {
            return VF_STRETCHED;
        }
        else
        {
            return VF_TOP_ALIGNED;
        }
    }

    HorizontalFormatting CLooksXMLHelper::stringToHorzFormat(const CLeeString& str)
    {
        if (str == _T("CentreAligned"))
        {
            return HF_CENTRE_ALIGNED;
        }
        else if (str == _T("RightAligned"))
        {
            return HF_RIGHT_ALIGNED;
        }
        else if (str == _T("Tiled"))
        {
            return HF_TILED;
        }
        else if (str == _T("Stretched"))
        {
            return HF_STRETCHED;
        }
        else
        {
            return HF_LEFT_ALIGNED;
        }
    }

    VerticalAlignment CLooksXMLHelper::stringToVertAlignment(const CLeeString& str)
    {
        if (str == _T("CentreAligned"))
        {
            return VA_CENTRE;
        }
        else if (str == _T("BottomAligned"))
        {
            return VA_BOTTOM;
        }
        else
        {
            return VA_TOP;
        }
    }

    HorizontalAlignment CLooksXMLHelper::stringToHorzAlignment(const CLeeString& str)
    {
        if (str == _T("CentreAligned"))
        {
            return HA_CENTRE;
        }
        else if (str == _T("RightAligned"))
        {
            return HA_RIGHT;
        }
        else
        {
            return HA_LEFT;
        }
    }

    DimensionType CLooksXMLHelper::stringToDimensionType(const CLeeString& str)
    {
        if (str == _T("LeftEdge"))
        {
            return DT_LEFT_EDGE;
        }
        else if (str == _T("XPosition"))
        {
            return DT_X_POSITION;
        }
        else if (str == _T("TopEdge"))
        {
            return DT_TOP_EDGE;
        }
        else if (str == _T("YPosition"))
        {
            return DT_Y_POSITION;
        }
        else if (str == _T("RightEdge"))
        {
            return DT_RIGHT_EDGE;
        }
        else if (str == _T("BottomEdge"))
        {
            return DT_BOTTOM_EDGE;
        }
        else if (str == _T("Width"))
        {
            return DT_WIDTH;
        }
        else if (str == _T("Height"))
        {
            return DT_HEIGHT;
        }
        else if (str == _T("XOffset"))
        {
            return DT_X_OFFSET;
        }
        else if (str == _T("YOffset"))
        {
            return DT_Y_OFFSET;
        }
        else
        {
            return DT_INVALID;
        }
    }

    VerticalTextFormatting CLooksXMLHelper::stringToVertTextFormat(const CLeeString& str)
    {
        if (str == _T("CentreAligned"))
        {
            return VTF_CENTRE_ALIGNED;
        }
        else if (str == _T("BottomAligned"))
        {
            return VTF_BOTTOM_ALIGNED;
        }
        else
        {
            return VTF_TOP_ALIGNED;
        }
    }

    HorizontalTextFormatting CLooksXMLHelper::stringToHorzTextFormat(const CLeeString& str)
    {
        if (str == _T("CentreAligned"))
        {
            return HTF_CENTRE_ALIGNED;
        }
        else if (str == _T("RightAligned"))
        {
            return HTF_RIGHT_ALIGNED;
        }
        else if (str == _T("Justified"))
        {
            return HTF_JUSTIFIED;
        }
        else if (str == _T("WordWrapLeftAligned"))
        {
            return HTF_WORDWRAP_LEFT_ALIGNED;
        }
        else if (str == _T("WordWrapCentreAligned"))
        {
            return HTF_WORDWRAP_CENTRE_ALIGNED;
        }
        else if (str == _T("WordWrapRightAligned"))
        {
            return HTF_WORDWRAP_RIGHT_ALIGNED;
        }
        else if (str == _T("WordWrapJustified"))
        {
            return HTF_WORDWRAP_JUSTIFIED;
        }
        else
        {
            return HTF_LEFT_ALIGNED;
        }
    }

    FontMetricType CLooksXMLHelper::stringToFontMetricType(const CLeeString& str)
    {
        if (str == _T("LineSpacing"))
        {
            return FMT_LINE_SPACING;
        }
        else if (str == _T("Baseline"))
        {
            return FMT_BASELINE;
        }
        else
        {
            return FMT_HORZ_EXTENT;
        }
    }

    DimensionOperator CLooksXMLHelper::stringToDimensionOperator(const CLeeString& str)
    {
        if (str == _T("Add"))
        {
            return DOP_ADD;
        }
        else if (str == _T("Subtract"))
        {
            return DOP_SUBTRACT;
        }
        else if (str == _T("Multiply"))
        {
            return DOP_MULTIPLY;
        }
        else if (str == _T("Divide"))
        {
            return DOP_DIVIDE;
        }
        else
        {
            return DOP_NOOP;
        }
    }

    FrameImageComponent CLooksXMLHelper::stringToFrameImageComponent(const CLeeString& str)
    {
        if (str == _T("TopLeftCorner"))
        {
            return FIC_TOP_LEFT_CORNER;
        }
        if (str == _T("TopRightCorner"))
        {
            return FIC_TOP_RIGHT_CORNER;
        }
        if (str == _T("BottomLeftCorner"))
        {
            return FIC_BOTTOM_LEFT_CORNER;
        }
        if (str == _T("BottomRightCorner"))
        {
            return FIC_BOTTOM_RIGHT_CORNER;
        }
        if (str == _T("LeftEdge"))
        {
            return FIC_LEFT_EDGE;
        }
        if (str == _T("RightEdge"))
        {
            return FIC_RIGHT_EDGE;
        }
        if (str == _T("TopEdge"))
        {
            return FIC_TOP_EDGE;
        }
        if (str == _T("BottomEdge"))
        {
            return FIC_BOTTOM_EDGE;
        }
        else
        {
            return FIC_BACKGROUND;
        }
    }


    CLeeString CLooksXMLHelper::vertFormatToString(VerticalFormatting format)
    {
        switch (format)
        {
        case VF_BOTTOM_ALIGNED:
            return CLeeString( _T("BottomAligned") );
            break;
        case VF_CENTRE_ALIGNED:
            return CLeeString( _T("CentreAligned") );
            break;
        case VF_TILED:
            return CLeeString( _T("Tiled") );
            break;
        case VF_STRETCHED:
            return CLeeString( _T("Stretched") );
            break;
        default:
            return CLeeString( _T("TopAligned") );
            break;
        }
    }

    CLeeString CLooksXMLHelper::horzFormatToString(HorizontalFormatting format)
    {
        switch (format)
        {
        case HF_RIGHT_ALIGNED:
            return CLeeString( _T("RightAligned") );
            break;
        case HF_CENTRE_ALIGNED:
            return CLeeString( _T("CentreAligned") );
            break;
        case HF_TILED:
            return CLeeString( _T("Tiled") );
            break;
        case HF_STRETCHED:
            return CLeeString( _T("Stretched") );
            break;
        default:
            return CLeeString(_T("LeftAligned") );
            break;
        }
    }

    CLeeString CLooksXMLHelper::vertAlignmentToString(VerticalAlignment alignment)
    {
        switch (alignment)
        {
        case VA_BOTTOM:
            return CLeeString( _T("BottomAligned") );
            break;
        case VA_CENTRE:
            return CLeeString( _T("CentreAligned") );
            break;
        default:
            return CLeeString( _T("TopAligned") );
            break;
        }
    }

    CLeeString CLooksXMLHelper::horzAlignmentToString(HorizontalAlignment alignment)
    {
        switch (alignment)
        {
        case HA_RIGHT:
            return CLeeString( _T("RightAligned") );
            break;
        case HA_CENTRE:
            return CLeeString( _T("CentreAligned") );
            break;
        default:
            return CLeeString( _T("LeftAligned") );
            break;
        }
    }

    CLeeString CLooksXMLHelper::dimensionTypeToString(DimensionType dim)
    {
        switch (dim)
        {
        case DT_LEFT_EDGE:
            return CLeeString( _T("LeftEdge") );
            break;
        case DT_X_POSITION:
            return CLeeString( _T("XPosition") );
            break;
        case DT_TOP_EDGE:
            return CLeeString( _T("TopEdge") );
            break;
        case DT_Y_POSITION:
            return CLeeString( _T("YPosition") );
            break;
        case DT_RIGHT_EDGE:
            return CLeeString( _T("RightEdge") );
            break;
        case DT_BOTTOM_EDGE:
            return CLeeString( _T("BottomEdge") );
            break;
        case DT_WIDTH:
            return CLeeString(_T("Width") );
            break;
        case DT_HEIGHT:
            return CLeeString( _T("Height") );
            break;
        case DT_X_OFFSET:
            return CLeeString( _T("XOffset") );
            break;
        case DT_Y_OFFSET:
            return CLeeString( _T("YOffset") );
            break;
        default:
            return CLeeString( _T("Invalid") );
            break;
        }
    }

    CLeeString CLooksXMLHelper::vertTextFormatToString(VerticalTextFormatting format)
    {
        switch (format)
        {
        case VTF_BOTTOM_ALIGNED:
            return CLeeString( _T("BottomAligned") );
            break;
        case VTF_CENTRE_ALIGNED:
            return CLeeString( _T("CentreAligned") );
            break;
        default:
            return CLeeString( _T("TopAligned") );
            break;
        }
    }

    CLeeString CLooksXMLHelper::horzTextFormatToString(HorizontalTextFormatting format)
    {
        switch (format)
        {
        case HTF_RIGHT_ALIGNED:
            return CLeeString( _T("RightAligned") );
            break;
        case HTF_CENTRE_ALIGNED:
            return CLeeString( _T("CentreAligned") );
            break;
        case HTF_JUSTIFIED:
            return CLeeString( _T("Justified") );
            break;
        case HTF_WORDWRAP_LEFT_ALIGNED:
            return CLeeString( _T("WordWrapLeftAligned") );
            break;
        case HTF_WORDWRAP_RIGHT_ALIGNED:
            return CLeeString( _T("WordWrapRightAligned") );
            break;
        case HTF_WORDWRAP_CENTRE_ALIGNED:
            return CLeeString( _T("WordWrapCentreAligned") );
            break;
        case HTF_WORDWRAP_JUSTIFIED:
            return CLeeString( _T("WordWrapJustified") );
            break;
        default:
            return CLeeString( _T("LeftAligned") );
            break;
        }
    }

    CLeeString CLooksXMLHelper::fontMetricTypeToString(FontMetricType metric)
    {
        switch (metric)
        {
        case FMT_BASELINE:
            return CLeeString( _T("Baseline") );
            break;
        case FMT_HORZ_EXTENT:
            return CLeeString( _T("HorzExtent") );
            break;
        default:
            return CLeeString( _T("LineSpacing") );
            break;
        }
    }

    CLeeString CLooksXMLHelper::dimensionOperatorToString(DimensionOperator op)
    {
        switch (op)
        {
        case DOP_ADD:
            return CLeeString( _T("Add") );
            break;
        case DOP_SUBTRACT:
            return CLeeString( _T("Subtract") );
            break;
        case DOP_MULTIPLY:
            return CLeeString( _T("Multiply") );
            break;
        case DOP_DIVIDE:
            return CLeeString( _T("Divide") );
            break;
        default:
            return CLeeString( _T("Noop") );
            break;
        }
    }

    CLeeString CLooksXMLHelper::frameImageComponentToString(FrameImageComponent imageComp)
    {
        switch (imageComp)
        {
        case FIC_TOP_LEFT_CORNER:
            return CLeeString( _T("TopLeftCorner") );
            break;
        case FIC_TOP_RIGHT_CORNER:
            return CLeeString( _T("TopRightCorner") );
            break;
        case FIC_BOTTOM_LEFT_CORNER:
            return CLeeString( _T("BottomLeftCorner") );
            break;
        case FIC_BOTTOM_RIGHT_CORNER:
            return CLeeString( _T("BottomRightCorner") );
            break;
        case FIC_LEFT_EDGE:
            return CLeeString( _T("LeftEdge") );
            break;
        case FIC_RIGHT_EDGE:
            return CLeeString( _T("RightEdge") );
            break;
        case FIC_TOP_EDGE:
            return CLeeString( _T("TopEdge") );
            break;
        case FIC_BOTTOM_EDGE:
            return CLeeString( _T("BottomEdge") );
            break;
        default:
            return CLeeString( _T("Background") );
            break;
        }
    }