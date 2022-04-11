
#ifndef _ImageDim_h_
#define _ImageDim_h_
#include "../SoarHeader/Dimensions.h"
    class  ImageDim : public BaseDim
    {
    public:
        ImageDim(const CLeeString& imageset, const CLeeString& image, DimensionType dim);
        void setSourceImage(const CLeeString& imageset, const CLeeString& image);
        void setSourceDimension(DimensionType dim);

    protected:
        // Implementation of the base class interface
        float getValue_impl(const RectF& ControlRect) const;
        float getValue_impl(const RectF& ControlRect, const RectF& container) const;
        BaseDim* clone_impl() const;

    private:
        CLeeString d_imageset;      //!< name of the Imageset containing the image.
        CLeeString d_image;         //!< name of the Image.
        DimensionType d_what;   //!< the dimension of the image that we are to represent.
    };
   
    class  PropertyDim : public BaseDim
    {
    public:
        PropertyDim(const CLeeString& name, const CLeeString& property, DimensionType type);

    protected:
        // Implementation of the base class interface
        float getValue_impl(const RectF& ControlRect) const;
        float getValue_impl(const RectF& ControlRect, const RectF& container) const;
        BaseDim* clone_impl() const;

    private:
        CLeeString d_property;      //目标属性名
        CLeeString d_childSuffix;   //子窗体.
		CLeeString  d_WidgetLook;   //由于我们的窗体与UI窗体不同，属性放在Look中
        DimensionType d_type;   //!< CLeeString to hold the type of dimension
    };
#endif  // end of guard _Dimensions_h_
