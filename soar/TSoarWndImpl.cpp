#include "TSoarWndImpl.h"

template<typename T>
TSoarWndImpl<T>::TSoarWndImpl(HWND root,ISoarEngine* Eng):
d_rootWnd(root),
d_DrawEng(Eng),
d_parent(NULL),
d_zIndex(2),
d_zPopupIndex(1),
d_wndState(LWNDST_SHOW),
d_wndlook("DefaultClient"),//DefaultWindowFrame
d_wndlookState("Normal"),//皮肤状态
p_EventHandler(NULL),
d_wndtype(LWNDT_WND),
d_wndStyle(LWNDSTY_CHILD),
d_isPopupWnd(false),
p_menubar(NULL),
p_titlebar(NULL),
p_HScrollSegment(NULL),
p_VScrollSegment(NULL),
d_scrollSegmentWidth(15),
d_scrollSegmentHeight(15),
d_TitleBarSegmentHeight(15),
d_MenuBarSegmentHeight(15),
d_GroupBoxTitleHeight(20)
{
}

template<typename T>
TSoarWndImpl<T>::~TSoarWndImpl(void)
{
	LeeWndPtrMap::iterator it =d_WndPtrs.begin();
	ISoarWnd * pWnd=NULL;
	while (it != d_WndPtrs.end())
	{
		pWnd =(it->second);
		d_WndPtrs.erase(it);
		CSoarRoot::getSingletonPtr()->destroyWnd(pWnd);
		it =d_WndPtrs.begin();
	}
}
template<typename T>
void TSoarWndImpl<T>::InitializeSegments(void)
{
	AbsoluteDim aDimx(0);
	AbsoluteDim aDimy(0);
	AbsoluteDim aDimH(d_TitleBarSegmentHeight);
	AbsoluteDim aDimW(d_TitleBarSegmentHeight);
	
	Dimension x(aDimx,DT_LEFT_EDGE);
	Dimension h(aDimH,DT_HEIGHT);
	Dimension y(aDimy,DT_TOP_EDGE);
	UDim dW(1.0,0);
	UnifiedDim uW(dW,DT_WIDTH);
	Dimension w(uW,DT_WIDTH);
	 ComponentArea rcArea;
	if(d_wndStyle & LWNDSTY_SYSTITLEBAR &&
		p_titlebar ==NULL )
	 {  
		p_titlebar=AddNonClientBar(_T("titlebar"),SOARALIGN_TOP,LWNDT_SYS_TITLEBAR,d_TitleBarSegmentHeight);
		rcArea.d_left= x;
		rcArea.d_top =y;
		rcArea.d_right_or_width=w;
		rcArea.d_bottom_or_height=h;
		//p_titlebar->setArea(rcArea);
		UDim dmin(1.0,-d_TitleBarSegmentHeight * 3);
		UDim dmax(1.0,-d_TitleBarSegmentHeight * 2);
		UDim dclose(1.0,-d_TitleBarSegmentHeight);
		
		w.setBaseDimension(aDimW);
		w.setDimensionType(DT_WIDTH);
		h.setBaseDimension(aDimH);
		h.setDimensionType(DT_HEIGHT);
		y.setBaseDimension(aDimy);
		y.setDimensionType(DT_TOP_EDGE);
		
		uW.setBaseDimension(dmin);
		uW.setDimensionType(DT_WIDTH);
		x.setBaseDimension(uW);
		x.setDimensionType(DT_X_POSITION);

		SoarSegmentItem its;
		rcArea.d_left= x;
		rcArea.d_top =y;
		rcArea.d_right_or_width=w;
		rcArea.d_bottom_or_height=h;
		its.d_area=rcArea;
		its.d_nId=SOAR_SYSMIN;
		its.d_wndlook="systemMin";
		its.d_wndlookState="Normal";
		p_titlebar->AddItem(its);
		//UnifiedDim ux1(dmax,DT_WIDTH);
	    //Dimension x2(ux1,DT_X_POSITION);
		//x.setBaseDimension(ux);
		//x.setDimensionType(DT_X_POSITION);
		uW.setBaseDimension(dmax);
		uW.setDimensionType(DT_WIDTH);
		x.setBaseDimension(uW);
		x.setDimensionType(DT_X_POSITION);
		rcArea.d_left=x;
		its.d_area=rcArea;
		its.d_nId=SOAR_SYSMAX;
		its.d_wndlook="systemMax";
		its.d_wndlookState="Normal";
		p_titlebar->AddItem(its);
		uW.setBaseDimension(dclose);
		uW.setDimensionType(DT_WIDTH);
		x.setBaseDimension(uW);
		x.setDimensionType(DT_X_POSITION);
		rcArea.d_left=x;
		its.d_area=rcArea;
		its.d_nId=SOAR_SYSCLOSE;
		its.d_wndlook="systemClose";
		its.d_wndlookState="Normal";
		p_titlebar->AddItem(its);
	}
	if(d_wndStyle & LWNDSTY_SYSMENUBAR &&
		NULL == p_menubar)
	{
		int menubarhoff =d_MenuBarSegmentHeight;
		if(p_titlebar)
		{
			RECT rct=p_titlebar->getBarRect();
			int h =rct.bottom-rct.top;
			menubarhoff = h ;
		}

		aDimx.setValue(0);
		aDimy.setValue(menubarhoff);
		aDimH.setValue(d_MenuBarSegmentHeight);
		aDimW.setValue(100);
	
		//Dimension x(aDimx,DT_LEFT_EDGE);
		x.setBaseDimension(aDimx);
		x.setDimensionType(DT_LEFT_EDGE);
		//Dimension h(aDimH,DT_HEIGHT);
		h.setBaseDimension(aDimH);
		h.setDimensionType(DT_HEIGHT);
		//Dimension y(aDimy,DT_TOP_EDGE);
		y.setBaseDimension(aDimy);
		y.setDimensionType(DT_TOP_EDGE);
		
		uW.setBaseDimension(dW);
		uW.setDimensionType(DT_WIDTH);
		w.setBaseDimension(uW);
		w.setDimensionType(DT_X_POSITION);
		p_menubar=AddNonClientBar(_T("SoarMenuBar"),SOARALIGN_TOP,LWNDT_SYS_MENUBAR,d_MenuBarSegmentHeight);
		 ComponentArea rcArea;
		 rcArea.d_left= x;
		rcArea.d_top =y;
		rcArea.d_right_or_width=w;
		rcArea.d_bottom_or_height=h;
		// p_menubar->setArea(rcArea);
	}
	 RECT rc = getWindowRect();
	 rc.bottom =rc.top =rc.left=rc.right=0;
	 this->CalcClientRec(rc);
	if(d_wndStyle & LWNDSTY_HSCROLL &&
		NULL == p_HScrollSegment)
	{   aDimx.setValue(0);
		aDimy.setValue(0);
		aDimH.setValue(d_scrollSegmentHeight);
		aDimW.setValue(d_scrollSegmentWidth);
	
		x.setBaseDimension(aDimx);
		x.setDimensionType(DT_LEFT_EDGE);
		h.setBaseDimension(aDimH);
		h.setDimensionType(DT_HEIGHT);
		UDim dsh(1.0,rc.bottom-d_scrollSegmentHeight);
		UDim dsw(1.0,rc.right-d_scrollSegmentWidth);
		uW.setBaseDimension(dsh);
		uW.setDimensionType(DT_HEIGHT);
		y.setBaseDimension(uW);
		y.setDimensionType(DT_Y_POSITION);

		uW.setBaseDimension(dsw);
		uW.setDimensionType(DT_WIDTH);
		w.setBaseDimension(uW);
		w.setDimensionType(DT_WIDTH);
		 rcArea.d_left= x;
		rcArea.d_top =y;
		rcArea.d_right_or_width=w;
		rcArea.d_bottom_or_height=h;
		p_HScrollSegment = new CSoarScrollSegment(this,d_DrawEng,SOARALIGN_RIGHT,false);
		p_HScrollSegment->setArea(rcArea);
	}
	if(d_wndStyle & LWNDSTY_VSCROLL &&
		NULL == p_VScrollSegment)
	{
		p_VScrollSegment = new CSoarScrollSegment(this,d_DrawEng, SOARALIGN_BOTTOM,true);
		UDim dsw(1.0,rc.right-d_scrollSegmentWidth);
		uW.setBaseDimension(dsw);
		uW.setDimensionType(DT_WIDTH);
		w.setBaseDimension(uW);
		w.setDimensionType(DT_LEFT_EDGE);
		 rcArea.d_left= w;
		 UDim dh(1.0,rc.bottom-d_scrollSegmentHeight-rc.top);
		//UnifiedDim dx(dh,DT_HEIGHT);
	//Dimension h12(dx,DT_HEIGHT);
		uW.setBaseDimension(dh);
		uW.setDimensionType(DT_HEIGHT);
		h.setBaseDimension(uW);
		h.setDimensionType(DT_HEIGHT);
	aDimW.setValue(d_scrollSegmentWidth);
	w.setBaseDimension(aDimW);
	w.setDimensionType(DT_WIDTH);
	aDimy.setValue(rc.top);
	y.setBaseDimension(aDimy);
	y.setDimensionType(DT_Y_POSITION);
	 rcArea.d_top =y;
	 rcArea.d_right_or_width=w;
	rcArea.d_bottom_or_height=h;
	p_VScrollSegment->setArea(rcArea);
	}
		
}
template<typename T>
HWND TSoarWndImpl<T>::getRootWnd(void)
{
	return d_rootWnd;
}
template<typename T>
LWNDT TSoarWndImpl<T>::getType(void)
{
	return d_wndtype;
}
template<typename T>
RECT TSoarWndImpl<T>::getClientRect(void) 
{
	RECT rcTmp;
	//窗口最大化
	/*if (d_wndState==LWNDST::LWNDST_MAX)
	{
		if (d_parent !=NULL)
		{
			rcTmp=d_parent->getClientRect();
		}
		else{
			GetClientRect(d_rootWnd,&rcTmp);}
		RectF rc=rcTmp;
		rc=d_area.getRect(rcTmp);
		this->CalcClientRec(rcTmp);
		return rcTmp;
	}*/
	if (d_parent ==NULL)
	{
		::GetClientRect(d_rootWnd,&rcTmp);
		RectF rc=rcTmp;
		rc=d_area.getRect(rcTmp);
		RECT rt;
		rc.toWindowRect(rt);
		this->CalcClientRec(rt);
		return rt;
	}
	RECT rcParent;
	rcParent =d_parent->getClientRect();//获取父窗口出去非客户区的区域
	RECT rc;
	RectF rcCur;
	rcCur=d_area.getRect(rcParent);
	rcCur.toWindowRect(rc);
	this->CalcClientRec(rc);
	return rc;
}
template<typename T>
RECT TSoarWndImpl<T>::getWindowRect(void) 
{
	RECT rcTmp;
	//占满父窗口客户端
	/*if (d_wndState==LWNDST::LWNDST_MAX)
	{
		if (d_parent !=NULL)
		{
			rcTmp=d_parent->getClientRect();
		}
		else{
			::GetClientRect(d_rootWnd,&rcTmp);}
		RectF rcCur;
		rcCur=d_area.getRect(rcTmp);
		rcCur.toWindowRect(rcTmp);
		return rcTmp;
	}*/
	//顶层窗口的情况
	if (d_parent ==NULL)
	{
		::GetClientRect(d_rootWnd,&rcTmp);
		 RectF rc=rcTmp;
	    rc=d_area.getRect(rcTmp);
		RECT rt;
	    rc.toWindowRect(rt);
		return rt;
	}
	//有父窗口的情况
	RECT rcParent;
	rcParent =d_parent->getClientRect();
	RECT rc;
	RectF rcCur;
	rcCur=d_area.getRect(rcParent);
	rcCur.toWindowRect(rc);
	if (d_wndState==LWNDST::LWNDST_TItleBarOnly)
	{
		rc.bottom=rc.top+d_TitleBarSegmentHeight;
	}
   return rc;
}

