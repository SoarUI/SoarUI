#include "StdAfx.h"
#include "CmyBut.h"

CmyBut::CmyBut(void):d_wndState(LWNDST_SHOW)
{
	d_wndtype =LWNDT_BUTTON;
	d_zIndex = 1;
	d_wndlookState ="Normal";
}

CmyBut::~CmyBut(void)
{
}
LWNDT CmyBut::getType(void)
{
   return d_wndtype;
}
RECT CmyBut::getClientRect(void) 
{
	return rcArea;
}
RECT CmyBut::getWindowRect(void) 
{
	RECT rcTmp;
	if (d_wndState==LWNDST::LWNDST_MAX)
	{
		GetClientRect(d_rootWnd,&rcTmp);
		return rcTmp;
	}
	if (d_parent ==NULL)
	{
		GetClientRect(d_rootWnd,&rcTmp);
		RectF rc=rcTmp;
		rc=d_area.getRect(rcTmp);
		RECT rt;
		rc.toWindowRect(rt);
		return rt;
	}
	RECT rcParent =d_parent->getWindowRect();
	RECT rc;
	RectF rcCur;
	rcCur=d_area.getRect(rcParent);
	rcCur.toWindowRect(rc);
	return rc;
}
void CmyBut::setArea(ComponentArea& area)
{
	d_area =area;
}
void CmyBut::setArea(RECT& rc)
{
	rcArea =rc;
	AbsoluteDim adim(0.0);
	adim.setValue(rc.left);
	d_area.d_left.setBaseDimension(adim);
	d_area.d_left.setDimensionType(DT_LEFT_EDGE);
	adim.setValue(rc.top);
	d_area.d_top.setBaseDimension(adim);
	d_area.d_top.setDimensionType(DT_TOP_EDGE);
	adim.setValue(rc.right);
	d_area.d_right_or_width.setBaseDimension(adim);
	d_area.d_right_or_width.setDimensionType(DT_RIGHT_EDGE);
	adim.setValue(rc.bottom);
	d_area.d_bottom_or_height.setBaseDimension(adim);
	d_area.d_bottom_or_height.setDimensionType(DT_BOTTOM_EDGE);
}
void CmyBut::setWindowPos(int x,int y)
{
	int xPos = x; int yPos =y;
	RECT rc =getWindowRect();//取得相对于Root窗口的偏移
	RECT rcPre=rc;
	int W =rc.right -rc.left;
	int H =rc.bottom -rc.top;
	rc.left =xPos ;//求出相对于父窗口的偏移量
	rc.top =yPos;//在Y轴上的偏移量
	rc.right =rc.left+W;
	rc.bottom=rc.top+H;
	rcArea =rc;
	AbsoluteDim adim(0.0);
	adim.setValue(rc.left);
	d_area.d_left.setBaseDimension(adim);
	d_area.d_left.setDimensionType(DT_LEFT_EDGE);
	adim.setValue(rc.top);
	d_area.d_top.setBaseDimension(adim);
	d_area.d_top.setDimensionType(DT_TOP_EDGE);
	adim.setValue(rc.right);
	d_area.d_right_or_width.setBaseDimension(adim);
	d_area.d_right_or_width.setDimensionType(DT_RIGHT_EDGE);
	adim.setValue(rc.bottom);
	d_area.d_bottom_or_height.setBaseDimension(adim);
	d_area.d_bottom_or_height.setDimensionType(DT_BOTTOM_EDGE);
}
ISoarWnd* CmyBut::GetParent(void)
{
	return d_parent;
}
void CmyBut::setParent(ISoarWnd * lParent) 
{
	d_parent =lParent;
}
INT CmyBut::setZindex(INT dwZindex) 
{
	d_zIndex =dwZindex;
	return 0;
}
INT CmyBut::getZindex(void)
{
	return d_zIndex;
}
CLeeString CmyBut::getText(void)
{
	return d_wndText;
}
void CmyBut::setText(CLeeString name)
{
	d_wndText =name;
}
void CmyBut::setWndName(CLeeString name) 
{
	d_wndName = name ;
}
CLeeString CmyBut::getWndName(void)
{
	return d_wndName ;
}
void CmyBut::setState(LWNDST state) 
{
	if (d_wndState != state)
	{
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
	}
}
LWNDST CmyBut::getState(void)
{
	return d_wndState;
}
CLeeString CmyBut::getDreamLook(void)
{
	return d_wndlook;
}
void CmyBut::setDreamLook(CLeeString name)
{
	d_wndlook =name;
}
void CmyBut::attachRoot(HWND root)
{
	d_rootWnd =root;
}
void CmyBut::checklookState(POINT &pt)
{
	RECT rc =getWindowRect();//取得相对于Root窗口的偏移
	CLeeString state=d_wndlookState;
	if (::PtInRect(&rc,pt))
	{
		d_wndlookState ="Hover";
	}
	else
		d_wndlookState ="Normal";
	if (state !=d_wndlookState)
	{
		InvalidateRect(d_rootWnd,&rc,1);//还是发送状态改变好呢，这样客户可以自己修改
	}
}
void CmyBut::DrawSelf(ILeeDrawInterface *DrawFuns) 
{
//	::SetBkColor(DrawFuns->hdc, RGB(100,10,25));
	RECT rc=getWindowRect();
	//主窗口绘制
	DrawFuns->LeeDrawWidget(d_wndName,d_wndlookState,rc,d_wndText,-1,-1);
	//子窗口绘制
	LeeWndPtrMap::iterator it =d_WndPtrs.begin();
	while (it != d_WndPtrs.end())
	{
		//子窗口中是否有窗口被激活
		if ( 
			(it->second)->getState()>LWNDST_HIDE ) 
		{
			(it->second)->DrawSelf(DrawFuns);
		}	
		++it;
	}
}
LRESULT CmyBut::HandleEvent ( UINT ,WPARAM ,LPARAM ) 
{
	return 0;
}
ISoarWnd* CmyBut::getChildWnd(const CLeeString & name)
{
	LeeWndPtrMap::iterator it =d_WndPtrs.find(name);
	if (it !=d_WndPtrs.end())
	{
		return (*it).second;
	}
	return this;
}
bool CmyBut::addChildWnd(ISoarWnd* pWnd)
{
	LeeWndPtrMap::iterator it =d_WndPtrs.find(pWnd->getWndName());
	if (it !=d_WndPtrs.end())
	{
		return true;
	}
	d_WndPtrs[pWnd->getWndName()] =pWnd;
}
void  CmyBut::setWindowHeight(int h) 
{
	AbsoluteDim adim(0.0);
	adim.setValue(h);
	d_area.d_bottom_or_height.setBaseDimension(adim);
	d_area.d_bottom_or_height.setDimensionType(DT_HEIGHT);
}
void  CmyBut::setWindowWidth(int w) 
{
	AbsoluteDim adim(0.0);
	adim.setValue(w);
	d_area.d_right_or_width.setBaseDimension(adim);
	d_area.d_right_or_width.setDimensionType(DT_WIDTH);
}
void  CmyBut::setWindowHeight(Dimension h) 
{
	d_area.d_bottom_or_height = h;
}
void  CmyBut::setWindowWidth(Dimension w) 
{
	d_area.d_right_or_width = w;
}