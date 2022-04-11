#include "SoarColumnItemSegment.h"
#include "../SoarRoot.h"
#include "../../SoarHeader/leeLog.h"

CSoarColumnItemSegment::CSoarColumnItemSegment(HWND root,ISoarEngine* Eng):d_DrawEng(Eng),
d_height(30),
d_width(60),
d_ID(0),
d_iIndex(0),
d_subindex(0),
d_checkState(false)
{
	d_wndtype =LWNDT::LWNDT_COLUMNITEM_SEGMENT;
	d_wndlook ="Static";
	d_wndlookState="Normal";
}

CSoarColumnItemSegment::~CSoarColumnItemSegment(void)
{
}
DWORD CSoarColumnItemSegment::getIndex(void)
{
	return d_iIndex;
}
DWORD CSoarColumnItemSegment::getsubIndex(void)
{
	return d_subindex;
}
DWORD CSoarColumnItemSegment::getID(void) 
{
	return d_ID;
}
CLeeString CSoarColumnItemSegment::getTitle(void) 
{
	return d_string;
}
LPVOID CSoarColumnItemSegment::getData(void) 
{
	return d_data;
}
int CSoarColumnItemSegment::getHeight(void) 
{
	RECT rc =getBarRect();
	int h =rc.bottom-rc.top;
	return h;
}
int CSoarColumnItemSegment::getWidght(void)
{
	RECT rc =getBarRect();
	int w =rc.right-rc.left;
	return w;
}
void CSoarColumnItemSegment::setIndex(DWORD dwIndex) 
{
	d_iIndex = dwIndex ;
}
void CSoarColumnItemSegment::setsubIndex(DWORD dwIndex) 
{
	d_subindex = dwIndex ;
}
void CSoarColumnItemSegment::setID(DWORD dwId) 
{
	d_ID =dwId ;
}
void CSoarColumnItemSegment::setTitle(CLeeString str) 
{
	d_string = str ;
}
void CSoarColumnItemSegment::setData(LPVOID data) 
{
	d_data = data ;
}
void CSoarColumnItemSegment::setHeight(int h) 
{
	d_height =h ;
}
void CSoarColumnItemSegment::setWidght(int w) 
{
	d_width = w ;
}
bool CSoarColumnItemSegment::getCheck(void)
{
	return d_checkState ;
}
void CSoarColumnItemSegment::setcheck(bool bcheck)
{
	if (d_checkState != bcheck)
	{
		d_checkState =bcheck;
	}
}
RECT CSoarColumnItemSegment::getBarRect(void) 
{
	RECT rcParent;
	if(!d_parent)
	{
		rcParent =d_OwnerWnd->getClientRect();
	}
	else{
		rcParent =d_parent->getBarRect();
	}
	RECT rcTmp;
	RECT rc;
	RectF rcCur;
	rcCur=d_area.getRect(rcParent);
	rcCur.toWindowRect(rc);
	return rc;
}
RECT CSoarColumnItemSegment::getBarCliperRect(void){
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
void CSoarColumnItemSegment::setArea(ComponentArea& area)
{
	d_area =area;
}
void CSoarColumnItemSegment::setArea(RECT& rc)
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
ComponentArea CSoarColumnItemSegment::getArea()
{
	return d_area;
}
BOOL CSoarColumnItemSegment::BarHitTest(void)
{

	POINT pt;
	::GetCursorPos(&pt);
	::ScreenToClient( d_OwnerWnd->getRootWnd() ,&pt);
	RECT rc =getBarRect();
	if(::PtInRect(&rc,pt))
		return true;
	return false;
}
LWNDT CSoarColumnItemSegment::getType(void) 
{
	return d_wndtype;
}
SOARBARALIGN CSoarColumnItemSegment::getBarAlign(void)
{
	return d_barAlign;
}
//////////////////////////
bool CSoarColumnItemSegment::AddItem(const SoarSegmentItem& item)
{
	//找一个合适的ID
	d_items.push_back(item);
	return true;
}
bool CSoarColumnItemSegment::removeItem(const DWORD dId)
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
SoarSegmentItem CSoarColumnItemSegment::getItem(const DWORD dId)
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
int CSoarColumnItemSegment::getItemCount(void)
{
	return d_items.size();
}
void CSoarColumnItemSegment::setState(LWNDST state) 
 {
	 if (d_wndState != state)
	 {
		d_wndState =state;
	 }
 }
 LWNDST CSoarColumnItemSegment::getState(void)
 {
	 return d_wndState;
 }
void CSoarColumnItemSegment::DrawSelf(ILeeDrawInterface *DrawFuns)
{
	if (d_OwnerWnd->getState()<LWNDST_SHOW)
	{
		return;
	}
	if(d_wndState<LWNDST_SHOW)
	{
		return;
	}
	RECT rc =getBarRect();
	RectF rcc=rc;
	RectF rcparent =d_parent->getBarCliperRect();
	//主窗口绘制
	if (d_DrawEng->hasWidgetlook(d_wndlook))
	{
		d_DrawEng->DrawWidgetlookFrame(d_wndlook,d_wndlookState,rcc,&rcparent,true);
		d_DrawEng->DrawWidgetlook(d_wndlook,d_wndlookState,rcc,d_string,-1,-1,&rcparent,true);
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
LRESULT CSoarColumnItemSegment::HandleEvent ( UINT uMsg ,WPARAM wParam ,LPARAM lParam) 
{
	if (d_OwnerWnd->getState()<LWNDST_SHOW)
	{
		return 0;
	}
	if(d_wndState<LWNDST_SHOW)
	{
		return 0;
	}
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
			leeMsg.message =SOAR_ITEMCOLUMNSELECTED;
			leeMsg.mouseEvent =CSoarRoot::getSingletonPtr()->translateMouseMessage(msg,false);
			leeMsg.sourceWnd =this;
			leeMsg.routeWnd=d_OwnerWnd;
			leeMsg.targetWnd =d_ReceiverWnd?d_ReceiverWnd:d_OwnerWnd;
			leeMsg.wParam =(*it).d_iIndex;
			leeMsg.lParam =(*it).d_subindex;
			leeMsg.Data =(LPVOID)(*it).d_nId;
			CSoarRoot::getSingletonPtr()->addOfflineMsg(leeMsg);
			//PostMessage(d_OwnerWnd->getRootWnd(),WM_COMMAND,MAKEWPARAM((*it).d_nId,BN_CLICKED),0);
			 return CSoarRoot::getSingletonPtr()->SoarDefWndProc(uMsg,wParam,lParam);//留系统底层处理;;
		 }
		 ++it;
	 }
	 //检测自己
	  rcc =rc;
		RECT rctest;
		rcc.toWindowRect(rctest);
		if(!::PtInRect(&rctest,pt))
		{
			return false;
		}
	if (uMsg == WM_LBUTTONUP)
	{
		SOARMSG leeMsg;
		leeMsg.message =SOAR_COMMAND;
		leeMsg.mouseEvent =SOAR_LCLICK_UP;
		leeMsg.sourceWnd =this;
		leeMsg.targetWnd =d_ReceiverWnd?d_ReceiverWnd:d_OwnerWnd;
		leeMsg.routeWnd=NULL;
		leeMsg.wParam =d_iIndex;
		leeMsg.lParam =d_ID;
		leeMsg.Data=NULL;
		leeMsg.msgSourceTag=SOAR_MSG_ORIG;
		CSoarRoot::getSingletonPtr()->addOfflineMsg(leeMsg);
		return true;//已经处理
	}
	if (uMsg == WM_RBUTTONUP)
	{
		SOARMSG leeMsg;
		leeMsg.message =SOAR_COMMAND;
		leeMsg.mouseEvent =SOAR_RCLICK_UP;
		leeMsg.sourceWnd =this;
		leeMsg.targetWnd =d_ReceiverWnd?d_ReceiverWnd:d_OwnerWnd;
		leeMsg.routeWnd=NULL;
		leeMsg.wParam =d_iIndex;
		leeMsg.lParam =d_subindex;
		leeMsg.Data=NULL;
		leeMsg.msgSourceTag=SOAR_MSG_ORIG;
		CSoarRoot::getSingletonPtr()->addOfflineMsg(leeMsg);
		return true;//已经处理
	}
	if (uMsg == WM_LBUTTONDBLCLK)
	{
		SOARMSG leeMsg;
		leeMsg.message =SOAR_COMMAND;
		leeMsg.mouseEvent =SOAR_LDBCLICK;
		leeMsg.sourceWnd =this;
		leeMsg.targetWnd =d_ReceiverWnd?d_ReceiverWnd:d_OwnerWnd;
		leeMsg.routeWnd=NULL;
		leeMsg.wParam =d_iIndex;
		leeMsg.lParam =d_ID;
		leeMsg.Data=NULL;
		leeMsg.msgSourceTag=SOAR_MSG_ORIG;
		CSoarRoot::getSingletonPtr()->addOfflineMsg(leeMsg);
		return true;//已经处理
	}
	return 0;
}