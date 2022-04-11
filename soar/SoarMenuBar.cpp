#include "SoarMenuBar.h"
#include "SoarRoot.h"

CSoarMenuBar::CSoarMenuBar(ISoarWnd *parent,ISoarEngine* Eng):
d_OwnerWnd(parent),d_DrawEng(Eng), d_iIndex(0),d_nItemHeight(18),d_nItemWidth(50),d_parent(NULL),d_ReceiverWnd(NULL)
{
	d_wndlook ="MenuBar";
	d_wndtype=LWNDT_SYS_MENUBAR;
	d_wndlookState="Normal";//皮肤状态
	d_barAlign = SOARALIGN_TOP;
	d_wndText ="";
	 d_wndState=LWNDST_SHOW;
}

CSoarMenuBar::~CSoarMenuBar(void)
{
	d_Items.clear();
}

bool CSoarMenuBar::AddItem(const SoarSegmentItem& item)
{
	//找一个合适的ID
	d_Items.push_back(item);
	return true;
}
int CSoarMenuBar::getItemCount(void )
{
	return d_Items.size();
}
bool CSoarMenuBar::AppendMenu(__in UINT nID,__in void* uData,__in_opt LPCSTR lpNewItem)
{
	SEGMENTITEMS::iterator it= d_Items.begin();
		RECT rt ={0,0,0,0};
		//列表list
		AbsoluteDim listy(0);
		Dimension listY(listy,DT_X_POSITION);
		AbsoluteDim listx(d_nItemWidth*d_iIndex);
		Dimension listX(listx,DT_Y_POSITION);
		AbsoluteDim listh(d_nItemHeight);
		Dimension listH(listh,DT_HEIGHT);
		UDim dminW(1.0,0);
		UnifiedDim listw(dminW,DT_WIDTH);
		Dimension listW(listw,DT_WIDTH);
		SoarSegmentItem menuItem;
		menuItem.d_wndlook="MenuBarItem";
		menuItem.d_wndlookState ="Normal";//皮肤状态
		menuItem.d_area.d_left= listX;
		menuItem.d_area.d_top =listY;
		menuItem.d_area.d_right_or_width=listW;
		menuItem.d_area.d_bottom_or_height=listH;
		menuItem.d_nId =nID;
		menuItem.d_iIndex =d_iIndex++;
		menuItem.d_pNextWnd = NULL;
		menuItem.d_wndText = lpNewItem;
		menuItem.udata = uData ;
		d_Items.push_back(menuItem);
		return true;
}
bool CSoarMenuBar::ModifyMenuString(__in UINT nID,__in_opt LPCSTR lpNewItem)
{
	SEGMENTITEMS::iterator it= d_Items.begin();
	while (it != d_Items.end())
	{
		if((*it).d_nId ==nID )
		{
			(*it).d_wndText = lpNewItem;
		return true;
		}
		++it;
	}
	return false ;
}
bool CSoarMenuBar::ModifyMenuData(__in UINT nID,__in void* uData)
{
	SEGMENTITEMS::iterator it= d_Items.begin();
	while (it != d_Items.end())
	{
		if((*it).d_nId ==nID )
		{
			(*it).udata = uData;
		return true;
		}
		++it;
	}
	return false ;
}
bool CSoarMenuBar::CheckItem(__in UINT uIDCheckItem,__in UINT uCheck)
{
	SEGMENTITEMS::iterator it= d_Items.begin();
	while (it != d_Items.end())
	{
		if((*it).d_nId ==uIDCheckItem )
		{
			(*it).d_wndlookState ="Normal";
		return true;
		}
		++it;
	}
	return false ;
}
bool CSoarMenuBar::EnableItem(__in UINT uIDEnableItem,__in LWNDST uEnable)
{
	SEGMENTITEMS::iterator it= d_Items.begin();
	while (it != d_Items.end())
	{
		if((*it).d_nId ==uIDEnableItem )
		{
			(*it).d_wndlookState ="Normal";
		return true;
		}
		++it;
	}
	return false ;
}

