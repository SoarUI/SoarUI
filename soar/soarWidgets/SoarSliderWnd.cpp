#include "SoarSliderWnd.h"
#include "../SoarRoot.h"

CSoarSliderWnd::CSoarSliderWnd(HWND root,ISoarEngine* Eng,bool bvert):CSoarWnd(root,Eng),d_bVertbar(bvert)
{
	d_wndlook ="MenuBar";
	d_wndtype=LWNDT_SYS_MENUBAR;
	d_wndlookState="Normal";//Æ¤·ô×´Ì¬
	d_wndText ="";
	 d_wndState=LWNDST_SHOW;
	d_lbuttondown = FALSE;
	d_max=100;
	 d_min=0;
}

CSoarSliderWnd::~CSoarSliderWnd(void)
{
}
void CSoarSliderWnd::InitializeSegments(void)
{
	AbsoluteDim aDimx(0);
	AbsoluteDim aDimy(0);
	AbsoluteDim aDimH(15);
	AbsoluteDim aDimW(15);
	
	
	UDim dclose(1.0,-15);
	Dimension w(aDimW,DT_WIDTH);
	Dimension h(aDimH,DT_HEIGHT);
	Dimension y(aDimy,DT_TOP_EDGE);
	Dimension x(aDimx,DT_LEFT_EDGE);
	if(d_bVertbar)
	{
		d_wndlook ="VertSlider";
		//top
		 ComponentArea rcArea;
		rcArea.d_left= x;
		rcArea.d_top =y;
		rcArea.d_right_or_width=w;
		rcArea.d_bottom_or_height=h;
		d_right_or_up.d_area =rcArea;
		d_right_or_up.d_wndlook = "UpButton" ;
		d_right_or_up.d_wndlookState ="Normal";//Æ¤·ô×´Ì¬
		//down
		UDim dmin(1.0,-15);
		UnifiedDim uy(dmin,DT_HEIGHT);
		Dimension y1(uy,DT_Y_POSITION);
		rcArea.d_left= x;
		rcArea.d_top =y1 ;
		rcArea.d_right_or_width=w;
		rcArea.d_bottom_or_height=h;
		d_left_or_down.d_area =rcArea;
		d_left_or_down.d_wndlook = "DownButton" ;
		d_left_or_down.d_wndlookState ="Normal";//Æ¤·ô×´Ì¬
		//
		UDim dthunder(1.0,0);
		UnifiedDim udimthunder(dthunder,DT_WIDTH);
		w.setBaseDimension(udimthunder);
		aDimH.setValue(0);
		Dimension y2(aDimH,DT_Y_POSITION);
		rcArea.d_left= x;
		rcArea.d_top =y2 ;
		rcArea.d_right_or_width=w;
		rcArea.d_bottom_or_height=h;
		d_thunder.d_area =rcArea;
		d_thunder.d_wndlook = "VertSliderThumb" ;
		d_thunder.d_wndlookState ="Normal";//Æ¤·ô×´Ì¬
	}
	else
	{
		d_wndlook ="MenuBar";
		 ComponentArea rcArea;
		rcArea.d_left= x;
		rcArea.d_top =y;
		rcArea.d_right_or_width=w;
		rcArea.d_bottom_or_height=h;
		d_left_or_down.d_area =rcArea;
		d_left_or_down.d_wndlook = "LeftButton" ;
		d_left_or_down.d_wndlookState ="Normal";//Æ¤·ô×´Ì¬
		//
		UDim dx(1.0,-15);
		UnifiedDim ux(dx,DT_WIDTH);
		Dimension x1(ux,DT_X_POSITION);
		rcArea.d_left= x1;
		rcArea.d_top =y;
		rcArea.d_right_or_width=w;
		rcArea.d_bottom_or_height=h;
		d_right_or_up.d_area =rcArea;
		d_right_or_up.d_wndlook = "RightButton" ;
		d_right_or_up.d_wndlookState ="Normal";//Æ¤·ô
		//
		UDim dthunder(1.0,0);
		UnifiedDim udimthunder(dthunder,DT_HEIGHT);
		h.setBaseDimension(udimthunder);
		Dimension x2(aDimH,DT_X_POSITION);
		rcArea.d_left= x;
		rcArea.d_top =y ;
		rcArea.d_right_or_width=w;
		rcArea.d_bottom_or_height=h;
		d_thunder.d_area =rcArea;
		d_thunder.d_wndlook = "HorzSliderThumb" ;
		d_thunder.d_wndlookState ="Normal";//Æ¤·ô×´Ì¬
	}
}
float CSoarSliderWnd::getPos(void)
{
	return d_curPos;
}
void CSoarSliderWnd::setPos(float pos)
{
	if(pos<0)
		pos=0;
	if((int)pos>100)
		pos=100;
	d_curPos =pos;
	RECT rc =getWindowRect();
	AbsoluteDim aDimx(0);
	AbsoluteDim aDimy(0);
	AbsoluteDim aDimH(15);
	AbsoluteDim aDimW(15);
	Dimension w(aDimW,DT_WIDTH);
	Dimension h(aDimH,DT_HEIGHT);
	Dimension y(aDimy,DT_TOP_EDGE);
	Dimension x(aDimx,DT_LEFT_EDGE);
	ComponentArea rcArea;
	rcArea.d_left= x;
	rcArea.d_top =y;
	rcArea.d_right_or_width=w;
	rcArea.d_bottom_or_height=h;
	float pross =(float)d_curPos/(d_max-d_min+1);
	
	if(d_bVertbar)
	{
		
		aDimy.setValue((rc.bottom-rc.top)*pross-7);
		y.setBaseDimension(aDimy);
		rcArea.d_top =y;
		UDim dthunder(1.0,0);
		UnifiedDim udimthunder(dthunder,DT_WIDTH);
		w.setBaseDimension(udimthunder);
		rcArea.d_right_or_width=w;
	}
	else
	{
		aDimx.setValue((rc.right-rc.left)*pross-7);
		x.setBaseDimension(aDimx);
		rcArea.d_left =x;
		//
		UDim dthunder(1.0,0);
		UnifiedDim udimthunder(dthunder,DT_HEIGHT);
		h.setBaseDimension(udimthunder);
		rcArea.d_bottom_or_height=h;
	}
	d_thunder.d_area =rcArea;
}
DWORD CSoarSliderWnd::setRange(int nMin,int nMax)
{
	d_min =nMin;
	d_max =nMax ;
	return 0;
}
bool  CSoarSliderWnd::getRange(int &nMin,int &nMax)
{
	nMax =d_max;
	nMin = d_min ;
	return true;
}


