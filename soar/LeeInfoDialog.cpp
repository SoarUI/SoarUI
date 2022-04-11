#include "LeeInfoDialog.h"
#include "SoarRoot.h"

CLeeInfoDialog::CLeeInfoDialog(HWND root,ISoarEngine* Eng):CSoarWnd(root,Eng)
{
	d_wndlook="TitleBar";
	d_wndtype =LWNDT::LWNDT_SYS_INFODIALOG;
	d_VertTextFormat=VTF_TOP_ALIGNED;
	d_horzTextFormat=HTF_LEFT_ALIGNED|HTF_WORDWRAP_LEFT_ALIGNED;
}

CLeeInfoDialog::~CLeeInfoDialog(void)
{
}
void CLeeInfoDialog::InitializeSegments(void)
{
	AbsoluteDim aDimH(30);
	AbsoluteDim aDimW(120);
	UDim ux(0.4,-140);
	UnifiedDim udx(ux,DT_WIDTH);
	Dimension x(udx,DT_X_POSITION);
	Dimension h(aDimH,DT_HEIGHT);
	Dimension w(aDimW,DT_WIDTH);
	UDim uy(1.0,-60);
	UnifiedDim udy(uy,DT_HEIGHT);
	Dimension y(udy,DT_Y_POSITION);
	CLeeString itemname ;
	itemname.Format("LeeInfoDialog__%p__%s__%d",this,"buttons",1);
	d_ok=CSoarRoot::getSingletonPtr()->createPopupWnd(itemname,x,y,w,h,LWNDT_BUTTON,this,d_zPopupIndex+1);
	d_ok->setText("确定");
	itemname.Format("LeeInfoDialog__%p__%s__%d",this,"buttons",2);
	UDim ux1(0.6,-45);
	UnifiedDim udx1(ux1,DT_WIDTH);
	Dimension x1(udx1,DT_X_POSITION);
	d_cancel=CSoarRoot::getSingletonPtr()->createPopupWnd(itemname,x1,y,w,h,LWNDT_BUTTON,this,d_zPopupIndex+1);
	d_cancel->setText("取消");
}
LRESULT CLeeInfoDialog::HandleEvent(UINT uMsg ,WPARAM wParam ,LPARAM lParam)
{
	if (uMsg== SOAR_COMMAND)
	{
		SOARMSG* pMsg=(SOARMSG*)wParam;
		if (pMsg->sourceWnd == d_cancel)
		{
			setState(LWNDST_HIDE);
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
		}
		if (pMsg->sourceWnd == d_ok)
		{
			setState(LWNDST_HIDE);
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
		}
	}
	return CSoarWnd::HandleEvent(uMsg,wParam,lParam);
}