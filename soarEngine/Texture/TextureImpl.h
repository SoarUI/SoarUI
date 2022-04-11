#pragma once
#include "../../SoarHeader/ILeeTexture.h"

class TextureImpl :
	public ILeeTexture
{
public:
	friend class DefaultRender;
	TextureImpl(void);
	TextureImpl(const CLeeString& filename);
	TextureImpl(const TextureImpl& other);
	TextureImpl & operator =(const TextureImpl& other);
public:
	virtual ~TextureImpl(void);
	//图像大小
	virtual const SizeF getImageSize() const ;
	//设置数据源与格式
	virtual void loadFromMemory(const void* buffer,
		const SizeF& buffer_size,PixelFormat fmt,bool btop_bottom);
	//输出数据源
	virtual void saveToMemory(void* buffer) ;
	//获取文件后缀
	virtual CLeeString getExtendFilter(void) ;//BMP PNG GIF JPG
	//获取透明色，只对没有Alpha通道的BMP 有效
	virtual DWORD getMaskColor(void);//不带Alpha通道的颜色掩码
	//是否使用Mask
	virtual bool  isMaskUsed(void) ;
	//获取文件名
	virtual CLeeString GetFullSourceName(void) ;
	//获取源数据起始点
	virtual void* getDataSource(void) ;
	//获取某坐标相对于起始点
	virtual void* getPixelAddress( int x, int y );
	//设置数据源大小
	virtual size_t getDataSourceSize(void) ;
	PixelFormat GetPixelFormat(void) ;
	int getChannels(void);
	//
	void setPixelFormat(PixelFormat datafmt);
	void setfilePath(const CLeeString& file) ;
	void setImageSize(SizeF size) ;
	void setMaskColor(DWORD transparent) ;
	void setExtendFilter(const CLeeString & ext) ;
	virtual const BITMAPINFO& getBitmapInfo(void) const;
private:
	BYTE * d_rawData;
	size_t d_sizebuffer;
	int    d_width;
	int    d_height;
	CLeeString d_file;
	CLeeString d_extfilter;
	DWORD d_maskColor;
	PixelFormat d_datafmt;
	int d_channels;
	BITMAPINFO d_bitInfo;//图像信息，用于快速构造图像
};
