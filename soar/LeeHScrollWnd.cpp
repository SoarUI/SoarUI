#include "LeeHScrollWnd.h"
#include "LeeRoot.h"

CLeeHScrollWnd::CLeeHScrollWnd(HWND root,ILookEngine* Eng):ILeeScrollWnd(root,Eng)
{
	d_wndlook ="HorzProgressBar";
	d_wndtype =LWNDT_SCROLLBAR;
	
	
}

CLeeHScrollWnd::~CLeeHScrollWnd(void)
{
}
void CLeeHScrollWnd::InitializeSegments(void)
{
	AbsoluteDim aDimx(0);
	AbsoluteDim aDimy(0);
	AbsoluteDim aDimH(15);
	AbsoluteDim aDimW(15);
	UDim dmin(1.0,-45);
	UDim dmax(1.0,-15);
	UDim dclose(1.0,-15);
	Dimension w(aDimW,DT_WIDTH);
	Dimension h(aDimH,DT_HEIGHT);
	Dimension y(aDimy,DT_TOP_EDGE);
	Dimension x(aDimx,DT_LEFT_EDGE);
	CLeeString itemname ;
	itemname.Format("FLYDREAM__%p__%s",this,"systemLeftButton");
	p_Leftbutton=CLeeRoot::getSingletonPtr()->createWnd(itemname,x,y,w,h,LWNDT_WND,this,d_zIndex);
	UnifiedDim ux(dmax,DT_WIDTH);
	Dimension x1(ux,DT_X_POSITION);
	itemname.Format("FLYDREAM__%p__%s",this,"systemRightButton");
	p_Rightbutton=CLeeRoot::getSingletonPtr()->createWnd(itemname,x1,y,w,h,LWNDT_WND,this,d_zIndex);
	p_Leftbutton->setDreamLook("systemMax");
	p_Rightbutton->setDreamLook("systemMin");
}
float CLeeHScrollWnd::getPos(void)
{
	return d_curPos;
}
void CLeeHScrollWnd::setPos(float pos)
{
	d_curPos =pos;
}
DWORD CLeeHScrollWnd::setRange(int nMin,int nMax)
{
	d_min =nMin;
	d_max =nMax ;
	return 0;
}
bool  CLeeHScrollWnd::getRange(int &nMin,int &nMax)
{
	nMax =d_max;
	nMin = d_min ;
	return true;
}
float  CLeeHScrollWnd::setStep(float stp)
{
	d_step = stp;
	return stp;
}
float  CLeeHScrollWnd::setPage(float page)
{
	d_cntPage = page;
	return 0;
}
int CLeeHScrollWnd::getPageIndex(void)
{
	return d_cntPage;
}
int CLeeHScrollWnd::getPageCount(void)
{
	return d_cntPage;
}
//事件处理
LRESULT CLeeHScrollWnd::HandleEvent ( UINT uMsg ,WPARAM wp ,LPARAM lp )
{
	if (uMsg==FLYD_COMMAND)
	{
		FLYDMSG* pMsg=(FLYDMSG*)wp;
		if (pMsg->sourceWnd==p_Rightbutton && pMsg->mouseEvent ==FLYD_LCLICK_UP)
		{
			FLYDMSG leeMsg;
			leeMsg.message =FLYD_HSCROLL;
			leeMsg.wParam =+1;
			leeMsg.lParam =lp;
			leeMsg.sourceWnd =this;
			leeMsg.targetWnd = d_parent;
			leeMsg.mouseEvent =pMsg->mouseEvent;
			CLeeRoot::getSingletonPtr()->addOfflineMsg(leeMsg);
		}
		if (pMsg->sourceWnd==p_Leftbutton && pMsg->mouseEvent ==FLYD_LCLICK_UP)
		{
			FLYDMSG leeMsg;
			leeMsg.message =FLYD_HSCROLL;
			leeMsg.wParam =-1;
			leeMsg.lParam =lp;
			leeMsg.sourceWnd =this;
			leeMsg.targetWnd = d_parent;
			leeMsg.mouseEvent =pMsg->mouseEvent;
			CLeeRoot::getSingletonPtr()->addOfflineMsg(leeMsg);
		}
	}
	return 0;
}