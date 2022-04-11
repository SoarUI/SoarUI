#include "LeeScrollWnd.h"
#include "LeeRoot.h"

CLeeScrollWnd::CLeeScrollWnd(HWND root,ILookEngine* Eng):ILeeScrollWnd(root,Eng)
{
	d_wndlook ="DefaultClient";
	d_wndtype =LWNDT_SCROLLBAR;
	
	
}

CLeeScrollWnd::~CLeeScrollWnd(void)
{
}
void CLeeScrollWnd::InitializeSegments(void)
{
	AbsoluteDim aDimx(0);
	AbsoluteDim aDimy(0);
	AbsoluteDim aDimH(15);
	AbsoluteDim aDimW(15);
	UDim dmin(1.0,-45);
	UDim dmax(1.0,-30);
	UDim dclose(1.0,-15);
	Dimension w(aDimW,DT_WIDTH);
	Dimension h(aDimH,DT_HEIGHT);
	Dimension y(aDimy,DT_TOP_EDGE);
	Dimension x(aDimx,DT_LEFT_EDGE);
	CLeeString itemname ;
	itemname.Format("FLYDREAM__%p__%s",this,"systemUpButton");
	if (d_isPopupWnd)
	{
		p_Upbutton=CLeeRoot::getSingletonPtr()->createPopupWnd(itemname,x,y,w,h,LWNDT_WND,this,d_zPopupIndex+1);
	} 
	else
	{
		p_Upbutton=CLeeRoot::getSingletonPtr()->createWnd(itemname,x,y,w,h,LWNDT_WND,this,d_zIndex+1);
	}
	
	UnifiedDim ux1(dmax,DT_HEIGHT);
	Dimension y1(ux1,DT_Y_POSITION);
	itemname.Format("FLYDREAM__%p__%s",this,"systemDownButton");
	if (d_isPopupWnd)
	{
		p_Downbutton=CLeeRoot::getSingletonPtr()->createPopupWnd(itemname,x,y1,w,h,LWNDT_WND,this,d_zPopupIndex+1);
	} 
	else
	{
		p_Downbutton=CLeeRoot::getSingletonPtr()->createWnd(itemname,x,y1,w,h,LWNDT_WND,this,d_zIndex+1);
	}
	
	p_Upbutton->setDreamLook("systemMax");
	p_Downbutton->setDreamLook("systemMin");
}
float CLeeScrollWnd::getPos(void)
{
	return d_curPos;
}
void CLeeScrollWnd::setPos(float pos)
{
	d_curPos =pos;
}
DWORD CLeeScrollWnd::setRange(int nMin,int nMax)
{
	d_min =nMin;
	d_max =nMax ;
	return 0;
}
bool  CLeeScrollWnd::getRange(int &nMin,int &nMax)
{
	nMax =d_max;
	nMin = d_min ;
	return true;
}
float  CLeeScrollWnd::setStep(float stp)
{
	d_step = stp;
	return stp;
}
float  CLeeScrollWnd::setPage(float page)
{
	d_cntPage = page;
	return 0;
}
int CLeeScrollWnd::getPageIndex(void)
{
	return d_cntPage;
}
int CLeeScrollWnd::getPageCount(void)
{
	return d_cntPage;
}
//事件处理
LRESULT CLeeScrollWnd::HandleEvent (UINT uMsg ,WPARAM wp ,LPARAM lp )
{
	if (uMsg==FLYD_COMMAND)
	{
		FLYDMSG* pMsg=(FLYDMSG*)wp;
		if (pMsg->sourceWnd==p_Upbutton && pMsg->mouseEvent ==FLYD_LCLICK_UP)
		{
			FLYDMSG leeMsg;
			leeMsg.message =FLYD_VSCROLL;
			leeMsg.wParam =-1;
			leeMsg.lParam =lp;
			leeMsg.sourceWnd =this;
			leeMsg.targetWnd = d_parent;
			leeMsg.mouseEvent =pMsg->mouseEvent;
			CLeeRoot::getSingletonPtr()->addOfflineMsg(leeMsg);
		}
		if (pMsg->sourceWnd==p_Downbutton && pMsg->mouseEvent ==FLYD_LCLICK_UP)
		{
			FLYDMSG leeMsg;
			leeMsg.message =FLYD_VSCROLL;
			leeMsg.wParam =1;
			leeMsg.lParam =lp;
			leeMsg.sourceWnd =this;
			leeMsg.targetWnd = d_parent;
			leeMsg.mouseEvent =pMsg->mouseEvent;
			CLeeRoot::getSingletonPtr()->addOfflineMsg(leeMsg);
		}
	}
	return 0;
}