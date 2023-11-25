#include "SoarUISheetWnd.h"

#include "SoarRoot.h"
#include "../SoarHeader/iSoarmenuwnd.h"
#include"soarTitleBar.h"
#include"SoarMenuBar.h"
#include"SoarScrollSegment.h"
#include"SoarMenuWnd.h"


CSoarUISheetWnd::CSoarUISheetWnd(HWND root,ISoarEngine* Eng):CSoarWnd(root,Eng),d_menu(NULL)
{
	d_wndlook ="DefaultClient";
	d_wndtype=LWNDT_SYS_UI_SHEET;
	d_wndStyle =(LWNDSTY)(LWNDSTY_SYSTITLEBAR|LWNDSTY_SYSMENUBAR);
}


CSoarUISheetWnd::~CSoarUISheetWnd(void)
{
}
void CSoarUISheetWnd::InitializeSegments(void)
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
	}
}
bool CSoarUISheetWnd::Bind2Window(HWND h)
{
	d_rootWnd =h;
	return h;
}
 RECT CSoarUISheetWnd::getClientRect(void) 
 {
	 
	 RECT rcTmp;
	 GetClientRect(d_rootWnd,&rcTmp);
	  RectF rcCur;
	rcCur=d_area.getRect(rcTmp);
	rcCur.toWindowRect(rcTmp);
	if (d_wndState == LWNDST::LWNDST_TItleBarOnly ||
		d_wndState == LWNDST::LWNDST_MINI)
	{
		rcTmp.bottom = rcTmp.top + 15;
		return rcTmp;
	}
	 this->CalcClientRec(rcTmp);
	 return rcTmp;
 }
 RECT CSoarUISheetWnd::getWindowRect(void) 
 {
	 RECT rcTmp;
	 GetClientRect(d_rootWnd,&rcTmp);
	 RectF rcCur;
	rcCur=d_area.getRect(rcTmp);
	rcCur.toWindowRect(rcTmp);
	if (d_wndState == LWNDST::LWNDST_TItleBarOnly ||
		d_wndState == LWNDST::LWNDST_MINI)
	{
		rcTmp.bottom = rcTmp.top + 15;
		return rcTmp;
	}
	 return rcTmp;
 }
 LRESULT CSoarUISheetWnd::HandleEvent(UINT uMsg ,WPARAM wParam ,LPARAM lParam)
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
			::PostMessage(d_rootWnd,WM_QUIT,0,0);
			return 0;
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
			return 0;
		}
	}
	if (uMsg == SOAR_MENUITEMSELECTED)
		{
			SOARMSG* pMsg=(SOARMSG*)wParam;
			HandleMenuEvent(pMsg->wParam,pMsg->lParam,pMsg);
		}
	return CSoarWnd::HandleEvent(uMsg,wParam,lParam);
}
 void CSoarUISheetWnd::HandleMenuEvent(int nPos,int nID,SOARMSG * pMsgInfo)
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
		zuozhe->setText("                            \r\n\r\n版本信息\r\n原始版本：2016.5.1 0001.001\r\n最新版本：2022.4.11 0001.0001.0\r\nSoar @CopyRight all reserved ecsuperpay@163.com\r\n\r\n\r\n\r\n\r\n");
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
		banben->setText("              \r\n\r\n作者简介\r\n程序作者：李兴顺,\r\nC++程序员.目前从事CAD二次开发(ARX/C#)、\r\nPDMS二次开发(C#/PML).熟悉MFC WIN32 lua一般C#\r\n该库第一次以SDK形式发布,平时工作忙,没有时间整理,\r\n业余开发,现在WEB UI库横行,希望对大家有用\r\n因为库早期设计的原因，不能滚动窗口\r\n后续版本不支持窗体滚动，但是list项是支持的(特殊设计)\r\n");
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
		zuozhe->setText("                            \r\n\r\n\r\n模态对话框测试\r\n原始版本：2018.2.26 0001.001\r\n最新版本：2018.3.24 0001.0001.0\r\nSoar @CopyRight all reserved ecsuperpay@163.com\r\n\r\n\r\n\r\n\r\n");
		zuozhe->setState(LWNDST_SHOW);
		//CSoarRoot::getSingleton().DoModal(zuozhe);
		CSoarRoot::getSingleton().StartModal(zuozhe);
	}
}
void CSoarUISheetWnd::setTitle(const CLeeString & name)
{
	if(p_titlebar)
	  p_titlebar->setTitle(name);
}