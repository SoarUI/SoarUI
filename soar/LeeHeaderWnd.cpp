#include "LeeHeaderWnd.h"
#include "SoarRoot.h"

CLeeHeaderWnd::CLeeHeaderWnd(ISoarWnd *parent,ISoarEngine* Eng,SOARBARALIGN soarAlign):CSoarNonClientSegmentBase(parent,Eng),
d_height(30),d_width(80),d_cntItem(0),d_curStartColIndex(0),d_barAlign(soarAlign),d_parent(NULL),d_ReceiverWnd(NULL)
{
	d_wndlook ="TitleBar";
	d_wndtype =LWNDT_SYS_HEADERWND;
	d_wndText ="";
	 d_wndState=LWNDST_SHOW;
}

CLeeHeaderWnd::~CLeeHeaderWnd(void)
{
	d_commItems.clear();
}
int CLeeHeaderWnd::getItemCount(void)
{
	return d_cntItem;
}
int CLeeHeaderWnd::addItem(int iColWidth,CLeeString str)
{
	RECT rt ={0,0,0,0};
	//列表list
	int rx =0;
	AbsoluteDim listx(rx);
	Dimension listX(listx,DT_X_POSITION);
	AbsoluteDim listy(0);
	Dimension listY(listy,DT_Y_POSITION);
	AbsoluteDim listh(d_height);
	Dimension listH(listh,DT_HEIGHT);
	AbsoluteDim listw(iColWidth);
	Dimension listW(listw,DT_WIDTH);
	SoarSegmentItem its;
	its.d_nId=d_cntItem;
	its.d_wndlook="MenuBarItem";
	its.d_wndlookState="Normal";
	its.d_wndText=str;
	its.udata = NULL;
	its.d_area.d_left= listX;
	its.d_area.d_top =listY;
	its.d_area.d_right_or_width=listW;
	its.d_area.d_bottom_or_height=listH;
	//p->setWidght(iColWidth);
	d_commItems[d_cntItem++]=its;
	return d_cntItem-1;
}
void CLeeHeaderWnd::setItemString(CLeeString str,int islot)
{
	LeeItemWndMap::iterator it =d_commItems.find(islot);
	if (it != d_commItems.end())
	{
		(it->second).d_wndText=str ;
	}
}
CLeeString CLeeHeaderWnd::getItemString(int iSlot)
{
	LeeItemWndMap::iterator it =d_commItems.find(iSlot);
	if (it != d_commItems.end())
	{
		 return (it->second).d_wndText;
	}
	return " " ;
}
int CLeeHeaderWnd::getHeight(void)
{
	return d_height;
}
int CLeeHeaderWnd::getItemWidth(int iCol)
{
	RECT rc =getBarRect();
	LeeItemWndMap::iterator it =d_commItems.find(iCol);
	if (it != d_commItems.end())
	{
		RectF rc1=(it->second).d_area.getRect(rc);
		return rc1.d_right-rc1.d_left;
	}
	return d_width;
}
SoarSegmentItem CLeeHeaderWnd::getItem(int iCol)
{
	LeeItemWndMap::iterator it =d_commItems.find(iCol);
	if (it != d_commItems.end())
	{
		return (it->second);
	}
	SoarSegmentItem its;
	return its;
}
int CLeeHeaderWnd::setHeight(int nHeight)
{
	AbsoluteDim adim(0.0);
	adim.setValue(nHeight);
	d_area.d_bottom_or_height.setBaseDimension(adim);
	d_area.d_bottom_or_height.setDimensionType(DT_HEIGHT);
	return 0;
}
void CLeeHeaderWnd::DrawSelf(ILeeDrawInterface *DrawFuns)
{
	RECT rc =getWindowRect();
	RectF rcc=rc;
	RectF rcparent =getBarCliperRect();
	if (d_DrawEng->hasWidgetlook(d_wndlook))
	{
	   // d_DrawEng->DrawWidgetlookImage("DefaultWindowFrame",d_wndlookState,rcc,&rcparent,true);
		//绘制文字
		d_DrawEng->DrawWidgetlook(d_wndlook,d_wndlookState,rcc,d_wndText,-1,-1,&rcparent,true);
	}
	//hua 
	//画项
	EnsureColVisible(d_curStartColIndex,d_commItems.size()-1);
	LeeItemWndMap::iterator it = d_commItems.find(d_curStartColIndex);
	while (it != d_commItems.end() )
	{
		//(it->second)->DrawSelf(DrawFuns);
		rcc =((it->second).d_area.getRect(rc));
		 d_DrawEng->DrawWidgetlook((it->second).d_wndlook,(it->second).d_wndlookState,rcc,(it->second).d_wndText,-1,-1,&rcparent,true);
		++it;
	}
}
void CLeeHeaderWnd::setState(LWNDST state) 
{
	if (d_wndState != state)
	{
	d_wndState =state;
	//	if (state==LWNDST::LWNDST_HIDE ||
	//		state==LWNDST::LWNDST_SHOW)
	//	{
	//		LeeItemWndMap::iterator it =d_commItems.begin();
	//		while (it != d_commItems.end())
	//		{
	//			//子窗口中是否有窗口被激活
	//			//(it->second)->setState(state);
	//			++it;
	//		}
	//	}

		//激发state chang 事件
	}

}
LWNDST CLeeHeaderWnd::getState(void){
	return d_wndState;
}
void CLeeHeaderWnd::updateStartCol(int iCol)
{
	if (iCol>=0 && iCol <d_commItems.size())
	{
		d_curStartColIndex =iCol;
	}
	
}
void CLeeHeaderWnd::EnsureColVisible(int iStartCol,int iEndCol)
{
	RECT rt ={0,0,0,0};
	int nStartCol =0;
	int nCountRow =iEndCol -iStartCol+1;
	//iStart之前的项隐藏
	LeeItemWndMap::iterator &ita=d_commItems.begin();

	//全部隐藏
	while(ita !=d_commItems.end() )
	{
		//(ita->second).d_wndlookState =LWNDST_HIDE;
		++ita;
	}
	static int nX = 0;
	static int nItemWidth =d_width;
	ita=d_commItems.find(nStartCol);
	nItemWidth=getItemWidth(iStartCol);
	 nX = 0;
	//第一项
	AbsoluteDim listx(0);
	Dimension listX(listx,DT_X_POSITION);
	AbsoluteDim listy(0);
	Dimension listY(listy,DT_Y_POSITION);
	AbsoluteDim listh(getHeight());
	Dimension listH(listh,DT_HEIGHT);
	AbsoluteDim listw(nItemWidth);
	Dimension listW(listw,DT_WIDTH);

	ita=d_commItems.find(iStartCol);
	
	
    //先固定第一项，从第二项开始循环
    if (ita !=d_commItems.end())
    {
		//(ita->second).d_wndlookState = LWNDST_SHOW;
		setItemPosition((ita->second),listX,listY,listW,listH);
    }
	nStartCol =1;//从第二项开始
	ita=d_commItems.find(iStartCol+1);
	//先累计宽度
	nX+=nItemWidth;
	
	while (ita !=d_commItems.end() && nCountRow>0)
	{
			//X,和对应的宽度
		    nItemWidth =getItemWidth(iStartCol+nStartCol);
			//nPrevWidth=getItemWidth(iStartCol+nStartCol);//之前的宽度,当nStarCol=0开始的
			listx.setValue(nX);
			listX.setBaseDimension(listx);
			listX.setDimensionType(DT_X_POSITION);
			listy.setValue(0);
			listY.setBaseDimension(listy);
			listY.setDimensionType(DT_Y_POSITION);
			listW.setDimensionType(DT_WIDTH);
			listw.setValue( nItemWidth );
			nX +=nItemWidth;
			listW.setBaseDimension(listw);
			//(ita->second).d_wndlookState = LWNDST_SHOW;
			setItemPosition((ita->second),listX,listY,listW,listH);
			++ita;
			nStartCol++;
			nCountRow--;

		}
}
void CLeeHeaderWnd::setItemPosition(SoarSegmentItem &lpWnd,Dimension& X,Dimension  &Y,Dimension& W,Dimension& H)
{
	ComponentArea rcArea;
	rcArea.d_left= X;
	rcArea.d_top =Y;
	rcArea.d_right_or_width=W;
	rcArea.d_bottom_or_height=H;
	lpWnd.d_area=rcArea;
}
RECT CLeeHeaderWnd::getBarRect(void) 
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
RECT CLeeHeaderWnd::getBarCliperRect(void){
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
void CLeeHeaderWnd::setArea(ComponentArea& area)
{
	d_area =area;
}
void CLeeHeaderWnd::setArea(RECT& rc)
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
ComponentArea CLeeHeaderWnd::getArea()
{
	return d_area;
}
BOOL CLeeHeaderWnd::BarHitTest(void)
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
LWNDT CLeeHeaderWnd::getType(void) 
{
	return d_wndtype;
}
SOARBARALIGN CLeeHeaderWnd::getBarAlign(void)
{
	return d_barAlign;
}
void CLeeHeaderWnd::setTitle(CLeeString name)
{
	d_wndText = name;
}
//////////////////////////
bool CLeeHeaderWnd::AddItem(const SoarSegmentItem& item)
{
	//找一个合适的ID
	//d_items.push_back(item);
	return true;
}
bool CLeeHeaderWnd::removeItem(const DWORD dId)
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
SoarSegmentItem CLeeHeaderWnd::getItem(const DWORD dId)
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
LRESULT CLeeHeaderWnd::HandleEvent ( UINT uMsg ,WPARAM wParam ,LPARAM lParam) 
{
	
	if (uMsg ==WM_LBUTTONDOWN)
	{
		int xPos = (int)(short)LOWORD(lParam); 
		int yPos =(int)(short)HIWORD(lParam);
		POINT pt;
		pt.x =xPos;
		pt.y = yPos;
		//SEGMENTITEMS::iterator it= d_Items.begin();//
		//while (it != d_Items.end())
		//{
		//	ISoarMenuWnd* tmp =(ISoarMenuWnd*)(it->udata);
		//	RECT rcParent =getBarRect();
		//	RectF rtc =it->d_area.getRect(rcParent);
		//	RECT rc;
		//	rtc.toWindowRect(rc);
		//	if (tmp && ::PtInRect(&rc,pt))
		//	{
		//		//调整菜单窗口出现位置
		//		CSoarRoot::getSingletonPtr()->setFocusWnd(tmp);
		//		DWORD x =rc.left;
		//		int y=0;
		//		AbsoluteDim ax(x);
		//		Dimension xx(ax,DT_X_POSITION);
		//		ComponentArea area;
		//		tmp->getArea(area);
		//		area.d_left =xx;
		//		UDim dminW(1.0,0);
		//		tmp->setArea(area);
		//		tmp->setState(LWNDST_SHOW);
		//	}
		//	++it;
		//}
	}
	return CSoarRoot::getSingletonPtr()->SoarDefWndProc(uMsg,wParam,lParam);//留系统底层处理
}