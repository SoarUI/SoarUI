#ifndef _IImgCodecBase_h_
#define _IImgCodecBase_h_
#include "leeString.h"
#include "IRawDataContainer.h"
#include "ILeeTexture.h"
class ITextureBase;
class IRawDataContainer;
class IImgCodecBase
{
public:
	IImgCodecBase(const CLeeString & codecID):d_identityString(codecID){}
	virtual ~IImgCodecBase(void){}
	const CLeeString& getIdentityString(void) const { return d_identityString ;}
	virtual ITextureBase * load(const IRawDataContainer& data,ILeeTexture**ppoutRawTexture)=0;
	virtual void Release() =0;
private:
	CLeeString d_identityString;
	//copy_control
private:
	IImgCodecBase(const IImgCodecBase& other);
	IImgCodecBase& operator =(const IImgCodecBase & other);
};
#endif