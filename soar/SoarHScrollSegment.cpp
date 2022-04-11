#include "LeeHScrollSegment.h"
#include "LeeRoot.h"

CLeeHScrollSegment::CLeeHScrollSegment(HWND root,ILookEngine* Eng):ILeeScrollWnd(root,Eng)
{
	d_wndlook ="HorzProgressBar";
	d_wndtype =LWNDT_SCROLLSEGMENT;
	
	
}

CLeeHScrollSegment::~CLeeHScrollSegment(void)
{
}
void CLeeHScrollSegment::InitializeSegments(void)
{
	AbsoluteDim aDimx(0);
	AbsoluteDim aDimy(0);
	AbsoluteDim aDimH(d_scrollSegmentHeight);
	AbsoluteDim aDimW(d_scrollSegmentWidth);
	UDim dmax(1.0,-d_scrollSegmentWidth);
	Dimension w(aDimW,DT_WIDTH);
	Dimension h(aDimH,DT_HEIGHT);
	Dimension y(aDimy,DT_TOP_EDGE);
	Dimension x(aDimx,DT_LEFT_EDGE);
	CLeeString itemname ;
	if (d_isPopupWnd)
	{
		itemname.Format("LeeHScrollSegment__%p__%s",this,"systemLeftButton");
		p_Leftbutton=CLeeRoot::getSingletonPtr()->createPopupWnd(itemname,x,y,w,h,LWNDT_WND,this,d_zPopupIndex+1);
		UnifiedDim ux(dmax,DT_WIDTH);
		Dimension x1(ux,DT_X_POSITION);
		itemname.Format("LeeHScrollSegment__%p__%s",this,"systemRightButton");
		p_Rightbutton=CLeeRoot::getSingletonPtr()->createPopupWnd(itemname,x1,y,w,h,LWNDT_WND,this,d_zPopupIndex+1);
	}
	else
	{
		itemname.Format("LeeHScrollSegment__%p__%s",this,"systemLeftButton");
		p_Leftbutton=CLeeRoot::getSingletonPtr()->createWnd(itemname,x,y,w,h,LWNDT_WND,this,d_zIndex+1);
		UnifiedDim ux(dmax,DT_WIDTH);
		Dimension x1(ux,DT_X_POSITION);
		itemname.Format("LeeHScrollSegment__%p__%s",this,"systemRightButton");
		p_Rightbutton=CLeeRoot::getSingletonPtr()->createWnd(itemname,x1,y,w,h,LWNDT_WND,this,d_zIndex+1);
	}
	
	p_Leftbutton->setDreamLook("systemMax");
	p_Rightbutton->setDreamLook("systemMin");
}
float CLeeHScrollSegment::getPos(void)
{
	return d_curPos;
}
void CLeeHScrollSegment::setPos(float pos)
{
	d_curPos =pos;
}
DWORD CLeeHScrollSegment::setRange(int nMin,int nMax)
{
	d_min =nMin;
	d_max =nMax ;
	return 0;
}
bool  CLeeHScrollSegment::getRange(int &nMin,int &nMax)
{
	nMax =d_max;
	nMin = d_min ;
	return true;
}
float  CLeeHScrollSegment::setStep(float stp)
{
	d_step = stp;
	return stp;
}
float  CLeeHScrollSegment::setPage(float page)
{
	d_cntPage = page;
	return 0;
}
int CLeeHScrollSegment::getPageIndex(void)
{
	return d_cntPage;
}
int CLeeHScrollSegment::getPageCount(void)
{
	return d_cntPage;
}
//事件处理
LRESULT CLeeHScrollSegment::HandleEvent ( UINT uMsg ,WPARAM wp ,LPARAM lp )
{
	if (uMsg==FLYD_COMMAND)
	{
		FLYDMSG *pMsg=(FLYDMSG*)wp;
		if (pMsg->sourceWnd==p_Rightbutton&& pMsg->mouseEvent==FLYD_LCLICK_UP)
		{
			FLYDMSG leeMsg;
			leeMsg.message =FLYD_HSCROLL;
			leeMsg.wParam =+1;
			leeMsg.lParam =lp;
			leeMsg.sourceWnd =this;
			leeMsg.targetWnd = d_parent;
			leeMsg.Data=NULL;
			leeMsg.msgSourceTag=FLYD_MSG_ORIG;
			CLeeRoot::getSingletonPtr()->addOfflineMsg(leeMsg);
		}
		if (pMsg->sourceWnd == p_Leftbutton&& pMsg->mouseEvent==FLYD_LCLICK_UP)
		{
			FLYDMSG leeMsg;
			leeMsg.message =FLYD_HSCROLL;
			leeMsg.wParam =-1;
			leeMsg.lParam =lp;
			leeMsg.sourceWnd =this;
			leeMsg.targetWnd = d_parent;
			leeMsg.Data=NULL;
			leeMsg.msgSourceTag=FLYD_MSG_ORIG;
			CLeeRoot::getSingletonPtr()->addOfflineMsg(leeMsg);
		}
	}
	if (uMsg==WM_MOUSEWHEEL  )
	{
		short  zDelta   =   (short)   HIWORD(wp);  
		FLYDMSG leeMsg;
		leeMsg.message =FLYD_HSCROLL;
		leeMsg.wParam =zDelta>0?-1:1;
		leeMsg.lParam =lp;
		leeMsg.sourceWnd =this;
		leeMsg.targetWnd = d_parent;
		leeMsg.Data=NULL;
		leeMsg.msgSourceTag=FLYD_MSG_ORIG;
		CLeeRoot::getSingletonPtr()->addOfflineMsg(leeMsg);
	}
	return 0;
}