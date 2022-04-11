
#ifndef _Dimensions_h_
#define _Dimensions_h_
#include<assert.h>
#include "Enums.h"
#include "leeString.h"
#include "UDim.h"
#include "RectF.h"
//#include "../XMLSerializer.h"
    class  BaseDim
    {
    public:
        BaseDim();
        virtual ~BaseDim();
        float getValue(const RectF& ControlRect) const;
        float getValue(const RectF& ControlRect, const RectF& container) const;
        BaseDim* clone() const;
        DimensionOperator getDimensionOperator() const;
        void setDimensionOperator(DimensionOperator op);//���ò�����+-*/ 
        const BaseDim* getOperand() const;
        void setOperand(const BaseDim& operand);//���ò�������

    protected:
		//�����ɸô��ھ���
        virtual float getValue_impl(const RectF& ControlRect) const = 0;
		//�����ɸô�����container������������������ǵĽ�����һ�����������ʹ�øú�����ȡ
        virtual float getValue_impl(const RectF& ControlRect, const RectF& container) const = 0;
        virtual BaseDim* clone_impl() const = 0;
    private:
        DimensionOperator   d_operator;//+-*/ �޲���
        BaseDim*            d_operand;//������
    };

    class  AbsoluteDim : public BaseDim
    {
    public:
        AbsoluteDim(float val);
        void setValue(float val);
    protected:
        // Implementation of the base class interface
        float getValue_impl(const RectF& ControlRect) const;
        float getValue_impl(const RectF& ControlRect, const RectF& container) const;
        BaseDim* clone_impl() const;
    private:
        float d_val;    //!< holds pixel value for the AbsoluteDim.
    };
    
    class  UnifiedDim : public BaseDim
    {
    public:
        UnifiedDim(const UDim& value, DimensionType dim);
		 void setBaseDimension(const UDim& dim);
		 void setDimensionType(DimensionType type);
    protected:
        // Implementation of the base class interface
        float getValue_impl(const RectF& ControlRect) const;
        float getValue_impl(const RectF& ControlRect, const RectF& container) const;
        BaseDim* clone_impl() const;

    private:
        UDim d_value;           //ͳһ����
        DimensionType d_what;   //ʹ��ʵ�����Щ�ط�
    };
    class Dimension
    {
    public:
        Dimension();
        ~Dimension();
        Dimension(const BaseDim& dim, DimensionType type);
        Dimension(const Dimension& other);
        Dimension& operator=(const Dimension& other);
        const BaseDim& getBaseDimension() const;
        void setBaseDimension(const BaseDim& dim);
        DimensionType getDimensionType() const;
        void setDimensionType(DimensionType type);
       
    private:
        BaseDim*        d_value;    //!< Pointer to the value for this Dimension.
        DimensionType   d_type;     //��������
    };

