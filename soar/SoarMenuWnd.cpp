#include "SoarMenuWnd.h"
#include "SoarRoot.h"
#include "../SoarHeader/leeLog.h"

CLeeMenuWnd::CLeeMenuWnd(HWND root,ISoarEngine* Eng):CSoarWnd(root,Eng),
d_nItemHeight(24),d_iIndex(0),d_MsgOwnerWnd(NULL)
{
	d_wndlook="PopupMenu";
	d_wndtype =LWNDT_POPMENU;
	d_wndlookState="Normal";//皮肤状态
}

CLeeMenuWnd::~CLeeMenuWnd(void)
{
	d_Items.clear();
}
bool CLeeMenuWnd::AppendMenu(__in UINT nID,__in void* uData,__in_opt LPCSTR lpNewItem)
{
	SEGMENTITEMS::iterator it= d_Items.begin();
		RECT rt ={0,0,0,0};
		//列表list
		AbsoluteDim listx(0);
		Dimension listX(listx,DT_X_POSITION);
		AbsoluteDim listy(d_nItemHeight*d_iIndex);
		Dimension listY(listy,DT_Y_POSITION);
		AbsoluteDim listh(d_nItemHeight);
		Dimension listH(listh,DT_HEIGHT);
		UDim dminW(1.0,0);
		UnifiedDim listw(dminW,DT_WIDTH);
		Dimension listW(listw,DT_WIDTH);
		SoarSegmentItem menuItem;
		menuItem.d_wndlook="PopupMenuItem";
		menuItem.d_wndlookState ="Normal";//皮肤状态
		menuItem.d_area.d_left= listX;
		menuItem.d_area.d_top =listY;
		menuItem.d_area.d_right_or_width=listW;
		menuItem.d_area.d_bottom_or_height=listH;
		menuItem.d_nId =nID;
		menuItem.d_iIndex =d_iIndex++;
		menuItem.d_pNextWnd= NULL;
		menuItem.d_wndText = lpNewItem;
		menuItem.udata = uData ;
		d_Items.push_back(menuItem);
		return true;
}
DWORD CLeeMenuWnd::GetMenuItemCount(void )
{
	return d_Items.size();
}
bool CLeeMenuWnd::ModifyMenu(__in UINT nID,__in void* uData,__in_opt LPCSTR lpNewItem)
{
	SEGMENTITEMS::iterator it= d_Items.begin();
	while (it != d_Items.end())
	{
		if( it->d_nId == nID )
		{
			it->udata= uData;
			it->d_wndText=lpNewItem;
		return true;
		}
		++it;
	}
	return false ;
}
bool CLeeMenuWnd::CheckMenuItem(__in UINT uIDCheckItem,__in UINT uCheck)
{
	SEGMENTITEMS::iterator it= d_Items.begin();
	while (it != d_Items.end())
	{
		if( it->d_nId == uIDCheckItem )
		{
			//it->udata= uData;
			//it->d_wndText=lpNewItem;
		return true;
		}
		++it;
	}
	return false ;
}
bool CLeeMenuWnd::EnableMenuItem(__in UINT uIDEnableItem,__in LWNDST uEnable)
{
	SEGMENTITEMS::iterator it= d_Items.begin();
	while (it != d_Items.end())
	{
		if( it->d_nId == uIDEnableItem )
		{
			 if (uEnable == LWNDST_DISABLED)
			 {
				 d_wndlookState ="Disabled";
			 }
			 else
				it->d_wndlookState ="Normal";
		return true;
		}
		++it;
	}
	return false ;
}
ISoarMenuWnd * CLeeMenuWnd::GetSubMenu(__in int nPos) 
{
	SEGMENTITEMS::iterator it= d_Items.begin();
	while (it != d_Items.end())
	{
		if( it->d_iIndex == nPos )
		{
			return (ISoarMenuWnd*)(it->d_pNextWnd);
		}
		++it;
	}
	return NULL; 
}
DWORD CLeeMenuWnd::GetMenuItemID(__in int nPos) 
{
	SEGMENTITEMS::iterator it= d_Items.begin();
	while (it != d_Items.end())
	{
		if( it->d_iIndex == nPos )
		{
		return (it->d_nId);
		}
		++it;
	}
	return NULL; 
	return 0 ;
}
//////////////////////////////////////////////////////////////////////////
void  CLeeMenuWnd::RemoveMenu(__in UINT uID)
{
	;
}



