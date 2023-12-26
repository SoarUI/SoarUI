#include "SoarWnd.h"
#include "SoarRoot.h"
#include "../SoarHeader/iSoareventhandler.h"
#include "../SoarHeader/leeLog.h"
#include "SoarRoot.h"
#include "../SoarHeader/iSoarmenuwnd.h"
#include"soarTitleBar.h"
#include"SoarMenuBar.h"
#include"SoarScrollSegment.h"
#include"./SoarWidgets/SoarSystemScrollSegment.h"
#include"SoarMenuWnd.h"
#include "LeeHeaderWnd.h"
#include"LeeTabCtrlHeader.h"
CSoarWnd::CSoarWnd(HWND root,ISoarEngine* Eng):
d_rootWnd(root),
d_DrawEng(Eng),
d_parent(NULL),
d_zIndex(2),
d_zPopupIndex(1),
d_wndState(LWNDST_SHOW),
d_wndVisibleState(LWNDST_SHOW),
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
d_GroupBoxTitleHeight(20),
d_windowframeborderWidth(2),
d_windowframeborderHeight(2),
d_bAutoDestroy(false),
d_bModalState(false),
d_bEnableAlphaBlend(false)
{
	d_VertTextFormat=-1;
	d_horzTextFormat=-1;
}

CSoarWnd::~CSoarWnd(void)
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
void CSoarWnd::InitializeSegments(void)
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
		p_menubar=AddNonClientBar(_T("SoarMenuBar"),SOARALIGN_TOP,LWNDT_SYS_MENUBAR,d_MenuBarSegmentHeight);
		
	}
	if(d_wndStyle & LWNDSTY_HSCROLL &&
		NULL == p_HScrollSegment)
	{  
		p_HScrollSegment=AddNonClientBar(_T("systemHScrollBarSeg"),SOARALIGN_BOTTOM,LWNDT_SYS_SCROLLBAR,d_scrollSegmentWidth);
		
	}
	if(d_wndStyle & LWNDSTY_VSCROLL &&
		NULL == p_VScrollSegment)
	{
		p_VScrollSegment =AddNonClientBar(_T("systemHScrollBarSeg"),SOARALIGN_RIGHT,LWNDT_SYS_SCROLLBAR,d_scrollSegmentWidth);
	}
		
}
HWND CSoarWnd::getRootWnd(void)
{
	return d_rootWnd;
}
LWNDT CSoarWnd::getType(void)
{
	return d_wndtype;
}
RECT CSoarWnd::CalcClientRec(RECT & rcWnd)
 {
	 if (d_wndState==LWNDST::LWNDST_TItleBarOnly)
	{
		rcWnd.bottom=rcWnd.top+ d_TitleBarSegmentHeight;
		return rcWnd;
	}
	 int h = 0;
	CalcClientTOP(h);
	rcWnd.top+=h;
	h = 0;
	CalcClientBOTTOM(h);
	rcWnd.bottom-=h;
	h = 0;
	CalcClientLEFT(h);
	rcWnd.left+=h;
	h = 0;
	CalcClientRIGHT(h);
	h += 1;
	rcWnd.right-=h;
	return rcWnd ;
 }
RECT CSoarWnd::getClientRect(void) 
{
	RECT rcTmp;
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
RECT CSoarWnd::getWindowRect(void) 
{
	RECT rcTmp;
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
//获取剪切视图
RECT CSoarWnd::getCliperRect(void)
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
		return  d_parent->getCliperRect(); 
	}
	RECT rcCur =getWindowRect();
	RectF rccCur =rcCur;
	RECT rcP =d_parent->getCliperRect();
    
	RectF rccP =rccCur.Intersection(rcP);
	RECT rc;
	rccP.toWindowRect(rc);
	return rc;
}