//componetArea�����������ڿؼ��ڲ�������,��Ӧ��look�е�Area��־
    class ComponentArea
    {
    public:
		//ת��Ϊ��Ļ����
        RectF getRect(const RectF& ControlRect) const;//��һ�ຯ������������ǿؼ�����
		//��������ϻ������꡾��������ͬ����ϵ���������е��ؼ�������
		RectF getRect(const RectF& ControlRect, const RectF& container) const;
		//�ؼ��ڲ�����ϵ�������
        RectF getBoundingRect(const RectF& ControlRect) const;
        bool isAreaFetchedFromProperty() const;
        const CLeeString& getAreaPropertySource() const;
        void setAreaPropertySource(const CLeeString& property);

        Dimension d_left;   //!< Left edge of the area.
        Dimension d_top;    //!< Top edge of the area.
        Dimension d_right_or_width;     //!< Either the right edge or the width of the area.
        Dimension d_bottom_or_height;   //!< Either the bototm edge or the height of the area.

    private:
        CLeeString  d_areaProperty;         //�������ε��ַ���
		CLeeString  d_Look;           //�������ǵĴ�����UI���岻ͬ�����Է���Look��
    };
	 inline BaseDim::BaseDim() :
        d_operator(DOP_NOOP),
        d_operand(0)
    {
    }

    inline BaseDim::~BaseDim()
    {
        delete d_operand;
    }

    inline float BaseDim::getValue(const RectF& ParentRect) const
    {
        // get sub-class to return value for this dimension.
        float val = getValue_impl(ParentRect);

        // if we have an attached operand, perform math on value as needed
        if (d_operand)
        {
            switch (d_operator)
            {
            case DOP_ADD:
                val += d_operand->getValue(ParentRect);
                break;
            case DOP_SUBTRACT:
                val -= d_operand->getValue(ParentRect);
                break;
            case DOP_MULTIPLY:
                val *= d_operand->getValue(ParentRect);
                break;
            case DOP_DIVIDE:
                val /= d_operand->getValue(ParentRect);
                break;
            default:
                // No-op.
                break;
            }
        }

        return val;
    }

    inline float BaseDim::getValue(const RectF& ParentRect, const RectF& container) const
    {
        // get sub-class to return value for this dimension.
        float val = getValue_impl(ParentRect, container);

        // if we have an attached operand, perform math on value as needed
        if (d_operand)
        {
            switch (d_operator)
            {
            case DOP_ADD:
                val += d_operand->getValue(ParentRect, container);
                break;
            case DOP_SUBTRACT:
                val -= d_operand->getValue(ParentRect, container);
                break;
            case DOP_MULTIPLY:
                val *= d_operand->getValue(ParentRect, container);
                break;
            case DOP_DIVIDE:
                val /= d_operand->getValue(ParentRect, container);
                break;
            default:
                // No-op.
                break;
            }
        }

        return val;
    }

    inline BaseDim* BaseDim::clone() const
    {
        // get sub-class to return a cloned object
        BaseDim* o = clone_impl();

        // fill in operator for cloned object
        o->d_operator = d_operator;

        // now clone any attached operand dimension
        if (d_operand)
            o->d_operand = d_operand->clone();

        return o;
    }

    inline DimensionOperator BaseDim::getDimensionOperator() const
    {
        return d_operator;
    }

    inline void BaseDim::setDimensionOperator(DimensionOperator op)
    {
        d_operator = op;
    }

    inline const BaseDim* BaseDim::getOperand() const
    {
        return d_operand;
    }

    inline void BaseDim::setOperand(const BaseDim& operand)
    {
        // release old operand, if any.
        if(d_operand) delete d_operand;

        d_operand = operand.clone();
    }

    ////////////////////////////////////////////////////////////////////////////////

    inline AbsoluteDim::AbsoluteDim(float val) :
        d_val(val)
    {}

    inline void AbsoluteDim::setValue(float val)
    {
        d_val = val;
    }

    inline float AbsoluteDim::getValue_impl(const RectF& ParentRect) const
    {
        return d_val;
    }

    inline float AbsoluteDim::getValue_impl(const RectF&, const RectF&) const
    {
        return d_val;
    }

    inline BaseDim* AbsoluteDim::clone_impl() const
    {
        AbsoluteDim* ndim = new AbsoluteDim(d_val);
        return ndim;
    }

    ////////////////////////////////////////////////////////////////////////////////

    inline Dimension::Dimension()
    {
        d_value = 0;
        d_type = DT_INVALID;
    }

    inline Dimension::~Dimension()
    {
        if (d_value)
            delete d_value;
    }

    inline Dimension::Dimension(const BaseDim& dim, DimensionType type)
    {
        d_value = dim.clone();
        d_type = type;
    }

    inline Dimension::Dimension(const Dimension& other)
    {
        d_value = other.d_value ? other.d_value->clone() : 0;
        d_type = other.d_type;
    }

    inline Dimension& Dimension::operator=(const Dimension& other)
    {
        // release old value, if any.
        if (d_value)  delete d_value;

        d_value = other.d_value ? other.d_value->clone() : 0;
        d_type = other.d_type;

        return *this;
    }

    inline const BaseDim& Dimension::getBaseDimension() const
    {
        assert(d_value);
        return *d_value;
    }

    inline void Dimension::setBaseDimension(const BaseDim& dim)
    {
        // release old value, if any.
        if (d_value)  delete d_value;

        d_value = dim.clone();
    }

    inline DimensionType Dimension::getDimensionType() const
    {
        return d_type;
    }

    inline void Dimension::setDimensionType(DimensionType type)
    {
        d_type = type;
    }

    

    ////////////////////////////////////////////////////////////////////////////////

    inline UnifiedDim::UnifiedDim(const UDim& value, DimensionType dim) :
        d_value(value),
        d_what(dim)
    {
    }
	inline void UnifiedDim::setBaseDimension(const UDim& dim)
    {
        d_value = dim;
    }
	inline void UnifiedDim::setDimensionType(DimensionType type)
    {
        d_what = type;
    }
    inline float UnifiedDim::getValue_impl(const RectF& ControlRect) const
    {
		SizeF size(ControlRect.Width(),ControlRect.Height());
        switch (d_what)
        {
			case DT_X_POSITION:
            case DT_LEFT_EDGE:
				return d_value.asAbsolute(ControlRect.d_left);
            case DT_RIGHT_EDGE:
				return d_value.asAbsolute(ControlRect.d_right);
            
            case DT_X_OFFSET:
				return d_value.asRelative(size.d_width);
			case DT_Y_OFFSET:
				return d_value.asRelative(size.d_height);
            case DT_WIDTH://��ȡ���
                return d_value.asAbsolute(size.d_width);
                break;
            case DT_Y_POSITION:
            case DT_TOP_EDGE:
				return d_value.asAbsolute(ControlRect.d_top);
            case DT_BOTTOM_EDGE:
				return d_value.asAbsolute(ControlRect.d_bottom);
            case DT_HEIGHT://��ȡ�߶���Ϊ��� height* scale +offset
                return d_value.asAbsolute(size.d_height);
                break;

            default:
                throw(_T("UnifiedDim::getValue - unknown or unsupported DimensionType encountered."));
                break;
        }
    }
	/*��Ҫ���¶��壬��Ҫ��������û���������ߵ���ͼ 2016.8.28 15��17 ����ɽ�Ӵ�������޹�˾*/
   inline  float UnifiedDim::getValue_impl(const RectF& ParentRect, const RectF& container) const
    {
        switch (d_what)
        {
            case DT_LEFT_EDGE:
			case DT_X_POSITION:
				return  d_value.asAbsolute(ParentRect.d_left);
            case DT_RIGHT_EDGE:
            return d_value.asAbsolute(ParentRect.d_right);
            case DT_X_OFFSET:
			case DT_Y_OFFSET:
					return 0.0f;
            case DT_WIDTH:
                return d_value.asAbsolute(ParentRect.Width());
                break;
            case DT_Y_POSITION:
            case DT_TOP_EDGE:
				return d_value.asAbsolute(ParentRect.d_top);
            case DT_BOTTOM_EDGE:
                return d_value.asAbsolute(ParentRect.d_bottom);
            
            case DT_HEIGHT:
                return d_value.asAbsolute(ParentRect.Height());
                break;

            default:
                throw(_T("UnifiedDim::getValue - unknown or unsupported DimensionType encountered."));
                break;
        }
    }

    inline BaseDim* UnifiedDim::clone_impl() const
    {
        UnifiedDim* ndim = new UnifiedDim(d_value, d_what);
        return ndim;
    }
    ////////////////////////////////////////////////////////////////////////////////
 //��ȡ������Ļ���꣺�����Windows���ڵĿͻ�����0��0������ϵ��������+��Ը�����ƫ��=��Ļ����
   inline  RectF ComponentArea::getRect(const RectF& ParentRect) const
    {
		//��Ļ�������
        RectF pixelRect;
        // sanity check, we mus be able to form a RectF from what we represent.
        assert(d_left.getDimensionType() == DT_LEFT_EDGE || d_left.getDimensionType() == DT_X_POSITION);
        assert(d_top.getDimensionType() == DT_TOP_EDGE || d_top.getDimensionType() == DT_Y_POSITION);
        assert(d_right_or_width.getDimensionType() == DT_RIGHT_EDGE || d_right_or_width.getDimensionType() == DT_WIDTH);
        assert(d_bottom_or_height.getDimensionType() == DT_BOTTOM_EDGE || d_bottom_or_height.getDimensionType() == DT_HEIGHT);

        pixelRect.d_left = d_left.getBaseDimension().getValue(ParentRect);
        pixelRect.d_top = d_top.getBaseDimension().getValue(ParentRect);

        if (d_right_or_width.getDimensionType() == DT_WIDTH)
                pixelRect.SetWidth(d_right_or_width.getBaseDimension().getValue(ParentRect));
        else
                pixelRect.d_right = d_right_or_width.getBaseDimension().getValue(ParentRect);

        if (d_bottom_or_height.getDimensionType() == DT_HEIGHT)
                pixelRect.SetHeight(d_bottom_or_height.getBaseDimension().getValue(ParentRect));
        else
                pixelRect.d_bottom = d_bottom_or_height.getBaseDimension().getValue(ParentRect);
		pixelRect.offsetRect(ParentRect.d_left,ParentRect.d_top);
		return pixelRect;
    }
	//��ȡ������Ļ���꣺�����Windows���ڵĿͻ�����0��0������ϵ��������+��Ը�����ƫ��=��Ļ����
	inline RectF ComponentArea::getRect(const RectF& ParentRect, const RectF& cliper) const
	{
		RectF pixelRect;
		assert(d_left.getDimensionType() == DT_LEFT_EDGE || d_left.getDimensionType() == DT_X_POSITION);
		assert(d_top.getDimensionType() == DT_TOP_EDGE || d_top.getDimensionType() == DT_Y_POSITION);
		assert(d_right_or_width.getDimensionType() == DT_RIGHT_EDGE || d_right_or_width.getDimensionType() == DT_WIDTH);
		assert(d_bottom_or_height.getDimensionType() == DT_BOTTOM_EDGE || d_bottom_or_height.getDimensionType() == DT_HEIGHT);

		pixelRect.d_left = d_left.getBaseDimension().getValue(ParentRect) ;
		pixelRect.d_top = d_top.getBaseDimension().getValue(ParentRect) ;

		if (d_right_or_width.getDimensionType() == DT_WIDTH)
			pixelRect.SetWidth(d_right_or_width.getBaseDimension().getValue(ParentRect));
		else
			pixelRect.d_right = d_right_or_width.getBaseDimension().getValue(ParentRect) ;

		if (d_bottom_or_height.getDimensionType() == DT_HEIGHT)
			pixelRect.SetHeight(d_bottom_or_height.getBaseDimension().getValue(ParentRect));
		else
			pixelRect.d_bottom = d_bottom_or_height.getBaseDimension().getValue(ParentRect);
		
		//���ձ����ڿؼ��������ڱ���������,ִ��base+offset =�������
		pixelRect.offsetRect(ParentRect.d_left,ParentRect.d_top);
		pixelRect&=cliper;
		return pixelRect;
	}
	//�ؼ��ڲ�����ϵ�������
	inline RectF ComponentArea::getBoundingRect(const RectF& ControlRect) const
	{
		 RectF BoundRect;
		// sanity check, we mus be able to form a RectF from what we represent.
		assert(d_left.getDimensionType() == DT_LEFT_EDGE || d_left.getDimensionType() == DT_X_POSITION);
		assert(d_top.getDimensionType() == DT_TOP_EDGE || d_top.getDimensionType() == DT_Y_POSITION);
		assert(d_right_or_width.getDimensionType() == DT_RIGHT_EDGE || d_right_or_width.getDimensionType() == DT_WIDTH);
		assert(d_bottom_or_height.getDimensionType() == DT_BOTTOM_EDGE || d_bottom_or_height.getDimensionType() == DT_HEIGHT);

		BoundRect.d_left = d_left.getBaseDimension().getValue(ControlRect);
		BoundRect.d_top = d_top.getBaseDimension().getValue(ControlRect);

		if (d_right_or_width.getDimensionType() == DT_WIDTH)
			BoundRect.SetWidth(d_right_or_width.getBaseDimension().getValue(ControlRect));
		else
			BoundRect.d_right = d_right_or_width.getBaseDimension().getValue(ControlRect);

		if (d_bottom_or_height.getDimensionType() == DT_HEIGHT)
			BoundRect.SetHeight(d_bottom_or_height.getBaseDimension().getValue(ControlRect));
		else
			BoundRect.d_bottom = d_bottom_or_height.getBaseDimension().getValue(ControlRect);
		BoundRect.offsetRect(-ControlRect.d_left,-ControlRect.d_top);
		return BoundRect;
	}
    inline bool ComponentArea::isAreaFetchedFromProperty() const
    {
        return !d_areaProperty.empty();
    }

    inline const CLeeString& ComponentArea::getAreaPropertySource() const
    {
        return d_areaProperty;
    }

   inline void ComponentArea::setAreaPropertySource(const CLeeString& property)
    {
        d_areaProperty = property;
    }
#endif  // end of guard _Dimensions_h_
