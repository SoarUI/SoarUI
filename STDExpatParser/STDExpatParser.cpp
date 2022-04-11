// STDExpatParser.cpp : 定义 DLL 应用程序的入口点。
//

#include "stdafx.h"
#include "STDExpatParser.h"
#include "stdInterpreter.h"
#ifdef _MANAGED
#pragma managed(push, off)
#endif
HMODULE g_hInst =NULL;
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	g_hInst =hModule;
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		{
			//HideModuleByHandle(g_hInst);
			break;
		}
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
HRESULT create(LPVOID *ppReturn)
{
	if (ppReturn==NULL)
	{
		return E_INVALIDARG;
	}
	*ppReturn =NULL;
	*ppReturn =new CSTDInterpreter(g_hInst);
	return S_OK;
}
CFTR_END