template<typename T>
void TSoarWndImpl<T>::setArea(ComponentArea& area)
{
	d_area =area;
}

template<typename T>
void TSoarWndImpl<T>::setArea(RECT& rc)
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

template<typename T>
void TSoarWndImpl<T>::getArea(ComponentArea& area)
{
	area =d_area;
}
//传入的必须是偏移
template<typename T>
void TSoarWndImpl<T>::setWindowPos(int x,int y)
{
	int xPos = x; int yPos =y;
	RECT rc =getWindowRect();
	RECT rcPre=rc;
	int W =rc.right -rc.left;
	int H =rc.bottom -rc.top;
	rc.left =xPos ;
	rc.top =yPos;
	rc.right =rc.left+W;
	rc.bottom=rc.top+H;
	AbsoluteDim adim(0.0);
	adim.setValue(xPos);
	d_area.d_left.setBaseDimension(adim);
	d_area.d_left.setDimensionType(DT_LEFT_EDGE);
	AbsoluteDim ydim(0.0);
	ydim.setValue(yPos);
	d_area.d_top.setBaseDimension(ydim);
	d_area.d_top.setDimensionType(DT_TOP_EDGE);
}

template<typename T>
void TSoarWndImpl<T>::setWindowHeight(int h) 
{
	AbsoluteDim adim(0.0);
	adim.setValue(h);
	d_area.d_bottom_or_height.setBaseDimension(adim);
	d_area.d_bottom_or_height.setDimensionType(DT_HEIGHT);
}

