#include "SoarItemSegment.h"
#include "SoarRoot.h"
#include "../SoarHeader/leeLog.h"

CSoarItemSegment::CSoarItemSegment(HWND root,ISoarEngine* Eng):d_DrawEng(Eng),
d_height(30),
d_width(60),
d_ID(0),
d_iIndex(0),
d_checkState(false)
{
	d_wndtype =LWNDT::LWNDT_ITEM_SEGMENT;
	d_wndlook ="Static";
	d_wndlookState="Normal";
}

CSoarItemSegment::~CSoarItemSegment(void)
{
}
DWORD CSoarItemSegment::getIndex(void)
{
	return d_iIndex;
}

DWORD CSoarItemSegment::getID(void) 
{
	return d_ID;
}
LPCTSTR CSoarItemSegment::getTitle(void)
{
	return d_string.c_str();
}
LPVOID CSoarItemSegment::getData(void) 
{
	return d_data;
}
int CSoarItemSegment::getHeight(void) 
{
	RECT rc =getBarRect();
	int h =rc.bottom-rc.top;
	return h;
}
int CSoarItemSegment::getWidght(void)
{
	RECT rc =getBarRect();
	int w =rc.right-rc.left;
	return w;
}
void CSoarItemSegment::setIndex(DWORD dwIndex) 
{
	d_iIndex = dwIndex ;
}

