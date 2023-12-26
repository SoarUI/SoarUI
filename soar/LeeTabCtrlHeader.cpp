#include "LeeTabCtrlHeader.h"
#include "SoarRoot.h"

CLeeTabCtrlHeader::CLeeTabCtrlHeader(ISoarWnd *parent,ISoarEngine* Eng,SOARBARALIGN soarAlign):CSoarNonClientSegmentBase(parent,Eng),
d_tabIndex(0),d_tabHeight(20),d_tabwidth(60),d_cntInLine(5),d_lineIndex(0),d_curSel(0),d_barAlign(soarAlign),d_parent(NULL),d_ReceiverWnd(NULL)
{
	d_wndlook="TabCtrlPane";
	d_wndtype =LWNDT_SYS_TABCTRHEADER ;
	d_wndlookState ="Normal";
	d_wndText ="";
	 d_wndState=LWNDST_SHOW;
}

CLeeTabCtrlHeader::~CLeeTabCtrlHeader(void)
{
	d_TabButtons.clear();
}
bool CLeeTabCtrlHeader::addTab(DWORD islot,CLeeString szTab,LPVOID data)
{
	LeeItemWndMap::iterator it =d_TabButtons.find(islot);
	if (it != d_TabButtons.end())
	{
		(it->second).d_wndText= szTab;
		return true;
	}
	int d_lineIndex =islot%d_cntInLine;
	int yindex =islot/d_cntInLine;
	AbsoluteDim aDimy(yindex*d_tabHeight);
	AbsoluteDim aDimx(d_lineIndex*d_tabwidth);
	AbsoluteDim aDimH(d_tabHeight);
	AbsoluteDim aDimW(d_tabwidth);


	Dimension w(aDimW,DT_WIDTH);
	Dimension h(aDimH,DT_HEIGHT);
	Dimension y(aDimy,DT_Y_POSITION);
	Dimension x(aDimx,DT_X_POSITION);
	
	SoarSegmentItem its;
	its.d_nId=islot;
	its.d_iIndex =islot;
	its.d_wndlook="MenuBarItem";
	its.d_wndlookState="Normal";
	its.d_wndText=szTab;
	its.udata =data;
	its.d_area.d_left= x;
	its.d_area.d_top =y;
	its.d_area.d_right_or_width=w;
	its.d_area.d_bottom_or_height=h;
	d_TabButtons[islot]=its;
	return true;
}
bool CLeeTabCtrlHeader::linkTab(DWORD islot,ISoarWnd* dataWnd)
{
	LeeItemWndMap::iterator it =d_TabButtons.find(islot);
	if (it != d_TabButtons.end())
	{
		(it->second).udata =dataWnd;
		if (dataWnd)
		{
			dataWnd->setParent(d_OwnerWnd);
			RECT rc;
			rc=d_OwnerWnd->getClientRect();
			int h =rc.bottom-rc.top;
			int w =rc.right-rc.left;
			rc.left =0;
			rc.top =0;
			rc.bottom=h;
			rc.right =w;
			dataWnd->setArea(rc);
		}
		return true ;
	}
	return false ;
}
int CLeeTabCtrlHeader::setTabHeight(int h)
{
	int tmp =d_tabHeight;
	d_tabHeight = h;
	return tmp ;
}
int CLeeTabCtrlHeader::setTabWidth(int w)
{
	int tmp =d_tabwidth ;
	d_tabwidth =w;
	return tmp ;
}
int CLeeTabCtrlHeader::setButtonsInoneLine(int n)
{
	int tmp =d_cntInLine;
	d_cntInLine=n;
	return tmp ;
}
void CLeeTabCtrlHeader::DrawSelf(ILeeDrawInterface *DrawFuns)
{
	RECT rc =getWindowRect();
	RectF rcc=rc;
	RectF rcparent =getBarCliperRect();
	//主窗口绘制
	if (d_DrawEng->hasWidgetlook(d_wndlook))
	{
		//d_DrawEng->DrawWidgetlook("DefaultWindowFrame",d_wndlookState,rcc," ",&rcparent,true);
		d_DrawEng->DrawWidgetlook(d_wndlook,d_wndlookState,rcc,d_wndText,-1,-1,&rcparent,true);
	}
	//buttons draw 
	LeeItemWndMap::iterator it =d_TabButtons.begin();
	CSoarWnd * pWnd=NULL;
	while (it != d_TabButtons.end())
	{
		rcc =((it->second).d_area.getRect(rc));
		 d_DrawEng->DrawWidgetlook((it->second).d_wndlook,(it->second).d_wndlookState,
			 rcc,(it->second).d_wndText,-1,-1,&rcparent,true);
		 pWnd=(CSoarWnd*)((it->second).udata);
		 int cursel=(it->second).d_iIndex;
		/*if (pWnd &&d_curSel == cursel)
		{
			pWnd->DrawSelf(DrawFuns);
		}*/
		++it;
	}
}
BOOL CLeeTabCtrlHeader::HandleEvent( UINT msg,WPARAM wParam ,LPARAM lParam, LRESULT& lr)
{
	if (msg == WM_LBUTTONDOWN)
	{
		int xPos = (int)(short)LOWORD(lParam); 
		int yPos =(int)(short)HIWORD(lParam);
		POINT pt;
		pt.x =xPos;
		pt.y = yPos;
		int tmp =d_curSel;
		LeeItemWndMap::iterator it =d_TabButtons.begin();
		CSoarWnd * hWind =NULL;
		/*while (it != d_TabButtons.end() )
		{
			hWind =(CSoarWnd*)((it->second).udata);
			RECT rcParent =getBarRect();
			RectF rtc =it->second.d_area.getRect(rcParent);
			RECT rc;
			rtc.toWindowRect(rc);
			if (hWind && ::PtInRect(&rc,pt))
			{
				hWind->setState(LWNDST_HIDE);
			}
			++it;
		}*/
		//
		it =d_TabButtons.begin();
		while (it != d_TabButtons.end()  )
		{
			RECT rcParent =getBarRect();
			RectF rtc =it->second.d_area.getRect(rcParent);
			RECT rc;
			rtc.toWindowRect(rc);
			if ( ::PtInRect(&rc,pt))
			{
				hWind =(CSoarWnd*)((it->second).udata);

				if (hWind)
				{
					RECT rc;
					hWind->setParent(d_OwnerWnd);
					rc=d_OwnerWnd->getClientRect();
					int h =rc.bottom-rc.top;
					int w =rc.right-rc.left;
					rc.left =0;
					rc.top =0;
					rc.bottom=h;
					rc.right =w;
					hWind->setArea(rc);
					hWind->setState(LWNDST_SHOW);
					CSoarRoot::getSingletonPtr()->setFocusWnd(hWind);
				}
				d_curSel =(it->second).d_iIndex;
				break;
			}
				
			++it;
		}
		//
		if (d_curSel !=tmp )
		{
			it =d_TabButtons.find(tmp);
			if (it != d_TabButtons.end())
			{
				hWind =(CSoarWnd*)((it->second).udata);
				if (hWind)
				{
						hWind->setState(LWNDST_HIDE);
				}
			}
			//向父窗口的父窗口发送消息SOAR_SELCHANGED
			SOARMSG leeMsg;
			leeMsg.message =SOAR_SELCHANGED;
			leeMsg.wParam =d_curSel;
			leeMsg.lParam =tmp;
			leeMsg.sourceWnd =d_OwnerWnd?d_OwnerWnd:(ISoarObject*)this;
			leeMsg.targetWnd =d_OwnerWnd?d_OwnerWnd->GetParent():d_ReceiverWnd;
			leeMsg.routeWnd=0;
			leeMsg.mouseEvent =0;
			CSoarRoot::getSingletonPtr()->addOfflineMsg(leeMsg);
		}

	}

	lr= CSoarRoot::getSingletonPtr()->SoarDefWndProc(msg,wParam,lParam);//留系统底层处理
	return true;
}
int CLeeTabCtrlHeader::getCurSel(void)
{
	return d_curSel;
}
void CLeeTabCtrlHeader::setCurSel(int islot)
{
	LeeItemWndMap::iterator it =d_TabButtons.find(islot);
	if (it != d_TabButtons.end())
	{
		if (d_curSel != islot )
		{
			d_curSel = islot;
			LeeItemWndMap::iterator ita =d_TabButtons.begin();
			CSoarWnd * hWind =NULL;
			while (ita != d_TabButtons.end())
			{
				hWind =(CSoarWnd*)((ita->second).udata);
				if (hWind && d_curSel != (ita->second).d_iIndex)
				{
					hWind->setState(LWNDST_HIDE);
				}

				++ita;
			}
		}
	}

}
RECT CLeeTabCtrlHeader::getBarRect(void) 
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
RECT CLeeTabCtrlHeader::getBarCliperRect(void){
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
void CLeeTabCtrlHeader::setArea(ComponentArea& area)
{
	d_area =area;
}
void CLeeTabCtrlHeader::setArea(RECT& rc)
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
ComponentArea CLeeTabCtrlHeader::getArea()
{
	return d_area;
}
BOOL CLeeTabCtrlHeader::BarHitTest(void)
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
LWNDT CLeeTabCtrlHeader::getType(void) 
{
	return d_wndtype;
}
SOARBARALIGN CLeeTabCtrlHeader::getBarAlign(void)
{
	return d_barAlign;
}
void CLeeTabCtrlHeader::setTitle(const CLeeString& name)
{
	d_wndText = name;
}
//////////////////////////
bool CLeeTabCtrlHeader::AddItem(const SoarSegmentItem& item)
{
	//找一个合适的ID
	//d_items.push_back(item);
	return true;
}
bool CLeeTabCtrlHeader::removeItem(const DWORD dId)
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
SoarSegmentItem CLeeTabCtrlHeader::getItem(const DWORD dId)
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
int CLeeTabCtrlHeader::getItemCount(void)
{
	return d_TabButtons.size();
}
void CLeeTabCtrlHeader::setState(LWNDST state) 
 {
	 if (d_wndState != state)
	 {
		d_wndState =state;
	 }
 }
 LWNDST CLeeTabCtrlHeader::getState(void)
 {
	 return d_wndState;
 }