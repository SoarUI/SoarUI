#include "LeeFrameWnd.h"
#include "LeeRoot.h"
#include "Dimensions.h"
#include "UDim.h"

CLeeFrameWnd::CLeeFrameWnd(HWND root,ILookEngine* Eng):ILeeFrameWnd(root,Eng)
{
	d_wndlook ="Static";
	d_wndtype =LWNDT_FRAME;
	d_wndStyle=LWNDSTY_SYSTITLEBAR;
}

CLeeFrameWnd::~CLeeFrameWnd(void)
{
}
void CLeeFrameWnd::InitializeSegments(void)
{
	d_wndStyle=(LWNDSTY)(d_wndStyle|LWNDSTY_SYSTITLEBAR);
	AbsoluteDim aDimx(0);
	AbsoluteDim aDimy(0);
	AbsoluteDim aDimH(d_TitleBarSegmentHeight);
	AbsoluteDim aDimW(15);
	UDim dmin(1.0,0);
	Dimension x(aDimx,DT_LEFT_EDGE);
	Dimension h(aDimH,DT_HEIGHT);
	Dimension y(aDimy,DT_TOP_EDGE);
	UnifiedDim ux(dmin,DT_WIDTH);
	Dimension w(ux,DT_WIDTH);
	CLeeString itemname ;
	itemname.Format("LeeFrameWnd__%p__%s",this,"TitleBar");
	if (d_isPopupWnd )
	{
		if(d_wndStyle & LWNDSTY_SYSTITLEBAR)
		 p_titlebar=CLeeRoot::getSingletonPtr()->createPopupWnd(itemname,x,y,w,h,LWNDT_TITLEBAR,this,d_zPopupIndex+1);
	} 
	else
	{
		if(d_wndStyle & LWNDSTY_SYSTITLEBAR)
		  p_titlebar=CLeeRoot::getSingletonPtr()->createWnd(itemname,x,y,w,h,LWNDT_TITLEBAR,this,d_zIndex+1);
	}
	
	itemname.Format("LeeFrameWnd__%p__%s",this,"MenuBar");
	AbsoluteDim aDimy1(0);
	Dimension y1(aDimy1,DT_TOP_EDGE);
	if (d_isPopupWnd)
	{
		aDimH.setValue(d_MenuBarSegmentHeight);
		if(d_wndStyle & LWNDSTY_SYSMENUBAR)
		p_menubar=CLeeRoot::getSingletonPtr()->createPopupWnd(itemname,x,y1,w,h,LWNDT_MENUBAR,this,d_zPopupIndex+1);
	} 
	else
	{
		aDimH.setValue(d_MenuBarSegmentHeight);
		if(d_wndStyle & LWNDSTY_SYSMENUBAR)
		 p_menubar=CLeeRoot::getSingletonPtr()->createWnd(itemname,x,y1,w,h,LWNDT_MENUBAR,this,d_zIndex+1);
	}
	
}
void CLeeFrameWnd::setText(CLeeString name)
{
	if(p_titlebar)
		p_titlebar->setText(name);
}
LRESULT CLeeFrameWnd::HandleEvent(UINT uMsg ,WPARAM wParam ,LPARAM lParam)
{
	if (uMsg== FLYD_TITLEBARMSG)
	{
		CLeeString itemname;
		itemname.Format("TitleBar__%p__%s",p_titlebar,"systemMin");
		FLYDMSG* pMsg=(FLYDMSG*)wParam;
		if (pMsg->routeWnd == p_titlebar && 
			pMsg->sourceWnd==p_titlebar->getChildWnd(itemname) && 
			pMsg->mouseEvent ==FLYD_LCLICK_DOWN)
		{
			if(getState()==LWNDST_TItleBarOnly)
			{
				setState(LWNDST_SHOW);
			}
			else
				setState(LWNDST_TItleBarOnly);
		}
		itemname.Format("TitleBar__%p__%s",p_titlebar,"systemMax");
		if (pMsg->routeWnd == p_titlebar 
			&& pMsg->sourceWnd==p_titlebar->getChildWnd(itemname)
			&& pMsg->mouseEvent ==FLYD_LCLICK_DOWN)
		{
			if(getState()==LWNDST_MAX)
			{
				setState(LWNDST_RESTORE);
			}
			else
				setState(LWNDST_MAX);
			
		}
		itemname.Format("TitleBar__%p__%s",p_titlebar,"systemClose");
		if (pMsg->routeWnd == p_titlebar 
			&& pMsg->sourceWnd==p_titlebar->getChildWnd(itemname)
			&& pMsg->mouseEvent ==FLYD_LCLICK_DOWN)
		{
			setState(LWNDST_HIDE);
		}
		
		return 0 ;
	}
	if (uMsg==FLYD_COMMAND)
	{
		FLYDMSG* pMsg=(FLYDMSG*)wParam;
		if (pMsg->sourceWnd == p_titlebar && pMsg->mouseEvent==FLYD_LDBCLICK)
		{
			if(getState()==LWNDST_MAX)
			{
				setState(LWNDST_RESTORE);
			}
			else
				setState(LWNDST_MAX);
		}
		return 0 ;
	}
	return CLeeWnd::HandleEvent(uMsg,wParam,lParam);
}