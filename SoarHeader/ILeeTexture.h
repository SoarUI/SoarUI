
#ifndef _ILeeTexture_h_
#define _ILeeTexture_h_
#include "leeString.h"
#include "SizeF.h"
/*
最终输出的是32位的图像 PF_RGB：alpha 为0 ,但是外部给它提供掩码
PF_RGBA，本身具有Alpha通道，直接使用Alphablend函数即可
*/
class  ILeeTexture
{
public:
    enum PixelFormat
    {
        //! Each pixel is 3 bytes. RGB in that order.
        PF_RGB,
        //! Each pixel is 4 bytes. RGBA in that order.
        PF_RGBA
    };
	//图像大小
    virtual const SizeF getImageSize() const = 0;
	//设置数据源与格式
    virtual void loadFromMemory(const void* buffer,
                                const SizeF& buffer_size,PixelFormat fmt,bool btop_bottom) = 0;
	//输出数据源
    virtual void saveToMemory(void* buffer) = 0;
	//获取文件后缀
    virtual CLeeString getExtendFilter(void) =0;//BMP PNG GIF JPG
	//获取透明色，只对没有Alpha通道的BMP 有效
	virtual DWORD getMaskColor(void)= 0;//不带Alpha通道的颜色掩码
	//是否使用Mask
	virtual bool  isMaskUsed(void) =0;
	//获取文件名
	virtual CLeeString GetFullSourceName(void) =0;
	//获取源数据
	virtual void* getDataSource(void) =0;
	virtual void* getPixelAddress( int x, int y ) =0;
	//设置数据源大小
	virtual size_t getDataSourceSize(void) =0;
	virtual PixelFormat GetPixelFormat(void) =0;
	virtual void setPixelFormat(PixelFormat datafmt) =0;
	virtual void setfilePath(const CLeeString& file) =0;
	virtual void setImageSize(SizeF size) =0;
	virtual void setMaskColor(DWORD transparent) =0;
	virtual void setExtendFilter(const CLeeString & ext) =0;
	virtual int getChannels(void) =0;
	virtual const BITMAPINFO& getBitmapInfo(void) const=0;
    virtual ~ILeeTexture() {}
};

#endif // end of guard _ILeeTexture_h_
