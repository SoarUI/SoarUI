#pragma once
#include "../SoarHeader/SoarExport.h"
#include "soarwnd.h"
class CLeeGroupBox :
	public CSoarWnd
{
public:
	CLeeGroupBox(HWND root,ISoarEngine* Eng);
	~CLeeGroupBox(void);
	virtual void DrawSelf(ILeeDrawInterface *DrawFuns);
    virtual bool AddControl(int topleft,int topY,ISoarWnd* pControl);
	LRESULT HandleEvent ( UINT uMsg ,WPARAM wParam ,LPARAM lParam) ;
	RECT getClientRect(void) ;
};
