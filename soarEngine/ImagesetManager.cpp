
#include "ImagesetManager.h"
#include "XMLHandler\XMLAnalyser.h"
#include "Mouduls\parsers\ExpatParser.h"
#include "XMLHandler\Imageset_xmlHandler.h"
#include "ResourcesManager.h"
#include "threads\ImagesetThread.h"
//----------------------------------------------------------------------------//
ImagesetManager::ImagesetManager()
{

}

//----------------------------------------------------------------------------//
ImagesetManager::~ImagesetManager()
{
    DestroyAll();
}

//----------------------------------------------------------------------------//
Imageset* ImagesetManager::createImageset(const CLeeString& name)
{
	if (isImagesetPresent(name))
	{
		return get(name);
	}
	Imageset object(name);
	d_imagesets[name]=object;
    return &(d_imagesets[name]);
}
//----------------------------------------------------------------------------//
 Imageset* ImagesetManager::get(const CLeeString& imageset_name)
{
	ImagesetRegistry::iterator	pos = d_imagesets.find(imageset_name);

	if (pos == d_imagesets.end())
	{
		return NULL;
	}

	return &(pos->second);
}
 const Imageset* ImagesetManager::get(const CLeeString& imageset_name)const
 {
	 ImagesetRegistry::const_iterator	pos = d_imagesets.find(imageset_name);

	 if (pos == d_imagesets.end())
	 {
		 return NULL;
	 }

	 return &(pos->second);
 }
bool ImagesetManager::isImagesetPresent(const CLeeString& imageSetname)
{
	ImagesetRegistry::const_iterator	pos = d_imagesets.find(imageSetname);

	if (pos == d_imagesets.end())
	{
		return false;
	}
	return true;
}
void ImagesetManager::DestroyAll()
{
	d_imagesets.clear();
}

void ImagesetManager::eraseImageset(const CLeeString& imageset)
{
	ImagesetRegistry::iterator	pos = d_imagesets.find(imageset);

	if (pos != d_imagesets.end())
	{
		d_imagesets.erase(pos);
	}
}
void ImagesetManager::addImageset( const Imageset& imageset)
{
	if (isImagesetPresent(imageset.getName()))
	{
		return ;
	}
	d_imagesets[imageset.getName()] =imageset;
}
bool ImagesetManager::parseFile(const CLeeString& filename )
{
	if (filename.empty())
	{
		throw(-1);
		return false ;
	}
	//Imageset_xmlHandler hadler;
	try
	{
		//IXMLAnalyser * Analyser =new ExpatParser();
		//if( Analyser->parseXMLFile(hadler,filename) )
		//{
		//	delete Analyser;
		//}
		CImageSetThread ythread;
		ythread.startThread(filename);
		return true ;
	}
	catch (...)
	{
		throw(-1);
	}
	return false ;
}