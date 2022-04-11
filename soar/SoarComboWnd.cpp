#include "SoarComboWnd.h"
#include "SoarRoot.h"

CLeeComboWnd::CLeeComboWnd(HWND root,ISoarEngine* Eng):CSoarWnd(root,Eng),
d_contentEdit(NULL),
d_downbutton(NULL),
d_listWnd(NULL),
d_bIsopen(false)
{
	d_wndlook="Combobox";
	d_wndtype =LWNDT::LWNDT_COMBOBOX;
	d_wndlookState="Normal";
	
}

CLeeComboWnd::~CLeeComboWnd(void)
{
}
void CLeeComboWnd::InitializeSegments(void)
{
	AbsoluteDim aDimx(5);
	AbsoluteDim aDimy(5);
	AbsoluteDim aDimH(20);
	AbsoluteDim aDimW(15);
	UDim dmin(1.0,-20);

	Dimension w(aDimW,DT_WIDTH);
	Dimension h(aDimH,DT_HEIGHT);
	Dimension y(aDimy,DT_TOP_EDGE);
	UnifiedDim ux(dmin,DT_WIDTH);
	Dimension x(ux,DT_LEFT_EDGE);
	CLeeString itemname ;
	itemname.Format("LeeComboWnd__%p__%s",this,"ccbsystembutton");
	if (d_isPopupWnd)
	{
		d_downbutton =CSoarRoot::getSingletonPtr()->createPopupWnd(itemname,x,y,w,h,LWNDT_BUTTON,this,d_zPopupIndex+1);
	} 
	else
	{
		d_downbutton =CSoarRoot::getSingletonPtr()->createWnd(itemname,x,y,w,h,LWNDT_BUTTON,this,d_zIndex+1);
	}
	
	d_downbutton->setDreamLook("SpinDownButton");
	Dimension y1(aDimy,DT_TOP_EDGE);
	Dimension x1(aDimx,DT_LEFT_EDGE);
	UDim dw(1.0,-30);
	UnifiedDim ux1(dw,DT_WIDTH);
	Dimension uw(ux1,DT_WIDTH);
	UDim dh(1.0,-5);
	UnifiedDim uh1(dh,DT_HEIGHT);
	Dimension uh(uh1,DT_BOTTOM_EDGE);
	itemname.Format("LeeComboWnd__%p__%s",this,"ccbsystemEdit");
	if (d_isPopupWnd)
	{
		d_contentEdit =CSoarRoot::getSingletonPtr()->createPopupWnd(itemname,x1,y1,uw,uh,LWNDT_EDIT,this,d_zPopupIndex+1);
	}
	else
	{
		d_contentEdit =CSoarRoot::getSingletonPtr()->createWnd(itemname,x1,y1,uw,uh,LWNDT_EDIT,this,d_zIndex+1);
	}
	

	RECT rt ={0,0,0,0};
	//列表list
	AbsoluteDim listx(0);
	Dimension listX(listx,DT_X_POSITION);
	UDim dminT(1.0,0);
	UnifiedDim Ypos(dminT,DT_HEIGHT);
	Dimension listY(Ypos,DT_TOP_EDGE);
	AbsoluteDim listh1(rt.bottom);
	AbsoluteDim listh(160);
	Dimension listH(listh,DT_HEIGHT);
	UDim dminW(1.0,0);
	UnifiedDim listw(dminW,DT_WIDTH);
	Dimension listW(listw,DT_WIDTH);
	itemname.Format("LeeComboWnd__%p__%s",this,"ccbsystemPopupListBox");
	d_listWnd =(CLeePopupListbox*)CSoarRoot::getSingletonPtr()->createPopupWnd(itemname,listX,listY,listW,listH,LWNDT_POPLISTBOX,this,d_zPopupIndex+1);
	d_listWnd->setOwnerWnd(this);
	d_listWnd->setState(LWNDST_HIDE);
}
void CLeeComboWnd::DrawSelf(ILeeDrawInterface *DrawFuns)
{
	if (d_parent !=NULL && d_parent->getState()<= LWNDST_HIDE)
	{
		return;
	}
	CSoarWnd::DrawSelf(DrawFuns);
	//RECT rc =getWindowRect();
	//RectF rcc=rc;
	//RectF rcparent =getCliperRect();
	//if (d_DrawEng->hasWidgetlook(d_wndlook))
	//{
	//	//d_DrawEng->DrawWidgetlookImage(d_wndlook,d_wndlookState,rcc);
	//	//绘制文字
	//	d_DrawEng->DrawWidgetlook(d_wndlook,d_wndlookState,rcc,d_wndText,&rcparent,true);
	//}
}
bool CLeeComboWnd::getButtonState(void)
{
	return d_bIsopen;
}
LRESULT CLeeComboWnd::HandleEvent ( UINT uMessage,WPARAM wp ,LPARAM lp ) 
{
	
	if (uMessage == SOAR_COMMAND)
	{
		SOARMSG *leeMsg=(SOARMSG*)wp;
		if (leeMsg->mouseEvent==SOAR_LCLICK_DOWN &&
			leeMsg->sourceWnd ==d_downbutton
			)
		{
			if (!d_bIsopen)
			{
				d_bIsopen =true;
				d_listWnd->setState(LWNDST_SHOW);

			}
			else
			{
				d_bIsopen =false;
				d_listWnd->setState(LWNDST_HIDE);
			}

		}
		
	}
	if (uMessage == SOAR_ITEMSELECTED)
	{
		SOARMSG *leeMsg=(SOARMSG*)wp;
		if (leeMsg->mouseEvent==SOAR_LCLICK_UP && 
			leeMsg->sourceWnd !=d_downbutton && 
			leeMsg->sourceWnd != d_contentEdit)
		{
			setCurSel(((ISoarItemWnd*)leeMsg->sourceWnd)->getIndex());
		}
	}
	return CSoarWnd::HandleEvent(uMessage,wp,lp);
}
CLeeString CLeeComboWnd::getText(void)
{
	return d_contentEdit->getText();
}
void CLeeComboWnd::setText(const CLeeString & name)
{
	d_contentEdit->setText(name);
}
DWORD CLeeComboWnd::getItemID(int iIndex)
{
	return d_listWnd->getItemID(iIndex) ;
}
 CLeeString CLeeComboWnd::getItemString(int iIndex)
 {
	 return d_listWnd->getItemString(iIndex) ;
 }
