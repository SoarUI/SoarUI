#include "SoarStaticWnd.h"

CLeeStaticWnd::CLeeStaticWnd(HWND root,ISoarEngine* Eng):CSoarWnd(root,Eng)

{
	d_wndlook="Static";
	d_wndlookState="Normal";
}

CLeeStaticWnd::~CLeeStaticWnd(void)
{
}
void CLeeStaticWnd::DrawSelf(ILeeDrawInterface *DrawFuns) 
{
	if (d_parent !=NULL && d_parent->getState()<= LWNDST_HIDE)
	{
		return;
	}
	if (getState()<= LWNDST_HIDE)
	{
		return;
	}
	RECT rc =getWindowRect();
	RectF rcc=rc;
	RectF rcparent =getCliperRect();
	if (d_DrawEng->hasWidgetlook(d_wndlook))
	{
		
		d_DrawEng->DrawWidgetlook(d_wndlook,d_wndlookState,rcc,d_wndText,d_VertTextFormat,d_horzTextFormat,&rcparent,true);
		
	}
}