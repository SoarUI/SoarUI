#include "soarTitleBar.h"
#include "soarRoot.h"

CSoarTitleBar::CSoarTitleBar(ISoarWnd *parent,ISoarEngine* Eng):d_DrawEng(Eng),d_OwnerWnd(parent),d_parent(NULL),d_ReceiverWnd(NULL)
{
	d_wndlook ="TitleBar";
	d_wndtype=LWNDT_SYS_TITLEBAR;
	d_wndlookState="Normal";//皮肤状态
	d_lbuttondown = FALSE;
	d_barAlign = SOARALIGN_TOP;
	d_wndText =_T("Soar auto your life");
	d_wndState=LWNDST_SHOW;
}

CSoarTitleBar::~CSoarTitleBar(void)
{
}
LRESULT CSoarTitleBar::HandleEvent ( UINT uMsg ,WPARAM wParam ,LPARAM lParam) 
{
	//优先检测小控件
	//子窗口绘制
	RECT rc =getBarRect();
	POINT pt;
		::GetCursorPos(&pt);
		::ScreenToClient(d_OwnerWnd->getRootWnd(),&pt);
		RectF rcc;
	SEGMENTITEMS::iterator it =d_items.begin();
	 while (it != d_items.end())
	 {
		 rcc =((*it).d_area.getRect(rc));
		 RECT rctest;
		 rcc.toWindowRect(rctest);
		 if(::PtInRect(&rctest,pt))
		 {
			 MSG msg={NULL,uMsg,wParam,lParam};
			SOARMSG leeMsg;
			leeMsg.message =SOAR_COMMAND;
			leeMsg.mouseEvent =CSoarRoot::getSingletonPtr()->translateMouseMessage(msg,false);
			leeMsg.sourceWnd =d_OwnerWnd;
			leeMsg.routeWnd=d_OwnerWnd;
			leeMsg.targetWnd =d_OwnerWnd;
			leeMsg.wParam =MAKEWPARAM((*it).d_nId,BN_CLICKED);
			leeMsg.lParam =0;
			CSoarRoot::getSingletonPtr()->addOfflineMsg(leeMsg);
			//PostMessage(d_OwnerWnd->getRootWnd(),WM_COMMAND,MAKEWPARAM((*it).d_nId,BN_CLICKED),0);
			 return CSoarRoot::getSingletonPtr()->SoarDefWndProc(uMsg,wParam,lParam);//留系统底层处理;;
		 }
		 ++it;
	 }
	if (d_OwnerWnd->getType()==LWNDT_SYS_SHEET)//与窗口绑定在一起的，可以操控窗口API，LWNDT_SYS_UI_SHEET:一个移动的层，不固定/固定在原点
	{
		return HandSheetEvent(uMsg,wParam,lParam);
	}
	return HandSimpleEvent(uMsg,wParam,lParam);
}
LRESULT CSoarTitleBar::HandSheetEvent(UINT uMsg ,WPARAM wParam ,LPARAM lParam)
{
	static int delX =0;
	static int delY =0;
	static RECT rcupdate;
	if (uMsg == WM_LBUTTONDOWN)
	{
		SetCapture(d_OwnerWnd->getRootWnd());
		d_lbuttondown = TRUE;
		//
		int xPos = (int)(short)LOWORD(lParam); 
		int yPos =(int)(short)HIWORD(lParam);
		RECT rc =getBarRect();//取得相对于本窗口的偏移,非系统窗口
		delX =xPos-rc.left;
		delY =yPos-rc.top;
	}
	if (uMsg == WM_LBUTTONUP)
	{
		d_lbuttondown = FALSE;
		ReleaseCapture();
	}
	if(uMsg == WM_LBUTTONDBLCLK)
	{
		 MSG msg={NULL,uMsg,wParam,lParam};
			SOARMSG leeMsg;
			leeMsg.message =SOAR_COMMAND;
			leeMsg.mouseEvent =CSoarRoot::getSingletonPtr()->translateMouseMessage(msg,false);
			leeMsg.sourceWnd =d_OwnerWnd;
			leeMsg.routeWnd=d_OwnerWnd;
			leeMsg.targetWnd =d_OwnerWnd;
			leeMsg.wParam =MAKEWPARAM(-1,BN_CLICKED);
			leeMsg.lParam =0;
			CSoarRoot::getSingletonPtr()->addOfflineMsg(leeMsg);
			return CSoarRoot::getSingletonPtr()->SoarDefWndProc(uMsg,wParam,lParam);//留系统底层处理;;
	}
	if (uMsg == WM_MOUSEMOVE)
	{
		POINT pt;
		::GetCursorPos(&pt);
		::ScreenToClient(d_OwnerWnd->getRootWnd(),&pt);


		if (d_lbuttondown)
		{
			int xPos = (int)(short)LOWORD(lParam); 
			int yPos =(int)(short)HIWORD(lParam);
			
			RECT rc ;
			::GetWindowRect( d_OwnerWnd->getRootWnd() ,&rc);
			int W =rc.right-rc.left;
			int H= rc.bottom-rc.top;
			int deltaX =xPos-delX ;//求出相对于父窗口的偏移
			int deltaY =yPos-delY;
			pt.x=deltaX;
			pt.y=deltaY;
			RECT rcWnd ;
			 //::GetWindowRect( d_OwnerWnd->getRootWnd(),&rcWnd);
			//偏移+原始屏幕坐标
			pt.x +=rc.left;
			pt.y +=rc.top;
			MoveWindow(d_OwnerWnd->getRootWnd() ,pt.x,pt.y,W,H,TRUE);

		}

	}
	return CSoarRoot::getSingletonPtr()->SoarDefWndProc(uMsg,wParam,lParam);//留系统底层处理;
}
LRESULT CSoarTitleBar::HandSimpleEvent(UINT uMsg ,WPARAM wParam ,LPARAM lParam)
{
	/*
	我们d_area存放的数据是相对于它的父窗口的，最后的结果是d_area的数据整体偏移父窗口的left，top，渲染的时候计算，我们有时使用SetWindowPos的时候，直接把与（0，0）坐标的偏移放
	进去，最后发现位置不对，那就 要查看下是否忘记减去父窗口的偏移2021-12-12
	*/
	static int posX =0;
	static int posY =0;
	static int delY_this_offset_parent_Top =0;
	static int delY_this_offset_parent_Bottom =0;
	static int delX_this_offset_parent =0;
	static int delX_this_offset_parent_Right =0;
	static RECT rcupdate;
	if (uMsg == WM_LBUTTONDOWN)
	{
		d_lbuttondown = TRUE;
		int xPos = (int)(short)LOWORD(lParam);
		int yPos =(int)(short)HIWORD(lParam);
		RECT rc =d_OwnerWnd->getWindowRect();//取得相对于父窗口的偏移
		delX_this_offset_parent =xPos-rc.left;//求出按下时,鼠标X离窗口左上角的偏移，以窗口为参照物
		delY_this_offset_parent_Top =yPos-rc.top;//求出按下时,鼠标X离窗口左上角的偏移
		RECT rcBar =getBarRect();
		delY_this_offset_parent_Bottom =(rcBar.bottom-rcBar.top)-delY_this_offset_parent_Top;
		delX_this_offset_parent_Right=rc.right-xPos;
		posX =xPos;//记录鼠标位置
		posY =yPos;
		//设置父窗口为激活窗口
		CSoarRoot::getSingletonPtr()->setFocusWnd(d_OwnerWnd);
	}
	if (uMsg == WM_LBUTTONUP)
	{
		d_lbuttondown = FALSE;
	}
	if (uMsg == WM_MOUSEMOVE)
	{
		

		if (d_lbuttondown)
		{
			/*
			以POwner的Parent作为参考系,在此过程中，parent相对不变
			*/
			int xPos = (int)(short)LOWORD(lParam); 
			int yPos =(int)(short)HIWORD(lParam);
			
			ISoarWnd* pParentWnd =d_OwnerWnd->GetParent();
			RECT rcpParent;
			if(pParentWnd){
				rcpParent =pParentWnd->getClientRect();
			}
			else{
				::GetClientRect(d_OwnerWnd->getRootWnd(),&rcpParent);
			}
			RECT rc =d_OwnerWnd->getWindowRect();
			//限制鼠标只能在父窗口的客户区
			//通过鼠标，可以获得移动后得矩形左边X与Y delX+？=pt.x
			int foffsetX =xPos-posX;
			int foffsetY =yPos-posY;
			int left =xPos-delX_this_offset_parent-rcpParent.left;//求出相对于父窗口的父窗口的偏移
			int top =yPos-delY_this_offset_parent_Top-rcpParent.top;
			//视野控制
			/*if(left<= rcpParent.left)
				left=0;*/
			if (left>=rcpParent.right)
			{
				left =rcpParent.right/*-delX_this_offset_parent*/-rcpParent.left;
			}
			RECT rcBar =getBarRect();
			int rcBarH =rcBar.bottom-rcBar.top;
			//Y方向有
			/*if(top<-delY_this_offset_parent_Top)
				top=0;*/
			if (top>=rcpParent.bottom-rcBarH)
			{
				top =rcpParent.bottom-rcBarH-rcpParent.top;
			}
			//顶层情况
			if(left<0)
				left=0;
			if(top<0)
				top=0;
			d_OwnerWnd->setWindowPos(left,top);//计算OWner与父窗口的相对量
		}
	}
	
	return CSoarRoot::getSingletonPtr()->SoarDefWndProc(uMsg,wParam,lParam);//留系统底层处理;
}
void CSoarTitleBar::DrawSelf(ILeeDrawInterface *DrawFuns) 
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
	SEGMENTITEMS::iterator it =d_items.begin();
	 while (it != d_items.end())
	 {
		 rcc =((*it).d_area.getRect(rc));
		 d_DrawEng->DrawWidgetlook((*it).d_wndlook,(*it).d_wndlookState,rcc,(*it).d_wndText,-1,-1,&rcparent,true);
		 ++it;
	 }
	 
}

