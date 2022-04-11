#include "LeePopupListViewWnd.h"
#include "SoarRoot.h"
#pragma comment ( lib,"Shlwapi.lib")
CLeePopupListViewWnd::CLeePopupListViewWnd(HWND root,ISoarEngine* Eng):CLeePopupBase(root,Eng),
d_cntRow(0),
d_nItemWidth(70),
d_nItemHeight(30),
d_curStartRowIndex(0),
d_curStartColIndex(0),
d_headerCtrl(NULL),
d_selectedRowIndex(-1),
d_OwnerWnd(NULL)
{
	d_wndlook="DefaultClient";
	d_wndtype =LWNDT_POPLISTVIEW;

}

CLeePopupListViewWnd::~CLeePopupListViewWnd(void)
{
	d_commItems.clear();
	d_gcitems.clear();
}
void CLeePopupListViewWnd::InitializeSegments(void)
{
	CLeeString itemname ;
	itemname.Format("LeePopupListViewWnd__%p__%s",this,"systemlistViewHeader");
	if(d_wndStyle & LWNDSTY_SYSHEADERCTRL)
	{
		//d_headerCtrl=(CLeeHeaderWnd *)CSoarRoot::getSingletonPtr()->createPopupWnd(itemname,
		//listX,listY,listW,listH,LWNDT_SYS_HEADERWND,this,d_zPopupIndex+1);
		d_headerCtrl=(CLeeHeaderWnd*)AddNonClientBar(itemname,SOARALIGN_TOP,LWNDT_SYS_HEADERWND,25);
		
	}
	CSoarWnd::InitializeSegments();
	
	
}
int CLeePopupListViewWnd::setHeaderHeight(int nHeight) 
{
	if (d_headerCtrl)
	{
		return d_headerCtrl->setHeight(nHeight);
	}
	d_nItemHeight =nHeight;
	return 0;
}
int CLeePopupListViewWnd::setHeaderWidth(int nWidth)
{
	d_nItemWidth = nWidth;
	return 0;
}
int CLeePopupListViewWnd::addHeaderItem(int iColWidth,CLeeString str)
{
	return d_headerCtrl->addItem(iColWidth,str);
}
int CLeePopupListViewWnd::addRow(CLeeString str,int nID,LPVOID data)
{
	RECT rt ={0,0,0,0};
	int nItemW =0;
	nItemW =d_nItemWidth;
	
	//列表list
	AbsoluteDim listx(0);
	Dimension listX(listx,DT_X_POSITION);
	AbsoluteDim listy(d_nItemHeight*d_cntRow);
	Dimension listY(listy,DT_Y_POSITION);
	AbsoluteDim listh(d_nItemHeight);
	Dimension listH(listh,DT_HEIGHT);
	AbsoluteDim listw(nItemW);
	Dimension listW(listw,DT_WIDTH);
	//没有文件头,占领全部空间
	if (d_headerCtrl==NULL)
	{
		UDim dx(1.0,0);
		UnifiedDim dux(dx,DT_WIDTH);
		listW.setBaseDimension(dux);
	}
	LeeGCItemWndMap::iterator itgc = d_gcitems.begin();
	ISoarItemWnd * llp =NULL;
	if (itgc != d_gcitems.end())
	{
		llp =itgc->second;
		ComponentArea area ;
		llp->getArea(area);
		area.d_right_or_width=listW;
		llp->setArea(area);
		llp->setWidght(nItemW);
		setItemPosition(llp,listX,listY,listW,listH);
		d_gcitems.erase(itgc);
	}
	else
	{
		CLeeString itemname ;
		itemname.Format("LeePopupListViewWnd__%p__%s__%d",this,"poplistViewItem",d_cntRow);
		llp =(ISoarItemWnd *)CSoarRoot::getSingletonPtr()->createPopupWnd(itemname,listX,listY,listW,listH,LWNDT_STAICITEMWND,this,d_zPopupIndex+1);
	}
	llp->setString(str);
	llp->setIndex(d_cntRow);
	llp->setID(nID);
	llp->setsubIndex(0);
	llp->setData(data);
	llp->setOwnerWnd(d_OwnerWnd);
	llp->setWidght(d_nItemWidth);
	LeeItemWndMap mp ;
	mp[0] =llp;
	d_commItems[d_cntRow++]=mp;
	d_curStartRowIndex==-1?d_curStartRowIndex=0:d_curStartRowIndex;
	return d_cntRow-1;
}
int CLeePopupListViewWnd::insertCol(int iRow,int nCol,CLeeString str,int nID,LPVOID data)
{
	//没有文件头不能添加列
	if (d_headerCtrl==NULL)
	{
		return -1;
	}
	int nItemW =0;
	nItemW =d_nItemWidth;
	LeeITEMMAP::iterator it = d_commItems.find(iRow);
	if (it != d_commItems.end() )// 检测行
	{
		LeeItemWndMap &mp =(it->second);
		LeeItemWndMap::iterator ita =mp.find(nCol);
		if (ita !=mp.end() )
		{
			(ita->second)->setsubIndex(nCol);
			(ita->second)->setString(str);
			(ita->second)->setData(data);
			(ita->second)->setOwnerWnd(d_parent);
			(ita->second)->setID(nID);
			return (ita->second)->getID();
		}
		else
		{
			int d_cntColItem=mp.size()-1;//重新计算列数
			RECT rt ={0,0,0,0};
			//列表list
			AbsoluteDim listx(d_nItemWidth*nCol);
			Dimension listX(listx,DT_X_POSITION);
			AbsoluteDim listy(d_nItemHeight*iRow);
			Dimension listY(listy,DT_Y_POSITION);
			AbsoluteDim listh(d_nItemHeight);
			Dimension listH(listh,DT_HEIGHT);
			AbsoluteDim listw(nItemW);
			Dimension listW(listw,DT_WIDTH);
			LeeGCItemWndMap::iterator itgc = d_gcitems.begin();
			ISoarItemWnd * llp =NULL ;
			if (itgc!= d_gcitems.end())//从垃圾堆中找项
			{
				llp =itgc->second;
				llp->setState(LWNDST_HIDE);
				setItemPosition(llp,listX,listY,listW,listH);
				d_gcitems.erase(itgc);
			}
			else
			{
				CLeeString itemname ;
				itemname.Format("LeePopupListViewWnd__%p__%s__%d__%d__%d",this,"systemlistViewitemCol",d_cntColItem,iRow,nCol);
				llp =(ISoarItemWnd *)CSoarRoot::getSingletonPtr()->createPopupWnd(itemname,listX,listY,listW,listH,
					LWNDT_STAICITEMWND,this,d_zPopupIndex+1);
			}
			llp->setString(str);
			llp->setIndex(d_cntRow);
			llp->setID(nID);
			llp->setsubIndex(nCol);
			llp->setOwnerWnd(d_OwnerWnd);
			mp[nCol] =llp;
			return iRow;
		}
	}
    
}
void CLeePopupListViewWnd::clear(void)
{
	LeeITEMMAP::iterator it = d_commItems.begin();
	while (it != d_commItems.end() )
	{
		LeeItemWndMap &mp =(it->second);
		LeeItemWndMap::iterator ita=mp.begin();
		while (ita !=mp.end())
		{
			(ita->second)->setState(LWNDST_HIDE);
			d_gcitems[ita->second] =ita->second;
			++ita;
		}
		//原来内容移除
		d_commItems.erase(it);
		//按行添加
		it = d_commItems.begin();
	}
	d_cntRow=0;
	d_curStartColIndex =0;
	d_curStartRowIndex =0;
	d_selectedRowIndex=-1;
	return;
}
int CLeePopupListViewWnd::removeRow(int iRow)
{
	LeeITEMMAP::iterator it = d_commItems.find(iRow);
	if (it != d_commItems.end() )
	{
		LeeItemWndMap &mp =(it->second);
		LeeItemWndMap::iterator ita=mp.begin();
		while (ita !=mp.end())
		{
			(ita->second)->setState(LWNDST_HIDE);
			d_gcitems[ita->second] =ita->second;
			++ita;
		}
		//整行的项放回垃圾回收
		mp.clear();
		//原来内容移除
		d_commItems.erase(it);
		sortRow(iRow+1,iRow);
		//如果只有一项或者
		if(d_curStartRowIndex==iRow)
		{
			if (d_commItems.empty())
			{
				d_curStartRowIndex=-1;
			}
		}
		d_cntRow--;
	}
	
	return 0;
}
void CLeePopupListViewWnd::sortRow(int iStart,int itargetStart)
{
	LeeITEMMAP::iterator it = d_commItems.find(iStart);
	RECT rt ={0,0,0,0};
	int nItemW =0;
	nItemW =d_headerCtrl?d_headerCtrl->getItemWidth(0):d_nItemWidth;
	//列表list
	AbsoluteDim listx(d_nItemWidth*0);
	Dimension listX(listx,DT_X_POSITION);
	AbsoluteDim listy(d_nItemHeight*itargetStart);
	Dimension listY(listy,DT_Y_POSITION);
	AbsoluteDim listh(d_nItemHeight);
	Dimension listH(listh,DT_HEIGHT);
	AbsoluteDim listw(nItemW);
	Dimension listW(listw,DT_WIDTH);
	UDim dx(1.0,0);
	UnifiedDim dux(dx,DT_WIDTH);
	//没有文件头,占领全部空间
	if (d_headerCtrl==NULL)
	{
		UDim dx(1.0,0);
		UnifiedDim dux(dx,DT_WIDTH);
		listW.setBaseDimension(dux);
	}
	while (it != d_commItems.end() )
	{
		LeeItemWndMap &mp =(it->second);
		LeeItemWndMap::iterator ita=mp.begin();
		while (ita !=mp.end())
		{
			//
			nItemW =d_headerCtrl?d_headerCtrl->getItemWidth(itargetStart):d_nItemWidth;
			listx.setValue(d_nItemWidth*(ita->second)->getsubIndex());
			listX.setBaseDimension(listx);
			listX.setDimensionType(DT_X_POSITION);
			listy.setValue(d_nItemHeight*itargetStart);
			listY.setBaseDimension(listy);
			listY.setDimensionType(DT_Y_POSITION);
			
			//没有文件头,占领全部空间
			if (d_headerCtrl==NULL)
			{
				listW.setBaseDimension(dux);
			}
			else
			{
				listw.setValue(nItemW);
				listW.setBaseDimension(listw);
			}
			(ita->second)->setIndex(itargetStart);
			setItemPosition((ita->second),listX,listY,listW,listH);
			++ita;
		}
		//
		++it;
		++itargetStart;
	}
}
void CLeePopupListViewWnd::setItemPosition(ISoarItemWnd* lpWnd,Dimension& X,Dimension  &Y,Dimension& W,Dimension& H)
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
void CLeePopupListViewWnd::EnsureColVisible(int iStartRow,int iEndRow,int iStartCol,int iEndCol)
{
	int nCountRow =iEndRow -iStartRow+1;
	int nStartRow =0;
	//小于iStartRow的行
	LeeITEMMAP::iterator it = d_commItems.begin();
	while(it != d_commItems.end() )
	{
		LeeItemWndMap &mp =(it->second);
		//////////////////////////////////////////////////////////////////////////
		//小于iStartCol
		LeeItemWndMap::iterator ita=mp.begin();
		while (ita != mp.end())
		{
			(ita->second)->setState(LWNDST_HIDE);
			++ita;
		}
		++it;
	}
	//////////////////////////////////////////////////////////////////////////
	it = d_commItems.find(iStartRow);
	RECT rt ={0,0,0,0};
	//列表list
	AbsoluteDim listx(d_nItemWidth*0);
	Dimension listX(listx,DT_X_POSITION);
	AbsoluteDim listy(d_nItemHeight*nStartRow);
	Dimension listY(listy,DT_Y_POSITION);
	AbsoluteDim listh(d_nItemHeight);
	Dimension listH(listh,DT_HEIGHT);
	AbsoluteDim listw(d_nItemWidth);
	Dimension listW(listw,DT_WIDTH);
	static int nWidth =0;//记录每列X的坐标值
	static int nW =0;//每项的宽度
	UDim dx(1.0,0);
	UnifiedDim dux(dx,DT_WIDTH);
	while (nCountRow>0 && it != d_commItems.end())
	{
		LeeItemWndMap &mp =(it->second);
		LeeItemWndMap::iterator ita=mp.find(iStartCol);
		int nStartCol =iStartCol;
		nWidth =0;//记录每列X的坐标值
		while (ita !=mp.end())
		{
			//根据头控件的宽度来确定项的宽度
			nW=d_headerCtrl?d_headerCtrl->getItemWidth(nStartCol):d_nItemWidth;
			listx.setValue(nWidth);//先设置后赋值
			nWidth +=nW;
			listX.setBaseDimension(listx);
			listX.setDimensionType(DT_X_POSITION);
			listy.setValue(d_nItemHeight*nStartRow);
			listY.setBaseDimension(listy);
			listY.setDimensionType(DT_Y_POSITION);
			//没有文件头,占领全部空间
			if (d_headerCtrl==NULL)
			{
				listW.setBaseDimension(dux);
			}
			else
			{
				listw.setValue(nW);
				listW.setBaseDimension(listw);
			}
			if (nStartCol>=iEndCol)
			{
				(ita->second)->setState(LWNDST_HIDE);
			}
			else
				(ita->second)->setState(LWNDST_SHOW);
			setItemPosition((ita->second),listX,listY,listW,listH);
			++ita;
			nStartCol++;
			

		}
		++it;
		nStartRow++;
		nCountRow--;
	}
}
int CLeePopupListViewWnd::removeCol(int iRow,int nCol)
{
	LeeITEMMAP::iterator it = d_commItems.find(iRow);
	if (it != d_commItems.end() )
	{
		LeeItemWndMap &mp =(it->second);
		LeeItemWndMap::iterator ita =mp.find(nCol);
		if (ita !=mp.end() )
		{
			//添加到gc中
			LeeGCItemWndMap::iterator ita1 =d_gcitems.find(ita->second);
			if (ita1==d_gcitems.end())
			{
				(ita->second)->setState(LWNDST_HIDE);
				d_gcitems[ita->second]=ita->second;
			}
			mp.erase(ita);
		}
	}
	return 1;
}
void CLeePopupListViewWnd::DrawSelf(ILeeDrawInterface *DrawFuns)
{
	if (d_parent !=NULL && d_parent->getState()<= LWNDST_HIDE)
	{
		return;
	}
	if (getState()<= LWNDST_HIDE)
	{
		return;
	}
	RECT rc =getWindowRect();
	RectF rcc=rc;
	RectF rcparent =getCliperRect();
	if (d_DrawEng->hasWidgetlook(d_wndlook))
	{
		d_DrawEng->DrawWidgetlookFrame(d_wndlook,d_wndlookState,rcc,&rcparent,false);
		//绘制文字
		d_DrawEng->DrawWidgetlook(d_wndlook,d_wndlookState,rcc,d_wndText,d_VertTextFormat,d_horzTextFormat,&rcparent,true);
	}
	int visibleRow, visibleCol;
	getVsibleRange(visibleRow,visibleCol);
	//调整位置
	EnsureColVisible(d_curStartRowIndex,d_curStartRowIndex+visibleRow,d_curStartColIndex,d_curStartColIndex+visibleCol);
	//画项
	LeeITEMMAP::iterator it = d_commItems.find(d_curStartRowIndex);
	int nDraw =d_curStartRowIndex+visibleRow ;
	while (it != d_commItems.end() &&
		nDraw >0)//优化点
	{
		LeeItemWndMap mp =(it->second);
		LeeItemWndMap::iterator ita =mp.find(d_curStartColIndex);
		while (ita !=mp.end() )
		{
			(ita->second)->DrawSelf(DrawFuns);
			++ita;
		}
		++it;
	}
}
void CLeePopupListViewWnd::setState(LWNDST state) 
{
	if (d_wndState != state)
	{
		if (state==LWNDST::LWNDST_HIDE )
		{
			//画项
			LeeITEMMAP::iterator it = d_commItems.begin();
			while (it != d_commItems.end() )
			{
				LeeItemWndMap mp =(it->second);
				LeeItemWndMap::iterator ita =mp.begin();
				while (ita !=mp.end() )
				{
					(ita->second)->setState(state);
					++ita;
				}
				++it;
			}
		}

		d_wndState =state;
		if (state==LWNDST::LWNDST_HIDE ||
			state==LWNDST::LWNDST_SHOW)
		{
			LeeWndPtrMap::iterator it =d_WndPtrs.begin();
			while (it != d_WndPtrs.end())
			{
				//子窗口中是否有窗口被激活
			   (it->second)->setState(state);
				++it;
			}

       }
		//激发state chang 事件
		LeeGCItemWndMap::iterator itgc = d_gcitems.begin();
		while (itgc != d_gcitems.end() )
		{
			ISoarItemWnd* pItem =(itgc->second);
			if (pItem)
			{
				pItem->setState(LWNDST_HIDE);
			}
			++itgc;
		}
	}

}
CLeeString CLeePopupListViewWnd::getItemString(int iRow,int nCol)
{
	LeeITEMMAP::iterator it = d_commItems.find(iRow);
	if (it != d_commItems.end() )
	{
		LeeItemWndMap mp =(it->second);
		LeeItemWndMap::iterator ita =mp.find(nCol);
		if (ita !=mp.end() )
		{
			return (ita->second)->getString();
		}
	}
	return "" ;
}
LPVOID CLeePopupListViewWnd::getItemData(int iRow,int nCol)
{
	LeeITEMMAP::iterator it = d_commItems.find(iRow);
	if (it != d_commItems.end() )
	{
		LeeItemWndMap mp =(it->second);
		LeeItemWndMap::iterator ita =mp.find(nCol);
		if (ita !=mp.end() )
		{
			return (ita->second)->getData();
		}
	}
	return (void*)-1;
}
DWORD CLeePopupListViewWnd::getItemID(int iRow,int nCol)
{
	LeeITEMMAP::iterator it = d_commItems.find(iRow);
	if (it != d_commItems.end() )
	{
		LeeItemWndMap mp =(it->second);
		LeeItemWndMap::iterator ita =mp.find(nCol);
		if (ita !=mp.end() )
		{
			return (ita->second)->getID();
		}
	}
	return 0;
}
void CLeePopupListViewWnd::setItemString(int iIndex,int subItem,CLeeString &str )
{
	LeeITEMMAP::iterator it = d_commItems.find(iIndex);
	if (it != d_commItems.end() )
	{
		LeeItemWndMap mp =(it->second);
		LeeItemWndMap::iterator ita =mp.find(subItem);
		if (ita !=mp.end() )
		{
			return (ita->second)->setString(str);
		}
	}
	return ;
}
void CLeePopupListViewWnd::setItemData(int iIndex,int subItem,LPVOID data) 
{
	LeeITEMMAP::iterator it = d_commItems.find(iIndex);
	if (it != d_commItems.end() )
	{
		LeeItemWndMap mp =(it->second);
		LeeItemWndMap::iterator ita =mp.find(subItem);
		if (ita !=mp.end() )
		{
			return (ita->second)->setData(data);
		}
	}
	return ;
}
void CLeePopupListViewWnd::setItemID(int iIndex,int subItem,INT nID) 
{
	LeeITEMMAP::iterator it = d_commItems.find(iIndex);
	if (it != d_commItems.end() )
	{
		LeeItemWndMap mp =(it->second);
		LeeItemWndMap::iterator ita =mp.find(subItem);
		if (ita !=mp.end() )
		{
			return (ita->second)->setID(nID);
		}
	}
	return ;
}
LRESULT CLeePopupListViewWnd::HandleEvent ( UINT uMsg ,WPARAM wParam ,LPARAM lParam) 
{
	if (uMsg == SOAR_COMMAND)
	{	
		SOARMSG *pMsg=(SOARMSG*)wParam;
		if (pMsg->mouseEvent==SOAR_LCLICK_UP && pMsg->sourceWnd->getType()==LWNDT_STAICITEMWND )
		{
			d_selectedRowIndex=((ISoarItemWnd*)pMsg->sourceWnd)->getIndex();
		}
		if (pMsg->mouseEvent==SOAR_RCLICK_UP && pMsg->sourceWnd->getType()==LWNDT_STAICITEMWND )
		{
			d_selectedRowIndex=((ISoarItemWnd*)pMsg->sourceWnd)->getIndex();
		}
		if (pMsg->mouseEvent==SOAR_LDBCLICK && pMsg->sourceWnd->getType()==LWNDT_STAICITEMWND )
		{
			SOARMSG *pMsg=(SOARMSG*)wParam;
			d_selectedRowIndex=((ISoarItemWnd*)pMsg->sourceWnd)->getIndex();
		}
	}
	if (uMsg==WM_MOUSEWHEEL  )
	{
		short  zDelta   =   (short)   HIWORD(wParam);   
		d_curStartRowIndex +=zDelta>0?-1:1;
		d_curStartRowIndex=d_curStartRowIndex>=0?d_curStartRowIndex:0;
	}
	if (uMsg == SOAR_VSCROLL)
	{
		SOARMSG *pMsg=(SOARMSG*)wParam;
		d_curStartRowIndex +=pMsg->wParam;
		d_curStartRowIndex=d_curStartRowIndex>=0?d_curStartRowIndex:0;
	}
	if (uMsg == SOAR_HSCROLL)
	{
		SOARMSG *pMsg=(SOARMSG*)wParam;
		d_curStartColIndex +=pMsg->wParam;
		d_curStartColIndex=d_curStartColIndex>=0?d_curStartColIndex:0;
		if (d_headerCtrl)
		{
			d_headerCtrl->updateStartCol(d_curStartColIndex);
		}
		
	}
	if (uMsg == SOAR_LOSTFOCUS)
	{
		setState(LWNDST_HIDE);
	}
	return CSoarRoot::getSingletonPtr()->SoarDefWndProc(uMsg,wParam,lParam);//留系统底层处理
}
bool CLeePopupListViewWnd::getVsibleRange(int & nRow,int& nCol)
{
	RECT rc =getClientRect();
	int nW = rc.right-rc.left;
	int nH =rc.bottom -rc.top;
	nRow =nH/d_nItemHeight-1;
	//以下意见由剪切实现
	if(d_headerCtrl)
		nCol =d_headerCtrl->getItemCount();
	else
	    nCol =nW/d_nItemWidth;
	//确保有一项显示
	  nCol=nCol>0?nCol:1;
     return false;
}
//
RECT CLeePopupListViewWnd::getClientRect(void) 
{
	RECT rcTmp;
	RECT rcParent;
	rcParent =d_parent->getClientRect();//获取父窗口包括非客户区
	RECT rc;
	RectF rcCur;
	rcCur=d_area.getRect(rcParent);
	rcCur.toWindowRect(rc);
	if (d_headerCtrl)
	{
		rc.top +=d_headerCtrl->getHeight();
	}
	if (p_HScrollSegment)
	{
		rc.bottom-= d_scrollSegmentHeight;
	}
	if (p_VScrollSegment)
	{
		rc.right-= d_scrollSegmentWidth;
	}
	return rc;
}
int CLeePopupListViewWnd::getSelectedItemIndex(void)
{
	return d_selectedRowIndex;
}
void CLeePopupListViewWnd::setCurSel(int islot)
{
	d_selectedRowIndex = islot ;
}
int CLeePopupListViewWnd::getItemCount(void)
{
	return d_commItems.size();
}
