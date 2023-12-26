#pragma once
#include "../Texture/TexturePackage.h"
#include "../../XML/XMLAttributes.h"
#include "../../XML/IXMLHandler.h"
class TexturePackage;
class Texture_xmlHandler :
	public IXMLHandler
{
public:
	Texture_xmlHandler(void);
	// xml tag names
	static const CLeeString TextureElement;
	static const CLeeString TexElement;

	// xml attribute names
	static const CLeeString FilenameAttribute;
	static const CLeeString NameAttribute;
	static const CLeeString TextureAttribute;
	static const CLeeString StringAttribute;
	static const CLeeString GroupAttribute;
	static const CLeeString NativeHorzResAttribute;
	static const CLeeString NativeVertResAttribute;
	static const CLeeString AutoScaledAttribute;
	static const CLeeString useMaskAttribute;
	static const CLeeString TypeAttribute;
	static const CLeeString TransparentAttribute;
	static const CLeeString FontColorAttribute;

	// functions to handle the various elements
	void handleTextureElement(const XMLAttributes& attr);
	void handleTexElement(const XMLAttributes& attr);
	void handleTextureElementEnd(void);
	void handleTexElementEnd(void);
public:
	virtual ~Texture_xmlHandler(void);
	void elementStart(const CLeeString& element, const XMLAttributes& attributes);
	void elementEnd(const CLeeString& element);
	void text(const CLeeString & text){}
private:
	TexturePackage* d_packet;
};