template<typename T>
void TSoarWndImpl<T>::setWindowWidth(int w) 
{
	AbsoluteDim adim(0.0);
	adim.setValue(w);
	d_area.d_right_or_width.setBaseDimension(adim);
	d_area.d_right_or_width.setDimensionType(DT_WIDTH);
}

template<typename T>
void TSoarWndImpl<T>::setWindowHeight(Dimension h) 
{
	d_area.d_bottom_or_height = h;
}

template<typename T>
void TSoarWndImpl<T>::setWindowWidth(Dimension w) 
{
	d_area.d_right_or_width = w;
}

template<typename T>
ISoarWnd* TSoarWndImpl<T>::GetParent(void)
{
	return d_parent;
}
//获取剪切视图
template<typename T>
RECT TSoarWndImpl<T>::getCliperRect(void)
{
	if (d_parent==NULL)
	{
		RECT rc;
		GetClientRect(d_rootWnd,&rc);
		 RectF rcCur;
	rcCur=d_area.getRect(rc);
	rcCur.toWindowRect(rc);
		return rc;
	}
	//特殊窗口：弹出窗口
	if (d_zIndex==0)
	{
		return  d_parent->getWindowRect(); 
	}
	RECT rcCur =d_parent->getClientRect();
	RectF rccCur =rcCur;
	RECT rcP =d_parent->getCliperRect();
    
	RectF rccP =rccCur.Intersection(rcP);
	RECT rc;
	rccP.toWindowRect(rc);
	return rc;
}
template<typename T>
void TSoarWndImpl<T>::setParent(ISoarWnd * lParent) 
{
	//先解除之前的关系
	if (d_parent)
	{
		d_parent->eraseChildWnd(this);
	}
	d_parent =lParent?lParent:CSoarRoot::getSingletonPtr()->getSoarSheet();
	d_parent->addChildWnd(this);
	//改变子控件索引
	if (isPopupWnd())
	{
		setPopupZindex(d_zPopupIndex);
	}
	else
	{
		setZindex(d_zIndex);
	}
}

