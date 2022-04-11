#define LEESDK_DLL
#include "Soar.h"
// DefaultBlender.cpp : 定义 DLL 应用程序的入口点。
//

//#include "stdafx.h"

#include "../SoarHeader/leemacro.h"

#ifdef _MANAGED
#pragma managed(push, off)
#endif
HMODULE d_gMoudule =NULL;
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	d_gMoudule = hModule;
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
    return TRUE;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif
CFTR_BEGIN
HRESULT LEESDK_API create(LPVOID *ppReturn)
{
	if (ppReturn==NULL)
	{
		return E_INVALIDARG;
	}
	*ppReturn =NULL;
	CSoar *newPtrObj =new CSoar(d_gMoudule);
	if (!newPtrObj)
	{
		*ppReturn =NULL;
		return S_FALSE;
	}
	*ppReturn =newPtrObj;
	return S_OK;
}
CFTR_END