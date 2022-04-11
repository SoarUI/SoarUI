#pragma once
#include "leestaticitemwnd.h"

class CLeeMenuItemWnd :
	public CLeeStaticItemWnd
{
public:
	CLeeMenuItemWnd(HWND root,ISoarEngine* Eng);
	~CLeeMenuItemWnd(void);
	bool getCheck(void);
	void setcheck(bool bcheck);
	LRESULT HandleEvent ( UINT uMsg ,WPARAM wParam ,LPARAM lParam) ;
	void DrawSelf(ILeeDrawInterface *DrawFuns);
	
};
