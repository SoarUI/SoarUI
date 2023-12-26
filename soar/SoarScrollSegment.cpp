#include "SoarScrollSegment.h"
#include "SoarRoot.h"

CSoarScrollSegment::CSoarScrollSegment(ISoarWnd *parent,ISoarEngine* Eng,SOARBARALIGN soarAlign,bool bvert):
d_OwnerWnd(parent),d_DrawEng(Eng),d_bVertbar(bvert),d_barAlign(soarAlign),d_parent(NULL),d_ReceiverWnd(NULL)
{
	d_wndlook ="DefaultClient";
	d_wndtype =LWNDT_SYS_SCROLLBAR;
	d_wndlookState="Normal";//皮肤状态
	d_lbuttondown = FALSE;
	d_wndText ="";
	 d_wndState=LWNDST_SHOW;
}

CSoarScrollSegment::~CSoarScrollSegment(void)
{
}
void CSoarScrollSegment::InitializeSegments(void)
{
	AbsoluteDim aDimx(0);
	AbsoluteDim aDimy(0);
	AbsoluteDim aDimH(15);
	AbsoluteDim aDimW(15);
	
	
	UDim dclose(1.0,-15);
	Dimension w(aDimW,DT_WIDTH);
	Dimension h(aDimH,DT_HEIGHT);
	Dimension y(aDimy,DT_TOP_EDGE);
	Dimension x(aDimx,DT_LEFT_EDGE);
	if(d_bVertbar)
	{
		d_wndlook ="VertProgressBar";
		//top
		 ComponentArea rcArea;
		rcArea.d_left= x;
		rcArea.d_top =y;
		rcArea.d_right_or_width=w;
		rcArea.d_bottom_or_height=h;
		d_right_or_up.d_area =rcArea;
		d_right_or_up.d_wndlook = "UpButton" ;
		d_right_or_up.d_wndlookState ="Normal";//皮肤状态
		//down
		UDim dmin(1.0,-15);
		UnifiedDim uy(dmin,DT_HEIGHT);
		Dimension y1(uy,DT_Y_POSITION);
		rcArea.d_left= x;
		rcArea.d_top =y1 ;
		rcArea.d_right_or_width=w;
		rcArea.d_bottom_or_height=h;
		d_left_or_down.d_area =rcArea;
		d_left_or_down.d_wndlook = "DownButton" ;
		d_left_or_down.d_wndlookState ="Normal";//皮肤状态
	}
	else
	{
		d_wndlook ="HorzProgressBar";
		 ComponentArea rcArea;
		rcArea.d_left= x;
		rcArea.d_top =y;
		rcArea.d_right_or_width=w;
		rcArea.d_bottom_or_height=h;
		d_left_or_down.d_area =rcArea;
		d_left_or_down.d_wndlook = "LeftButton" ;
		d_left_or_down.d_wndlookState ="Normal";//皮肤状态
		//
		UDim dx(1.0,-15);
		UnifiedDim ux(dx,DT_WIDTH);
		Dimension x1(ux,DT_X_POSITION);
		rcArea.d_left= x1;
		rcArea.d_top =y;
		rcArea.d_right_or_width=w;
		rcArea.d_bottom_or_height=h;
		d_right_or_up.d_area =rcArea;
		d_right_or_up.d_wndlook = "RightButton" ;
		d_right_or_up.d_wndlookState ="Normal";//皮肤
	}
}
float CSoarScrollSegment::getPos(void)
{
	return d_curPos;
}
void CSoarScrollSegment::setPos(float pos)
{
	d_curPos =pos;
}
DWORD CSoarScrollSegment::setRange(int nMin,int nMax)
{
	d_min =nMin;
	d_max =nMax ;
	return 0;
}
bool  CSoarScrollSegment::getRange(int &nMin,int &nMax)
{
	nMax =d_max;
	nMin = d_min ;
	return true;
}
float  CSoarScrollSegment::setStep(float stp)
{
	d_step = stp;
	return stp;
}
float  CSoarScrollSegment::setPage(float page)
{
	d_cntPage = page;
	return 0;
}
int CSoarScrollSegment::getPageIndex(void)
{
	return d_cntPage;
}
int CSoarScrollSegment::getPageCount(void)
{
	return d_cntPage;
}
RECT CSoarScrollSegment::getBarRect(void) 
{
	RECT rcTmp;
	RECT rcParent;
	rcParent =d_OwnerWnd->getWindowRect();//获取父窗口包括非客户区
	RECT rc;
	RectF rcCur;
	rcCur=d_area.getRect(rcParent);
	rcCur.toWindowRect(rc);
	return rc;
}
RECT CSoarScrollSegment::getBarCliperRect(void){
	RECT rcCur;
	rcCur =getBarRect();
	RectF rccCur =rcCur;
	RECT rcP ;
    if(!d_parent)
	{
		rcP =d_OwnerWnd->getCliperRect();
	}
	else{
		rcP =d_parent->getBarCliperRect();
	}
    
	RectF rccP =rccCur.Intersection(rcP);
	RECT rc;
	rccP.toWindowRect(rc);
	return rc;
}
void CSoarScrollSegment::setArea(ComponentArea& area)
{
	d_area =area;
	this->InitializeSegments();
}
void CSoarScrollSegment::setArea(RECT& rc)
{
	 AbsoluteDim adim(0.0);
	 adim.setValue(rc.left);
	 d_area.d_left.setBaseDimension(adim);
	 d_area.d_left.setDimensionType(DT_LEFT_EDGE);
	 adim.setValue(rc.top);
	 d_area.d_top.setBaseDimension(adim);
	 d_area.d_top.setDimensionType(DT_TOP_EDGE);
	 adim.setValue(rc.right-rc.left);
	 d_area.d_right_or_width.setBaseDimension(adim);
	 d_area.d_right_or_width.setDimensionType(DT_WIDTH);
	 adim.setValue(rc.bottom-rc.top);
	 d_area.d_bottom_or_height.setBaseDimension(adim);
	 d_area.d_bottom_or_height.setDimensionType(DT_HEIGHT);
	 this->InitializeSegments();
}
void CSoarScrollSegment::getArea(ComponentArea& area)
{
	area =d_area;
	this->InitializeSegments();
}
BOOL CSoarScrollSegment::BarHitTest(void)
{
	POINT pt;
	::GetCursorPos(&pt);
	::ScreenToClient( d_OwnerWnd->getRootWnd() ,&pt);
	RECT rc =getBarRect();
	if(::PtInRect(&rc,pt))
		return true;
	//鼠标按下地情况，主要时WM_MOUSEMOVE的处理
	if(d_lbuttondown)
		return true;
	return false;
}
LWNDT CSoarScrollSegment::getType(void) 
{
	return d_wndtype;
}
SOARBARALIGN CSoarScrollSegment::getBarAlign(void)
{
	return d_barAlign;
}
void CSoarScrollSegment::setTitle(CLeeString name)
{
	d_wndText = name;
}
void CSoarScrollSegment::setState(LWNDST state) 
 {
	 if (d_wndState != state)
	 {
		d_wndState =state;
	 }
 }
 LWNDST CSoarScrollSegment::getState(void)
 {
	 return d_wndState;
 }