void CSoarWnd::setArea(ComponentArea& area)
{
	d_area =area;
}
void CSoarWnd::setArea(RECT& rc)
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
void CSoarWnd::getArea(ComponentArea& area)
{
	area =d_area;
}
//传入的是屏幕坐标
//void CSoarWnd::setWindowPosEx(int x,int y)
//{
//	RECT rcParent;
//	//顶层窗口的情况
//	if (d_parent ==NULL)
//	{
//		RECT rcTmp;
//		::GetClientRect(d_rootWnd,&rcTmp);
//		 RectF rc=rcTmp;
//	    rc=d_area.getRect(rcTmp);
//		RECT rt;
//	    rc.toWindowRect(rcParent);
//	}
//	else{
//		//有父窗口的情况
//	rcParent =d_parent->getClientRect();
//	}
//	int xPos = x-rcParent.left; int yPos =y-rcParent.top;
//	AbsoluteDim adim(0.0);
//	adim.setValue(xPos);
//	d_area.d_left.setBaseDimension(adim);
//	d_area.d_left.setDimensionType(DT_LEFT_EDGE);
//	AbsoluteDim ydim(0.0);
//	ydim.setValue(yPos);
//	d_area.d_top.setBaseDimension(ydim);
//	d_area.d_top.setDimensionType(DT_TOP_EDGE);
//}
//传入的必须是偏移(相对于父窗口左上角)
void CSoarWnd::setWindowPos(int x,int y)
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
void CSoarWnd::setWindowHeight(int h) 
{
	AbsoluteDim adim(0.0);
	adim.setValue(h);
	d_area.d_bottom_or_height.setBaseDimension(adim);
	d_area.d_bottom_or_height.setDimensionType(DT_HEIGHT);
}
void CSoarWnd::setWindowWidth(int w) 
{
	AbsoluteDim adim(0.0);
	adim.setValue(w);
	d_area.d_right_or_width.setBaseDimension(adim);
	d_area.d_right_or_width.setDimensionType(DT_WIDTH);
}
void CSoarWnd::setWindowHeight(Dimension h) 
{
	d_area.d_bottom_or_height = h;
}
void CSoarWnd::setWindowWidth(Dimension w) 
{
	d_area.d_right_or_width = w;
}
ISoarWnd* CSoarWnd::GetParent(void)
{
	return d_parent;
}

void CSoarWnd::setParent(ISoarWnd * lParent) 
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
INT CSoarWnd::setZindex(INT dwZindex) 
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
INT CSoarWnd::getZindex(void)
{
	return d_zIndex;
}
INT CSoarWnd::setPopupZindex(INT dwZindex) 
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
INT CSoarWnd::getPopupZindex(void)
{
	return d_zPopupIndex;
}
LPCTSTR CSoarWnd::getText(void)
{
	return d_wndText.c_str();
}
 void CSoarWnd::setText(const CLeeString & name)
{
	d_wndText =name;
}
 void CSoarWnd::setTitle(const CLeeString & name)
{
	if(p_titlebar)
	  p_titlebar->setTitle(name);
}
void CSoarWnd::setWndName(const CLeeString & name) 
{
	d_wndName = name ;
}
LPCTSTR CSoarWnd::getWndName(void)
{
	return d_wndName.c_str() ;
}
 void CSoarWnd::setState(LWNDST state) 
 {
	 if (d_wndState != state)
	 {
		d_wndState =state;
		 if (state == LWNDST::LWNDST_TItleBarOnly ||
			 state == LWNDST::LWNDST_MINI)
		 {
			 LeeWndPtrMap::iterator it =d_WndPtrs.begin();
			 while (it != d_WndPtrs.end())
			 {
				if((it->second)->getType() != LWNDT_SYS_TITLEBAR)
				 (it->second)->setVisibleState(LWNDST_HIDE);
				++it;
			 }
		 }
		 else 
		 {
			 LWNDST tmpstate = state;
			 if (state == LWNDST_MAX || 
				 state == LWNDST_RESTORE )
			 {
				 tmpstate = LWNDST_SHOW;
			 }
			 LeeWndPtrMap::iterator it =d_WndPtrs.begin();
			 while (it != d_WndPtrs.end())
			 {
				 if (d_isPopupWnd && //本身是弹出窗口
					 it->second->getType() != LWNDT_POPLISTBOX &&
					 it->second->getType() != LWNDT_POPLISTVIEW
					 )//不是主体的不设置
				 {
					 (it->second)->setVisibleState(tmpstate);
				 }
				 else//本身不是弹出窗口，只对非弹出窗口设置
				 {
					 if((it->second)->isPopupWnd()==false)
						 (it->second)->setVisibleState(tmpstate);
				 }

				 ++it;
			 }
		 }
	 }
	  
 }
 LWNDST CSoarWnd::getState(void)
 {
	 return d_wndState;
 }
 void CSoarWnd::setVisibleState(LWNDST state) //新增 由系统管理
 {
	 if (d_wndVisibleState != state && d_wndState > LWNDST_HIDE)
	 {
		 d_wndVisibleState = state;
		 
		LeeWndPtrMap::iterator it = d_WndPtrs.begin();
		while (it != d_WndPtrs.end())
		{
			ISoarWnd* pWnd = it->second;
			if (pWnd)
				pWnd->setVisibleState(state);
			++it;
		}
	 }
 }
 LWNDST CSoarWnd::getVisibleState(void)
 {
	 return d_wndVisibleState;
 }
 LPCTSTR CSoarWnd::getDreamLook(void)
 {
	 return d_wndlook.c_str();
 }
 void CSoarWnd::setDreamLook(const CLeeString & name)
 {
	 d_wndlook =name;
 }
 void CSoarWnd::attachRoot(HWND root)
 {
	 d_rootWnd =root;
 }
