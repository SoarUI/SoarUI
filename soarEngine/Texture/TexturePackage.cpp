#include "TexturePackage.h"
#include "../Mouduls/PluginManager.h"
#include "../Mouduls/PluginAnyManager.h"
#include "../datacontainer.h"
#include "../Texture/TextureImpl.h"
#include "../ResourcesManager.h"
#include "../../threadpool/leePathPlus.h"
#include "../../SoarHeader/IResourceProvider.h"
#include "../../SoarHeader/leelog.h"
TexturePackage::TexturePackage(void)
{
}
TexturePackage::TexturePackage(const TexturePackage& rhs):d_useMask(rhs.d_useMask),
d_name(rhs.d_name),d_nativeHorzRes(rhs.d_nativeHorzRes),d_nativeVertRes(rhs.d_nativeVertRes),
d_pathfile(rhs.d_pathfile),d_textureFile(rhs.d_textureFile),d_horzScaling(rhs.d_horzScaling),
d_vertScaling(rhs.d_vertScaling),d_transparent(rhs.d_transparent),d_type(rhs.d_type), d_fontArgb(rhs.d_fontArgb)
{	
	
}
TexturePackage::~TexturePackage(void)
{
	
}
const CLeeString& TexturePackage::getName(void) const
{
	return d_name;
}
void  TexturePackage::setName(const CLeeString& name)
{
	d_name= name;
}
void  TexturePackage::setimgfile(const CLeeString& filename,const CLeeString& type1,DWORD dwMaskColor,DWORD dwFontColor)
{
	//从主入口获取paser
	Dbgc_DumpX(_T("leeduires"),
		_T("===== putting  resources texture file in package: file:%s ====="),filename.c_str());
	d_textureFile =filename;
	d_transparent = dwMaskColor;//没有转换
	d_fontArgb = dwFontColor;
	d_type = type1;
	CLeeString final_filename =_T("./");
	ResourcesManager *groupMgr=ResourcesManager::getSingletonPtr();
	if (groupMgr)
	{
		final_filename =groupMgr->getDirectory(RT_TEXTURE);
	} 
	if (final_filename.substr(final_filename.length()-1,1)==_T("/"))
	{
		;
	}
	else if (final_filename.substr(final_filename.length()-1,1)==_T("\\"))
	{
		;
	}
	else
	{
		final_filename +=_T("/");
	}
	final_filename+=filename;
	//路径解决器
	CLeePathPlus PathFixedor;
	//
	d_pathfile = PathFixedor.getRevisePath(final_filename);
	
	Dbgc_DumpX(_T("leeduires"),
		_T("===== loading resources texture end: file:%s ====="),filename.c_str());
	
}
void TexturePackage::setUseMask(bool setting)
{
	if (setting != d_useMask)
	{
		d_useMask = setting;
	}

}
void TexturePackage::setNativeResolution(const SizeF& size)
{
	d_nativeHorzRes = size.d_width;
	d_nativeVertRes = size.d_height;
}
const CLeeString& TexturePackage::getTexture(void) const
{
	return d_pathfile;
}
bool TexturePackage::getUseMask() const
{
	return d_useMask;
}
CLeeString TexturePackage::getimgfile() const
{
	return d_textureFile;
}
CLeeString TexturePackage::getpathfile() const
{
	return d_pathfile;
}
DWORD TexturePackage::getTransparentARGB(void)
{
	return d_transparent;
}
DWORD TexturePackage::getFontColorARGB(void)
{
	return d_fontArgb;
}