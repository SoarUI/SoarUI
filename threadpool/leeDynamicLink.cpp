#include "leeDynamicLink.h"

CLeeDynamicLink::CLeeDynamicLink(void):d_handle(NULL)
{
}

CLeeDynamicLink::~CLeeDynamicLink(void)
{
	unload();
}

void* CLeeDynamicLink::getFunction(LPCSTR func)
{
	if (NULL==d_handle)
	{
		return d_handle;
	}
	return (void*)::GetProcAddress((HMODULE)d_handle,func);
}
void* CLeeDynamicLink::load(const CLeeString & pathname,const CLeeString & name)
{
	unload();
	d_name =name;
	d_handle=::LoadLibrary(pathname.c_str());
	 if (NULL==d_handle)
		 {
			 char msg[256]={0,};
			 sprintf(msg,"º”‘ÿDLL<%s> ß∞‹£∫¥ÌŒÛ¬Î<%d>",d_name.c_str(),GetLastError());
			 MessageBoxA(NULL,msg," ß∞‹¡À",MB_OK);
			 return 0;
		 }
	return d_handle;
}
void CLeeDynamicLink::unload(void)
{
	::FreeLibrary((HMODULE)d_handle);
	d_handle =NULL;
}