void CSoarWnd::checklookState(POINT &pt)
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
void CSoarWnd::setVertFormatting(HorizontalTextFormatting hfmt)
{
	d_horzTextFormat=hfmt;
}
void CSoarWnd::setHorzFormatting(VerticalTextFormatting vfmt){
	d_VertTextFormat= vfmt;
}
 //////////////////////////////////////////////////////////////////////////
 void CSoarWnd::DrawSelf(ILeeDrawInterface *DrawFuns) 
{
	if (getState()<= LWNDST_HIDE || 
		getVisibleState()<= LWNDST_HIDE)
	{
		return;
	}
	bool bPreState =DrawFuns->graphic_AlphaBlendState();
	DrawFuns->graphic_AlphaBlend(d_bEnableAlphaBlend);
	RECT rc =getWindowRect();
	RectF rcc=rc;
	RectF rcparent =getCliperRect();
	//主窗口绘制
	if (d_DrawEng->hasWidgetlook(d_wndlook))
	{
		d_DrawEng->DrawWidgetlook(d_wndlook,d_wndlookState,rcc,d_wndText,d_VertTextFormat,d_horzTextFormat,&rcparent,true);
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
	DrawFuns->graphic_AlphaBlend(bPreState);
}
 //Graphic operation
 bool CSoarWnd::enableAlphaBlend(bool bEnable)
 {
	 if(d_bEnableAlphaBlend != bEnable){
		 d_bEnableAlphaBlend =bEnable;
		 return true;
	 }
	 return false;
 }
 //事件处理(先让用户处理再让相关类内部处理)
 BOOL  CSoarWnd::HandleEvent ( ISoarWnd* pOwner,UINT uMsg ,WPARAM wp ,LPARAM lp, LRESULT&lr)
 {
	 if (p_EventHandler)
	 {
		 if(p_EventHandler->CallbackEvent(this,uMsg,wp,lp,lr) )
			 return true;
	 }
	 return  this->HandleEvent(uMsg,wp,lp,lr);
 }
 //处理内部信息
BOOL CSoarWnd::HandleEvent ( UINT uMsg ,WPARAM wParam ,LPARAM lParam, LRESULT& lr)
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
			lr = 0;
			return TRUE;
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
			lr = 0;
			return TRUE;
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
			lr = 0;
			return TRUE;
		}
	 }
	 lr= CSoarRoot::getSingletonPtr()->SoarDefWndProc(uMsg, wParam, lParam);//留系统底层处理
	 return TRUE;
 }
