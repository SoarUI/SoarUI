#include "LeeTabWnd.h"
#include "SoarRoot.h"
#include "../SoarHeader/iSoarmenuwnd.h"

CLeeTabWnd::CLeeTabWnd(HWND root,ISoarEngine* Eng):CSoarWnd(root,Eng),
d_tabHeader(NULL)
{
	d_wndlook="TabCtrlPane";
	d_wndtype =LWNDT_TABCTRL;
	d_wndlookState ="Normal";
}

CLeeTabWnd::~CLeeTabWnd(void)
{
}
void CLeeTabWnd::InitializeSegments(void)
{
	CSoarWnd::InitializeSegments();
	CLeeString itemname ;
	itemname.Format("LeeTabWnd__%p__%s__%d",this,"systemTabHeader",1536);
	d_tabHeader =(CLeeTabCtrlHeader*)AddNonClientBar(itemname,SOARALIGN_TOP,LWNDT_SYS_TABCTRHEADER,25);
	d_tabHeader->setParent(NULL);
	d_tabHeader->setMsgReceiver(this);
}
bool CLeeTabWnd::addTab(DWORD islot,CLeeString szTab,LPVOID data)
{
	return d_tabHeader->addTab(islot,szTab,data);
}
bool CLeeTabWnd::linkTab(DWORD islot,ISoarWnd* dataWnd)
{
	return d_tabHeader->linkTab(islot,dataWnd);
}
int CLeeTabWnd::setTabHeight(int h)
{
	return d_tabHeader->setTabHeight(h);
}
int CLeeTabWnd::setTabWidth(int w)
{
	return d_tabHeader->setTabWidth(w);
}
int CLeeTabWnd::setButtonsInoneLine(int n)
{
	return d_tabHeader->setButtonsInoneLine(n) ;
}
RECT CLeeTabWnd::getClientRect(void) 
{
	return CSoarWnd::getClientRect();
}
int CLeeTabWnd::getCurSel(void)
{
	return d_tabHeader->getCurSel();
}
void CLeeTabWnd::setCurSel(int islot)
{
	d_tabHeader->setCurSel(islot);
	
}