//事件处理
BOOL CSoarScrollSegment::HandleEvent ( UINT uMsg ,WPARAM wp ,LPARAM lp, LRESULT& lr)
{
	//子窗口绘制
	RECT rc =getBarRect();
	POINT pt;
		::GetCursorPos(&pt);
		::ScreenToClient(d_OwnerWnd->getRootWnd(),&pt);
		RectF rcc;
		RECT rctest;
		if(uMsg==WM_LBUTTONDOWN)
		{
			rcc =d_right_or_up.d_area.getRect(rc);
			 rcc.toWindowRect(rctest);
			 if(::PtInRect(&rctest,pt))
			 {
				 SOARMSG leeMsg;
				if(d_bVertbar)
				{
					leeMsg.message =SOAR_VSCROLL;
				}
				else
				{
					leeMsg.message =SOAR_HSCROLL;
				}
				leeMsg.wParam =+1;
				leeMsg.lParam =lp;
				leeMsg.sourceWnd = this;
				leeMsg.targetWnd = d_OwnerWnd;
				leeMsg.routeWnd=NULL;
				leeMsg.Data=NULL;
				leeMsg.msgSourceTag=SOAR_MSG_ORIG;
				CSoarRoot::getSingletonPtr()->addOfflineMsg(leeMsg);
				lr = 1;
				return true;
			 }
			 rcc =d_left_or_down.d_area.getRect(rc);
			 rcc.toWindowRect(rctest);
			 if(::PtInRect(&rctest,pt))
			 {
				 SOARMSG leeMsg;
				if(d_bVertbar)
				{
					leeMsg.message =SOAR_VSCROLL;
				}
				else
				{
					leeMsg.message =SOAR_HSCROLL;
				}
				leeMsg.wParam =-1;
				leeMsg.lParam =lp;
				leeMsg.sourceWnd = this;
				leeMsg.targetWnd = d_OwnerWnd;
				leeMsg.routeWnd=NULL;
				leeMsg.Data=NULL;
				leeMsg.msgSourceTag=SOAR_MSG_ORIG;
				CSoarRoot::getSingletonPtr()->addOfflineMsg(leeMsg);
				lr = 1;
				return TRUE;
			 }
			 lr = 0;
			 return TRUE;
		}
		
	if (uMsg==WM_MOUSEWHEEL  )
	{
		short  zDelta   =   (short)   HIWORD(wp);  
		SOARMSG leeMsg;
		if(d_bVertbar)
		{
			leeMsg.message =SOAR_VSCROLL;
		}
		else
		{
			leeMsg.message =SOAR_HSCROLL;
		}
		
		leeMsg.wParam =zDelta>0?-1:1;
		leeMsg.lParam =lp;
		leeMsg.sourceWnd =this;
		leeMsg.targetWnd = d_OwnerWnd;
		leeMsg.routeWnd=NULL;
		leeMsg.Data=NULL;
		leeMsg.msgSourceTag=SOAR_MSG_ORIG;
		CSoarRoot::getSingletonPtr()->addOfflineMsg(leeMsg);
	}
	return FALSE;
}
void CSoarScrollSegment::DrawSelf(ILeeDrawInterface *DrawFuns) 
{
	if ((d_OwnerWnd ==NULL || 
		d_OwnerWnd->getState()<= LWNDST_HIDE) )
	{
		return;
	}
	RECT rc =getBarRect();
	RectF rcc=rc;
	RectF rcparent =getBarCliperRect();
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
	 //thunder

	
}
