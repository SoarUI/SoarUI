
#include "../XMLHandler/Imageset_xmlHandler.h"
#include "../XMLHandler/XMLAnalyser.h"
//#include "../Imageset.h"
#include "../ImagesetManager.h"
#include "../../SoarHeader/leelog.h"
//----------------------------------------------------------------------------//
const CLeeString Imageset_xmlHandler::ImagesetElement( _T("Imageset") );
const CLeeString Imageset_xmlHandler::ImageElement( _T("Image") );
const CLeeString Imageset_xmlHandler::ImagesetImageFileAttribute( _T("Imagefile") );
const CLeeString Imageset_xmlHandler::ImagesetNameAttribute( _T("Name") );
const CLeeString Imageset_xmlHandler::ImagesetNativeHorzResAttribute( _T("NativeHorzRes") );
const CLeeString Imageset_xmlHandler::ImagesetNativeVertResAttribute( _T("NativeVertRes") );
const CLeeString Imageset_xmlHandler::ImagesetAutoScaledAttribute( _T("AutoScaled") );
const CLeeString Imageset_xmlHandler::ImageNameAttribute( _T("Name") );
const CLeeString Imageset_xmlHandler::ImageXPosAttribute( _T("XPos") );
const CLeeString Imageset_xmlHandler::ImageYPosAttribute( _T("YPos") );
const CLeeString Imageset_xmlHandler::ImageWidthAttribute( _T("Width") );
const CLeeString Imageset_xmlHandler::ImageHeightAttribute( _T("Height") );
const CLeeString Imageset_xmlHandler::ImageXOffsetAttribute( _T("XOffset") );
const CLeeString Imageset_xmlHandler::ImageYOffsetAttribute( _T("YOffset") );

//----------------------------------------------------------------------------//
Imageset_xmlHandler::Imageset_xmlHandler(void) :
    d_imageset(0),
    d_objectRead(false)
{
}

//----------------------------------------------------------------------------//
Imageset_xmlHandler::~Imageset_xmlHandler()
{
    if (!d_objectRead)
        delete d_imageset;
}

//----------------------------------------------------------------------------//
const CLeeString& Imageset_xmlHandler::getObjectName() const
{
    return d_imageset->getName();
}

//----------------------------------------------------------------------------//
Imageset* Imageset_xmlHandler::getObject() const
{
    d_objectRead = true;
    return d_imageset;
}

//----------------------------------------------------------------------------//
void Imageset_xmlHandler::elementStart(const CLeeString& element,
                                       const XMLAttributes& attributes)
{
    // handle an Image element
    if (element == ImageElement)
        elementImageStart(attributes);
    // handle root Imageset element
    else if (element == ImagesetElement)
        elementImagesetStart(attributes);
}

//----------------------------------------------------------------------------//
void Imageset_xmlHandler::elementEnd(const CLeeString& element)
{
    if (element == ImagesetElement)
        elementImagesetEnd();
}

//----------------------------------------------------------------------------//
void Imageset_xmlHandler::elementImagesetStart(const XMLAttributes& attributes)
{
    // get name of the imageset.
    const CLeeString name(attributes.getValueAsString(ImagesetNameAttribute));
    // get texture image filename
    const CLeeString filename(
        attributes.getValueAsString(ImagesetImageFileAttribute));

    // Create imageset object from image file
    d_imageset = new Imageset(name);
    d_objectRead =TRUE ;
	Dbgc_DumpX(_T("leeduires"),
		_T("===== loading resources imageset:<%s> parsing begins ====="),name.c_str());
}

//----------------------------------------------------------------------------//
void Imageset_xmlHandler::elementImageStart(const XMLAttributes& attributes)
{
    if (!d_imageset)
		return;
    const CLeeString name(attributes.getValueAsString(ImageNameAttribute));

    RectF    rect;
    rect.d_left =
        static_cast<float>(attributes.getValueAsInteger(ImageXPosAttribute));
    rect.d_top  =
        static_cast<float>(attributes.getValueAsInteger(ImageYPosAttribute));
    rect.SetWidth(
        static_cast<float>(attributes.getValueAsInteger(ImageWidthAttribute)));
    rect.SetHeight(
        static_cast<float>(attributes.getValueAsInteger(ImageHeightAttribute)));

    const PointF2D offset(
        static_cast<float>(attributes.getValueAsInteger(ImageXOffsetAttribute, 0)),
        static_cast<float>(attributes.getValueAsInteger(ImageYOffsetAttribute, 0)));

    d_imageset->defineImage(name, rect, offset);
}

//----------------------------------------------------------------------------//
void Imageset_xmlHandler::elementImagesetEnd()
{
	Dbgc_DumpX(_T("leeduires"),
		_T("==============End of Imageset:<%s> "),
		d_imageset->getName().c_str());
	ImagesetManager * imgagesetMgr = ImagesetManager::getSingletonPtr();
	if (imgagesetMgr)
	{
		imgagesetMgr->addImageset(*d_imageset);
	}
	else
	{
		imgagesetMgr = new ImagesetManager();
		imgagesetMgr->addImageset(*d_imageset);
	}
	delete d_imageset;
	d_imageset =0;
}

//----------------------------------------------------------------------------//