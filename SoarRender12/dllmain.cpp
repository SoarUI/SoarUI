// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include"CSoarRender12.h"
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
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
CFTR_BEGIN
HRESULT create(LPVOID* ppReturn)
{
    if (ppReturn == NULL)
    {
        return E_INVALIDARG;
    }
    *ppReturn = NULL;
    *ppReturn = new CSoarRender12();
    return S_OK;
}
CFTR_END
