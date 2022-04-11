#pragma once
//对DLl的包装类
#include "../SoarHeader/leeString.h"
class CLeeDynamicLink
{
public:
	CLeeDynamicLink(void);
public:
	virtual ~CLeeDynamicLink(void);
	void* getFunction(LPCSTR func);
	void* load(const CLeeString & pathname,const CLeeString & name);
	void unload(void);
private:
	void* d_handle;
	CLeeString d_name;
};
