#include "SoarSpinWnd.h"

CLeeSpinWnd::CLeeSpinWnd(HWND root,ISoarEngine* Eng):CSoarWnd(root,Eng)
{
	d_wndlook ="MenuBar";
	d_wndtype=LWNDT_SYS_MENUBAR;
	d_wndlookState="Normal";//皮肤状态
	d_wndText ="";
	 d_wndState=LWNDST_SHOW;
	 d_linkwnd=NULL;
}

CLeeSpinWnd::~CLeeSpinWnd(void)
{
}
DWORD CLeeSpinWnd::setRange(int nMin,int nMax)
{
	d_min =nMin;
	d_max =nMax ;
	return 0;
}
bool CLeeSpinWnd::getRange(int &nMin,int &nMax)
{
	nMax =d_max;
	nMin = d_min ;
	return true;
}
void CLeeSpinWnd::setlinkWnd(ISoarWnd* pTargetWnd)
{
	d_linkwnd =pTargetWnd;
}
void CLeeSpinWnd::InitializeSegments(void)
{
	AbsoluteDim aDimx(0);
	AbsoluteDim aDimy(0);
	AbsoluteDim aDimH(15);
	AbsoluteDim aDimW(15);
	int nH=0.5;
	
	UDim uHeight(0.5,0);
	UDim uWidth(1.0,-2);
	UnifiedDim uw(uWidth,DT_WIDTH);
	UnifiedDim uh(uHeight,DT_HEIGHT);
	Dimension w(uw,DT_WIDTH);
	Dimension h(uh,DT_HEIGHT);
	Dimension y(aDimy,DT_TOP_EDGE);
	Dimension x(aDimx,DT_LEFT_EDGE);
	{
		//top
		 ComponentArea rcArea;
		rcArea.d_left= x;
		rcArea.d_top =y;
		rcArea.d_right_or_width=w;
		rcArea.d_bottom_or_height=h;
		d_right_or_up.d_area =rcArea;
		d_right_or_up.d_wndlook = "SpinUpButton" ;
		d_right_or_up.d_wndlookState ="Normal";//皮肤状态
		//down
		UDim dmin(0.5,1);
		UnifiedDim uy(dmin,DT_HEIGHT);
		Dimension y1(uy,DT_Y_POSITION);
		rcArea.d_left= x;
		rcArea.d_top =y1 ;
		rcArea.d_right_or_width=w;
		rcArea.d_bottom_or_height=h;
		d_left_or_down.d_area =rcArea;
		d_left_or_down.d_wndlook = "SpinDownButton" ;
		d_left_or_down.d_wndlookState ="Normal";//皮肤状态
	}
	
}
void CLeeSpinWnd::DrawSelf(ILeeDrawInterface *DrawFuns) 
{
	if ((getState()<= LWNDST_HIDE) )
	{
		return;
	}
	bool bPreState =DrawFuns->graphic_AlphaBlendState();
	DrawFuns->graphic_AlphaBlend(d_bEnableAlphaBlend);
	RECT rc =getWindowRect();
	RectF rcc=rc;
	RectF rcparent =getCliperRect();
	//主窗口绘制
	if (d_DrawEng->hasWidgetlook(d_wndlook))
	{
		d_DrawEng->DrawWidgetlookFrame(d_wndlook,d_wndlookState,rcc,&rcparent,true);
		d_DrawEng->DrawWidgetlook(d_wndlook,d_wndlookState,rcc,d_wndText,-1,-1,&rcparent,true);
	}
	//子窗口绘制
	//左按钮
	rcc =d_left_or_down.d_area.getRect(rc);
	 d_DrawEng->DrawWidgetlook(d_left_or_down.d_wndlook,d_left_or_down.d_wndlookState,
		 rcc,d_left_or_down.d_wndText,-1,-1,&rcparent,true);
	 //右按钮
	 rcc =d_right_or_up.d_area.getRect(rc);
	 d_DrawEng->DrawWidgetlook(d_right_or_up.d_wndlook,d_right_or_up.d_wndlookState,
		 rcc,d_right_or_up.d_wndText,-1,-1,&rcparent,true);
	 //恢复原来的混合状态
	 DrawFuns->graphic_AlphaBlend(bPreState);
	
}
BOOL CLeeSpinWnd::HandleEvent ( UINT uMsg ,WPARAM wp ,LPARAM lp, LRESULT& lr)
{
	//子窗口绘制
	RECT rc =getWindowRect();
	POINT pt;
		::GetCursorPos(&pt);
		::ScreenToClient(getRootWnd(),&pt);
		RectF rcc;
		RECT rctest;
		if(uMsg==WM_LBUTTONDOWN && d_linkwnd)
		{
			CLeeString sdata=d_linkwnd->getText();
			int data=atoi(sdata.c_str());
			rcc =d_right_or_up.d_area.getRect(rc);
			 rcc.toWindowRect(rctest);
			 if(::PtInRect(&rctest,pt))
			 {
				data++;
				if(data>d_max)
				{
					data=d_max;
				}
				sdata.Format("%d",data);
				d_linkwnd->setText(sdata);
				return 1;
			 }
			 rcc =d_left_or_down.d_area.getRect(rc);
			 rcc.toWindowRect(rctest);
			 if(::PtInRect(&rctest,pt))
			 {
				data--;
				if(data<d_min)
				{
					data=d_min;
				}
				sdata.Format("%d",data);
				d_linkwnd->setText(sdata);
				return 1;
			 }
			 lr = 0;
			 return true;
		}
		
	
	return CSoarWnd::HandleEvent(uMsg,wp,lp,lr);//留系统底层处理
}
void CLeeSpinWnd::checklookState(POINT &pt)
{
	RECT rc =getWindowRect();//取得相对于Root窗口的偏移
	RectF rcc;
	RECT rctest;
	rcc =d_right_or_up.d_area.getRect(rc);
	rcc.toWindowRect(rctest);
	if(::PtInRect(&rctest,pt))
	{
		d_right_or_up.d_wndlookState ="Hover";//皮肤状态		
		return ;
	}
	rcc =d_left_or_down.d_area.getRect(rc);
	rcc.toWindowRect(rctest);
	if(::PtInRect(&rctest,pt))
	{	
		d_left_or_down.d_wndlookState ="Hover";//皮肤状态
		return ;
	}
	
	CLeeString state=d_wndlookState;
	if (::PtInRect(&rc,pt))
	{
		d_wndlookState ="Hover";
	}
	else
		d_wndlookState ="Normal";
}