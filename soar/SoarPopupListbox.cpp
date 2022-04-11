#include "SoarPopupListbox.h"
#include "SoarStaticItemWnd.h"
#include "SoarRoot.h"
#include "SoarComboWnd.h"

CLeePopupListbox::CLeePopupListbox(HWND root,ISoarEngine* Eng):
CLeePopupBase(root,Eng),
d_iIndex(0),
d_curStartRowIndex(0),
d_nItemHeight(20),
d_selectedIndex(-1),
d_OwnerWnd(NULL)
{
	d_wndlook="PopupMenu";
	d_wndtype =LWNDT::LWNDT_POPLISTBOX;
	d_wndlookState="Normal";
}

CLeePopupListbox::~CLeePopupListbox(void)
{
	d_commItems.clear();
}
bool CLeePopupListbox::addItem(const CLeeString & str,int nID,LPVOID data)
{
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
	//先考虑从回收器中获取
	LeeGCItemWndMap::iterator itgc = d_gcitems.begin();
	ISoarItemWnd * llp =NULL;
	if (itgc != d_gcitems.end())
	{
		llp =itgc->second;
		ComponentArea area ;
		llp->getArea(area);
		area.d_right_or_width=listW;
		llp->setArea(area);
		d_gcitems.erase(itgc);
		setItemPosition(llp,listX,listY,listW,listH);
	}
	else
	{
		CLeeString itemname ;
		itemname.Format("LeeListbox__%p__%s__%d",this,"listBoxitem",d_iIndex);//非常大的风险：覆盖
		llp=(ISoarItemWnd *)CSoarRoot::getSingletonPtr()->createPopupWnd(itemname,listX,listY,listW,listH,LWNDT_STAICITEMWND,this,d_zPopupIndex+1);
	}
	llp->setDreamLook("ListBoxItem");
	llp->setString(str);
	llp->setIndex(d_iIndex);
	llp->setID(nID);
	llp->setData(data);
	llp->setOwnerWnd(d_OwnerWnd);
	d_commItems[d_iIndex++]=llp;
	this->setState(LWNDST_HIDE);
	d_curStartRowIndex==-1?d_curStartRowIndex=0:d_curStartRowIndex;
	return true;
}
bool CLeePopupListbox::delItem(int iIndex)
{
	LeeItemWndMap::iterator it =d_commItems.find(iIndex);
	if (it != d_commItems.end())
	{
		CSoarRoot::getSingletonPtr()->destroyWnd(it->second);
		d_commItems.erase(it);
	}
	return true;
}
int CLeePopupListbox::removeItem(int iRow)
{
	LeeItemWndMap::iterator it = d_commItems.find(iRow);
	if (it != d_commItems.end() )
	{
		ISoarItemWnd * llp =it->second;
		//原来内容移除
		d_commItems.erase(it);
		//回收至垃圾回收器
		llp->setState(LWNDST_HIDE);
		d_gcitems[llp] =llp;
		sortRow(iRow+1,iRow);
		//如果只有一项或者
		if(d_curStartRowIndex==iRow)
		{
			if (d_commItems.empty())
			{
				d_curStartRowIndex=-1;
			}
		}
	}

	return 0;
}
DWORD CLeePopupListbox::getItemID(int iIndex) 
{
	LeeItemWndMap::iterator it =d_commItems.find(iIndex);
	if (it != d_commItems.end())
	{
		return (it->second)->getID();
	}
	return 0;
}
CLeeString CLeePopupListbox::getItemString(int iIndex) 
{
	LeeItemWndMap::iterator it =d_commItems.find(iIndex);
	if (it != d_commItems.end())
	{
		return (it->second)->getString();
	}
	return "";
}
DWORD CLeePopupListbox::getItemIndex(int iIndex)
{
	LeeItemWndMap::iterator it =d_commItems.find(iIndex);
	if (it != d_commItems.end())
	{
		return (it->second)->getIndex();
	}
	return 0;
}
LPVOID CLeePopupListbox::getItemData(int iIndex) 
{
	LeeItemWndMap::iterator it =d_commItems.find(iIndex);
	if (it != d_commItems.end())
	{
		return (it->second)->getData();
	}
	return (void*)-1;
}
void CLeePopupListbox::setItemString(int iIndex,CLeeString &str )
{
	LeeItemWndMap::iterator it =d_commItems.find(iIndex);
	if (it != d_commItems.end())
	{
		(it->second)->setString(str);
	}
}
void CLeePopupListbox::setItemData(int iIndex,LPVOID data)
{
	LeeItemWndMap::iterator it =d_commItems.find(iIndex);
	if (it != d_commItems.end())
	{
		(it->second)->setData(data);
	}
}
void CLeePopupListbox::setItemID(int iIndex,INT nID) 
{
	LeeItemWndMap::iterator it =d_commItems.find(iIndex);
	if (it != d_commItems.end())
	{
		return (it->second)->setID(nID);
	}
	return ;
}
int CLeePopupListbox::getHeight(int iIndex)
{
	LeeItemWndMap::iterator it =d_commItems.find(iIndex);
	if (it != d_commItems.end())
	{
		return (it->second)->getHeight();
	}
	return 0;
}
int CLeePopupListbox::getWidght(int iIndex) 
{
	LeeItemWndMap::iterator it =d_commItems.find(iIndex);
	if (it != d_commItems.end())
	{
		return (it->second)->getWidght();
	}
	return 0;
}
int CLeePopupListbox::getSelectedItemIndex(void)
{
	return d_selectedIndex ;
}
void CLeePopupListbox::setCurSel(int islot)
{
	d_selectedIndex =islot;
}
void CLeePopupListbox::DrawSelf(ILeeDrawInterface *DrawFuns)
{
	if (d_parent !=NULL && d_parent->getState()<= LWNDST_HIDE)
	{
		return;
	}
	RECT rc =getWindowRect();
	RectF rcc=rc;
	RectF rcparent =getCliperRect();
	//主窗口绘制
	if (d_DrawEng->hasWidgetlook(d_wndlook))
	{
		d_DrawEng->DrawWidgetlook(d_wndlook,d_wndlookState,rcc,"",-1,-1,&rcparent,true);
	}
	int visibleRow=0;
	getVsibleRange(visibleRow);
	//调整位置
	EnsureColVisible(d_curStartRowIndex,d_curStartRowIndex+visibleRow);
	LeeItemWndMap::iterator it =d_commItems.find(d_curStartRowIndex);
	int nDraw =d_curStartRowIndex+visibleRow ;
	while (it != d_commItems.end() &&
		nDraw >0)//优化点
	{
		(it->second)->DrawSelf(DrawFuns);
		++it;
		--nDraw;
	}
}
int CLeePopupListbox::getItemCount(void)
{
	return d_commItems.size();
}
void CLeePopupListbox::setState(LWNDST state) 
{
	if (d_wndState != state)
	{
		d_wndState =state;
		if (state==LWNDST::LWNDST_HIDE ||
			state==LWNDST::LWNDST_SHOW)
		{
			LeeItemWndMap::iterator it =d_commItems.begin();
			while (it != d_commItems.end())
			{
				//子窗口中是否有窗口被激活
				(it->second)->setState(state);
				++it;
			}
		}

		//激发state chang 事件
	}

}
LRESULT CLeePopupListbox::HandleEvent ( UINT uMsg ,WPARAM wParam ,LPARAM lParam) 
{
	if (uMsg == SOAR_COMMAND)
	{
		SOARMSG *pMsg=(SOARMSG*)wParam;
		if (pMsg->mouseEvent==SOAR_LCLICK_UP && pMsg->sourceWnd->getType()==LWNDT_STAICITEMWND )
		{
			d_selectedIndex=((ISoarItemWnd*)pMsg->sourceWnd)->getIndex();
		}
		setState(LWNDST_HIDE);
	}
	if (uMsg == SOAR_LOSTFOCUS)
	{
		setState(LWNDST_HIDE);
	}
	return CSoarWnd::HandleEvent(uMsg,wParam,lParam);//留系统底层处理
}
void CLeePopupListbox::sortRow(int iStart,int itargetStart)
{
	LeeItemWndMap::iterator it = d_commItems.find(iStart);
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
	while (it != d_commItems.end() )
	{
		//
		listy.setValue(d_nItemHeight*itargetStart);
		listY.setBaseDimension(listy);
		listY.setDimensionType(DT_Y_POSITION);
		(it->second)->setIndex(itargetStart);
		setItemPosition((it->second),listX,listY,listW,listH);
		//
		++it;
		++itargetStart;
	}
}
void CLeePopupListbox::setItemPosition(ISoarItemWnd* lpWnd,Dimension& X,Dimension  &Y,Dimension& W,Dimension& H)
{
	if (!lpWnd)
	{
		return ;
	}
	ComponentArea rcArea;
	rcArea.d_left= X;
	rcArea.d_top =Y;
	rcArea.d_right_or_width=W;
	rcArea.d_bottom_or_height=H;
	lpWnd->setArea(rcArea);
}
void CLeePopupListbox::EnsureColVisible(int iStartRow,int iEndRow)
{
	int nCountRow =iEndRow -iStartRow+1;
	int nStartRow =0;
	//小于iStartRow的行
	LeeItemWndMap::iterator it = d_commItems.begin();
	while( it != d_commItems.end() )
	{
		(it->second)->setState(LWNDST_HIDE);
		++it;
	}
	//////////////////////////////////////////////////////////////////////////
	it = d_commItems.find(iStartRow);
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
	static int nWidth =0;//记录每列X的坐标值
	static int nW =0;//每项的宽度
	while (nCountRow>0 && it != d_commItems.end())
	{
		listy.setValue(d_nItemHeight*nStartRow);
		listY.setBaseDimension(listy);
		listY.setDimensionType(DT_Y_POSITION);
		(it->second)->setState(LWNDST_SHOW);
		setItemPosition((it->second),listX,listY,listW,listH);
		++it;
		nStartRow++;
		nCountRow--;
	}
}
bool CLeePopupListbox::getVsibleRange(int & nRow)
{
	RECT rc =getClientRect();
	int nW = rc.right-rc.left;
	int nH =rc.bottom -rc.top;
	nRow =nH/d_nItemHeight;
	return false;
}