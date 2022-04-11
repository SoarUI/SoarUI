#ifndef _STDExpatParser_h_
#define _STDExpatParser_h_
#include "../SoarHeader/leemacro.h"
//export DLL
#define LEESDK_DLL
CFTR_BEGIN
HRESULT LEESDK_API create(LPVOID *ppReturn);
void    LEESDK_API destroy(LPVOID lpIobj);
CFTR_END
#endif