void CSoarWnd::HandleMenuEvent(int nPos,int nID,SOARMSG * pMsgInfo)
{
	;
}
ISoarEventHandler* CSoarWnd::RegisterEventHandler( ISoarEventHandler * pEventHandler) 
{
	ISoarEventHandler * tmp = p_EventHandler;
	p_EventHandler =pEventHandler;
	return tmp;
}
ISoarWnd* CSoarWnd::getChildWnd(const CLeeString & name)
{
	LeeWndPtrMap::iterator it =d_WndPtrs.find(name);
	if (it !=d_WndPtrs.end())
	{
		return (*it).second;
	}
	return this;
}
bool CSoarWnd::addChildWnd(ISoarWnd* pWnd)
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
bool CSoarWnd::eraseChildWnd(ISoarWnd* pWnd)
{
	LeeWndPtrMap::iterator it =d_WndPtrs.find(pWnd->getWndName());
	if (it !=d_WndPtrs.end())
	{
		d_WndPtrs.erase(it);
		return true;
	}
	return false;
}
LWNDSTY CSoarWnd::getWndStyle(void) 
{
	return d_wndStyle ;
}
bool  CSoarWnd::setWndStyle(LWNDSTY lstyle) 
{
	d_wndStyle=(LWNDSTY)(d_wndStyle|lstyle);
	return true;
}
 bool   CSoarWnd::isPopupWnd(void) 
{
	return d_isPopupWnd;
}
 void   CSoarWnd::setWndPopup(bool bPopup)
{
	d_isPopupWnd = bPopup;
}
bool  CSoarWnd::isModalWnd(void) {
	return d_bModalState;
}
void  CSoarWnd::setWndModal(bool bModal) {
	d_bModalState =bModal;
}
 //////////////////////////////////////////////////////////////////////////
 ISoarBar* CSoarWnd::getMenuBar(void)
 {
	 return p_menubar;
 }
 ISoarBar* CSoarWnd::getTitleBar(void)
 {
	 return p_titlebar;
 }
 ISoarBar* CSoarWnd::getScrollSegment(bool vert)
 {
	 if (vert)
	 {
		 return p_VScrollSegment;
	 }
	 return p_HScrollSegment;
 }
 int CSoarWnd::setScrollInfo(int width,int height)
 {
	 d_scrollSegmentHeight =height;
	 d_scrollSegmentWidth =width;
	 return 0;
 }
 int CSoarWnd::getScrollInfo(bool bHeight)
 {
	 if (bHeight)
	 {
		 return d_scrollSegmentHeight ;
	 }
	 return d_scrollSegmentWidth ;
 }
 int CSoarWnd::setTitleBarSegment(int nHeight)
 {
	 int tmp =d_TitleBarSegmentHeight;
	 d_TitleBarSegmentHeight =nHeight;
	 return tmp;
 }
 int CSoarWnd::getTitleBarSegment(void)
 {
	return d_TitleBarSegmentHeight;
 }
 int CSoarWnd::setMenuBarSegment(int nHeight)
 {
	 int tmp =d_MenuBarSegmentHeight;
	 d_MenuBarSegmentHeight =nHeight;
	 return tmp;
 }
 int CSoarWnd::getMenuBarSegment(void)
 {
	 return d_MenuBarSegmentHeight;
 }
 //////////////////////////////////////
 
 
