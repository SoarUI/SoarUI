#include "SoarCheckButtonWnd.h"

CLeeCheckButtonWnd::CLeeCheckButtonWnd(HWND root,ISoarEngine* Eng):CSoarWnd(root,Eng)
{
	d_wndlook="CheckButton";
	d_wndtype =LWNDT::LWNDT_RADIOBTN;
	d_wndlookState="Normal";
	d_checked = false;
}

CLeeCheckButtonWnd::~CLeeCheckButtonWnd(void)
{
}
void CLeeCheckButtonWnd::DrawSelf(ILeeDrawInterface *DrawFuns)
{
	if (d_parent !=NULL && d_parent->getState()<= LWNDST_HIDE)
	{
		return;
	}
	RECT rc =getWindowRect();
	
	POINT pt;
	::GetCursorPos(&pt);
	::ScreenToClient(d_rootWnd,&pt);
	if (d_checked)
	{
		d_wndlookState="SelectedNormal";
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
LRESULT CLeeCheckButtonWnd::HandleEvent ( UINT uMessage,WPARAM wp ,LPARAM lp ) 
{
	if (uMessage==WM_LBUTTONDOWN)
	{
		if (d_checked)
		{
			setCheck(false);
		}
		else
			setCheck(true);
	}
	return CSoarWnd::HandleEvent(uMessage,wp,lp);
}
bool CLeeCheckButtonWnd::getCheck(void)
{
	return d_checked;
}
void CLeeCheckButtonWnd::setCheck(bool bChecked)
{
	if ( (d_checked && bChecked)||
		(!d_checked && !bChecked))
	{
		return ;
	}
	
	if (bChecked)
	{
		d_checked =true;
	}
	else
	{
		d_checked =false;
	}
}