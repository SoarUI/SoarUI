#include "SoarGroupBox.h"
#include "SoarRoot.h"

CLeeGroupBox::CLeeGroupBox(HWND root,ISoarEngine* Eng):CSoarWnd(root,Eng)
{
	d_wndlook="GroupBox";
	d_wndtype =LWNDT_GROUPBOX;
}

CLeeGroupBox::~CLeeGroupBox(void)
{
}
void CLeeGroupBox::DrawSelf(ILeeDrawInterface *DrawFuns)
{
	RECT rc =getWindowRect();
	RectF rcc=rc;
	RectF rcparent =getCliperRect();
	//主窗口绘制
	if (d_DrawEng->hasWidgetlook(d_wndlook))
	{
		d_DrawEng->DrawWidgetlook(d_wndlook,d_wndlookState,rcc,d_wndText,d_VertTextFormat,d_horzTextFormat,&rcparent,true);
	}
}
bool CLeeGroupBox::AddControl(int topleft,int topY,ISoarWnd* pControl)
{
	pControl->setParent(this);
	AbsoluteDim ax(topleft);
	Dimension xx(ax,DT_X_POSITION);
	AbsoluteDim ay(topY);
	Dimension yy(ay,DT_Y_POSITION);
	ComponentArea area;
	pControl->getArea(area);
	area.d_left =xx;
	area.d_top =yy;
	pControl->setArea(area);
	pControl->setState(LWNDST_SHOW);
	return true;
}
BOOL CLeeGroupBox::HandleEvent ( UINT uMsg ,WPARAM wParam ,LPARAM lParam, LRESULT& lr)
{
	if (uMsg== SOAR_COMMAND)
	{
		SOARMSG* pMsg=(SOARMSG*)wParam;
		if (pMsg!=NULL )
		{
			SOARMSG leeMsg;
			leeMsg.message =SOAR_GROUPBOXMSG;
			leeMsg.mouseEvent =pMsg->mouseEvent;
			leeMsg.sourceWnd =pMsg->sourceWnd;
			leeMsg.routeWnd=this;
			leeMsg.msgSourceTag =SOAR_MSG_TRANSFER;
			leeMsg.targetWnd =d_parent;
			leeMsg.wParam =pMsg->wParam;
			leeMsg.lParam =pMsg->lParam;
			CSoarRoot::getSingletonPtr()->addOfflineMsg(leeMsg);
		}
	}
	return CSoarWnd::HandleEvent(uMsg,wParam,lParam,lr);
}
RECT CLeeGroupBox::getClientRect(void) 
{
	RECT rcTmp;
	if (d_parent ==NULL)
	{
		GetClientRect(d_rootWnd,&rcTmp);
		RectF rc=rcTmp;
		rc=d_area.getRect(rcTmp);
		RECT rt;
		rc.toWindowRect(rt);
		//减去按钮的区域
		rt.top +=d_GroupBoxTitleHeight;
		return rt;
	}
	RECT rc =d_parent->getClientRect();
	RectF rcCur=d_area.getRect(rc);
	rcCur.toWindowRect(rc);
	//减去按钮的区域
	rc.top +=d_GroupBoxTitleHeight;
	return rc;
}