void CSoarWnd::DrawNonClientBars(ILeeDrawInterface *DrawFuns) 
{
	ANYBARS::iterator it = d_nonclientbars_TOP.begin() ;
		while( it != d_nonclientbars_TOP.end() )
		{
			(*it)->DrawSelf(DrawFuns);
			++it;
		}
		it = d_nonclientbars_BOTTOM.begin() ;
		while( it != d_nonclientbars_BOTTOM.end() )
		{
			(*it)->DrawSelf(DrawFuns);
			++it;
		}
		it = d_nonclientbars_LEFT.begin() ;
		while( it != d_nonclientbars_LEFT.end() )
		{
			(*it)->DrawSelf(DrawFuns);
			++it;
		}
		it = d_nonclientbars_RIGHT.begin() ;
		while( it != d_nonclientbars_RIGHT.end() )
		{
			(*it)->DrawSelf(DrawFuns);
			++it;
		}
}
BOOL CSoarWnd::HandleNonClientBarsEvent( UINT umsg ,WPARAM wp ,LPARAM lp ,LRESULT &lr)
{
	ANYBARS::iterator it = d_nonclientbars_TOP.begin() ;
		while( it != d_nonclientbars_TOP.end() )
		{
			if((*it)->BarHitTest())
			{
				return (*it)->HandleEvent(umsg,wp,lp,lr);
			}
			++it;
		}
		it = d_nonclientbars_BOTTOM.begin() ;
		while( it != d_nonclientbars_BOTTOM.end() )
		{
			if((*it)->BarHitTest())
			{
				return (*it)->HandleEvent(umsg,wp,lp,lr);
			}
			++it;
		}
		it = d_nonclientbars_LEFT.begin() ;
		while( it != d_nonclientbars_LEFT.end() )
		{
			if((*it)->BarHitTest())
			{
				return (*it)->HandleEvent(umsg,wp,lp,lr);
			}
			++it;
		}
		it = d_nonclientbars_RIGHT.begin() ;
		while( it != d_nonclientbars_RIGHT.end() )
		{
			if((*it)->BarHitTest())
			{
				return (*it)->HandleEvent(umsg,wp,lp,lr);
			}
			++it;
		}
		return false;
}
/*
上下固定长，左右，在添加后需做调整
*/
 ISoarBar* CSoarWnd::AddNonClientBar(const CLeeString& elementName,SOARBARALIGN align,LWNDT classType,int nHeight_or_Width)
 {
	 //这里的便宜是相对于父窗口的，用的是相对坐标
	 RECT rc = {0,0,0};
	 rc=getWindowRect();
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
	 int top=0;
	 int bottom=0;
	 int left=0;
	 int right=0;
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
		case LWNDT_SYS_SCROLLBAR:
		{
			bool bVert =(align==SOARALIGN_LEFT||align==SOARALIGN_RIGHT)?true:false;
			newBar = new CSoarSystemScrollSegment(this,d_DrawEng,bVert);
			break;
		}
	
	 };
	 switch(align)
	 {
	 case SOARALIGN_LEFT:
		 {
			 CalcClientLEFT(left);
			 CalcClientTOP(top);
			 CalcClientBOTTOM(bottom);
			 aDimx.setValue(left);
			 aDimy.setValue(top);
			 
			 x.setBaseDimension(aDimx);
			 x.setDimensionType(DT_LEFT_EDGE);
			 y.setBaseDimension(aDimy);
			 y.setDimensionType(DT_TOP_EDGE);
			 UDim uH(1.0,-bottom-top);
			 UnifiedDim unH( uH,DT_HEIGHT);
			 H.setBaseDimension(unH);
			aDimW.setValue(nHeight_or_Width);
			W.setBaseDimension(aDimW);
			rcArea.d_left= x;
		    rcArea.d_top =y;
		    rcArea.d_right_or_width=W;
		    rcArea.d_bottom_or_height=H;
		    newBar->setArea(rcArea);
		   d_nonclientbars_LEFT.push_back(newBar);
		   break;
		 }
		 case SOARALIGN_RIGHT:
		 {
			  CalcClientRIGHT(right);
			 CalcClientTOP(top);
			 CalcClientBOTTOM(bottom);
			 dW.d_scale =1.0;
			dW.d_offset =-right-nHeight_or_Width;
			uW.setBaseDimension(dW);
			uW.setDimensionType(DT_WIDTH);
			 aDimx.setValue(left);
			 aDimy.setValue(top);
			
			 aDimW.setValue(nHeight_or_Width);
			 y.setBaseDimension(aDimy);
			 x.setDimensionType(DT_LEFT_EDGE);
			 x.setBaseDimension(uW);
			 y.setDimensionType(DT_TOP_EDGE);
			 //height
			 UDim uH(1.0,-bottom-top);
			 UnifiedDim unH( uH,DT_HEIGHT);
			 H.setBaseDimension(unH);
			W.setBaseDimension(aDimW);
			rcArea.d_left= x;
			rcArea.d_top =y;
			rcArea.d_right_or_width=W;
			rcArea.d_bottom_or_height=H;
			newBar->setArea(rcArea);
			d_nonclientbars_RIGHT.push_back(newBar);
			break;
		 }
		 case SOARALIGN_TOP:
		 {
			CalcClientTOP(top);
			 aDimx.setValue(0);
			 aDimy.setValue(top);
			 aDimH.setValue(nHeight_or_Width);
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
		d_nonclientbars_TOP.push_back(newBar);
		//调整左右的高度
		CalcLeft_Right_Bars();
		break;
		 }
		 case SOARALIGN_BOTTOM:
		 {
			 CalcClientBOTTOM(bottom);
			 aDimx.setValue(0);
			 aDimy.setValue(rc.bottom-bottom-nHeight_or_Width);
			 aDimH.setValue(nHeight_or_Width);
			 aDimW.setValue(rc.right - rc.left);
			 x.setBaseDimension(aDimx);
			 x.setDimensionType(DT_LEFT_EDGE);
			 //
			 UDim dY(1.0,-bottom-nHeight_or_Width);
			UnifiedDim uY(dY,DT_HEIGHT);
			Dimension Y(uY,DT_TOP_EDGE);
			 h.setBaseDimension(aDimH);
			h.setDimensionType(DT_HEIGHT);
			rcArea.d_left= x;
		rcArea.d_top =Y;
		rcArea.d_right_or_width=W;
		rcArea.d_bottom_or_height=h;
		newBar->setArea(rcArea);
		d_nonclientbars_BOTTOM.push_back(newBar);
		//调整左右的高度
		CalcLeft_Right_Bars();
		break;
		 }
		
 };
	 
	 return newBar;
}
int CSoarWnd::CalcClientTOP(int & rcTop) {
	 RECT rc = {0,0,0};
	 rcTop += d_windowframeborderHeight;
	 int h= 0;
	ANYBARS::iterator it = d_nonclientbars_TOP.begin() ;
		while( it != d_nonclientbars_TOP.end() )
		{
			rc=(*it)->getBarRect();
			h =rc.bottom-rc.top;
			rcTop  += h ;
			++it;
		}
		return rcTop;
}
int CSoarWnd::CalcClientBOTTOM(int & rcBottom) {
	 RECT rc = {0,0,0};
	 rcBottom += d_windowframeborderHeight;
	 int h = 0;
	ANYBARS::iterator it = d_nonclientbars_BOTTOM.begin() ;
		while( it != d_nonclientbars_BOTTOM.end() )
		{
			rc=(*it)->getBarRect();
			h =rc.bottom-rc.top;
			rcBottom  += h ;
			++it;
		}
		return rcBottom;
}
int CSoarWnd::CalcClientLEFT(int & rcLeft) {
	RECT rc = {0,0,0};
	rcLeft += d_windowframeborderWidth;
	 int l= 0;
	ANYBARS::iterator it = d_nonclientbars_LEFT.begin() ;
		while( it != d_nonclientbars_LEFT.end() )
		{
			rc=(*it)->getBarRect();
			l =rc.right-rc.left;
			rcLeft  += l ;
			++it;
		}
		return rcLeft;
}
int CSoarWnd::CalcClientRIGHT(int & rcRight) {
	RECT rc = {0,0,0};
	rcRight += d_windowframeborderWidth;
	 int l= 0;
	ANYBARS::iterator it = d_nonclientbars_RIGHT.begin() ;
		while( it != d_nonclientbars_RIGHT.end() )
		{
			rc=(*it)->getBarRect();
			l =rc.right-rc.left;
			rcRight  += l ;
			++it;
		}
		return rcRight;
}
void CSoarWnd::CalcLeft_Right_Bars()
{
	 RECT rc = {0,0,0};
	 rc=getWindowRect();
	  int top=0;
	 int bottom=0;
	 int left=0;
	 int right=0;
	 CalcClientTOP(top);
	CalcClientBOTTOM(bottom);
	UDim dH(1.0,-bottom-top);
	UnifiedDim uH(dH,DT_HEIGHT);
	Dimension H(uH,DT_HEIGHT);
	AbsoluteDim aDimy(top);
	Dimension y(aDimy,DT_TOP_EDGE);
	 ComponentArea rcArea;
	 ANYBARS::iterator it = d_nonclientbars_RIGHT.begin() ;
		while( it != d_nonclientbars_RIGHT.end() )
		{
			rcArea=(*it)->getArea();
			rcArea.d_top=y;
			rcArea.d_bottom_or_height=H;
			(*it)->setArea(rcArea);
			++it;
		}
		it = d_nonclientbars_LEFT.begin() ;
		while( it != d_nonclientbars_LEFT.end() )
		{
			rcArea=(*it)->getArea();
			rcArea.d_top=y;
			rcArea.d_bottom_or_height=H;
			(*it)->setArea(rcArea);
			++it;
		}
}
void CSoarWnd::setAlphaValue(unsigned char value)
{
}
void CSoarWnd::scrollwnd(int cx,int cy)
{
	//由于早期设计窗口系统时，没有考虑窗口的滚动，所以该功能如果要添加的话需要大改，所以放弃了窗体滚动
	//另外一种滚动思路：请看listview类，就是太费对象了2022-4-11
}