//ÊÂ¼þ´¦Àí
BOOL CSoarSliderWnd::HandleEvent ( UINT uMsg ,WPARAM wp ,LPARAM lp, LRESULT& lr)
{
	//×Ó´°¿Ú»æÖÆ
	RECT rc =getWindowRect();
	POINT pt;
		::GetCursorPos(&pt);
		::ScreenToClient(d_rootWnd,&pt);
		RectF rcc;
		RECT rctest;
		if(uMsg==WM_LBUTTONDOWN)
		{
			int xPos = (int)(short)LOWORD(lp);
			int yPos =(int)(short)HIWORD(lp);
			rcc =d_thunder.d_area.getRect(rc);
			 rcc.toWindowRect(rctest);
			 if(!::PtInRect(&rctest,pt))
			 {
				return false;
			 }
			 d_lbuttondown = TRUE;
			 lr = 0;
			 return true;
		}
		if(uMsg==WM_LBUTTONUP)
		{
			d_lbuttondown = false;
			int xPos = (int)(short)LOWORD(lp);
			int yPos =(int)(short)HIWORD(lp);
			 float pross =0;
			 rcc=rc;
			 if(d_bVertbar)
			{
				pross =(float)(yPos-rcc.d_top)/rcc.Height()*(d_max-d_min+1);
			}
			else
			{
				pross =(float)(xPos-rcc.d_left)/rcc.Width()*(d_max-d_min+1);
			}
			 if(d_lbuttondown)
			{
				int d_prePos=d_curPos;
				setPos(pross);
					  // Ïò¸¸´°¿Ú·¢ËÍWM_POSCHANGING
				SOARMSG leeMsg;
				leeMsg.message =SOAR_SLIDERCHANGED;
				leeMsg.mouseEvent =SOAR_LCLICK_UP;
				leeMsg.targetWnd =d_parent;
				leeMsg.sourceWnd=this;
				leeMsg.wParam =d_prePos;
				leeMsg.lParam =pross;
				leeMsg.Data=NULL;
				leeMsg.msgSourceTag=SOAR_MSG_ORIG;
				CSoarRoot::getSingletonPtr()->addOfflineMsg(leeMsg);
			 }
			 lr = 0;
			 return true;
		}
		
	if (uMsg==WM_MOUSEMOVE )
	{
		int xPos = (int)(short)LOWORD(lp);
		int yPos =(int)(short)HIWORD(lp);
		if(d_lbuttondown){
			 rcc=rc;
			  float pross =0;
			 if(d_bVertbar)
			{
				pross =(float)(yPos-rcc.d_top)/rcc.Height()*(d_max-d_min+1);
			}
			else
			{
				pross =(float)(xPos-rcc.d_left)/rcc.Width()*(d_max-d_min+1);
			}
			int d_prePos=d_curPos;
			setPos(pross);
					// Ïò¸¸´°¿Ú·¢ËÍWM_POSCHANGING
			SOARMSG leeMsg;
			leeMsg.message =SOAR_SLIDERCHANGED;
			leeMsg.mouseEvent =SOAR_LCLICK_UP;
			leeMsg.targetWnd =d_parent;
			leeMsg.sourceWnd=this;
			leeMsg.wParam =d_prePos;
			leeMsg.lParam =pross;
			leeMsg.Data=NULL;
			leeMsg.msgSourceTag=SOAR_MSG_ORIG;
			CSoarRoot::getSingletonPtr()->addOfflineMsg(leeMsg);
		}
	}
	return CSoarWnd::HandleEvent(uMsg, wp, lp,lr);//ÁôÏµÍ³µ×²ã´¦Àí
}
void CSoarSliderWnd::DrawSelf(ILeeDrawInterface *DrawFuns) 
{
	if ((getState()<= LWNDST_HIDE) )
	{
		return;
	}
	bool bPreState =DrawFuns->graphic_AlphaBlendState();
	DrawFuns->graphic_AlphaBlend(d_bEnableAlphaBlend);
	RECT rc =getWindowRect();
	RectF rcc=rc;
	RectF rcparent =getCliperRect();
	//Ö÷´°¿Ú»æÖÆ
	if (d_DrawEng->hasWidgetlook(d_wndlook))
	{
		d_DrawEng->DrawWidgetlookFrame(d_wndlook,d_wndlookState,rcc,&rcparent,true);
		d_DrawEng->DrawWidgetlook(d_wndlook,d_wndlookState,rcc,d_wndText,-1,-1,&rcparent,true);
	}
	//×Ó´°¿Ú»æÖÆ
	//×ó°´Å¥
	rcc =d_left_or_down.d_area.getRect(rc);
	// d_DrawEng->DrawWidgetlook(d_left_or_down.d_wndlook,d_left_or_down.d_wndlookState,
	//	 rcc,d_left_or_down.d_wndText,-1,-1,&rcparent,true);
	 //ÓÒ°´Å¥
	 rcc =d_right_or_up.d_area.getRect(rc);
	// d_DrawEng->DrawWidgetlook(d_right_or_up.d_wndlook,d_right_or_up.d_wndlookState,
	//	 rcc,d_right_or_up.d_wndText,-1,-1,&rcparent,true);
	 //thunder HorzSliderThumb
	 //½ø¶È
	 rcc =d_thunder.d_area.getRect(rc);
	 d_DrawEng->DrawWidgetlook(d_thunder.d_wndlook,d_thunder.d_wndlookState,
		 rcc,d_thunder.d_wndText,-1,-1,&rcparent,true);
	//»Ö¸´Ô­À´µÄ»ìºÏ×´Ì¬
	 DrawFuns->graphic_AlphaBlend(bPreState);
}
void CSoarSliderWnd::checklookState(POINT &pt)
{
	RECT rc =getWindowRect();//È¡µÃÏà¶ÔÓÚRoot´°¿ÚµÄÆ«ÒÆ
	RectF rcc;
	RECT rctest;
	rcc =d_right_or_up.d_area.getRect(rc);
	rcc.toWindowRect(rctest);
	if(::PtInRect(&rctest,pt))
	{
		d_right_or_up.d_wndlookState ="Hover";//Æ¤·ô×´Ì¬		
		return ;
	}
	rcc =d_left_or_down.d_area.getRect(rc);
	rcc.toWindowRect(rctest);
	if(::PtInRect(&rctest,pt))
	{	
		d_left_or_down.d_wndlookState ="Hover";//Æ¤·ô×´Ì¬
		return ;
	}
	
	CLeeString state=d_wndlookState;
	if (::PtInRect(&rc,pt))
	{
		d_wndlookState ="Hover";
	}
	else
		d_wndlookState ="Normal";
}