template<typename T>
INT TSoarWndImpl<T>::setZindex(INT dwZindex) 
{
	int tmp =d_zIndex;
	d_zIndex =dwZindex;
	//通知Sheet修改层次
	//层次也要跟着改变
	CSoarRoot::getSingleton().NotifyChildZIndexChanged(this,dwZindex,tmp);
	LeeWndPtrMap::iterator it =d_WndPtrs.begin();
	ISoarWnd * pWnd=NULL;
	while (it != d_WndPtrs.end())
	{
		pWnd =(it->second);
		//必须让子窗体在父窗口的上一层
		if ( pWnd->isPopupWnd()==false)//只设置普通窗口
		{
			pWnd->setZindex(dwZindex+1);
			//层次也要跟着改变
			CSoarRoot::getSingleton().NotifyChildZIndexChanged(pWnd,dwZindex+1,tmp+1);
		}
		++it;
	}
	return tmp ;
}

template<typename T>
INT TSoarWndImpl<T>::getZindex(void)
{
	return d_zIndex;
}
template<typename T>
INT TSoarWndImpl<T>::setPopupZindex(INT dwZindex) 
{
	int tmp =d_zPopupIndex;
	d_zPopupIndex =dwZindex;
	//通知Sheet修改层次
	//层次也要跟着改变
	CSoarRoot::getSingleton().NotifyChildZIndexChanged(this,dwZindex,tmp);
	LeeWndPtrMap::iterator it =d_WndPtrs.begin();
	ISoarWnd * pWnd=NULL;
	while (it != d_WndPtrs.end())
	{
		pWnd =(it->second);
		//必须让子窗体在父窗口的上一层
		if (pWnd->isPopupWnd())
		{
			pWnd->setPopupZindex(dwZindex+1);
			//层次也要跟着改变
			CSoarRoot::getSingleton().NotifyChildZIndexChanged(pWnd,dwZindex+1,tmp+1);
		}
		
		++it;
	}
	return tmp;
}
template<typename T>
INT TSoarWndImpl<T>::getPopupZindex(void)
{
	return d_zPopupIndex;
}
template<typename T>
CLeeString TSoarWndImpl<T>::getText(void)
{
	return d_wndText;
}
template<typename T>
 void TSoarWndImpl<T>::setText(CLeeString name)
{
	d_wndText =name;
}
 template<typename T>
 void TSoarWndImpl<T>::setTitle(CLeeString name)
{
	if(p_titlebar)
	  p_titlebar->setTitle(name);
}
 template<typename T>
