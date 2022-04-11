#include "SoarStaticItemWnd.h"
#include "SoarRoot.h"
#include "../SoarHeader/leeLog.h"

CLeeStaticItemWnd::CLeeStaticItemWnd(HWND root,ISoarEngine* Eng):CSoarWnd(root,Eng),
d_height(30),
d_width(60),
d_ID(0),
d_iIndex(0),
d_subindex(0),
d_checkState(false),
d_data((void*)-1)
{
	d_wndtype =LWNDT::LWNDT_STAICITEMWND;
	d_wndlook ="Static";
	d_wndlookState="Normal";
}

CLeeStaticItemWnd::~CLeeStaticItemWnd(void)
{
}
DWORD CLeeStaticItemWnd::getIndex(void)
{
	return d_iIndex;
}
DWORD CLeeStaticItemWnd::getsubIndex(void)
{
	return d_subindex;
}
DWORD CLeeStaticItemWnd::getID(void) 
{
	return d_ID;
}
CLeeString CLeeStaticItemWnd::getString(void) 
{
	return d_string;
}
LPVOID CLeeStaticItemWnd::getData(void) 
{
	return d_data;
}
int CLeeStaticItemWnd::getHeight(void) 
{
	RECT rc =getWindowRect();
	int h =rc.bottom-rc.top;
	return h;
}
int CLeeStaticItemWnd::getWidght(void)
{
	RECT rc =getWindowRect();
	int w =rc.right-rc.left;
	return w;
}
void CLeeStaticItemWnd::setIndex(DWORD dwIndex) 
{
	d_iIndex = dwIndex ;
}
void CLeeStaticItemWnd::setsubIndex(DWORD dwIndex) 
{
	d_subindex = dwIndex ;
}
void CLeeStaticItemWnd::setID(DWORD dwId) 
{
	d_ID =dwId ;
}
void CLeeStaticItemWnd::setString(const CLeeString & str) 
{
	d_string = str ;
}
void CLeeStaticItemWnd::setData(LPVOID data) 
{
	d_data = data ;
}
void CLeeStaticItemWnd::setHeight(int h) 
{
	d_height =h ;
}
void CLeeStaticItemWnd::setWidght(int w) 
{
	d_width = w ;
}
bool CLeeStaticItemWnd::getCheck(void)
{
	return d_checkState ;
}
void CLeeStaticItemWnd::setcheck(bool bcheck)
{
	if (d_checkState != bcheck)
	{
		d_checkState =bcheck;
	}
}
void CLeeStaticItemWnd::DrawSelf(ILeeDrawInterface *DrawFuns)
{
	if (d_parent->getState()<LWNDST_SHOW)
	{
		d_wndState =d_parent->getState();
		return;
	}
	if(getState()<LWNDST_SHOW)
	{
		return;
	}
	RECT rc =getWindowRect();
	RectF rcc=rc;
	RectF rcparent =getCliperRect();
	//
	rcparent.d_top +=4;
	rcparent.d_left +=4;
	//主窗口绘制
	if (d_DrawEng->hasWidgetlook(d_wndlook))
	{
		//d_DrawEng->DrawWidgetlook("DefaultClient",d_wndlookState,rcc,d_string);
		d_DrawEng->DrawWidgetlook(d_wndlook,d_wndlookState,rcc,d_string,d_VertTextFormat,d_horzTextFormat,&rcparent,true);
	}
}
LRESULT CLeeStaticItemWnd::HandleEvent ( UINT uMsg ,WPARAM wParam ,LPARAM lParam) 
{
	if (uMsg == WM_LBUTTONUP)
	{
		SOARMSG leeMsg;
		leeMsg.message =SOAR_ITEMSELECTED;
		leeMsg.mouseEvent =SOAR_LCLICK_UP;
		leeMsg.sourceWnd =this;
		leeMsg.targetWnd =d_OwnerWnd?d_OwnerWnd:d_parent;
		leeMsg.wParam =d_iIndex;
		leeMsg.lParam =d_ID;
		leeMsg.Data=NULL;
		leeMsg.msgSourceTag=SOAR_MSG_ORIG;
		CSoarRoot::getSingletonPtr()->addOfflineMsg(leeMsg);
	}
	if (uMsg == WM_RBUTTONUP)
	{
		SOARMSG leeMsg;
		leeMsg.message =SOAR_ITEMSELECTED;
		leeMsg.mouseEvent =SOAR_RCLICK_UP;
		leeMsg.sourceWnd =this;
		leeMsg.targetWnd =d_OwnerWnd?d_OwnerWnd:d_parent;
		leeMsg.wParam =d_iIndex;
		leeMsg.lParam =d_ID;
		leeMsg.Data=NULL;
		leeMsg.msgSourceTag=SOAR_MSG_ORIG;
		CSoarRoot::getSingletonPtr()->addOfflineMsg(leeMsg);
	}
	if (uMsg == WM_LBUTTONDBLCLK)
	{
		SOARMSG leeMsg;
		leeMsg.message =SOAR_ITEMSELECTED;
		leeMsg.mouseEvent =SOAR_LDBCLICK;
		leeMsg.sourceWnd =this;
		leeMsg.targetWnd =d_OwnerWnd?d_OwnerWnd:d_parent;
		leeMsg.wParam =d_iIndex;
		leeMsg.lParam =d_ID;
		leeMsg.Data=NULL;
		leeMsg.msgSourceTag=SOAR_MSG_ORIG;
		CSoarRoot::getSingletonPtr()->addOfflineMsg(leeMsg);
	}
	return CSoarRoot::getSingletonPtr()->SoarDefWndProc(uMsg,wParam,lParam);//留系统底层处理
}