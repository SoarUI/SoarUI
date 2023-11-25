#include "SoarListViewWnd.h"
#include "SoarRoot.h"
#pragma comment ( lib,"Shlwapi.lib")
#include"SoarItemSegment.h"
#include"soarWidgets\SoarColumnItemSegment.h"
CLeeListViewWnd::CLeeListViewWnd(HWND root,ISoarEngine* Eng):CSoarWnd(root,Eng),
d_cntRow(0),
d_nItemWidth(70),
d_nItemHeight(30),
d_curStartRowIndex(0),
d_curStartColIndex(0),
d_selectedRowIndex(-1),
d_selectedColIndex(0),
d_OwnerWnd(NULL),
d_headerCtrl(NULL)
{
	d_wndlook="DefaultClient";
	d_wndtype =LWNDT_LISTVIEWWND;
	
	
}

CLeeListViewWnd::~CLeeListViewWnd(void)
{
	d_commItems.clear();
}
void CLeeListViewWnd::InitializeSegments(void)
{
	CSoarWnd::InitializeSegments();
	CLeeString itemname ;
	itemname.Format("LeeListViewWnd__%p__%s",this,"systemlistViewHeader");
	if(d_wndStyle & LWNDSTY_SYSHEADERCTRL)
		{
			d_headerCtrl=(CLeeHeaderWnd*)AddNonClientBar(itemname,SOARALIGN_TOP,LWNDT_SYS_HEADERWND,25);
		}
}
int CLeeListViewWnd::setHeaderHeight(int nHeight) 
{
	if (d_headerCtrl)
	{
		return d_headerCtrl->setHeight(nHeight);
	}
	d_nItemHeight =nHeight;
	return 0;
}
int CLeeListViewWnd::setHeaderWidth(int nWidth)
{
	d_nItemWidth = nWidth;
	return 0;
}
int CLeeListViewWnd::addHeaderItem(int iColWidth,CLeeString str)
{
	if (d_headerCtrl)
	{
		return d_headerCtrl->addItem(iColWidth,str);
	}
	return -1;
}
int CLeeListViewWnd::addRow(CLeeString str,int nID,LPVOID data)
{
	RECT rt ={0,0,0,0};
	rt=getClientRect();
	int nItemW =0;
	nItemW =rt.right-rt.left;
	//列表list
	AbsoluteDim listx(2);
	Dimension listX(listx,DT_X_POSITION);
	AbsoluteDim listy(d_nItemHeight*d_cntRow);
	Dimension listY(listy,DT_Y_POSITION);
	AbsoluteDim listh(d_nItemHeight);
	Dimension listH(listh,DT_HEIGHT);
	AbsoluteDim listw(nItemW);
	Dimension listW(listw,DT_WIDTH);
	LeeGCItemWndMap::iterator itgc = d_gcitems.begin();
	ISoarItemBase * llp =NULL;
	if (itgc != d_gcitems.end())
	{
		llp =itgc->second;
		ComponentArea area ;
		area.d_left=listX;
		area.d_top=listY;
		area.d_right_or_width=listW;
		area.d_bottom_or_height=listH;
		area=llp->getArea();
		area.d_right_or_width=listW;
		llp->setArea(area);
		llp->setWidght(nItemW);
		d_gcitems.erase(itgc);
		//setItemPosition(llp,listX,listY,listW,listH);
	}
	else
	{
		//CLeeString itemname ;
		//itemname.Format("LeeListViewWnd__%p__%s__%d",this,"systemlistView",d_cntRow);
		llp =(ISoarItemBase *)new CSoarItemSegment(d_rootWnd,d_DrawEng);
		ComponentArea area ;
		area.d_left=listX;
		area.d_top=listY;
		area.d_right_or_width=listW;
		area.d_bottom_or_height=listH;
		llp->setArea(area);
		llp->setOwnerWnd(this);
		llp->setMsgReceiver(this);
		llp->setParent(NULL);
	//	itemname,listX,listY,listW,listH,LWNDT_STAICITEMWND,this,d_zIndex+1);
		
	}
	int ncurRow =d_cntRow;
	llp->setTitle(str);
	llp->setIndex(d_cntRow);//行序号
	llp->setID(nID);
	llp->setData(data);
	llp->setState(LWNDST_SHOW);
	d_commItems[d_cntRow++]=llp;
	d_curStartRowIndex=d_curStartRowIndex==-1?d_curStartRowIndex=0:d_curStartRowIndex;
	
	insertCol(ncurRow,0,str,nID,data);
	return ncurRow;
}
int CLeeListViewWnd::insertCol(int iRow,int nCol,CLeeString str,int nID,LPVOID data)
{
	int nItemW =0;
	nItemW =d_nItemWidth;
	ItemSegmentMap::iterator it = d_commItems.find(iRow);
	if (it != d_commItems.end() )
	{
		ISoarItemBase* pItemSeg =it->second;
		ISoarColumnItemBase* pColItem =pItemSeg->getItem(nCol);
		if (pColItem !=NULL ) //找到列
		{
			pColItem->setsubIndex(nCol);
			pColItem->setTitle(str);
			pColItem->setData(data);
			pColItem->setID(nID);
			return pColItem->getIndex();
		}
		else//新建一个列
		{
			int nColWidth=d_headerCtrl?d_headerCtrl->getItemWidth(nCol):d_nItemWidth;//要和头控件同步
			ISoarColumnItemBase* pColItem =new CSoarColumnItemSegment(d_rootWnd,d_DrawEng);;
			pColItem->setsubIndex(nCol);
			pColItem->setTitle(str);
			pColItem->setData(data);
			pColItem->setID(nID);
			RECT rt ={0,0,0,0};
			int nColStart =CalcColumnStartX(nCol);
			//列表list
			AbsoluteDim listx(nColStart);
			Dimension listX(listx,DT_X_POSITION);
			AbsoluteDim listy(0);
			Dimension listY(listy,DT_Y_POSITION);
			AbsoluteDim listh(d_nItemHeight);
			Dimension listH(listh,DT_HEIGHT);
			AbsoluteDim listw(nColWidth);
			Dimension listW(listw,DT_WIDTH);
			ComponentArea area ;
			area.d_left=listX;
			area.d_top=listY;
			area.d_right_or_width=listW;
			area.d_bottom_or_height=listH;
			pColItem->setArea(area);
			pColItem->setOwnerWnd(this);
			pColItem->setMsgReceiver(this);
			pColItem->setTitle(str);
			pColItem->setIndex(iRow);//行索引
			pColItem->setID(nID);
			pColItem->setsubIndex(nCol);//列索引
			pColItem->setParent(pItemSeg);
			pColItem->setState(LWNDST_SHOW);
			pItemSeg->AddItem(pColItem);
			return iRow;
		}
	}
    
}
void CLeeListViewWnd::clear(void)
{
	ItemSegmentMap::iterator it = d_commItems.begin();
	while (it != d_commItems.end() )
	{
		(it->second)->destroyAllItems();
		delete (it->second);
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
int CLeeListViewWnd::removeRow(int iRow)
{
	ItemSegmentMap::iterator it = d_commItems.find(iRow);
	if (it != d_commItems.end() )
	{
		d_gcitems[it->second] =it->second;
		it->second->setState(LWNDST_HIDE);
		//整行的项放回垃圾回收
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
//移除第i行，后面的修正位置
void CLeeListViewWnd::sortRow(int iStart,int itargetStart)
{
	ItemSegmentMap::iterator it = d_commItems.find(iStart);
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
	while (it != d_commItems.end() )
	{
			//
			nItemW =d_headerCtrl?d_headerCtrl->getItemWidth(itargetStart):d_nItemWidth;
			listx.setValue(0);
			listX.setBaseDimension(listx);
			listX.setDimensionType(DT_X_POSITION);
			listy.setValue(d_nItemHeight*itargetStart);
			listY.setBaseDimension(listy);
			listY.setDimensionType(DT_Y_POSITION);
			listW.setDimensionType(DT_WIDTH);
			listw.setValue(nItemW);
			listW.setBaseDimension(listw);
			(it->second)->setIndex(itargetStart);
			ComponentArea area ;
			area.d_left=listX;
			area.d_top=listY;
			area.d_right_or_width=listW;
			area.d_bottom_or_height=listH;
			(it->second)->setArea(area);
			//setItemPosition((ita->second),listX,listY,listW,listH);
		//
		++it;
		++itargetStart;
	}
}
void CLeeListViewWnd::setItemPosition(ISoarItemBase* lpWnd,Dimension& X,Dimension  &Y,Dimension& W,Dimension& H)
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
void CLeeListViewWnd::EnsureColVisible(int iStartRow,int iEndRow,int iStartCol,int iEndCol)
{
	int nCountRow =iEndRow -iStartRow+1;
	int nStartRow =0;
	//小于iStartRow的行
	ItemSegmentMap::iterator it = d_commItems.begin();
	while( it != d_commItems.end() )
	{
		
		//////////////////////////////////////////////////////////////////////////
		//处理行
		if(it->first<iStartRow||it->first>iEndRow){
			(it->second)->setState(LWNDST_HIDE);
		}
		//////////////////////////////////////////////////////////////////////////
		++it;
	}
	//////////////////////////////////////////////////////////////////////////
	it = d_commItems.find(iStartRow);
	RECT rt ={0,0,0,0};
	rt=getClientRect();
	
	int nRowWidth =0;
	nRowWidth =rt.right-rt.left;
	int visualWidth = nRowWidth- d_windowframeborderWidth;
	//列表list
	AbsoluteDim listx(d_windowframeborderWidth);
	Dimension listX(listx,DT_X_POSITION);
	AbsoluteDim listy(d_windowframeborderHeight + d_nItemHeight*nStartRow );
	Dimension listY(listy,DT_Y_POSITION);
	AbsoluteDim listh(d_nItemHeight);
	Dimension listH(listh,DT_HEIGHT);
	AbsoluteDim listw(visualWidth);
	Dimension listW(listw,DT_WIDTH);
	int nColStartX =0;//记录每列X的坐标值
	int nColWidth =0;//每项的宽度
	int nStartCol =iStartCol;
	int nRowtartY =0;//记录每行Y的坐标值
	nStartRow =0;
	
	
	
	while (nCountRow>0 && it != d_commItems.end())
	{
		//处理该行位置从0-N
		nRowtartY = d_windowframeborderHeight + d_nItemHeight*nStartRow;
		listy.setValue(nRowtartY);
		listY.setBaseDimension(listy);
		ComponentArea RowArea ;
		listx.setValue(d_windowframeborderWidth);
		listX.setBaseDimension(listx);
		listX.setDimensionType(DT_X_POSITION);
		listW.setDimensionType(DT_WIDTH);
		listw.setValue(visualWidth);
		listW.setBaseDimension(listw);
		listh.setValue(d_nItemHeight);
		listH.setBaseDimension(listh);
		RowArea.d_left=listX;
		RowArea.d_top=listY;
		RowArea.d_right_or_width=listW;
		RowArea.d_bottom_or_height=listH;
		it->second->setArea(RowArea);
		it->second->setState(LWNDST_SHOW);
		nColStartX =0;//记录每列X的坐标值
		//处理显示列的位置
		nStartCol =iStartCol;
		int nCout =it->second->getItemCount();
		ISoarColumnItemBase* pSub =NULL;
		for(int i=0;i<nCout;i++){
			pSub =it->second->getItem(i);
			if(i<iStartCol ||i>iEndCol){
				if(pSub)
				{
					pSub->setState(LWNDST_HIDE);
				}
				continue;
			}
			if (nStartCol>=iEndCol)
			{
				if(pSub)
				{
					pSub->setState(LWNDST_HIDE);
				}
			}
			pSub =it->second->getItem(i);
			if(pSub){
			//根据头控件的宽度来确定项的宽度
			nColWidth=d_headerCtrl?d_headerCtrl->getItemWidth(nStartCol):d_nItemWidth;//要和头控件同步
			listx.setValue(nColStartX);//先设置后赋值
			nColStartX +=nColWidth;
			listX.setBaseDimension(listx);
			listX.setDimensionType(DT_X_POSITION);
			listy.setValue(0);
			listY.setBaseDimension(listy);
			listY.setDimensionType(DT_Y_POSITION);
			listW.setDimensionType(DT_WIDTH);
			//last col width can not extend to scrollbar!!
			if (visualWidth > nColStartX )
			{
				listw.setValue(nColWidth);
			}
			else
			{
				listw.setValue(nColWidth-(nColStartX - visualWidth ));
			}
			listW.setBaseDimension(listw);
			listh.setValue(d_nItemHeight);
			listH.setBaseDimension(listh);
			ComponentArea area ;
			area.d_left=listX;
			area.d_top=listY;
			area.d_right_or_width=listW;
			area.d_bottom_or_height=listH;
			pSub->setArea(area);
			pSub->setState(LWNDST_SHOW);
			}
			
			nStartCol++;
		}
		++it;
		nStartRow++;
		nCountRow--;
	}
}
int CLeeListViewWnd::removeCol(int iRow,int nCol)
{
	ItemSegmentMap::iterator it = d_commItems.find(iRow);
	if (it != d_commItems.end() )
	{
		it->second->removeItem(nCol);
	}
	return 1;
}

void CLeeListViewWnd::setState(LWNDST state) 
{
	if (d_wndState != state)
	{
		//d_wndState =state;
		if (state==LWNDST::LWNDST_HIDE )
		{
			//画项
			ItemSegmentMap::iterator it = d_commItems.begin();
			while (it != d_commItems.end() )
			{
				(it->second)->setState(state);	
				++it;
			}
		}
		CSoarWnd::setState(state);
		//激发state chang 事件
		LeeGCItemWndMap::iterator itgc = d_gcitems.begin();
		while (itgc != d_gcitems.end() )
		{
			ISoarItemBase* pItem =(itgc->second);
			if (pItem)
			{
				pItem->setState(LWNDST_HIDE);
			}
			++itgc;
		}
	}

}
CLeeString CLeeListViewWnd::getItemString(int iRow,int nCol)
{
	ItemSegmentMap::iterator it = d_commItems.find(iRow);
	if (it != d_commItems.end() )
	{
		ISoarColumnItemBase* pSub =it->second->getItem(nCol);
		if(pSub)
		{
			return pSub->getTitle();
		}
	}
	return "" ;
}
LPVOID CLeeListViewWnd::getItemData(int iRow,int nCol)
{
	ItemSegmentMap::iterator it = d_commItems.find(iRow);
	if (it != d_commItems.end() )
	{
		ISoarColumnItemBase* pSub =it->second->getItem(nCol);
		if(pSub)
		{
			return pSub->getData();
		}
	}
	return (void*)-1;
}
DWORD CLeeListViewWnd::getItemID(int iRow,int nCol)
{
	ItemSegmentMap::iterator it = d_commItems.find(iRow);
	if (it != d_commItems.end() )
	{
		ISoarColumnItemBase* pSub =it->second->getItem(nCol);
		if(pSub)
		{
			return pSub->getID();
		}
	}
	return 0;
}
void CLeeListViewWnd::setItemString(int iIndex,int subItem,CLeeString &str )
{
	ItemSegmentMap::iterator it = d_commItems.find(iIndex);
	if (it != d_commItems.end() )
	{
		ISoarColumnItemBase* pSub =it->second->getItem(subItem);
		if(pSub)
		{
			pSub->setTitle(str);
		}
	}
	return ;
}
void CLeeListViewWnd::setItemData(int iIndex,int subItem,LPVOID data) 
{
	ItemSegmentMap::iterator it = d_commItems.find(iIndex);
	if (it != d_commItems.end() )
	{
		ISoarColumnItemBase* pSub =it->second->getItem(subItem);
		if(pSub)
		{
			pSub->setData(data);
		}
	}
}
void CLeeListViewWnd::setItemID(int iIndex,int subItem,INT nID) 
{
	ItemSegmentMap::iterator it = d_commItems.find(iIndex);
	if (it != d_commItems.end() )
	{
		ISoarColumnItemBase* pSub =it->second->getItem(subItem);
		if(pSub)
		{
			pSub->setID(nID);
		}
	}
}
void CLeeListViewWnd::DrawSelf(ILeeDrawInterface *DrawFuns)
{
	if (d_parent !=NULL && d_parent->getState()<= LWNDST_HIDE)
	{
		return;
	}
	if (getState()<= LWNDST_HIDE)
	{
		return;
	}
	RECT rc =getClientRect();
	RectF rcc=rc;
	RectF rcparent =getCliperRect();
	CSoarWnd::DrawSelf(DrawFuns);
	//

	int visibleRow, visibleCol;
	getVsibleRange(visibleRow,visibleCol);
	//调整位置
	EnsureColVisible(d_curStartRowIndex,d_curStartRowIndex+visibleRow-1,
		d_curStartColIndex,d_curStartColIndex+visibleCol);
	//画项
	ItemSegmentMap::iterator it = d_commItems.find(d_curStartRowIndex);
	int nDraw =d_curStartRowIndex+visibleRow ;
	while (it != d_commItems.end() &&
		   nDraw >0)//优化点
	{
		(it->second)->DrawSelf(DrawFuns);
		++it;
		--nDraw;
	}
}
LRESULT CLeeListViewWnd::HandleEvent ( UINT uMsg ,WPARAM wParam ,LPARAM lParam) 
{
	//先给segment有限处理先
	if(uMsg>=WM_MOUSEFIRST && uMsg<WM_MOUSELAST && uMsg != WM_MOUSEMOVE){

	ItemSegmentMap::iterator it = d_commItems.begin();
	while (it != d_commItems.end())//优化点
	{
		if(it->second->HandleEvent(uMsg,wParam,lParam))
		{
			return 1;
		}
		++it;
	}
	}
	if (uMsg == SOAR_COMMAND)
	{	
		SOARMSG *pMsg=(SOARMSG*)wParam;
		int tRoe =d_selectedRowIndex;
		int tCol=d_selectedColIndex;
		if(pMsg->sourceWnd){
				if (pMsg->mouseEvent==SOAR_LCLICK_UP && pMsg->sourceWnd->getType()==LWNDT_ITEM_SEGMENT )
			{
				d_selectedRowIndex=((ISoarItemBase*)pMsg->sourceWnd)->getIndex();
				d_selectedColIndex=0;
			}
			if (pMsg->mouseEvent==SOAR_RCLICK_UP && pMsg->sourceWnd->getType()==LWNDT_ITEM_SEGMENT )
			{
				d_selectedRowIndex=((ISoarItemBase*)pMsg->sourceWnd)->getIndex();
				d_selectedColIndex=0;
			}
			if (pMsg->mouseEvent==SOAR_LDBCLICK && pMsg->sourceWnd->getType()==LWNDT_ITEM_SEGMENT )
			{
				
				d_selectedRowIndex=((ISoarItemBase*)pMsg->sourceWnd)->getIndex();
				d_selectedColIndex=0;
			}
			if(pMsg->mouseEvent==SOAR_LCLICK_UP && pMsg->sourceWnd->getType()==LWNDT_COLUMNITEM_SEGMENT ||
				pMsg->mouseEvent==SOAR_RCLICK_UP && pMsg->sourceWnd->getType()==LWNDT_COLUMNITEM_SEGMENT 
				){
				d_selectedRowIndex=((ISoarColumnItemBase*)pMsg->sourceWnd)->getIndex();//还可以知道是哪列
				d_selectedColIndex=((ISoarColumnItemBase*)pMsg->sourceWnd)->getsubIndex();
			}
			//如果行发生变化
			if(tRoe != d_selectedRowIndex){
				SOARMSG leeMsg;
				leeMsg.message =SOAR_SELCHANGED;
				leeMsg.mouseEvent =pMsg->mouseEvent;
				leeMsg.sourceWnd =this;
				leeMsg.routeWnd=d_OwnerWnd?d_OwnerWnd:d_parent;
				leeMsg.targetWnd =d_OwnerWnd?d_OwnerWnd:d_parent;
				leeMsg.wParam =d_selectedRowIndex;
				leeMsg.lParam =d_selectedColIndex;
				leeMsg.Data =(LPVOID)pMsg->sourceWnd;
				CSoarRoot::getSingletonPtr()->addOfflineMsg(leeMsg);
				return 1;
			}
			else if (tCol!=d_selectedColIndex){
				SOARMSG leeMsg;
				leeMsg.message =SOAR_SELITEMCOLUMNCHANGED;
				leeMsg.mouseEvent =pMsg->mouseEvent;
				leeMsg.sourceWnd =this;
				leeMsg.routeWnd=d_OwnerWnd?d_OwnerWnd:d_parent;
				leeMsg.targetWnd =d_OwnerWnd?d_OwnerWnd:d_parent;
				leeMsg.wParam =d_selectedRowIndex;
				leeMsg.lParam =d_selectedColIndex;
				leeMsg.Data =(LPVOID)pMsg->sourceWnd;
				CSoarRoot::getSingletonPtr()->addOfflineMsg(leeMsg);
				return 1;
			}
				
				return 1;
		}
		
		if (pMsg->mouseEvent==SOAR_XWHEEL  )
		{
			SOARMSG *pMsg=(SOARMSG*)wParam;
			short  zDelta   = 0;  
			if(pMsg->sourceWnd)
			{
				 zDelta   =(short)   HIWORD((pMsg->wParam))+1;
			}
			else if(pMsg->sourceWnd->getType()>=LWNDT_SEGMENT)
			{
				 zDelta   =(short)   HIWORD(((ISoarItemBase*)pMsg->sourceWnd)->getIndex())+1;  
			}
			 
			d_curStartRowIndex +=zDelta>0?-1:1;
			d_curStartRowIndex=d_curStartRowIndex>=0?d_curStartRowIndex:0;
			
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
		return 0;
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
		return 0;
	}
	return CSoarRoot::getSingletonPtr()->SoarDefWndProc(uMsg,wParam,lParam);//留系统底层处理
}
bool CLeeListViewWnd::getVsibleRange(int & nRow,int& nCol)
{
	RECT rc =getClientRect();
	//RECT rc =getCliperRect();
	int nW = rc.right-rc.left;
	int nH =rc.bottom -rc.top;
	nRow =nH/d_nItemHeight;
	//以下意见由剪切实现
	if(d_headerCtrl)
		nCol =d_headerCtrl->getItemCount();
	else
		nCol =nW/d_nItemWidth;
	//确保有一项显示
	nCol=nCol>0?nCol:1;
	return false;
}
int CLeeListViewWnd::getSelectedItemIndex(void)
{
	return d_selectedRowIndex;
}
void CLeeListViewWnd::setCurSel(int islot)
{
	d_selectedRowIndex =islot;
}
int CLeeListViewWnd::getItemCount(void)
{
	return d_commItems.size();
}
//
RECT CLeeListViewWnd::getClientRect(void) 
{
	return CSoarWnd::getClientRect();//获取父窗口包括非客户区;
}
int CLeeListViewWnd::CalcColumnStartX(int nCol){
	if(d_headerCtrl==NULL){
		return nCol*d_nItemWidth;
	}
	int nHeaderItemCount =d_headerCtrl->getItemCount();
	int nColumnX =0;
	for(int i=0;i<nCol;i++){
		nColumnX +=d_headerCtrl->getItemWidth(i);
	}
	return nColumnX;
}