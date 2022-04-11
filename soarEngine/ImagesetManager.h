
#ifndef _ImagesetManager_h_
#define _ImagesetManager_h_
#include "../SoarHeader/Singleton_t.h"
#include "Imageset.h"
//#include "XMLHandler/Imageset_xmlHandler.h"
#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4275)
#   pragma warning(disable : 4251)
#endif

class  ImagesetManager :
	public LEE::Singleton_t<ImagesetManager>
{
public:
	typedef	std::map<CLeeString, Imageset, CLeeString::FastLessCompare>	ImagesetRegistry;
    ImagesetManager();
    ~ImagesetManager();
	bool parseFile(const CLeeString& filename);
	void eraseImageset(const CLeeString& imageset);
	void addImageset( const Imageset& imageset);
    Imageset* createImageset(const CLeeString& name);
     Imageset* get(const CLeeString& imageset_name);
	 const Imageset* get(const CLeeString& imageset_name)const;
	bool isImagesetPresent(const CLeeString& imageSetname);
	 void  DestroyAll(void);
protected:
	ImagesetRegistry	d_imagesets;
};


#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif  // end of guard _ImageSetManager_h_
