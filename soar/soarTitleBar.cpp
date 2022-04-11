#include "soarTitleBar.h"
#include "soarRoot.h"

CSoarTitleBar::CSoarTitleBar(ISoarWnd *parent,ISoarEngine* Eng):d_DrawEng(Eng),d_OwnerWnd(parent),d_parent(NULL),d_ReceiverWnd(NULL)
{
	d_wndlook ="TitleBar";
	d_wndtype=LWNDT_SYS_TITLEBAR;
	d_wndlookState="Normal";//Ƥ��״̬
	d_lbuttondown = FALSE;
	d_barAlign = SOARALIGN_TOP;
	d_wndText =_T("Soar auto your life");
	d_wndState=LWNDST_SHOW;
}

CSoarTitleBar::~CSoarTitleBar(void)
{
}
LRESULT CSoarTitleBar::HandleEvent ( UINT uMsg ,WPARAM wParam ,LPARAM lParam) 
{
	//���ȼ��С�ؼ�
	//�Ӵ��ڻ���
	RECT rc =getBarRect();
	POINT pt;
		::GetCursorPos(&pt);
		::ScreenToClient(d_OwnerWnd->getRootWnd(),&pt);
		RectF rcc;
	SEGMENTITEMS::iterator it =d_items.begin();
	 while (it != d_items.end())
	 {
		 rcc =((*it).d_area.getRect(rc));
		 RECT rctest;
		 rcc.toWindowRect(rctest);
		 if(::PtInRect(&rctest,pt))
		 {
			 MSG msg={NULL,uMsg,wParam,lParam};
			SOARMSG leeMsg;
			leeMsg.message =SOAR_COMMAND;
			leeMsg.mouseEvent =CSoarRoot::getSingletonPtr()->translateMouseMessage(msg,false);
			leeMsg.sourceWnd =d_OwnerWnd;
			leeMsg.routeWnd=d_OwnerWnd;
			leeMsg.targetWnd =d_OwnerWnd;
			leeMsg.wParam =MAKEWPARAM((*it).d_nId,BN_CLICKED);
			leeMsg.lParam =0;
			CSoarRoot::getSingletonPtr()->addOfflineMsg(leeMsg);
			//PostMessage(d_OwnerWnd->getRootWnd(),WM_COMMAND,MAKEWPARAM((*it).d_nId,BN_CLICKED),0);
			 return CSoarRoot::getSingletonPtr()->SoarDefWndProc(uMsg,wParam,lParam);//��ϵͳ�ײ㴦��;;
		 }
		 ++it;
	 }
	if (d_OwnerWnd->getType()==LWNDT_SYS_SHEET)//�봰�ڰ���һ��ģ����Բٿش���API��LWNDT_SYS_UI_SHEET:һ���ƶ��Ĳ㣬���̶�/�̶���ԭ��
	{
		return HandSheetEvent(uMsg,wParam,lParam);
	}
	return HandSimpleEvent(uMsg,wParam,lParam);
}
LRESULT CSoarTitleBar::HandSheetEvent(UINT uMsg ,WPARAM wParam ,LPARAM lParam)
{
	static int delX =0;
	static int delY =0;
	static RECT rcupdate;
	if (uMsg == WM_LBUTTONDOWN)
	{
		SetCapture(d_OwnerWnd->getRootWnd());
		d_lbuttondown = TRUE;
		//
		int xPos = (int)(short)LOWORD(lParam); 
		int yPos =(int)(short)HIWORD(lParam);
		RECT rc =getBarRect();//ȡ������ڱ����ڵ�ƫ��,��ϵͳ����
		delX =xPos-rc.left;
		delY =yPos-rc.top;
	}
	if (uMsg == WM_LBUTTONUP)
	{
		d_lbuttondown = FALSE;
		ReleaseCapture();
	}
	if(uMsg == WM_LBUTTONDBLCLK)
	{
		 MSG msg={NULL,uMsg,wParam,lParam};
			SOARMSG leeMsg;
			leeMsg.message =SOAR_COMMAND;
			leeMsg.mouseEvent =CSoarRoot::getSingletonPtr()->translateMouseMessage(msg,false);
			leeMsg.sourceWnd =d_OwnerWnd;
			leeMsg.routeWnd=d_OwnerWnd;
			leeMsg.targetWnd =d_OwnerWnd;
			leeMsg.wParam =MAKEWPARAM(-1,BN_CLICKED);
			leeMsg.lParam =0;
			CSoarRoot::getSingletonPtr()->addOfflineMsg(leeMsg);
			return CSoarRoot::getSingletonPtr()->SoarDefWndProc(uMsg,wParam,lParam);//��ϵͳ�ײ㴦��;;
	}
	if (uMsg == WM_MOUSEMOVE)
	{
		POINT pt;
		::GetCursorPos(&pt);
		::ScreenToClient(d_OwnerWnd->getRootWnd(),&pt);


		if (d_lbuttondown)
		{
			int xPos = (int)(short)LOWORD(lParam); 
			int yPos =(int)(short)HIWORD(lParam);
			
			RECT rc ;
			::GetWindowRect( d_OwnerWnd->getRootWnd() ,&rc);
			int W =rc.right-rc.left;
			int H= rc.bottom-rc.top;
			int deltaX =xPos-delX ;//�������ڸ����ڵ�ƫ��
			int deltaY =yPos-delY;
			pt.x=deltaX;
			pt.y=deltaY;
			RECT rcWnd ;
			 //::GetWindowRect( d_OwnerWnd->getRootWnd(),&rcWnd);
			//ƫ��+ԭʼ��Ļ����
			pt.x +=rc.left;
			pt.y +=rc.top;
			MoveWindow(d_OwnerWnd->getRootWnd() ,pt.x,pt.y,W,H,TRUE);

		}

	}
	return CSoarRoot::getSingletonPtr()->SoarDefWndProc(uMsg,wParam,lParam);//��ϵͳ�ײ㴦��;
}
LRESULT CSoarTitleBar::HandSimpleEvent(UINT uMsg ,WPARAM wParam ,LPARAM lParam)
{
	/*
	����d_area��ŵ���������������ĸ����ڵģ����Ľ����d_area����������ƫ�Ƹ����ڵ�left��top����Ⱦ��ʱ����㣬������ʱʹ��SetWindowPos��ʱ��ֱ�Ӱ��루0��0�������ƫ�Ʒ�
	��ȥ�������λ�ò��ԣ��Ǿ� Ҫ�鿴���Ƿ����Ǽ�ȥ�����ڵ�ƫ��2021-12-12
	*/
	static int posX =0;
	static int posY =0;
	static int delY_this_offset_parent_Top =0;
	static int delY_this_offset_parent_Bottom =0;
	static int delX_this_offset_parent =0;
	static int delX_this_offset_parent_Right =0;
	static RECT rcupdate;
	if (uMsg == WM_LBUTTONDOWN)
	{
		d_lbuttondown = TRUE;
		int xPos = (int)(short)LOWORD(lParam);
		int yPos =(int)(short)HIWORD(lParam);
		RECT rc =d_OwnerWnd->getWindowRect();//ȡ������ڸ����ڵ�ƫ��
		delX_this_offset_parent =xPos-rc.left;//�������ʱ,���X�봰�����Ͻǵ�ƫ�ƣ��Դ���Ϊ������
		delY_this_offset_parent_Top =yPos-rc.top;//�������ʱ,���X�봰�����Ͻǵ�ƫ��
		RECT rcBar =getBarRect();
		delY_this_offset_parent_Bottom =(rcBar.bottom-rcBar.top)-delY_this_offset_parent_Top;
		delX_this_offset_parent_Right=rc.right-xPos;
		posX =xPos;//��¼���λ��
		posY =yPos;
		//���ø�����Ϊ�����
		CSoarRoot::getSingletonPtr()->setFocusWnd(d_OwnerWnd);
	}
	if (uMsg == WM_LBUTTONUP)
	{
		d_lbuttondown = FALSE;
	}
	if (uMsg == WM_MOUSEMOVE)
	{
		

		if (d_lbuttondown)
		{
			/*
			��POwner��Parent��Ϊ�ο�ϵ,�ڴ˹����У�parent��Բ���
			*/
			int xPos = (int)(short)LOWORD(lParam); 
			int yPos =(int)(short)HIWORD(lParam);
			
			ISoarWnd* pParentWnd =d_OwnerWnd->GetParent();
			RECT rcpParent;
			if(pParentWnd){
				rcpParent =pParentWnd->getClientRect();
			}
			else{
				::GetClientRect(d_OwnerWnd->getRootWnd(),&rcpParent);
			}
			RECT rc =d_OwnerWnd->getWindowRect();
			//�������ֻ���ڸ����ڵĿͻ���
			//ͨ����꣬���Ի���ƶ���þ������X��Y delX+��=pt.x
			int foffsetX =xPos-posX;
			int foffsetY =yPos-posY;
			int left =xPos-delX_this_offset_parent-rcpParent.left;//�������ڸ����ڵĸ����ڵ�ƫ��
			int top =yPos-delY_this_offset_parent_Top-rcpParent.top;
			//��Ұ����
			/*if(left<= rcpParent.left)
				left=0;*/
			if (left>=rcpParent.right)
			{
				left =rcpParent.right/*-delX_this_offset_parent*/-rcpParent.left;
			}
			RECT rcBar =getBarRect();
			int rcBarH =rcBar.bottom-rcBar.top;
			//Y������
			/*if(top<-delY_this_offset_parent_Top)
				top=0;*/
			if (top>=rcpParent.bottom-rcBarH)
			{
				top =rcpParent.bottom-rcBarH-rcpParent.top;
			}
			//�������
			if(left<0)
				left=0;
			if(top<0)
				top=0;
			d_OwnerWnd->setWindowPos(left,top);//����OWner�븸���ڵ������
		}
	}
	
	return CSoarRoot::getSingletonPtr()->SoarDefWndProc(uMsg,wParam,lParam);//��ϵͳ�ײ㴦��;
}
void CSoarTitleBar::DrawSelf(ILeeDrawInterface *DrawFuns) 
{
	if ((d_OwnerWnd ==NULL || 
		d_OwnerWnd->getState()<= LWNDST_HIDE) )
	{
		return;
	}
	RECT rc =getBarRect();
	RectF rcc=rc;
	RectF rcparent =getBarCliperRect();
	//�����ڻ���
	if (d_DrawEng->hasWidgetlook(d_wndlook))
	{
		d_DrawEng->DrawWidgetlookFrame(d_wndlook,d_wndlookState,rcc,&rcparent,true);
		d_DrawEng->DrawWidgetlook(d_wndlook,d_wndlookState,rcc,d_wndText,-1,-1,&rcparent,true);
	}
	//�Ӵ��ڻ���
	SEGMENTITEMS::iterator it =d_items.begin();
	 while (it != d_items.end())
	 {
		 rcc =((*it).d_area.getRect(rc));
		 d_DrawEng->DrawWidgetlook((*it).d_wndlook,(*it).d_wndlookState,rcc,(*it).d_wndText,-1,-1,&rcparent,true);
		 ++it;
	 }
	 
}