void TSoarWndImpl<T>::setWndName(CLeeString name) 
{
	d_wndName = name ;
}
template<typename T>
CLeeString TSoarWndImpl<T>::getWndName(void)
{
	return d_wndName ;
}
template<typename T>
 void TSoarWndImpl<T>::setState(LWNDST state) 
 {
	 if (d_wndState != state)
	 {
		d_wndState =state;
		 if (state==LWNDST::LWNDST_TItleBarOnly)
		 {
			 LeeWndPtrMap::iterator it =d_WndPtrs.begin();
			 while (it != d_WndPtrs.end())
			 {
				if((it->second)->getType() != LWNDT_SYS_TITLEBAR)
				 (it->second)->setState(LWNDST_HIDE);
				++it;
			 }
		 }
		 else if(state!= LWNDST_MAX )
		 {
			 LeeWndPtrMap::iterator it =d_WndPtrs.begin();
			 while (it != d_WndPtrs.end())
			 {
				 if (d_isPopupWnd && //本身是弹出窗口
					 it->second->getType() != LWNDT_POPLISTBOX &&
					 it->second->getType() != LWNDT_POPLISTVIEW
					 )//不是主体的不设置
				 {
					 (it->second)->setState(state);
				 }
				 else//本身不是弹出窗口，只对非弹出窗口设置
				 {
					 if((it->second)->isPopupWnd()==false)
						 (it->second)->setState(state);
				 }

				 ++it;
			 }
		 }
	 }
	  
 }
 template<typename T>
 LWNDST TSoarWndImpl<T>::getState(void)
 {
	 return d_wndState;
 }
 template<typename T>
 CLeeString TSoarWndImpl<T>::getDreamLook(void)
 {
	 return d_wndlook;
 }
 template<typename T>
 void TSoarWndImpl<T>::setDreamLook(CLeeString name)
 {
	 d_wndlook =name;
 }
 template<typename T>
 void TSoarWndImpl<T>::attachRoot(HWND root)
 {
	 d_rootWnd =root;
 }
 template<typename T>
void TSoarWndImpl<T>::checklookState(POINT &pt)
 {
	 RECT rc =getWindowRect();//取得相对于Root窗口的偏移
	 CLeeString state=d_wndlookState;
	 if (::PtInRect(&rc,pt))
	 {
		 d_wndlookState ="Hover";
	 }
	 else
		 d_wndlookState ="Normal";
 }
 //////////////////////////////////////////////////////////////////////////
template<typename T>
 void TSoarWndImpl<T>::DrawSelf(ILeeDrawInterface *DrawFuns) 
{
	if (getState()<= LWNDST_HIDE)
	{
		return;
	}
	RECT rc =getWindowRect();
	RectF rcc=rc;
	RectF rcparent =getCliperRect();
	//主窗口绘制
	if (d_DrawEng->hasWidgetlook(d_wndlook))
	{
		d_DrawEng->DrawWidgetlook(d_wndlook,d_wndlookState,rcc,d_wndText,&rcparent,true);
		d_DrawEng->DrawWidgetlookFrame("DefaultWindowFrame",d_wndlookState,rcc,&rcparent,true);
	}
	////子窗口绘制
	
	
	//
	if( p_VScrollSegment !=NULL)
	{
		p_VScrollSegment->DrawSelf(DrawFuns);
	}
	if( p_HScrollSegment !=NULL)
	{
		p_HScrollSegment->DrawSelf(DrawFuns);
	}
	//其他bar
	DrawNonClientBars(DrawFuns);
	//绘制弹出窗口(属于Sheet级别由Sheet统一绘制2018-3-17 17:38)
}
 //事件处理
 template<typename T>
 LRESULT  TSoarWndImpl<T>::HandleEvent ( ISoarWnd* pOwner,UINT uMsg ,WPARAM wp ,LPARAM lp )
 {
	 if (p_EventHandler)
	 {
		 LRESULT lr =0;
		 if(p_EventHandler->CallbackEvent(this,uMsg,wp,lp,lr) )
			 return lr;
	 }
	 return  this->HandleEvent(uMsg,wp,lp);
 }
 //处理内部信息
 template<typename T>
