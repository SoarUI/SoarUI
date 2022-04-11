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
	//ͼ���С
	virtual const SizeF getImageSize() const ;
	//��������Դ���ʽ
	virtual void loadFromMemory(const void* buffer,
		const SizeF& buffer_size,PixelFormat fmt,bool btop_bottom);
	//�������Դ
	virtual void saveToMemory(void* buffer) ;
	//��ȡ�ļ���׺
	virtual CLeeString getExtendFilter(void) ;//BMP PNG GIF JPG
	//��ȡ͸��ɫ��ֻ��û��Alphaͨ����BMP ��Ч
	virtual DWORD getMaskColor(void);//����Alphaͨ������ɫ����
	//�Ƿ�ʹ��Mask
	virtual bool  isMaskUsed(void) ;
	//��ȡ�ļ���
	virtual CLeeString GetFullSourceName(void) ;
	//��ȡԴ������ʼ��
	virtual void* getDataSource(void) ;
	//��ȡĳ�����������ʼ��
	virtual void* getPixelAddress( int x, int y );
	//��������Դ��С
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
	BITMAPINFO d_bitInfo;//ͼ����Ϣ�����ڿ��ٹ���ͼ��
};