//////////////////////////////////////////////////////////////////////////
 void CLeeMenuWnd::DrawSelf(ILeeDrawInterface *DrawFuns)
 {
	 RECT rc =getWindowRect();
	 RectF rcc=rc;
	 RectF rcparent =getCliperRect();
	 //主窗口绘制
	 if (d_DrawEng->hasWidgetlook(d_wndlook))
	 {
		 d_DrawEng->DrawWidgetlook(d_wndlook,d_wndlookState,rcc," ",-1,-1,&rcparent,false);
	 }
	 SEGMENTITEMS::iterator it =d_Items.begin();
	 while (it != d_Items.end())
	 {
		 rcc =((*it).d_area.getRect(rc));
		 d_DrawEng->DrawWidgetlook(it->d_wndlook,it->d_wndlookState,rcc,it->d_wndText,d_VertTextFormat,d_horzTextFormat,&rcparent,false);
		 ++it;
	 }
 }
 void CLeeMenuWnd::setState(LWNDST state) 
 {
	 if (d_wndState != state)
	 {
		 d_wndState =state;
		// if (state==LWNDST::LWNDST_HIDE ||
		//	 state==LWNDST::LWNDST_SHOW)
		// {
		//	 LeeMenuItemWndMap::iterator it =d_Items.begin();
		//	 while (it != d_Items.end())
		//	 {
		//		 //子窗口中是否有窗口被激活
		//		 (it->second)->setState(state);
		//		 ++it;
		//	 }
		// }

		// //激发state chang 事件
	 }

 }
 LRESULT CLeeMenuWnd::HandleEvent ( UINT uMsg ,WPARAM wParam ,LPARAM lParam) 
 {
	 if (uMsg == SOAR_LOSTFOCUS)
	 {
		 setState(LWNDST_HIDE);
	 }
	 if (uMsg == SOAR_SETFOCUS)
	 {
		 setState(LWNDST_SHOW);
	 }
	 if (uMsg ==WM_LBUTTONDOWN)
	{
		//ID:lparam  nPos:wparam
		int xPos = (int)(short)LOWORD(lParam); 
		int yPos =(int)(short)HIWORD(lParam);
		POINT pt;
		pt.x =xPos;
		pt.y = yPos;
		SEGMENTITEMS::iterator it= d_Items.begin();//
		while (it != d_Items.end())
		{
			//ISoarMenuWnd* tmp =(ISoarMenuWnd*)(it->phandleWnd);
			RECT rcParent =getWindowRect();
			RectF rtc =it->d_area.getRect(rcParent);
			RECT rc;
			rtc.toWindowRect(rc);
			if (::PtInRect(&rc,pt))
			{
				this->setState(LWNDST_HIDE);
				//向菜单响应窗口发送信息
				 MSG msg={NULL,uMsg,wParam,lParam};
				SOARMSG leeMsg;
				leeMsg.message =SOAR_MENUITEMSELECTED;
				leeMsg.mouseEvent =CSoarRoot::getSingletonPtr()->translateMouseMessage(msg,false);
				leeMsg.sourceWnd =this;
				leeMsg.routeWnd=this;
				leeMsg.targetWnd =d_MsgOwnerWnd;
				leeMsg.wParam =(*it).d_iIndex;
				leeMsg.lParam =(*it).d_nId;
				CSoarRoot::getSingletonPtr()->addOfflineMsg(leeMsg);
				CSoarRoot::getSingletonPtr()->setFocusWnd(d_MsgOwnerWnd);
			return CSoarRoot::getSingletonPtr()->SoarDefWndProc(uMsg,wParam,lParam);//留系统底层处理;;
			}
			++it;
		}
	}
	 return  CSoarRoot::getSingletonPtr()->SoarDefWndProc(uMsg,wParam,lParam);//留系统底层处理
 }