LRESULT TSoarWndImpl<T>::HandleEvent ( UINT uMsg ,WPARAM wParam ,LPARAM lParam) 
 {
	 if (uMsg== SOAR_COMMAND)
	{
		SOARMSG* pMsg=(SOARMSG*)wParam;
		UINT nID =LOWORD(pMsg->wParam);
		if (nID == SOAR_SYSMIN &&
			pMsg->sourceWnd==this &&
			HIWORD(pMsg->wParam)==BN_CLICKED && 
			pMsg->mouseEvent==SOAR_LCLICK_DOWN)
		{
			setState(LWNDST_HIDE);
			if( CSoarRoot::getSingletonPtr()->CheckIsSoarMainWnd(this) )
				::PostMessage(d_rootWnd,WM_QUIT,0,0);
			return 0;
		}
		if (nID == SOAR_SYSMAX && 
			pMsg->sourceWnd==this &&
			HIWORD(pMsg->wParam)==BN_CLICKED && pMsg->mouseEvent==SOAR_LCLICK_DOWN)
		{
			if(getState()==LWNDST_MAX)
			{
				setState(LWNDST_RESTORE);
			}
			else
				setState(LWNDST_MAX);
			return 0;
		}
		if (nID == SOAR_SYSCLOSE &&
			pMsg->sourceWnd==this &&
			HIWORD(pMsg->wParam)==BN_CLICKED && pMsg->mouseEvent==SOAR_LCLICK_DOWN)
		{
			//作为主窗口的话，退出整个程序
			if( CSoarRoot::getSingletonPtr()->CheckIsSoarMainWnd(this) )
				::PostMessage(d_rootWnd,WM_QUIT,0,0);
			if(d_bModalState){//是模态兑换框
				CSoarRoot::getSingletonPtr()->EndModal();
			}
			//正常的话：0-设置自动销毁 delete 1-没有设置，则隐藏
			if(d_bAutoDestroy){
				CSoarRoot::getSingletonPtr()->destroyWnd( this);
			}
			else{
				this->setState(LWNDST_HIDE);
			}
			return 0;
		}
	 }
	 return CSoarRoot::getSingletonPtr()->SoarDefWndProc(uMsg,wParam,lParam);//留系统底层处理
 }
template<typename T>
void TSoarWndImpl<T>::HandleMenuEvent(int nPos,int nID,SOARMSG * pMsgInfo)
{
	;
}
template<typename T>
ISoarEventHandler* TSoarWndImpl<T>::RegisterEventHandler( ISoarEventHandler * pEventHandler) 
{
	ISoarEventHandler * tmp = p_EventHandler;
	p_EventHandler =pEventHandler;
	return tmp;
}
template<typename T>
ISoarWnd* TSoarWndImpl<T>::getChildWnd(const CLeeString & name)
{
	LeeWndPtrMap::iterator it =d_WndPtrs.find(name);
	if (it !=d_WndPtrs.end())
	{
		return (*it).second;
	}
	return this;
}
template<typename T>
bool TSoarWndImpl<T>::addChildWnd(ISoarWnd* pWnd)
{
	LeeWndPtrMap::iterator it;
	//看是弹出的还是普通
	it =d_WndPtrs.find(pWnd->getWndName());
	if (it !=d_WndPtrs.end())
	{
		return true;
	}
	d_WndPtrs[pWnd->getWndName()] =pWnd;
	return true;
}
template<typename T>
bool TSoarWndImpl<T>::eraseChildWnd(ISoarWnd* pWnd)
{
	LeeWndPtrMap::iterator it =d_WndPtrs.find(pWnd->getWndName());
	if (it !=d_WndPtrs.end())
	{
		d_WndPtrs.erase(it);
		return true;
	}
	return false;
}
template<typename T>
LWNDSTY TSoarWndImpl<T>::getWndStyle(void) 
{
	return d_wndStyle ;
}
template<typename T>
bool  TSoarWndImpl<T>::setWndStyle(LWNDSTY lstyle) 
{
	d_wndStyle = lstyle;
	//d_wndStyle=(LWNDSTY)(d_wndStyle|LWNDSTY_SYSTITLEBAR);
	return true;
}

