#ifndef _TextureManager_h_
#define _TextureManager_h_
#include "../../SoarHeader/Singleton_t.h"
#include "../../SoarHeader/SingletonSync.h"
#include "../../SoarHeader/leeString.h"
#include <map>
#include "TexturePackage.h"
class TextureManager: public LEE::Singleton_t<TextureManager>,public CSingletonSync<TextureManager>
{
public:
	TextureManager(void);
public:
	virtual ~TextureManager(void);
	bool parseFile(const CLeeString& filename);
	CLeeString getTexture(const CLeeString& tex) ;
	//RGBA
	DWORD    getTransparentARGB(const CLeeString& tex) ;
	DWORD    getFontColorARGB(const CLeeString& tex);
	bool    getUseMask(const CLeeString& tex);
	//获取子组件
	void eraseTexture(const CLeeString& tex);
	void clearAllTextures(void);
	void addTexture( const TexturePackage& lookpack);
	bool isTexturePresent(const CLeeString& tex);
	bool isTextureAvailable(const CLeeString& tex) ;
	
public:
	typedef std::map<CLeeString,  TexturePackage, CLeeString::FastLessCompare> WidgetTextureList;
private:
	WidgetTextureList  d_widgetTexturePackages;
};
#endif