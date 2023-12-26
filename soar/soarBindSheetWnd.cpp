#include "SoarBindSheetWnd.h"
#include "SoarRoot.h"
#include "../SoarHeader/iSoarmenuwnd.h"
#include"soarTitleBar.h"
#include"SoarMenuBar.h"
#include"SoarScrollSegment.h"
#include"SoarMenuWnd.h"
CSoarBindSheetWnd::CSoarBindSheetWnd(HWND root,ISoarEngine* Eng):CSoarWnd(root,Eng),d_menu(NULL)
{
	d_wndlook ="DefaultClient";
	d_wndtype=LWNDT_SYS_SHEET;
	d_wndStyle =(LWNDSTY)(LWNDSTY_SYSTITLEBAR|LWNDSTY_SYSMENUBAR);
}

CSoarBindSheetWnd::~CSoarBindSheetWnd(void)
{
}
void CSoarBindSheetWnd::InitializeSegments(void)
{
	
	CSoarWnd::InitializeSegments();
	if(d_wndStyle & LWNDSTY_SYSMENUBAR)
	{
		int menubarhoff =d_MenuBarSegmentHeight;
		if(p_titlebar)
		{
			RECT rct=p_titlebar->getBarRect();
			int h =rct.bottom-rct.top;
			menubarhoff = h ;
		}

		AbsoluteDim aDimx(0);
		AbsoluteDim aDimy(menubarhoff);
		AbsoluteDim aDimH(d_MenuBarSegmentHeight);
		AbsoluteDim aDimW(100);
	
		Dimension x(aDimx,DT_LEFT_EDGE);
		Dimension h(aDimH,DT_HEIGHT);
		Dimension y(aDimy,DT_TOP_EDGE);
		UDim dmin(1.0,0);
		UnifiedDim ux(dmin,DT_WIDTH);
		Dimension w(ux,DT_WIDTH);
		 ComponentArea rcArea;
		 rcArea.d_left= x;
		rcArea.d_top =y;
		rcArea.d_right_or_width=w;
		rcArea.d_bottom_or_height=h;
		UDim dmax(0,100);
		UDim dclose(0,200);
		aDimy.setValue(0);
		aDimx.setValue(0);
		Dimension w1(aDimW,DT_WIDTH);
		Dimension h1(aDimH,DT_HEIGHT);
		Dimension y1(aDimy,DT_TOP_EDGE);
		Dimension x1(aDimx,DT_X_POSITION);
		/////////////////////////menubar
		AbsoluteDim aDimy2(0);
		AbsoluteDim aDimx2(0);
		AbsoluteDim aDimH2(100);
		AbsoluteDim aDimW2(150);
		Dimension w2(aDimW2,DT_WIDTH);
		Dimension h2(aDimH2,DT_HEIGHT);
		Dimension y2(aDimy2,DT_Y_POSITION);
		Dimension x22(aDimx2,DT_X_POSITION);
		CLeeString itemname;
		SoarSegmentItem its;
		rcArea.d_left= x1;
		rcArea.d_top =y1;
		rcArea.d_right_or_width=w1;
		rcArea.d_bottom_or_height=h1;
		its.d_area=rcArea;
		its.d_nId=SOAR_SYSMIN;
		its.d_wndlook="MenuBarItem";
		its.d_wndlookState="Normal";
		its.d_wndText="系统换肤";
		itemname.Format("systempopMenu%d%p",0,this);
		ISoarMenuWnd *menu =(ISoarMenuWnd*)CSoarRoot::getSingletonPtr()->createPopupWnd(itemname,x22,y2,w2,h2,LWNDT_POPMENU,this,1);
		menu->setOwnerWnd(this);
		menu->AppendMenu(0x210,NULL,"默认皮肤");
		menu->AppendMenu(0x211,NULL,"Windows样式");
		menu->setState(LWNDST_HIDE);
		its.udata =(void*)menu;
		p_menubar->AddItem(its);
		
		UnifiedDim ux1(dmax,DT_WIDTH);
	    Dimension x2(ux1,DT_X_POSITION);
		rcArea.d_left=x2;
		its.d_area=rcArea;
		its.d_nId=SOAR_SYSMAX;
		its.d_wndlook="MenuBarItem";
		its.d_wndlookState="Normal";
		its.d_wndText="模态对话框";
		itemname.Format("systempopMenu%d%p",1,this);
		menu =(ISoarMenuWnd*)CSoarRoot::getSingletonPtr()->createPopupWnd(itemname,x22,y2,w2,h2,LWNDT_POPMENU,this,1);
		menu->setOwnerWnd(this);
		menu->AppendMenu(0x310,NULL,"模态对话框");
		menu->setState(LWNDST_HIDE);
		its.udata =(void*)menu;
		p_menubar->AddItem(its);
		UnifiedDim ux2(dclose,DT_WIDTH);
		Dimension x3(ux2,DT_X_POSITION);
		rcArea.d_left=x3;
		its.d_area=rcArea;
		its.d_nId=SOAR_SYSCLOSE;
		its.d_wndlook="MenuBarItem";
		its.d_wndlookState="Normal";
		its.d_wndText="系统信息";
		
		//创建相应的菜单
		
		itemname.Format("systempopMenu%d%p",2,this);
		d_menu =(ISoarMenuWnd*)CSoarRoot::getSingletonPtr()->createPopupWnd(itemname,x22,y2,w2,h2,LWNDT_POPMENU,this,1);
		d_menu->setOwnerWnd(this);
		d_menu->AppendMenu(0x110,NULL,"作者简介");
		d_menu->AppendMenu(0x111,NULL,"版本信息");
		d_menu->setState(LWNDST_HIDE);
		its.udata =(void*)d_menu;
		p_menubar->AddItem(its);
	   // p_menubar->ModifyItemMenu(0x103,d_menu,NULL);
	}
	
}
 bool CSoarBindSheetWnd::Bind2Window(HWND h)
{
	d_rootWnd =h;
	return h;
}
 RECT CSoarBindSheetWnd::getClientRect(void) 
 {
	 RECT rcTmp;
	 GetClientRect(d_rootWnd,&rcTmp);
	 this->CalcClientRec(rcTmp);
	 return rcTmp;
 }
 RECT CSoarBindSheetWnd::getWindowRect(void) 
 {
	 RECT rcTmp;
	 GetClientRect(d_rootWnd,&rcTmp);
	 return rcTmp;
 }
 void CSoarBindSheetWnd::setState(LWNDST state)
 {
	 //系统窗口状态的变化，MS没有告诉我们所以要检测
	 WINDOWPLACEMENT   ws;   
	 ws.length   =   sizeof(WINDOWPLACEMENT);   
	 GetWindowPlacement(d_rootWnd,   &ws);   
	 switch   (ws.showCmd)   
	 {   
	 case   SW_HIDE://普通; 
		 { 
			 d_wndState =LWNDST_HIDE;
			 break;
		 }
	 case SW_SHOW:
		 { 
			 d_wndState =LWNDST_SHOW;
			 break;
		 }
	 case   SW_MINIMIZE://最小化; 
		 { 
			 d_wndState =LWNDST_MINI;
			 break;
		 }
	 case   SW_MAXIMIZE://最大化;   
		 {
			 d_wndState =LWNDST_MAX;
			 break;
		 }
	 }
	 if ( !IsIconic(d_rootWnd) &&
		 d_wndState ==LWNDST_MINI)
	 {
		 d_wndState =LWNDST_SHOW;
	 }
	 if (d_wndState != state)
	 {
		  d_wndState =state;
		 switch(state)
		 {
			  
		 case LWNDST_HIDE:
			 {
				 ::ShowWindow(d_rootWnd,SW_HIDE);
				
				 break;
			 }
		 case LWNDST_SHOW:
			 {
				 ::ShowWindow(d_rootWnd,SW_SHOW);
				 break;
			 }
		 case LWNDST_MAX:
			 {
				 ::PostMessage(d_rootWnd,WM_SYSCOMMAND,SC_MAXIMIZE,0);
				 break;
			 }
		 case LWNDST_RESTORE:
			 {
				 ::PostMessage(d_rootWnd,WM_SYSCOMMAND,SC_RESTORE,0);
				 break;
			 }
		 case LWNDST_MINI:
			 {
				 ::PostMessage(d_rootWnd,WM_SYSCOMMAND,SC_MINIMIZE,0);
				 break;
			 }
		 }
	 }
	 
 }
 void CSoarBindSheetWnd::MoveWindow(__in int X, __in int Y, __in int nWidth, __in int nHeight, __in BOOL bRepaint)
 {
	 ::MoveWindow(d_rootWnd,X,Y,nWidth,nHeight,bRepaint);
 }