RECT CSoarTitleBar::getBarRect(void) 
{
	RECT rcTmp;
	RECT rcParent;
	rcParent =d_OwnerWnd->getWindowRect();//��ȡ�����ڰ����ǿͻ���
	RECT rc;
	RectF rcCur;
	rcCur=d_area.getRect(rcParent);
	rcCur.toWindowRect(rc);
	return rc;
}
RECT CSoarTitleBar::getBarCliperRect(void){
	RECT rcCur;
	if(!d_parent)
	{
		rcCur =getBarRect();
	}
	else{
		rcCur =d_parent->getBarRect();
	}
	RectF rccCur =rcCur;
	RECT rcP ;
    if(!d_parent)
	{
		rcP =d_OwnerWnd->getCliperRect();
	}
	else{
		rcP =d_parent->getBarCliperRect();
	}
	RectF rccP =rccCur.Intersection(rcP);
	RECT rc;
	rccP.toWindowRect(rc);
	return rc;
}
void CSoarTitleBar::setArea(ComponentArea& area)
{
	d_area =area;
}
void CSoarTitleBar::setArea(RECT& rc)
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
ComponentArea CSoarTitleBar::getArea()
{
	return d_area;
}
BOOL CSoarTitleBar::BarHitTest(void)
{

	POINT pt;
	::GetCursorPos(&pt);
	::ScreenToClient( d_OwnerWnd->getRootWnd() ,&pt);
	RECT rc =getBarRect();
	if(::PtInRect(&rc,pt))
		return true;
	//��갴�µ��������ҪʱWM_MOUSEMOVE�Ĵ���
	if(d_lbuttondown)
		return true;
	return false;
}
LWNDT CSoarTitleBar::getType(void) 
{
	return d_wndtype;
}
SOARBARALIGN CSoarTitleBar::getBarAlign(void)
{
	return d_barAlign;
}
void CSoarTitleBar::setTitle(CLeeString name)
{
	d_wndText = name;
}
//////////////////////////
bool CSoarTitleBar::AddItem(const SoarSegmentItem& item)
{
	//��һ�����ʵ�ID
	d_items.push_back(item);
	return true;
}
bool CSoarTitleBar::removeItem(const DWORD dId)
{
	SEGMENTITEMS::iterator it= d_items.begin();
	while(it != d_items.end())
	{
		if( it->d_nId == dId)
		{
			d_items.erase(it);
			return true;
		}
		++it;
	}
	return false ;
}
SoarSegmentItem CSoarTitleBar::getItem(const DWORD dId)
{
	SEGMENTITEMS::iterator it= d_items.begin();
	while(it != d_items.end())
	{
		if( it->d_nId == dId)
		{
			
			return (*it);
		}
		++it;
	}
	SoarSegmentItem itempty;
	return itempty ;
}
int CSoarTitleBar::getItemCount(void)
{
	return d_items.size();
}
void CSoarTitleBar::setState(LWNDST state) 
 {
	 if (d_wndState != state)
	 {
		d_wndState =state;
	 }
 }
 LWNDST CSoarTitleBar::getState(void)
 {
	 return d_wndState;
 }