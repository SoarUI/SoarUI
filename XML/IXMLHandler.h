#ifndef _IXMLHandler_h_
#define _IXMLHandler_h_
#include "../SoarHeader/LeeString.h"
#include "XMLAttributes.h"
class IXMLHandler
{
public:
	IXMLHandler(void){}
	virtual ~IXMLHandler(void){}
	virtual void elementStart(const CLeeString& element, const XMLAttributes& attributes) LEEPURE;
	virtual void elementEnd(const CLeeString& element) LEEPURE ;
	virtual void text(const CLeeString& text) LEEPURE ;
};
#endif