LPVOID CLeeComboWnd::getItemData(int iIndex)
{
	return d_listWnd->getItemData(iIndex) ;
}
int CLeeComboWnd::getSelectedItemIndex(void)
{
	return d_listWnd->getSelectedItemIndex() ;
}
int CLeeComboWnd::addItem(const CLeeString & str,int nID,LPVOID data)
{
	return d_listWnd->addItem(str,nID,data) ;
}
void CLeeComboWnd::setCurSel(int dwIdex)
{
	if (d_listWnd)
	{
		d_listWnd->setCurSel(dwIdex);
		CLeeString str =d_listWnd->getItemString(dwIdex);
		d_contentEdit->setText(str);
		d_bIsopen =false;
		SOARMSG leeMsg;
		leeMsg.message =SOAR_SELCHANGED;
		leeMsg.mouseEvent=SOAR_LCLICK_UP;
		leeMsg.sourceWnd =this;
		leeMsg.targetWnd =d_parent;
		leeMsg.wParam =dwIdex;
		leeMsg.lParam =d_listWnd->getItemID(dwIdex);
		leeMsg.msgSourceTag = SOAR_MSG_ORIG;
		CSoarRoot::getSingletonPtr()->addOfflineMsg(leeMsg);
	}
}
int CLeeComboWnd::getItemCount(void)
{
	return d_listWnd->getItemCount();
}