#include "SoarButtonWnd.h"

CLeeButtonWnd::CLeeButtonWnd(HWND root,ISoarEngine* Eng):CSoarWnd(root,Eng)
{
	d_wndlook="PushButton";
	d_wndtype =LWNDT::LWNDT_BUTTON;
	d_VertTextFormat=VTF_CENTRE_ALIGNED;
	d_horzTextFormat=HTF_CENTRE_ALIGNED;
}

CLeeButtonWnd::~CLeeButtonWnd(void)
{
}
void CLeeButtonWnd::DrawSelf(ILeeDrawInterface *DrawFuns) 
{
	if (d_parent !=NULL && d_parent->getState()<= LWNDST_HIDE)
	{
		return;
	}
	RECT rc =getClientRect();

	POINT pt;
	::GetCursorPos(&pt);
	::ScreenToClient(d_rootWnd,&pt);
	if (::PtInRect(&rc,pt))
	{
		d_wndlookState="Hover";
	}
	else
	{
		d_wndlookState="Normal";
	}
	RectF rcc=rc;
	RectF rcparent =getCliperRect();
	if (d_DrawEng->hasWidgetlook(d_wndlook))
	{
		d_DrawEng->DrawWidgetlook(d_wndlook,d_wndlookState,rcc,d_wndText,d_VertTextFormat,d_horzTextFormat,&rcparent,true);
	}
	
}
BOOL CLeeButtonWnd::HandleEvent ( UINT uMessage,WPARAM wp ,LPARAM lp, LRESULT& lr)
{
	return CSoarWnd::HandleEvent(uMessage,wp,lp,lr);
}
void CLeeButtonWnd::checklookState(POINT &pt)
{
	RECT rc =getWindowRect();//取得相对于Root窗口的偏移
	CLeeString state=d_wndlookState;
	if (::PtInRect(&rc,pt))
	{
		d_wndlookState ="Hover";
	}
	else
		d_wndlookState ="Normal";
}