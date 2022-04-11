#pragma once
#include "../SoarHeader/SoarExport.h"
#include "SoarWnd.h"
class CLeeStaticWnd :
	public CSoarWnd
{
public:
	CLeeStaticWnd(HWND root,ISoarEngine* Eng);
public:
	virtual ~CLeeStaticWnd(void);
	virtual void DrawSelf(ILeeDrawInterface *DrawFuns) ;
};
