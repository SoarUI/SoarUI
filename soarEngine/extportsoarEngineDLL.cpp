// EncryptProvider.cpp : 定义 DLL 应用程序的入口点。
//
#define LEESDK_DLL
#include "SoarEngine.h"
#include "../SoarHeader/leemacro.h"

#ifdef _MANAGED
#pragma managed(push, off)
#endif
HMODULE g_staticHmodule =NULL;
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	g_staticHmodule=hModule;
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
	*ppReturn =new CSoarEngine(g_staticHmodule);
	return S_OK;
}
void LEESDK_API Release(LPVOID *ppReturn)
{
	if (ppReturn==NULL)
	{
		return ;
	}
	delete (*ppReturn);
	*ppReturn  =NULL;
}
CFTR_END