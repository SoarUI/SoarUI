#pragma once
/********************************************************************
    Descibe:      对texture文件包装，控制JPG等文件的绘制信息
    created:      2012/8/15 11:29
    FullName:     TexturePackage.h
    Path:         
    ClassName:    TexturePackage
    FileType:     h
    Author:       nicklisir@163.com
    NeededLib:    
    requires:      
*********************************************************************/
#include "../Texture/TextureImpl.h"
#include "../objectReferentBase.h"
#include "../colour.h"
class TexturePackage:public objectReferentBase
{
public:
	TexturePackage(void);
	TexturePackage(const TexturePackage& rhs);
public:
	virtual ~TexturePackage(void);
	const CLeeString& getName(void) const;
	void  setName(const CLeeString& name);
	void  setimgfile(const CLeeString& filename,const CLeeString& type,DWORD dwMaskColor);
	CLeeString getimgfile(void) const;
	CLeeString getpathfile(void) const;
	SizeF	getNativeResolution(void) const	{ return SizeF(d_nativeHorzRes, d_nativeVertRes); }
	void	setUseMask(bool setting);
	bool    getUseMask(void) const;
	void	setNativeResolution(const SizeF& size);
	void	unload(void);//卸载数据
	void	setTexture(ILeeTexture* texture);
	const CLeeString& getTexture(void) const;
	DWORD    getTransparentRGBA(void);
private:
	bool	d_useMask;			//是否使用掩码
	float	d_horzScaling;		//水平缩放因子.
	float	d_vertScaling;		//垂直缩放因子
	float	d_nativeHorzRes;	//原始水平大小
	float	d_nativeVertRes;	//原始垂直大小
	//TextureImpl *d_texture;		//图像集中真正数据
	CLeeString  d_textureFile;  //真正数据文件名
	CLeeString  d_pathfile;  //包含路径的名字
	CLeeString d_name;          //名称
	//透明色：
	Argb d_transparent;
	//类型：
	CLeeString d_type;//0-BMP  1-JPG 2-TGA 3-PNG
};
