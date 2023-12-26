#include "TextureManager.h"
#include "../Handler/Texture_xmlHandler.h"
#include "../../SoarHeader/leelog.h"
#include "../Theme/WidgetThemeManager.h"
#include "../XMLHandler/XMLAnalyser.h"
#include "../Mouduls/parsers/ExpatParser.h"
#include "../ResourcesManager.h"
#include "../threads/TextureThread.h"
//
TextureManager::TextureManager(void)
{
}

TextureManager::~TextureManager(void)
{
	clearAllTextures();
}

//--------------------------------------------------------------------------//

bool TextureManager::isTexturePresent(const CLeeString& tex)
{
	CThreadSync Sync;
	return d_widgetTexturePackages.find(tex) != d_widgetTexturePackages.end();
}
bool TextureManager::isTextureAvailable(const CLeeString& tex)
{
	CThreadSync Sync;
	return d_widgetTexturePackages.find(tex) != d_widgetTexturePackages.end();
}
CLeeString TextureManager::getTexture(const CLeeString& tex) 
{
	CThreadSync Sync;
	WidgetTextureList::iterator wlf = d_widgetTexturePackages.find(tex);
	if (wlf != d_widgetTexturePackages.end())
	{
		return(*wlf).second.getTexture();
	}
	return " ";
}
//RGBA
DWORD TextureManager::getTransparentARGB(const CLeeString& tex)
{
	CThreadSync Sync;
	WidgetTextureList::iterator wlf = d_widgetTexturePackages.find(tex);
	if (wlf != d_widgetTexturePackages.end())
	{
		return(*wlf).second.getTransparentARGB();
	}
	return 0x000000FF;
}
DWORD TextureManager::getFontColorARGB(const CLeeString& tex)
{
	CThreadSync Sync;
	WidgetTextureList::iterator wlf = d_widgetTexturePackages.find(tex);
	if (wlf != d_widgetTexturePackages.end())
	{
		return(*wlf).second.getFontColorARGB();
	}
	return 0x000000FF;
}
bool TextureManager::getUseMask(const CLeeString& tex)
{
	CThreadSync Sync;
	WidgetTextureList::iterator wlf = d_widgetTexturePackages.find(tex);
	if (wlf != d_widgetTexturePackages.end())
	{
		return(*wlf).second.getUseMask();
	}
	return false;
}
//获取子组件
void TextureManager::eraseTexture(const CLeeString& tex)
{
	CThreadSync Sync;
	//检测是否有主题链接它，是的话，返回
	WidgetTextureList::iterator wlf = d_widgetTexturePackages.find(tex);
	if (wlf != d_widgetTexturePackages.end())
	{
		//减少资源计数
		wlf->second.release();
		if (wlf->second.getRef()>0)
		{
			return ;
		}
		//检测该资源的引用列表，如果还有引用，不能删除
		d_widgetTexturePackages.erase(wlf);
	}
	
}
void TextureManager::clearAllTextures(void)
{
	CThreadSync Sync;
	d_widgetTexturePackages.clear();
}
void TextureManager::addTexture( const TexturePackage& lookpack)
{
	CThreadSync Sync;
	if (isTexturePresent(lookpack.getName()))
	{
		return ;
	}
	CLeeString name =lookpack.getName();
	//d_widgetTexturePackages[name] = lookpack;
	d_widgetTexturePackages.insert(std::make_pair(name,lookpack));
	Dbgc_DumpX(_T("leeduires"),
		_T("===== texture completed ====="),"");
}
bool TextureManager::parseFile(const CLeeString& filename )
{
	if (filename.empty())
	{
		throw(-1);
		return false;
	}
	//Texture_xmlHandler hadler;
	try
	{
		//IXMLAnalyser * Analyser =new ExpatParser();
		//if( Analyser->parseXMLFile(hadler,filename) )
		//{
		//	delete Analyser;
		TextureThread ythread;
		ythread.startThread(filename);
		
		//}
		return false ;
	}
	catch (...)
	{
		throw(-1);
	}
   return false ;
}