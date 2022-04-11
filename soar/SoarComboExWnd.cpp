#include "SoarComboExWnd.h"
#include "SoarRoot.h"

CLeeComboExWnd::CLeeComboExWnd(HWND root,ISoarEngine* Eng):CSoarWnd(root,Eng),
d_contentEdit(NULL),
d_downbutton(NULL),
d_listWnd(NULL),
d_bIsopen(false)
{
	d_wndlook="Combobox";
	d_wndtype =LWNDT::LWNDT_COMBOEX;
	d_wndlookState="Normal";
	
	
}

CLeeComboExWnd::~CLeeComboExWnd(void)
{
	
}
void CLeeComboExWnd::InitializeSegments(void)
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
	itemname.Format("LeeComboExWnd__%p__%s",this,"ccbsystembutton");
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
	itemname.Format("LeeComboExWnd__%p__%s",this,"ccbsystemEdit");
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
	//Dimension listY(listh1,DT_TOP_EDGE);
	AbsoluteDim listh(160);
	Dimension listH(listh,DT_HEIGHT);
	UDim dminW(1.0,0);
	UnifiedDim listw(dminW,DT_WIDTH);
	Dimension listW(listw,DT_WIDTH);
	itemname.Format("LeeComboExWnd__%p__%s",this,"ccbsystemPopupListBox");
	d_listWnd =(CLeePopupListViewWnd*)CSoarRoot::getSingletonPtr()->createPopupWnd(itemname,listX,listY,listW,listH,LWNDT_POPLISTVIEW,this,d_zPopupIndex+1,(LWNDSTY)(LWNDSTY_CHILD|LWNDSTY_VSCROLL));
	d_listWnd->setOwnerWnd(this);
	d_listWnd->setState(LWNDST_HIDE);
}
void CLeeComboExWnd::DrawSelf(ILeeDrawInterface *DrawFuns)
{
	if (d_parent !=NULL && d_parent->getState()<= LWNDST_HIDE)
	{
		return;
	}
	RECT rc =getWindowRect();
	RectF rcc=rc;
	RectF rcparent =getCliperRect();
	if (d_DrawEng->hasWidgetlook(d_wndlook))
	{
		//d_DrawEng->DrawWidgetlookImage(d_wndlook,d_wndlookState,rcc);
		//绘制文字
		d_DrawEng->DrawWidgetlook(d_wndlook,d_wndlookState,rcc,d_wndText,d_VertTextFormat,d_horzTextFormat,&rcparent,true);
	}
}
int CLeeComboExWnd::IsGBK(LPTSTR str)
{
	unsigned char sqChar[20];  
	sqChar[0]=*str;  
	if (sqChar[0]>=0xa1)  
		if (sqChar[0]==0xa3)   
			return 1;//全角字符  
		else          
			return 2;//汉字  
	else  
		return 0;//英文、数字、英文标点  
}
bool CLeeComboExWnd::getButtonState(void)
{
	return d_bIsopen;
}
LRESULT CLeeComboExWnd::HandleEvent ( UINT uMessage,WPARAM wp ,LPARAM lp ) 
{
	switch( uMessage )
	{
		// Make sure that while editing, the keyup and keydown messages associated with 
		// WM_CHAR messages don't go to any non-focused controls or cameras
	case WM_KEYUP:
	case WM_KEYDOWN:
		return true;
	case WM_CHAR:
		{
			switch( ( TCHAR )wp )
			{
				// Backspace
			case VK_BACK:
				{
					TCHAR ascii_code = wp;
					if (d_wndText.length()>=2)
					{
						WCHAR *utf8 =(WCHAR*)(d_wndText.data()+d_wndText.length()-2);
						if (IsGBK((LPTSTR)utf8)==2)
						{
							d_wndText= d_wndText.substr(0,d_wndText.length()-2);
						}
						else
						{
							d_wndText= d_wndText.substr(0,d_wndText.length()-1);
						}
						
					}
					else
					{
						d_wndText= d_wndText.substr(0,d_wndText.length()-1);
					}
					//先取最后2个字符，判断是否为中文
					
					return true;
					break;
				}

			case 24:        // Ctrl-X Cut
			case VK_CANCEL: // Ctrl-C Copy
				{
					//CopyToClipboard();

					// If the key is Ctrl-X, delete the selection too.
					if( ( CHAR )wp == 24 )
					{
					//	DeleteSelectionText();
					}

					break;
				}

				// Ctrl-V Paste
			case 22:
				{
					//PasteFromClipboard();
					break;
				}

				// Ctrl-A Select All
			case 1:
				
				break;

			case VK_RETURN:
				break;
			default:
				{
					
				}
			}
			char ascii_code = wp;
			d_wndText.Format("%s%c",d_wndText.c_str(),ascii_code);
			//RECT rc =getWindowRect();
			//::InvalidateRect(d_rootWnd,&rc,0);
			return true;
		}
	}//switch
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
	if (uMessage == SOAR_COMMAND)
	{
		SOARMSG *leeMsg=(SOARMSG*)wp;
		if (leeMsg->mouseEvent==SOAR_LCLICK_UP && leeMsg->sourceWnd ==d_downbutton
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
	return CSoarWnd::HandleEvent(uMessage,wp,lp);
}
CLeeString CLeeComboExWnd::getText(void)
{
	return d_contentEdit->getText();
}
void CLeeComboExWnd::setText(const CLeeString &name)
{
	d_contentEdit->setText(name);
}
DWORD CLeeComboExWnd::getItemID(int iIndex)
{
	return d_listWnd->getItemID(iIndex,0) ;
}
 CLeeString CLeeComboExWnd::getItemString(int iIndex)
 {
	 return d_listWnd->getItemString(iIndex,0) ;
 }
LPVOID CLeeComboExWnd::getItemData(int iIndex)
{
	return d_listWnd->getItemData(iIndex,0) ;
}
int CLeeComboExWnd::getSelectedItemIndex(void)
{
	return d_listWnd->getSelectedItemIndex();
}
void CLeeComboExWnd::setCurSel(int dwIdex)
{
	if (d_listWnd)
	{
		d_listWnd->setCurSel(dwIdex);
		CLeeString str =d_listWnd->getItemString(dwIdex,0);
		d_contentEdit->setText(str);
		d_bIsopen =false;
		d_listWnd->setState(LWNDST_HIDE);
		SOARMSG leeMsg;
		leeMsg.message =SOAR_SELCHANGED;
		leeMsg.mouseEvent=SOAR_LCLICK_UP;
		leeMsg.sourceWnd =this;
		leeMsg.targetWnd =d_parent;
		leeMsg.wParam =dwIdex;
		leeMsg.lParam =dwIdex;
		leeMsg.msgSourceTag = SOAR_MSG_TRANSFER;
		CSoarRoot::getSingletonPtr()->addOfflineMsg(leeMsg);
	}
}
int CLeeComboExWnd::addItem(const CLeeString & str,int nID,LPVOID data)
{
	return d_listWnd->addRow(str,nID,data) ;
}
int CLeeComboExWnd::getItemCount(void)
{
	return d_listWnd->getItemCount();
}
