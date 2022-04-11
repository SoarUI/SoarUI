
#ifndef _Imageset_h_
#define _Imageset_h_

#include "../SoarHeader/leeString.h"
#include "../SoarHeader/RectF.h"
#include "Image.h"
#include <map>


#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4251)
#endif


class  Imageset
{
	typedef	std::map<CLeeString, Image, CLeeString::FastLessCompare>	ImageRegistry;

public:
	Imageset(void);
    Imageset(const CLeeString& name);
	Imageset(const Imageset& rhs);
	Imageset& operator=(const Imageset& other);
	~Imageset(void);
	/*************************************************************************
		Public interface
	*************************************************************************/
	const CLeeString&	getName(void) const						{return d_name;}
	unsigned int    getImageCount(void) const               {return (size_t)d_images.size();}
	bool isImageDefined(const CLeeString& name) const	{ if (d_images.size()<=0){return false;}return d_images.find(name) != d_images.end();}

	Image*	getImage(const CLeeString& name) ;
	const Image*getImage(const CLeeString& name) const;
	void	undefineImage(const CLeeString& name);
	void	undefineAllImages(void);
	SizeF	getImageSize(const CLeeString& name) const;
	float	getImageWidth(const CLeeString& name) const;
	float	getImageHeight(const CLeeString& name) const;
	PointF2D getImageOffset(const CLeeString& name) const;
	float	getImageOffsetX(const CLeeString& name) const	;
	float	getImageOffsetY(const CLeeString& name) const	;
	void	defineImage(const CLeeString& name, const PointF2D& position, const SizeF& size, const PointF2D& render_offset)
	{
		defineImage(name, RectF(position.d_x, position.d_y,
			position.d_x + size.d_width, position.d_y + size.d_height), render_offset);
	}
	void	defineImage(const CLeeString& name, const RectF& image_rect, const PointF2D& render_offset);
	/*
	�ú���Ϊ�м����������PixelOffsetXY�����������Ļ��ƫ������ʱ�����պϳ�Ϊ��Ļ���꣬���Թ���DX GL��ʹ��
	��Ϊ��������ʱ����Ϊ�������
	*/
    void draw(const RectF& source_rect,
              const RectF& dest_rect, const RectF* clip_rect,const PointF2D& PixelOffsetXY) const;
protected:
	/*************************************************************************
		Implementation Functions
	*************************************************************************/
	void	unload(void);//ж������
	void	updateImageScalingFactors(void);

	/*************************************************************************
		Implementation Data
	*************************************************************************/
	CLeeString			d_name;			  //ͼ������.
	ImageRegistry	d_images;		     //ͼ�񼯺�
};

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif

#endif	// end of guard _Imageset_h_
