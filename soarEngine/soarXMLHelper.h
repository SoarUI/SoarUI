
#ifndef _XMLEnumHelper_h_
#define _XMLEnumHelper_h_

#include "../SoarHeader/leeString.h"
#include "../SoarHeader/Enums.h"

    class CLooksXMLHelper
    {
    public:
        static VerticalFormatting stringToVertFormat(const CLeeString& str);
        static HorizontalFormatting stringToHorzFormat(const CLeeString& str);
        static VerticalAlignment stringToVertAlignment(const CLeeString& str);
        static HorizontalAlignment stringToHorzAlignment(const CLeeString& str);
        static DimensionType stringToDimensionType(const CLeeString& str);
        static VerticalTextFormatting stringToVertTextFormat(const CLeeString& str);
        static HorizontalTextFormatting stringToHorzTextFormat(const CLeeString& str);
        static FontMetricType stringToFontMetricType(const CLeeString& str);
        static DimensionOperator stringToDimensionOperator(const CLeeString& str);
        static FrameImageComponent stringToFrameImageComponent(const CLeeString& str);

        static CLeeString vertFormatToString(VerticalFormatting format);
        static CLeeString horzFormatToString(HorizontalFormatting format);
        static CLeeString vertAlignmentToString(VerticalAlignment alignment);
        static CLeeString horzAlignmentToString(HorizontalAlignment alignment);
        static CLeeString dimensionTypeToString(DimensionType dim);
        static CLeeString vertTextFormatToString(VerticalTextFormatting format);
        static CLeeString horzTextFormatToString(HorizontalTextFormatting format);
        static CLeeString fontMetricTypeToString(FontMetricType metric);
        static CLeeString dimensionOperatorToString(DimensionOperator op);
        static CLeeString frameImageComponentToString(FrameImageComponent imageComp);
    };


#endif  // end of guard _XMLEnumHelper_h_
