
#ifndef _ILeeTexture_h_
#define _ILeeTexture_h_
#include "leeString.h"
#include "SizeF.h"
/*
�����������32λ��ͼ�� PF_RGB��alpha Ϊ0 ,�����ⲿ�����ṩ����
PF_RGBA���������Alphaͨ����ֱ��ʹ��Alphablend��������
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
	//ͼ���С
    virtual const SizeF getImageSize() const = 0;
	//��������Դ���ʽ
    virtual void loadFromMemory(const void* buffer,
                                const SizeF& buffer_size,PixelFormat fmt,bool btop_bottom) = 0;
	//�������Դ
    virtual void saveToMemory(void* buffer) = 0;
	//��ȡ�ļ���׺
    virtual CLeeString getExtendFilter(void) =0;//BMP PNG GIF JPG
	//��ȡ͸��ɫ��ֻ��û��Alphaͨ����BMP ��Ч
	virtual DWORD getMaskColor(void)= 0;//����Alphaͨ������ɫ����
	//�Ƿ�ʹ��Mask
	virtual bool  isMaskUsed(void) =0;
	//��ȡ�ļ���
	virtual CLeeString GetFullSourceName(void) =0;
	//��ȡԴ����
	virtual void* getDataSource(void) =0;
	virtual void* getPixelAddress( int x, int y ) =0;
	//��������Դ��С
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
