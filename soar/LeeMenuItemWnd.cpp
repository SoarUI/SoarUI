#include "LeeMenuItemWnd.h"
#include "LeeRoot.h"
#include <leelog.h>

CLeeMenuItemWnd::CLeeMenuItemWnd(HWND root,ILookEngine* Eng):CLeeStaticItemWnd(root,Eng)
{
	d_wndtype =LWNDT_STATICPOPMENUITEM;
	d_checkState=false;
	d_wndlook="TitleBar";
}

CLeeMenuItemWnd::~CLeeMenuItemWnd(void)
{
}
bool CLeeMenuItemWnd::getCheck(void)
{
	return d_checkState ;
}
void CLeeMenuItemWnd::setcheck(bool bcheck)
{
	if (d_checkState != bcheck)
	{
		d_checkState =bcheck;
	}
}
LRESULT CLeeMenuItemWnd::HandleEvent ( UINT uMsg ,WPARAM wParam ,LPARAM lParam) 
{
	
	if (uMsg == WM_LBUTTONUP)
	{
		FLYDMSG leeMsg;
		leeMsg.message =FLYD_ITEMSELECTED;
		leeMsg.mouseEvent =FLYD_LCLICK_UP;
		leeMsg.sourceWnd =this;
		leeMsg.targetWnd =d_OwnerWnd;
		leeMsg.wParam =d_iIndex;
		leeMsg.lParam =d_ID;
		CLeeRoot::getSingletonPtr()->addOfflineMsg(leeMsg);
	}
	if (uMsg == WM_RBUTTONUP)
	{
		FLYDMSG leeMsg;
		leeMsg.message =FLYD_ITEMSELECTED;
		leeMsg.mouseEvent =FLYD_RCLICK_UP;
		leeMsg.sourceWnd =this;
		leeMsg.targetWnd =d_OwnerWnd;
		leeMsg.wParam =d_iIndex;
		leeMsg.lParam =d_ID;
		CLeeRoot::getSingletonPtr()->addOfflineMsg(leeMsg);
	}
	if (uMsg == WM_LBUTTONDBLCLK)
	{
		FLYDMSG leeMsg;
		leeMsg.message =FLYD_ITEMSELECTED;
		leeMsg.mouseEvent =FLYD_LDBCLICK;
		leeMsg.sourceWnd =this;
		leeMsg.targetWnd =d_OwnerWnd;
		leeMsg.wParam =d_iIndex;
		leeMsg.lParam =d_ID;
		CLeeRoot::getSingletonPtr()->addOfflineMsg(leeMsg);
	}
	return 0;//留系统底层处理
}
void CLeeMenuItemWnd::DrawSelf(ILeeDrawInterface *DrawFuns)
{
	RECT rc =getWindowRect();
	RectF rcc=rc;
	RectF rcparent =getCliperRect();
	//主窗口绘制
	if (d_DrawEng->hasWidgetlook(d_wndlook))
	{
		//d_DrawEng->DrawWidgetlook("DefaultClient",d_wndlookState,rcc,d_string);
		d_DrawEng->DrawWidgetlook(d_wndlook,d_wndlookState,rcc,d_string,&rcparent,true);
	}
}