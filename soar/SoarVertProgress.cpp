#include "SoarVertProgress.h"

CLeeVertProgress::CLeeVertProgress(HWND root,ISoarEngine* Eng):CSoarWnd(root,Eng),
d_step(10),
d_curPos(0),
d_min(0),
d_max(100)
{
	d_wndlook =_T("VertProgressBar");
	d_wndtype =LWNDT_PROGRESSBAR;
	d_progressSegment =_T("VertProgressBarSegment");
	d_wndStyle =LWNDSTY_VERTWND;
	d_wndlookState="Normal";
}

CLeeVertProgress::~CLeeVertProgress(void)
{
}
float CLeeVertProgress::getPos(void)
{
	return d_curPos;
}
void CLeeVertProgress::setPos(float pos)
{
	d_curPos =pos;
}
DWORD CLeeVertProgress::setRange(int nMin,int nMax)
{
	d_max =nMax;
	d_min = nMin ;
	return 0;
}
bool  CLeeVertProgress::getRange(int &nMin,int &nMax)
{
	nMax = d_max;
	nMin = d_min;
	return true ;
}
float  CLeeVertProgress::setStep(float stp)
{
	d_step =stp;
	return stp;
}
void  CLeeVertProgress::DrawSelf(ILeeDrawInterface *DrawFuns) 
{
	if (d_parent !=NULL && d_parent->getState()<= LWNDST_HIDE)
	{
		return;
	}
	RECT rc =getWindowRect();
	RectF rcc=rc;
	RectF rcparent =getCliperRect();
	//进度
	float pross =(float)d_curPos/(d_max-d_min+1);
	RECT rcProg =rc;
	rcProg.top =rc.bottom-(rc.bottom-rc.top)*pross;
	rcProg.bottom-=3;
	rcProg.top +=3;
	RectF rcp=rcProg;
	//绘制背景
	if (d_DrawEng->hasWidgetlook(d_wndlook))
	{
		d_DrawEng->DrawWidgetlook(d_wndlook,d_wndlookState,rcc,d_wndText,-1,-1,&rcparent,true);
	}
	//绘制进度
	if (d_DrawEng->hasWidgetlook(d_progressSegment))
	{
		d_DrawEng->DrawWidgetlook(d_progressSegment,d_wndlookState,rcp,d_wndText,-1,-1,&rcparent,true);
	}
}