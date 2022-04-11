
#ifndef _FeelEnums_h_
#define _FeelEnums_h_
#include "leeString.h"
    enum DimensionType
    {
        DT_LEFT_EDGE,       //实体的左边沿 (same as DT_X_POSITION).
        DT_X_POSITION,      //实体的X位置 (same as DT_LEFT_EDGE).
        DT_TOP_EDGE,        //实体的顶端(same as DT_Y_POSITION).
        DT_Y_POSITION,      //实体Y位置(same as DT_TOP_EDGE).
        DT_RIGHT_EDGE,      //实体右边沿.
        DT_BOTTOM_EDGE,     //实体底边沿
        DT_WIDTH,           //实体宽度
        DT_HEIGHT,          //实体高度 
		DT_X_OFFSET,        //实体的X偏移 (usually only applies to an Image entity).
        DT_Y_OFFSET,        //实体y偏移 (usually only applies to an Image entity).
        DT_INVALID          //非法类型
    };
    enum VerticalFormatting //垂直格式
    {
        VF_TOP_ALIGNED,         //!< Top of Image should be aligned with the top of the destination area.
        VF_CENTRE_ALIGNED,      //!< Image should be vertically centred within the destination area.
        VF_BOTTOM_ALIGNED,      //!< Bottom of Image should be aligned with the bottom of the destination area.
        VF_STRETCHED,           //!< Image should be stretched vertically to fill the destination area.
        VF_TILED                //!< Image should be tiled vertically to fill the destination area (bottom-most tile may be clipped).
    };
    enum HorizontalFormatting //水平格式
    {
        HF_LEFT_ALIGNED,        //!< Left of Image should be aligned with the left of the destination area.
        HF_CENTRE_ALIGNED,      //!< Image should be horizontally centred within the destination area.
        HF_RIGHT_ALIGNED,       //!< Right of Image should be aligned with the right of the destination area.
        HF_STRETCHED,           //!< Image should be stretched horizontally to fill the destination area.
        HF_TILED                //!< Image should be tiled horizontally to fill the destination area (right-most tile may be clipped).
    };
    enum VerticalTextFormatting  //文字垂直格式
    {
        VTF_TOP_ALIGNED=1,         //!< Top of text should be aligned with the top of the destination area.
        VTF_CENTRE_ALIGNED,      //!< text should be vertically centred within the destination area.
        VTF_BOTTOM_ALIGNED       //!< Bottom of text should be aligned with the bottom of the destination area.
    };
    enum HorizontalTextFormatting   //文字水平格式
    {
        HTF_LEFT_ALIGNED=4,        //!< Left of text should be aligned with the left of the destination area (single line of text only).
        HTF_RIGHT_ALIGNED,       //!< Right of text should be aligned with the right of the destination area  (single line of text only).
        HTF_CENTRE_ALIGNED,      //!< text should be horizontally centred within the destination area  (single line of text only).
        HTF_JUSTIFIED,           //!< text should be spaced so that it takes the full width of the destination area (single line of text only).
        HTF_WORDWRAP_LEFT_ALIGNED,    //!< Left of text should be aligned with the left of the destination area (word wrapped to multiple lines as needed).
        HTF_WORDWRAP_RIGHT_ALIGNED,   //!< Right of text should be aligned with the right of the destination area  (word wrapped to multiple lines as needed).
        HTF_WORDWRAP_CENTRE_ALIGNED,  //!< text should be horizontally centred within the destination area  (word wrapped to multiple lines as needed).
        HTF_WORDWRAP_JUSTIFIED        //!< text should be spaced so that it takes the full width of the destination area (word wrapped to multiple lines as needed).
    };
    enum FontMetricType  //
    {
        FMT_LINE_SPACING,       //!< Vertical line spacing value for font.
        FMT_BASELINE,           //!< Vertical baseline value for font.
        FMT_HORZ_EXTENT         //!< Horizontal extent of a string.
    };
    enum DimensionOperator   //操作符
    {
        DOP_NOOP,       //!< Do nothing operator.
        DOP_ADD,        //!< Dims should be added.
        DOP_SUBTRACT,   //!< Dims should be subtracted.
        DOP_MULTIPLY,   //!< Dims should be multiplied.
        DOP_DIVIDE      //!< Dims should be divided.
    };
    enum FrameImageComponent //图片用途
    {
        FIC_BACKGROUND,             //!< References image used for the background.
        FIC_TOP_LEFT_CORNER,        //!< References image used for the top-left corner.
        FIC_TOP_RIGHT_CORNER,       //!< References image used for the top-right corner.
        FIC_BOTTOM_LEFT_CORNER,     //!< References image used for the bottom-left corner.
        FIC_BOTTOM_RIGHT_CORNER,    //!< References image used for the bottom-right corner.
        FIC_LEFT_EDGE,              //!< References image used for the left edge.
        FIC_RIGHT_EDGE,             //!< References image used for the right edge.
        FIC_TOP_EDGE,               //!< References image used for the top edge.
        FIC_BOTTOM_EDGE,            //!< References image used for the bottom edge.
        FIC_FRAME_IMAGE_COUNT       //!< Max number of images for a frame.
    };
	enum VerticalAlignment  //窗口垂直位置
{
    /**
     * Window's position specifies an offset of it's top edge from the top edge
     * of it's parent.
     */
    VA_TOP,
    /**
     * Window's position specifies an offset of it's vertical centre from the
     * vertical centre of it's parent.
     */
    VA_CENTRE,
    /**
     * Window's position specifies an offset of it's bottom edge from the
     * bottom edge of it's parent.
     */
    VA_BOTTOM
};
enum HorizontalAlignment   //窗口水平格式
{
    /**
     * Window's position specifies an offset of it's left edge from the left
     * edge of it's parent.
     */
    HA_LEFT,
    /**
     * Window's position specifies an offset of it's horizontal centre from the
     * horizontal centre of it's parent.
     */
    HA_CENTRE,
    /**
     * Window's position specifies an offset of it's right edge from the right
     * edge of it's parent.
     */
    HA_RIGHT
};
enum TextFormatAligned
{
ALIGNED_TOP =0x00000000,
ALIGNED_LEFT=0x00000000,
ALIGNED_CENTER=0x00000001,
ALIGNED_RIGHT=0x00000002,
ALIGNED_VCENTER=0x00000004,
ALIGNED_BOTTOM=0x00000008,
ALIGNED_WORDBREAK=0x00000010,
ALIGNED_SINGLELINE=0x00000020,
ALIGNED_EXPANALIGNEDABS=0x00000040,
ALIGNED_TABSTOP=0x00000080,
ALIGNED_NOCLIP=0x00000100,
ALIGNED_EXTERNALLEADING=0x00000200,
ALIGNED_CALCRECT=0x00000400,
ALIGNED_NOPREFIX=0x00000800,
ALIGNED_INTERNAL=0x00001000
};
#endif  // end of guard _FeelEnums_h_
