#include "SoarSystemScrollSegment.h"
#include "../SoarRoot.h"

CSoarSystemScrollSegment::CSoarSystemScrollSegment(ISoarWnd *parent,ISoarEngine* Eng,bool bvert):
d_OwnerWnd(parent),d_DrawEng(Eng),d_bVertbar(bvert),/*d_barAlign(soarAlign),*/d_parent(NULL),d_ReceiverWnd(parent)
{
	d_wndlook ="DefaultClient";
	d_wndtype =LWNDT_SYS_SCROLLBAR;
	d_wndlookState="Normal";//皮肤状态
	d_lbuttondown = FALSE;
	d_wndText ="";
	 d_wndState=LWNDST_SHOW;
	 d_max=100;
	 d_min=0;
}

CSoarSystemScrollSegment::~CSoarSystemScrollSegment(void)
{
}
void CSoarSystemScrollSegment::InitializeSegments(void)
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
		//
		UDim dthunder(1.0,0);
		UnifiedDim udimthunder(dthunder,DT_WIDTH);
		w.setBaseDimension(udimthunder);
		aDimH.setValue(15);
		Dimension y2(aDimH,DT_Y_POSITION);
		rcArea.d_left= x;
		rcArea.d_top =y2 ;
		rcArea.d_right_or_width=w;
		rcArea.d_bottom_or_height=h;
		d_thunder.d_area =rcArea;
		d_thunder.d_wndlook = "VertSliderThumb" ;
		d_thunder.d_wndlookState ="Normal";//皮肤状态
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
		//
		UDim dthunder(1.0,0);
		UnifiedDim udimthunder(dthunder,DT_HEIGHT);
		h.setBaseDimension(udimthunder);
		Dimension x2(aDimH,DT_X_POSITION);
		rcArea.d_left= x2;
		rcArea.d_top =y ;
		rcArea.d_right_or_width=w;
		rcArea.d_bottom_or_height=h;
		d_thunder.d_area =rcArea;
		d_thunder.d_wndlook = "HorzSliderThumb" ;
		d_thunder.d_wndlookState ="Normal";//皮肤状态
	}
}
float CSoarSystemScrollSegment::getPos(void)
{
	return d_curPos;
}
void CSoarSystemScrollSegment::setPos(float pos)
{
	if(pos<0)
		pos=0;
	if(pos>100)
		pos=100;
	d_curPos =pos;
	RECT rc =getBarRect();
	AbsoluteDim aDimx(0);
	AbsoluteDim aDimy(0);
	AbsoluteDim aDimH(15);
	AbsoluteDim aDimW(15);
	Dimension w(aDimW,DT_WIDTH);
	Dimension h(aDimH,DT_HEIGHT);
	Dimension y(aDimy,DT_TOP_EDGE);
	Dimension x(aDimx,DT_LEFT_EDGE);
	ComponentArea rcArea;
	rcArea.d_left= x;
	rcArea.d_top =y;
	rcArea.d_right_or_width=w;
	rcArea.d_bottom_or_height=h;
	float pross =(float)d_curPos/(d_max-d_min+1);
	
	if(d_bVertbar)
	{
		
		aDimy.setValue(15+(rc.bottom-rc.top-45)*pross);
		y.setBaseDimension(aDimy);
		rcArea.d_top =y;
		UDim dthunder(1.0,0);
		UnifiedDim udimthunder(dthunder,DT_WIDTH);
		w.setBaseDimension(udimthunder);
		rcArea.d_right_or_width=w;
	}
	else
	{
		aDimx.setValue(15+(rc.right-rc.left-45)*pross);
		x.setBaseDimension(aDimx);
		rcArea.d_left =x;
		//
		UDim dthunder(1.0,0);
		UnifiedDim udimthunder(dthunder,DT_HEIGHT);
		h.setBaseDimension(udimthunder);
		rcArea.d_bottom_or_height=h;
	}
	d_thunder.d_area =rcArea;
}
DWORD CSoarSystemScrollSegment::setRange(int nMin,int nMax)
{
	d_min =nMin;
	d_max =nMax ;
	return 0;
}
bool  CSoarSystemScrollSegment::getRange(int &nMin,int &nMax)
{
	nMax =d_max;
	nMin = d_min ;
	return true;
}
float  CSoarSystemScrollSegment::setStep(float stp)
{
	d_step = stp;
	return stp;
}
float  CSoarSystemScrollSegment::setPage(float page)
{
	d_cntPage = page;
	return 0;
}
int CSoarSystemScrollSegment::getPageIndex(void)
{
	return d_cntPage;
}
int CSoarSystemScrollSegment::getPageCount(void)
{
	return d_cntPage;
}
RECT CSoarSystemScrollSegment::getBarRect(void) 
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
RECT CSoarSystemScrollSegment::getBarCliperRect(void){
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
void CSoarSystemScrollSegment::setArea(ComponentArea& area)
{
	d_area =area;
	this->InitializeSegments();
}
void CSoarSystemScrollSegment::setArea(RECT& rc)
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
ComponentArea CSoarSystemScrollSegment::getArea()
{
	return d_area;
}
BOOL CSoarSystemScrollSegment::BarHitTest(void)
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
LWNDT CSoarSystemScrollSegment::getType(void) 
{
	return d_wndtype;
}
SOARBARALIGN CSoarSystemScrollSegment::getBarAlign(void)
{
	return d_barAlign;
}
void CSoarSystemScrollSegment::setTitle(const CLeeString& name)
{
	d_wndText = name;
}
void CSoarSystemScrollSegment::setState(LWNDST state) 
 {
	 if (d_wndState != state)
	 {
		d_wndState =state;
	 }
 }
 LWNDST CSoarSystemScrollSegment::getState(void)
 {
	 return d_wndState;
 }
//事件处理
BOOL CSoarSystemScrollSegment::HandleEvent ( UINT uMsg ,WPARAM wp ,LPARAM lp, LRESULT& lr)
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
				lr = 0;
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
				lr = 0;
				return true;
			 }
			 //滑块
			 rcc =d_thunder.d_area.getRect(rc);
			 rcc.toWindowRect(rctest);
			 if(!::PtInRect(&rctest,pt))
			 {
				 lr = 0;
				return true;
			 }
			 d_lbuttondown = TRUE;
			 lr = 0;
			 return true;
		}
		if(uMsg==WM_LBUTTONUP)
		{
			d_lbuttondown = false;
			int xPos = (int)(short)LOWORD(lp);
			int yPos =(int)(short)HIWORD(lp);
			 float pross =0;
			 rcc=rc;
			 SOARMSG leeMsg;
			 if(d_bVertbar)
			{
				pross =(float)(yPos-rcc.d_top)/rcc.Height()*(d_max-d_min+1);
				leeMsg.message =SOAR_VSCROLL;
			}
			else
			{
				pross =(float)(xPos-rcc.d_left)/rcc.Width()*(d_max-d_min+1);
				leeMsg.message =SOAR_HSCROLL;
			}
			 if(d_lbuttondown)
			{
				int d_prePos=d_curPos;
				setPos(pross);
					  // 向父窗口发送WM_POSCHANGING
				
				//leeMsg.message =SOAR_SLIDERCHANGED;
				leeMsg.mouseEvent =SOAR_LCLICK_UP;
				leeMsg.targetWnd =d_OwnerWnd;
				leeMsg.sourceWnd=this;
				//leeMsg.wParam =d_prePos;
				//leeMsg.lParam =pross;
				leeMsg.wParam =1;
				leeMsg.lParam =lp;
				leeMsg.Data=NULL;
				leeMsg.msgSourceTag=SOAR_MSG_ORIG;
				CSoarRoot::getSingletonPtr()->addOfflineMsg(leeMsg);
			 }
			 lr = 0;
			 return true;
		}
		if (uMsg==WM_MOUSEMOVE )
		{
			int xPos = (int)(short)LOWORD(lp);
			int yPos =(int)(short)HIWORD(lp);
			if(d_lbuttondown){
				 rcc=rc;
				  float pross =0;
				  SOARMSG leeMsg;
				 if(d_bVertbar)
				{
					pross =(float)(yPos-rcc.d_top)/rcc.Height()*(d_max-d_min+1);
					leeMsg.message =SOAR_VSCROLL;
				}
				else
				{
					pross =(float)(xPos-rcc.d_left)/rcc.Width()*(d_max-d_min+1);
					leeMsg.message =SOAR_HSCROLL;
				}
				int d_prePos=d_curPos;
				setPos(pross);
						// 向父窗口发送WM_POSCHANGING
				
				//leeMsg.message =SOAR_SLIDERCHANGED;
				leeMsg.mouseEvent =SOAR_LCLICK_UP;
				leeMsg.targetWnd =d_OwnerWnd;
				leeMsg.sourceWnd=this;
				//leeMsg.wParam =d_prePos;
				//leeMsg.lParam =pross;
				leeMsg.wParam =-1;
				leeMsg.lParam =lp;
				leeMsg.Data=NULL;
				leeMsg.msgSourceTag=SOAR_MSG_ORIG;
				//CSoarRoot::getSingletonPtr()->addOfflineMsg(leeMsg);
			}
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
	if (uMsg==WM_LBUTTONDBLCLK  )
	{
		lr = 0;
		return true;
	}
	return false;//留系统底层处理
}
void CSoarSystemScrollSegment::DrawSelf(ILeeDrawInterface *DrawFuns) 
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
	  rcc =d_thunder.d_area.getRect(rc);
	 d_DrawEng->DrawWidgetlook(d_thunder.d_wndlook,d_thunder.d_wndlookState,
		 rcc,d_thunder.d_wndText,-1,-1,&rcparent,true);
	
}
bool CSoarSystemScrollSegment::AddItem(const SoarSegmentItem& item)
{
	//找一个合适的ID
	d_items.push_back(item);
	return true;
}
bool CSoarSystemScrollSegment::removeItem(const DWORD dId)
{
	SEGMENTITEMS::iterator it= d_items.begin();
	while(it != d_items.end())
	{
		if( it->d_nId == dId)
		{
			d_items.erase(it);
			return true;
		}
		++it;
	}
	return false ;
}
SoarSegmentItem CSoarSystemScrollSegment::getItem(const DWORD dId)
{
	SEGMENTITEMS::iterator it= d_items.begin();
	while(it != d_items.end())
	{
		if( it->d_nId == dId)
		{
			
			return (*it);
		}
		++it;
	}
	SoarSegmentItem itempty;
	return itempty ;
}
int CSoarSystemScrollSegment::getItemCount(void)
{
	return d_items.size();
}