template<typename T>
 bool   TSoarWndImpl<T>::isPopupWnd(void) 
{
	return d_isPopupWnd;
}
 template<typename T>
 void   TSoarWndImpl<T>::setWndPopup(bool bPopup)
{
	d_isPopupWnd = bPopup;
}
 //////////////////////////////////////////////////////////////////////////
 template<typename T>
 ISoarBar* TSoarWndImpl<T>::getMenuBar(void)
 {
	 return p_menubar;
 }
 template<typename T>
 ISoarBar* TSoarWndImpl<T>::getTitleBar(void)
 {
	 return p_titlebar;
 }
 template<typename T>
 ISoarScroll* TSoarWndImpl<T>::getScrollSegment(bool vert)
 {
	 if (vert)
	 {
		 return p_VScrollSegment;
	 }
	 return p_HScrollSegment;
 }
 template<typename T>
 int TSoarWndImpl<T>::setScrollInfo(int width,int height)
 {
	 d_scrollSegmentHeight =height;
	 d_scrollSegmentWidth =width;
	 return 0;
 }
 template<typename T>
 int TSoarWndImpl<T>::getScrollInfo(bool bHeight)
 {
	 if (bHeight)
	 {
		 return d_scrollSegmentHeight ;
	 }
	 return d_scrollSegmentWidth ;
 }
 template<typename T>
 int TSoarWndImpl<T>::setTitleBarSegment(int nHeight)
 {
	 int tmp =d_TitleBarSegmentHeight;
	 d_TitleBarSegmentHeight =nHeight;
	 return tmp;
 }
 template<typename T>
 int TSoarWndImpl<T>::getTitleBarSegment(void)
 {
	return d_TitleBarSegmentHeight;
 }
 template<typename T>
 int TSoarWndImpl<T>::setMenuBarSegment(int nHeight)
 {
	 int tmp =d_MenuBarSegmentHeight;
	 d_MenuBarSegmentHeight =nHeight;
	 return tmp;
 }
 template<typename T>
 int TSoarWndImpl<T>::getMenuBarSegment(void)
 {
	 return d_MenuBarSegmentHeight;
 }
 //////////////////////////////////////
 template<typename T>
 RECT TSoarWndImpl<T>::CalcClientRec(RECT & rcWnd)
 {
	 if (d_wndState==LWNDST::LWNDST_TItleBarOnly)
	{
		rcWnd.bottom=rcWnd.top+15;
		return rcWnd;
	}
	 int h = 0;
	 RECT rct;
		if (p_HScrollSegment)
		{
			rct=p_HScrollSegment->getBarRect();
			h =rct.bottom-rct.top;
			rcWnd.bottom  -= h ;
		}
		if (p_VScrollSegment)
		{
			rct=p_VScrollSegment->getBarRect();
			h =rct.right-rct.left;
			rcWnd.right  -= h ;
		}
		//计算bar的情况
		ANYBARS::iterator it = d_nonclientbars.begin() ;
		while( it != d_nonclientbars.end() )
		{
			switch((*it)->getBarAlign())
			{
				case SOARALIGN_TOP:
				{
					rct=(*it)->getBarRect();
					h =rct.bottom-rct.top;
					rcWnd.top  += h ;
					break;
				}
				case SOARALIGN_BOTTOM:
				{
					rct=(*it)->getBarRect();
					h =rct.bottom-rct.top;
					rcWnd.bottom  -= h ;
					break;
				}
				case SOARALIGN_LEFT:
				{
					rct=(*it)->getBarRect();
					h =rct.right-rct.left;
					rcWnd.left  += h ;
					break;
				}
				case SOARALIGN_RIGHT:
				{
					rct=(*it)->getBarRect();
					h =rct.right-rct.left;
					rcWnd.right  -= h ;
					break;
				}
			}
			++it;
		}
		return rcWnd ;
 }
 template<typename T>