void CSoarBindSheetWnd::DrawSelf(ILeeDrawInterface *DrawFuns)
{
	CSoarWnd::DrawSelf(DrawFuns);
}
BOOL CSoarBindSheetWnd::HandleEvent(UINT uMsg ,WPARAM wParam ,LPARAM lParam, LRESULT& lr)
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
			setState(LWNDST_MINI);
			lr = 0;
			return true;
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
			return true;
		}
		if (nID == SOAR_SYSCLOSE &&
			pMsg->sourceWnd==this &&
			HIWORD(pMsg->wParam)==BN_CLICKED && pMsg->mouseEvent==SOAR_LCLICK_DOWN)
		{
			::PostMessage(d_rootWnd,WM_QUIT,0,0);
			lr = 0;
			return true;
		}
		if (HIWORD(pMsg->wParam)==BN_CLICKED &&
			pMsg->sourceWnd==this &&
			pMsg->mouseEvent==SOAR_LDBCLICK)
		{
			
			if(getState()==LWNDST_MAX)
			{
				setState(LWNDST_RESTORE);
			}
			else
			{
					setState(LWNDST_MAX);
					
			}
			lr = 0;
			return true;
		}
	}
	if (uMsg == SOAR_MENUITEMSELECTED)
		{
			SOARMSG* pMsg=(SOARMSG*)wParam;
			HandleMenuEvent(pMsg->wParam,pMsg->lParam,pMsg);
		}
	return CSoarWnd::HandleEvent(uMsg,wParam,lParam,lr);
}
void CSoarBindSheetWnd::HandleMenuEvent(int nPos,int nID,SOARMSG * pMsgInfo)
{
	if (nID==0x111)
	{
		UDim dmin(0.5,-300);
		UnifiedDim ux(dmin,DT_WIDTH);
		UDim dman(0.5,0);
		UnifiedDim uy(dman,DT_HEIGHT);
		AbsoluteDim aDimH2(200);
		AbsoluteDim aDimW2(600);
		Dimension w2(aDimW2,DT_WIDTH);
		Dimension h2(aDimH2,DT_HEIGHT);
		Dimension y2(uy,DT_Y_POSITION);
		Dimension x2(ux,DT_X_POSITION);
		CLeeString itemname;
		itemname.Format("systempopMenu%d%p",100,this);
		ISoarWnd* zuozhe =(ISoarWnd*)CSoarRoot::getSingletonPtr()->createPopupWnd(itemname,x2,y2,w2,h2,LWNDT_SYS_INFODIALOG,this,1);
		zuozhe->setText("                            \r\n版本信息\r\n原始版本：2016.5.1 0001.001\r\n最新版本：2018.3.24 0001.0001.0\r\nSoar @CopyRight all reserved ecsuperpay@163.com\r\n\r\n\r\n\r\n\r\n");
		zuozhe->setState(LWNDST_SHOW);
		zuozhe->enableAlphaBlend(true);
	}
	if(nID==0x110)
	{
		UDim dmin(0.5,-300);
		UnifiedDim ux(dmin,DT_WIDTH);
		UDim dman(0.5,0);
		UnifiedDim uy(dman,DT_HEIGHT);
		AbsoluteDim aDimH2(240);
		AbsoluteDim aDimW2(640);
		Dimension w2(aDimW2,DT_WIDTH);
		Dimension h2(aDimH2,DT_HEIGHT);
		Dimension y2(uy,DT_Y_POSITION);
		Dimension x2(ux,DT_X_POSITION);
		CLeeString itemname;
		itemname.Format("systempopMenu%d%p",1001,this);
		ISoarWnd* banben=(ISoarWnd*)CSoarRoot::getSingletonPtr()->createPopupWnd(itemname,x2,y2,w2,h2,LWNDT_SYS_INFODIALOG,this,1);
		banben->setText("              \r\n\r\n作者简介\r\n程序作者：李兴顺,\r\nC++程序员.目前从事CAD二次开发(ARX/C#)、\r\nPDMS二次开发(C#/PML).熟悉MFC WIN32 lua一般C#\r\n该库第一次以SDK形式发布,平时工作忙,没有时间整理,\r\n业余开发,现在WEB UI库横行,希望对大家有用\r\n\r\n是否继续开发，等待反馈\r\n后续版本可能添加异性窗口,透明等DX特效\r\n");
		banben->setState(LWNDST_SHOW);
		//banben->enableAlphaBlend(true);

	}
	//
	if(nID==0x210)
	{
		d_DrawEng->engine_loadtheme("TaharezLook");
	}
	if(nID==0x211)
	{
		d_DrawEng->engine_loadtheme("WindowsLook");
	}
	//模态对话框
	if (nID==0x310)
	{
		
		UDim dmin(0.5,-300);
		UnifiedDim ux(dmin,DT_WIDTH);
		UDim dman(0.5,0);
		UnifiedDim uy(dman,DT_HEIGHT);
		AbsoluteDim aDimH2(200);
		AbsoluteDim aDimW2(600);
		Dimension w2(aDimW2,DT_WIDTH);
		Dimension h2(aDimH2,DT_HEIGHT);
		Dimension y2(uy,DT_Y_POSITION);
		Dimension x2(ux,DT_X_POSITION);
		CLeeString itemname;
		itemname.Format("systempopMenu%d%p",nID,this);
		ISoarWnd* zuozhe =(ISoarWnd*)CSoarRoot::getSingletonPtr()->createPopupWnd(itemname,x2,y2,w2,h2,LWNDT_SYS_INFODIALOG,this,1);
		zuozhe->setText("                      \r\n\r\n\r\n模态对话框测试\r\n原始版本：2018.2.26 0001.001\r\n最新版本：2018.3.24 0001.0001.0\r\nSoar @CopyRight all reserved ecsuperpay@163.com\r\n\r\n\r\n\r\n\r\n");
		zuozhe->setState(LWNDST_SHOW);
		CSoarRoot::getSingleton().StartModal(zuozhe);
		
	}
}
void CSoarBindSheetWnd::setTitle(const CLeeString & name)
{
	if(p_titlebar)
	  p_titlebar->setTitle(name);
}