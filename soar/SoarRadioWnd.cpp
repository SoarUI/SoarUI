#include "SoarRadioWnd.h"
CLeeRadioWnd::LeeGroupWndPtrMap CLeeRadioWnd::s_groupCheckedIDs;
CLeeRadioWnd::CLeeRadioWnd(HWND root,ISoarEngine* Eng):CSoarWnd(root,Eng)
{
	d_wndlook="RadioButton";
	d_wndtype =LWNDT::LWNDT_RADIOBTN;
	d_wndlookState="Normal";
	d_checked = false;
	d_groupId="flydream_radio1";
}

CLeeRadioWnd::~CLeeRadioWnd(void)
{
}
void CLeeRadioWnd::DrawSelf(ILeeDrawInterface *DrawFuns)
{
	if (d_parent !=NULL && d_parent->getState()<= LWNDST_HIDE)
	{
		return;
	}
	RECT rc =getWindowRect();
	
	POINT pt;
	::GetCursorPos(&pt);
	::ScreenToClient(d_rootWnd,&pt);
	if (d_checked)
	{
		d_wndlookState="SelectedNormal";
	}
	else
	{
		d_wndlookState="Normal";
	}
	RectF rcc=rc;
	RectF rcparent =getCliperRect();
	if (d_DrawEng->hasWidgetlook(d_wndlook))
	{
		d_DrawEng->DrawWidgetlook(d_wndlook,d_wndlookState,rcc,d_wndText,d_VertTextFormat,d_horzTextFormat,&rcparent,true);
	}
	
}
LRESULT CLeeRadioWnd::HandleEvent ( UINT uMessage,WPARAM wp ,LPARAM lp ) 
{
	if (uMessage==WM_LBUTTONDOWN)
	{
			setCheck(true);
	}
	return CSoarWnd::HandleEvent(uMessage,wp,lp);
}
bool CLeeRadioWnd::getCheck(void)
{
	return d_checked;
}
void CLeeRadioWnd::setCheck(bool bChecked)
{
	if ( (d_checked && bChecked)||
		(!d_checked && !bChecked))
	{
		return ;
	}
	LeeGroupWndPtrMap::iterator it =s_groupCheckedIDs.find(d_groupId);
	if (it !=s_groupCheckedIDs.end())//已经有选中的对象
	{
		if (bChecked)
		{
			if (it->second==this)
			{
				return ;
			}
			else
			{
				//取消，设置新的对象
				CLeeRadioWnd * prevChecked =(CLeeRadioWnd*)(it->second);
				s_groupCheckedIDs[d_groupId]=this;
				prevChecked->setCheck(false);
				d_checked =true;
				RECT rc =getWindowRect();
				//InvalidateRect(d_rootWnd,&rc,0);//重绘新区域
			}
		}
		else
		{
			d_checked =false;
			RECT rc =getWindowRect();
			//InvalidateRect(d_rootWnd,&rc,0);//重绘新区域
		}
	}
	else
	{
		if (bChecked)
		{
			d_checked =true;
			s_groupCheckedIDs[d_groupId]=this;
			RECT rc =getWindowRect();
			//InvalidateRect(d_rootWnd,&rc,0);//重绘新区域
		}
	}

}
void CLeeRadioWnd::setRadioGroup(CLeeString & group)
{
	d_groupId=group;
}