#ifndef _MYFeelOfficeProvider_h_
#define _MYFeelOfficeProvider_h_
//export DLL
#define LEESDK_DLL

#include "../SoarHeader/leeString.h"
#include "SoarResourceProvider.h"
CFTR_BEGIN
HRESULT LEESDK_API create(LPVOID *ppReturn);
CFTR_END
#endif