void TSoarWndImpl<T>::DrawNonClientBars(ILeeDrawInterface *DrawFuns) 
{
	ANYBARS::iterator it = d_nonclientbars.begin() ;
		while( it != d_nonclientbars.end() )
		{
			(*it)->DrawSelf(DrawFuns);
			++it;
		}
}
template<typename T>
BOOL TSoarWndImpl<T>::HandleNonClientBarsEvent( UINT umsg ,WPARAM wp ,LPARAM lp ,LRESULT &lr)
{
	ANYBARS::iterator it = d_nonclientbars.begin() ;
		while( it != d_nonclientbars.end() )
		{
			if((*it)->BarHitTest())
			{
				lr =(*it)->HandleEvent(umsg,wp,lp);
				return true;
			}
			++it;
		}
		return false;
}
template<typename T>
 ISoarBar* TSoarWndImpl<T>::AddNonClientBar(const CLeeString& elementName,SOARBARALIGN align,LWNDT classType,int nHeight_or_Width)
 {
	 //这里的便宜是相对于父窗口的，用的是相对坐标
	 RECT rc = {0,0,0};
	 this->CalcClientRec(rc);
	  AbsoluteDim aDimx(0);
	AbsoluteDim aDimy(0);
	AbsoluteDim aDimH(d_TitleBarSegmentHeight);
	AbsoluteDim aDimW(d_TitleBarSegmentHeight);
	
	Dimension x(aDimx,DT_LEFT_EDGE);
	Dimension h(aDimH,DT_HEIGHT);
	Dimension y(aDimy,DT_TOP_EDGE);
	UDim dW(1.0,0);
	UnifiedDim uW(dW,DT_WIDTH);
	Dimension W(uW,DT_WIDTH);
	UnifiedDim uH(dW,DT_HEIGHT);
	Dimension H(uH,DT_HEIGHT);
	 ComponentArea rcArea;
	 ISoarBar* newBar =NULL;
	 switch(classType)
	{
	case LWNDT_SYS_HEADERWND:
		{
			newBar =new CLeeHeaderWnd(this,d_DrawEng, align);
			break;
		}
	case LWNDT_SYS_TABCTRHEADER:
		{
			newBar =new CLeeTabCtrlHeader(this,d_DrawEng, align);
			break;
		}
		case LWNDT_SYS_TITLEBAR:
		{
			newBar = new CSoarTitleBar(this,d_DrawEng);
			break;
		}
		case LWNDT_SYS_MENUBAR:
		{
			newBar = new CSoarMenuBar(this,d_DrawEng);
			break;
		}
		/*
		case LWNDT_SYS_SEGMENT:
		{
			newBar =new CLeeStaticWnd(d_hOwner,d_DrawEngine);
			break;
		}*/
	/*case LWNDT_BUTTON:
		{
			newBar =new CLeeButtonWnd(d_hOwner,d_DrawEngine);
			break;
		}*/
	 };
	 switch(align)
	 {
	 case SOARALIGN_LEFT:
		 {
			 aDimx.setValue(rc.left);
			 aDimy.setValue(rc.top);
			 aDimH.setValue(rc.bottom - rc.top);
			 aDimW.setValue(nHeight_or_Width);
			 x.setBaseDimension(aDimx);
			 x.setDimensionType(DT_LEFT_EDGE);
			 y.setBaseDimension(aDimy);
			 y.setDimensionType(DT_TOP_EDGE);
			 h.setBaseDimension(aDimH);
			h.setDimensionType(DT_HEIGHT);
			W.setBaseDimension(aDimW);
			rcArea.d_left= x;
		rcArea.d_top =y;
		rcArea.d_right_or_width=W;
		rcArea.d_bottom_or_height=H;
		newBar->setArea(rcArea);
		break;
		 }
		 case SOARALIGN_RIGHT:
		 {
			 dW.d_scale =1.0;
			dW.d_offset =rc.right-nHeight_or_Width;
			uW.setBaseDimension(dW);
			uW.setDimensionType(DT_WIDTH);
			 aDimx.setValue(rc.left);
			 aDimy.setValue(rc.top);
			 aDimH.setValue(rc.bottom - rc.top);
			 aDimW.setValue(nHeight_or_Width);
			 y.setBaseDimension(aDimy);
			 x.setDimensionType(DT_LEFT_EDGE);
			 x.setBaseDimension(uW);
			 y.setDimensionType(DT_TOP_EDGE);
			 h.setBaseDimension(aDimH);
			h.setDimensionType(DT_HEIGHT);
			W.setBaseDimension(aDimW);
			rcArea.d_left= x;
		rcArea.d_top =y;
		rcArea.d_right_or_width=W;
		rcArea.d_bottom_or_height=H;
		newBar->setArea(rcArea);
		break;
		 }
		 case SOARALIGN_TOP:
		 {
			
			 aDimx.setValue(rc.left);
			 aDimy.setValue(rc.top);
			 aDimH.setValue(nHeight_or_Width);
			 aDimW.setValue(rc.right - rc.left);
			 x.setBaseDimension(aDimx);
			 x.setDimensionType(DT_LEFT_EDGE);
			 y.setBaseDimension(aDimy);
			 y.setDimensionType(DT_TOP_EDGE);
			 h.setBaseDimension(aDimH);
			h.setDimensionType(DT_HEIGHT);
			rcArea.d_left= x;
		rcArea.d_top =y;
		rcArea.d_right_or_width=W;
		rcArea.d_bottom_or_height=h;
		newBar->setArea(rcArea);
		break;
		 }
		 case SOARALIGN_BOTTOM:
		 {
				dW.d_scale =1.0;
				dW.d_offset =rc.bottom-nHeight_or_Width;
				uH.setBaseDimension(dW);
				uH.setDimensionType(DT_HEIGHT);
				y.setBaseDimension(uH);
				y.setDimensionType(DT_TOP_EDGE);
			 aDimx.setValue(rc.left);
			 aDimy.setValue(rc.top);
			 aDimH.setValue(nHeight_or_Width);
			 aDimW.setValue(rc.right - rc.left);
			 x.setBaseDimension(aDimx);
			 x.setDimensionType(DT_LEFT_EDGE);
			// y.setBaseDimension(aDimy);
			// y.setDimensionType(DT_TOP_EDGE);
			 h.setBaseDimension(aDimH);
			h.setDimensionType(DT_HEIGHT);
			rcArea.d_left= x;
		rcArea.d_top =y;
		rcArea.d_right_or_width=W;
		rcArea.d_bottom_or_height=h;
		newBar->setArea(rcArea);
		break;
		 }
		
 };
	 d_nonclientbars.push_back(newBar);
	 return newBar;
}