SoarSegmentItem CSoarMenuBar::getItem(const DWORD dId)
{
	SEGMENTITEMS::iterator it= d_Items.begin();
	while(it != d_Items.end())
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
//////////////////////////////////////////////////////////////////////////
bool CSoarMenuBar::removeItem( DWORD uID)
{
	return false;
}
//////////////////////////////////////////////////////////////////////////
void CSoarMenuBar::DrawSelf(ILeeDrawInterface *DrawFuns) 
{
	if ((d_OwnerWnd ==NULL || d_OwnerWnd->getState()<= LWNDST_HIDE) )
	{
		return;
	}
	RECT rc =getBarRect();
	RectF rcc=rc;
	RectF rcparent =getBarCliperRect();
	//主窗口绘制
	if (d_DrawEng->hasWidgetlook(d_wndlook))
	{
		d_DrawEng->DrawWidgetlookFrame("DefaultWindowFrame",d_wndlookState,rcc,&rcparent,true);
		d_DrawEng->DrawWidgetlook(d_wndlook,d_wndlookState,rcc,"",-1,-1,&rcparent,true);
	}
	//子窗口绘制
	SEGMENTITEMS::iterator it =d_Items.begin();
	 while (it != d_Items.end())
	 {
		 rcc =((*it).d_area.getRect(rc));
		 d_DrawEng->DrawWidgetlook((*it).d_wndlook,(*it).d_wndlookState,rcc,(*it).d_wndText,-1,-1,&rcc,true);
		 ++it;
	 }
}
RECT CSoarMenuBar::getBarRect(void) 
{
	RECT rcTmp;
	RECT rcParent;
	rcParent =d_OwnerWnd->getWindowRect();//获取父窗口包括非客户区
	RECT rc;
	RectF rcCur,rcfparent;
	rcfparent=rcParent;
	rcCur=d_area.getRect(rcfparent);
	rcCur.toWindowRect(rc);
	return rc;
}
RECT CSoarMenuBar::getBarCliperRect(void){
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
void CSoarMenuBar::setArea(ComponentArea& area)
{
	d_area =area;
}
void CSoarMenuBar::setArea(RECT& rc)
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
ComponentArea CSoarMenuBar::getArea()
{
	return d_area;
}
BOOL CSoarMenuBar::BarHitTest(void)
{
	POINT pt;
	::GetCursorPos(&pt);
	::ScreenToClient( d_OwnerWnd->getRootWnd() ,&pt);
	RECT rc =getBarRect();
	if(::PtInRect(&rc,pt))
		return true;
	
	return false;
}
LWNDT CSoarMenuBar::getType(void) 
{
	return d_wndtype;
}
SOARBARALIGN CSoarMenuBar::getBarAlign(void)
{
	return d_barAlign;
}
void CSoarMenuBar::setTitle(CLeeString name)
{
	d_wndText = name;
}
LRESULT CSoarMenuBar::HandleEvent ( UINT uMsg ,WPARAM wParam ,LPARAM lParam) 
{
	
	if (uMsg ==WM_LBUTTONDOWN)
	{
		int xPos = (int)(short)LOWORD(lParam); 
		int yPos =(int)(short)HIWORD(lParam);
		POINT pt;
		pt.x =xPos;
		pt.y = yPos;
		SEGMENTITEMS::iterator it= d_Items.begin();//
		while (it != d_Items.end())
		{
			ISoarMenuWnd* tmp =(ISoarMenuWnd*)(it->udata);
			RECT rcParent =getBarRect();
			RectF rtc =it->d_area.getRect(rcParent);
			RECT rc;
			rtc.toWindowRect(rc);
			if (tmp && ::PtInRect(&rc,pt))
			{
				//调整菜单窗口出现位置
				CSoarRoot::getSingletonPtr()->setFocusWnd(tmp);
				DWORD x =rc.left-rcParent.left;
				AbsoluteDim ax(x);
				Dimension xx(ax,DT_X_POSITION);
				ComponentArea area;
				tmp->getArea(area);
				area.d_left =xx;
				UDim dminW(1.0,0);
				tmp->setArea(area);
				tmp->setState(LWNDST_SHOW);
				break;
			}
			++it;
		}
	}
	return CSoarRoot::getSingletonPtr()->SoarDefWndProc(uMsg,wParam,lParam);//留系统底层处理
}
void CSoarMenuBar::setState(LWNDST state) 
 {
	 if (d_wndState != state)
	 {
		d_wndState =state;
	 }
 }
 LWNDST CSoarMenuBar::getState(void)
 {
	 return d_wndState;
 }