#include "Texture_xmlHandler.h"
#include "../Texture/TextureManager.h"
#include "../Propertys/PropertyHelper.h"
// xml tag names
const CLeeString Texture_xmlHandler::TextureElement = _T("Texture");
const CLeeString Texture_xmlHandler::TexElement = _T("Tex");

// xml attribute names
const CLeeString Texture_xmlHandler::FilenameAttribute = _T("filename");
const CLeeString Texture_xmlHandler::NameAttribute = _T("name");
const CLeeString Texture_xmlHandler::TextureAttribute = _T("texture");
const CLeeString Texture_xmlHandler::StringAttribute = _T("string");
const CLeeString Texture_xmlHandler::GroupAttribute = _T("group");
const CLeeString Texture_xmlHandler::NativeHorzResAttribute( _T("NativeHorzRes") );
const CLeeString Texture_xmlHandler::NativeVertResAttribute( _T("NativeVertRes") );
const CLeeString Texture_xmlHandler::AutoScaledAttribute( _T("AutoScaled") );
const CLeeString Texture_xmlHandler::TypeAttribute( _T("Type") );
const CLeeString Texture_xmlHandler::TransparentAttribute( _T("Transparent") );
const CLeeString Texture_xmlHandler::useMaskAttribute( _T("useMask" ) );
Texture_xmlHandler::Texture_xmlHandler(void):d_packet(NULL)
{
	;
}
Texture_xmlHandler::~Texture_xmlHandler(void)
{
	;
}
void Texture_xmlHandler::elementStart(const CLeeString& element,
                                     const XMLAttributes& attributes)
{
    if (element == TextureElement)
        handleTextureElement(attributes);
    else if (element == TexElement)
        handleTexElement(attributes);
}

//----------------------------------------------------------------------------//
void Texture_xmlHandler::elementEnd(const CLeeString& element)
{
    if (element == TextureElement)
		handleTextureElementEnd();
	else if (element == TexElement)
	{
		handleTexElementEnd();
	}
	
       
}
void Texture_xmlHandler::handleTextureElement(const XMLAttributes& attr)
{
	assert(d_packet ==NULL);
	d_packet =new TexturePackage;
	//name
	d_packet->setName(attr.getValueAsString(NameAttribute));
}
void Texture_xmlHandler::handleTexElement(const XMLAttributes& attr)
{
  assert(d_packet!= NULL);
  //filename
  //nativewidth
  //nativeheight
  // set native resolution for imageset
  const float native_hres = static_cast<float>(
	  attr.getValueAsInteger(NativeHorzResAttribute, 640));
  const float native_vres = static_cast<float>(
	  attr.getValueAsInteger(NativeVertResAttribute, 480));
  d_packet->setNativeResolution(SizeF(native_hres, native_vres));

  // 设置掩码
  d_packet->setUseMask(
	  attr.getValueAsBool(useMaskAttribute, false));
  //获取掩码颜色
  //
  d_packet->setimgfile(attr.getValueAsString(FilenameAttribute),
	  attr.getValueAsString(TypeAttribute),PropertyHelper::stringToColour(attr.getValueAsString(TransparentAttribute) ) );
}
void Texture_xmlHandler::handleTexElementEnd(void)
{
  ;
}
void Texture_xmlHandler::handleTextureElementEnd(void)
{
	TextureManager * TextureMgr =TextureManager::getSingletonPtr();
	if (TextureMgr)
	{
		TextureMgr->addTexture(*d_packet);
		delete d_packet;
		d_packet =NULL;
	}
}