RECT CSoarTitleBar::getBarRect(void) 
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
RECT CSoarTitleBar::getBarCliperRect(void){
	RECT rcCur;
	if(!d_parent)
	{
		rcCur =getBarRect();
	}
	else{
		rcCur =d_parent->getBarRect();
	}
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
void CSoarTitleBar::setArea(ComponentArea& area)
{
	d_area =area;
}
void CSoarTitleBar::setArea(RECT& rc)
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
}
ComponentArea CSoarTitleBar::getArea()
{
	return d_area;
}
BOOL CSoarTitleBar::BarHitTest(void)
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
LWNDT CSoarTitleBar::getType(void) 
{
	return d_wndtype;
}
SOARBARALIGN CSoarTitleBar::getBarAlign(void)
{
	return d_barAlign;
}
void CSoarTitleBar::setTitle(CLeeString name)
{
	d_wndText = name;
}
//////////////////////////
bool CSoarTitleBar::AddItem(const SoarSegmentItem& item)
{
	//找一个合适的ID
	d_items.push_back(item);
	return true;
}
bool CSoarTitleBar::removeItem(const DWORD dId)
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
SoarSegmentItem CSoarTitleBar::getItem(const DWORD dId)
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
int CSoarTitleBar::getItemCount(void)
{
	return d_items.size();
}
void CSoarTitleBar::setState(LWNDST state) 
 {
	 if (d_wndState != state)
	 {
		d_wndState =state;
	 }
 }
 LWNDST CSoarTitleBar::getState(void)
 {
	 return d_wndState;
 }