void CSoarItemSegment::setID(DWORD dwId) 
{
	d_ID =dwId ;
}
void CSoarItemSegment::setTitle(const CLeeString& str)
{
	d_string = str ;
}
void CSoarItemSegment::setData(LPVOID data) 
{
	d_data = data ;
}
void CSoarItemSegment::setHeight(int h) 
{
	d_height =h ;
}
void CSoarItemSegment::setWidght(int w) 
{
	d_width = w ;
}
bool CSoarItemSegment::getCheck(void)
{
	return d_checkState ;
}
void CSoarItemSegment::setcheck(bool bcheck)
{
	if (d_checkState != bcheck)
	{
		d_checkState =bcheck;
	}
}
RECT CSoarItemSegment::getBarRect(void) 
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
RECT CSoarItemSegment::getBarCliperRect(void){
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
void CSoarItemSegment::setArea(ComponentArea& area)
{
	d_area =area;
}
void CSoarItemSegment::setArea(RECT& rc)
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
ComponentArea CSoarItemSegment::getArea()
{
	return d_area;
}
BOOL CSoarItemSegment::BarHitTest(void)
{

	POINT pt;
	::GetCursorPos(&pt);
	::ScreenToClient( d_OwnerWnd->getRootWnd() ,&pt);
	RECT rc =getBarRect();
	if(::PtInRect(&rc,pt))
		return true;
	return false;
}
LWNDT CSoarItemSegment::getType(void) 
{
	return d_wndtype;
}
SOARBARALIGN CSoarItemSegment::getBarAlign(void)
{
	return d_barAlign;
}
//////////////////////////
int CSoarItemSegment::AddItem(ISoarColumnItemBase* item)
{
	//找一个合适的ID
	ColumnSegmentMap::iterator it= d_colItems.begin();
	int ncol =d_colItems.size();
	d_colItems[ncol]=item;
	return ncol;
}
bool CSoarItemSegment::removeItem(int col)
{
	ColumnSegmentMap::iterator it= d_colItems.find(col);
	if(it != d_colItems.end())
	{
		delete it->second;
		d_colItems.erase(it);
		return true;
	}
	return false ;
}
ISoarColumnItemBase* CSoarItemSegment::getItem(int ncol)
{
	ColumnSegmentMap::iterator it= d_colItems.find(ncol);
	if(it != d_colItems.end())
	{
		return it->second;
	}
	return NULL ;
}
void CSoarItemSegment::destroyAllItems(){
	ColumnSegmentMap::iterator it= d_colItems.begin();
	while(it != d_colItems.end())
	{
		delete it->second;
		d_colItems.erase(it);
		it =d_colItems.begin();;
	}
}
int CSoarItemSegment::getItemCount(void)
{
	return d_colItems.size();
}
void CSoarItemSegment::setState(LWNDST state) 
 {
	 if (d_wndState != state)
	 {
		d_wndState =state;
		ColumnSegmentMap::iterator it =d_colItems.begin();
	 while (it != d_colItems.end())
	 {
		 ISoarColumnItemBase* pTem =it->second;
		 if(pTem){
			 pTem->setState(state);
		 }
		 ++it;
	 }
	 }
 }
 LWNDST CSoarItemSegment::getState(void)
 {
	 return d_wndState;
 }
void CSoarItemSegment::DrawSelf(ILeeDrawInterface *DrawFuns)
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
	RectF rcparent =getBarCliperRect();
	//主窗口绘制
	if (d_DrawEng->hasWidgetlook(d_wndlook))
	{
		d_DrawEng->DrawWidgetlookFrame(d_wndlook,d_wndlookState,rcc,&rcparent,true);
		//d_DrawEng->DrawWidgetlook(d_wndlook,d_wndlookState,rcc,d_string,&rcparent,true);
	}
	//子窗口绘制
	ColumnSegmentMap::iterator it =d_colItems.begin();
	 while (it != d_colItems.end())
	 {
		 ISoarColumnItemBase* pTem =it->second;
		 if(pTem){
			 pTem->DrawSelf(DrawFuns);
		 }
		 ++it;
	 }
}
BOOL CSoarItemSegment::HandleEvent ( UINT uMsg ,WPARAM wParam ,LPARAM lParam, LRESULT& lr)
{
	if (d_OwnerWnd->getState()<LWNDST_SHOW)
	{
		return false;
	} 
	if(d_wndState<LWNDST_SHOW)
	{
		return false;
	}
	//优先检测小控件
	//子窗口绘制
	RECT rc =getBarRect();
	POINT pt;
		::GetCursorPos(&pt);
		::ScreenToClient(d_OwnerWnd->getRootWnd(),&pt);
		RectF rcc;
	ColumnSegmentMap::iterator it=d_colItems.begin();
	 while (it != d_colItems.end())
	 {
		 ISoarColumnItemBase* pTem =it->second;
		 if(pTem && pTem->HandleEvent(uMsg,wParam,lParam,lr)){
			return true;
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
		leeMsg.targetWnd =d_ReceiverWnd?d_ReceiverWnd:d_OwnerWnd;
		leeMsg.sourceWnd=this;
		leeMsg.wParam =d_iIndex;
		leeMsg.lParam =d_ID;
		leeMsg.Data=NULL;
		leeMsg.msgSourceTag=SOAR_MSG_ORIG;
		CSoarRoot::getSingletonPtr()->addOfflineMsg(leeMsg);
		lr = 0;
		return true;//已经处理
	}
	if (uMsg == WM_RBUTTONUP)
	{
		SOARMSG leeMsg;
		leeMsg.message =SOAR_COMMAND;
		leeMsg.mouseEvent =SOAR_RCLICK_UP;
		leeMsg.targetWnd =d_ReceiverWnd?d_ReceiverWnd:d_OwnerWnd;
		leeMsg.sourceWnd=this;
		leeMsg.wParam =d_iIndex;
		leeMsg.lParam =d_ID;
		leeMsg.Data=NULL;
		leeMsg.msgSourceTag=SOAR_MSG_ORIG;
		CSoarRoot::getSingletonPtr()->addOfflineMsg(leeMsg);
		lr = 0;
		return true;//已经处理
	}
	if (uMsg == WM_LBUTTONDBLCLK)
	{
		SOARMSG leeMsg;
		leeMsg.message =SOAR_COMMAND;
		leeMsg.mouseEvent =SOAR_LDBCLICK;
		leeMsg.targetWnd =d_ReceiverWnd?d_ReceiverWnd:d_OwnerWnd;
		leeMsg.sourceWnd=this;
		leeMsg.wParam =d_iIndex;
		leeMsg.lParam =d_ID;
		leeMsg.Data=NULL;
		leeMsg.msgSourceTag=SOAR_MSG_ORIG;
		CSoarRoot::getSingletonPtr()->addOfflineMsg(leeMsg);
		lr = 0;
		return true;//已经处理
	}
	return false;
}