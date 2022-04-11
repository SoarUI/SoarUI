#include "TextureImpl.h"
TextureImpl::TextureImpl(void)
{
	//d_tex = 0;
}
TextureImpl::TextureImpl(const CLeeString& filename):d_file(d_file),d_channels(3),d_rawData(0),d_sizebuffer(0)
{
}
TextureImpl::~TextureImpl(void)
{
	if (d_rawData)
	{
		delete[]d_rawData;
		d_rawData =NULL;
	}
}
TextureImpl::TextureImpl(const TextureImpl& other):d_sizebuffer(other.d_sizebuffer),
d_width(other.d_width),
d_height(other.d_height),
d_maskColor(other.d_maskColor),
d_file(other.d_file),
d_datafmt(other.d_datafmt),
d_channels(other.d_channels),
d_bitInfo(other.d_bitInfo)
{
	if (d_rawData)
	{
		delete[]d_rawData;
		d_rawData =NULL;
	}
	d_rawData =new BYTE[other.d_sizebuffer+1];
	CopyMemory(d_rawData,other.d_rawData,other.d_sizebuffer);
	d_rawData[other.d_sizebuffer]=0;
}
TextureImpl & TextureImpl::operator =(const TextureImpl& other)
{
	if (this == &other)
	{
		return *this;
	}
	d_sizebuffer =other.d_sizebuffer;
	d_width =other.d_width;
	d_height =other.d_height;
	d_maskColor =other.d_maskColor;
	d_file =other.d_file;
	d_datafmt =other.d_datafmt;
	d_channels =other.d_channels;
	if (d_rawData)
	{
		delete[]d_rawData;
		d_rawData =NULL;
	}
	d_rawData =new BYTE[other.d_sizebuffer+1];
	CopyMemory(d_rawData,other.d_rawData,other.d_sizebuffer);
	d_rawData[other.d_sizebuffer]=0;
	return *this;
}

const SizeF TextureImpl::getImageSize() const
{
	return SizeF(d_width,d_height);
}
 size_t TextureImpl::getDataSourceSize() 
{
	return d_sizebuffer;
}
 //输出数据源
 void TextureImpl::saveToMemory(void* buffer) 
 {
	 ;
 }
 //获取文件后缀
 CLeeString TextureImpl::getExtendFilter(void) //BMP PNG GIF JPG
 {
	return d_extfilter;
 }
 //获取透明色，只对没有Alpha通道的BMP 有效
 DWORD TextureImpl::getMaskColor(void)//不带Alpha通道的颜色掩码
 {
	 return d_maskColor;
 }
 //是否使用Mask
 bool TextureImpl::isMaskUsed(void) 
 {
	 return d_datafmt !=PF_RGBA;
 }
 //获取文件名
 CLeeString TextureImpl::GetFullSourceName(void) 
 {
	 return d_file;
 }
 void* TextureImpl::getDataSource(void) 
 {
	 return (void*) d_rawData;
 }
 void* TextureImpl::getPixelAddress( int x, int y )
 {
	 return (BYTE*)d_rawData+d_width*d_channels*y+x*d_channels;
 }
 TextureImpl::PixelFormat TextureImpl::GetPixelFormat(void) 
 {
	 return d_datafmt;
 }
 void TextureImpl::setPixelFormat(PixelFormat datafmt)
 {
	 d_datafmt =datafmt;
 }
 void TextureImpl::setfilePath(const CLeeString& file)
 {
	 d_file =file;
 }
 void TextureImpl::setImageSize(SizeF size) 
 {
	 d_height =size.d_height;
	 d_width =size.d_width ;
 }
 void TextureImpl::setMaskColor(DWORD transparent) 
 {
	 d_maskColor =transparent;
 }
 void TextureImpl::setExtendFilter(const CLeeString & ext) 
 {
	 d_extfilter =ext;
 }
 int TextureImpl::getChannels(void)
 {
	 return d_channels;
 }
 const BITMAPINFO& TextureImpl::getBitmapInfo(void) const
 {
	return d_bitInfo;
 }
 //////////////////////////////////////////////////////////////////////////
 void TextureImpl::loadFromMemory(const void* buffer,
	 const SizeF& buffer_size,PixelFormat fmt,bool btop_bottom)
 {
	 if (d_rawData)
	 {
		 delete[]d_rawData;
		 d_rawData =NULL;
	 }
	 d_datafmt =fmt;
	  d_channels =3;
	 if (fmt==PF_RGBA)
	 {
		 d_channels =4;
	 }
	
	 d_sizebuffer = buffer_size.d_width*buffer_size.d_height*d_channels+1;
	 d_width =buffer_size.d_width;
	 d_height =buffer_size.d_height;
	 d_rawData =new BYTE[d_sizebuffer];
	 CopyMemory(d_rawData,buffer,d_sizebuffer-1);
	 d_rawData[d_sizebuffer-1]=0;
	d_bitInfo.bmiHeader.biSize = sizeof(d_bitInfo.bmiHeader);
	d_bitInfo.bmiHeader.biWidth = d_width;
	d_bitInfo.bmiHeader.biHeight = d_height;
	if (btop_bottom)//自顶到下
	{
		d_bitInfo.bmiHeader.biHeight =-d_bitInfo.bmiHeader.biHeight;
	}
	d_bitInfo.bmiHeader.biPlanes = 1;
	d_bitInfo.bmiHeader.biBitCount = d_channels*8;
	d_bitInfo.bmiHeader.biCompression =BI_RGB;
	d_bitInfo.bmiHeader.biClrUsed =0;
	d_bitInfo.bmiHeader.biXPelsPerMeter=0;
	d_bitInfo.bmiHeader.biClrImportant =0;
	d_bitInfo.bmiHeader.biYPelsPerMeter=0;
	d_bitInfo.bmiHeader.biSizeImage =0;
 }