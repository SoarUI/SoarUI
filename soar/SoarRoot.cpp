#include "SoarRoot.h"
#include "SoarSheet.h"
#include "Soar.h"
#include "SoarWnd.h"
#include "SoarListbox.h"
#include "SoarStaticItemWnd.h"
#include "SoarPopupListbox.h"
#include "LeeTabWnd.h"
#include "LeeHeaderWnd.h"
#include "SoarListViewWnd.h"
#include "SoarFileDialog.h"
#include "soarComboWnd.h"
#include "SoarHorzProgress.h"
#include "SoarVertProgress.h"
#include "SoarMenuWnd.h"
#include "SoarButtonWnd.h"
#include "SoarRadioWnd.h"
#include "SoarCheckButtonWnd.h"
#include "SoarStaticWnd.h"
#include "SoarInternetWnd.h"
#include "soarTitleBar.h"
#include "SoarBindSheetWnd.h"
#include "SoarEditWnd.h"
#include "SoarComboExWnd.h"
#include "LeePopupListViewWnd.h"
#include "LeeInfoDialog.h"
#include "SoarGroupBox.h"
#include "SoarUISheetWnd.h"
#include "SoarSpinWnd.h"
#include "SoarWidgets/SoarSliderWnd.h"


CSoarRoot::LeeWndMap CSoarRoot::d_Wnds;
CSoarRoot::LeeWndZMap CSoarRoot::d_WndZs;
CSoarRoot::LeeWndMap CSoarRoot::d_PopupWnds;
CSoarRoot::LeeWndZMap CSoarRoot::d_PopupWndZs;
ISoarEngine* CSoarRoot::d_DrawEngine =NULL;
//ISoarWnd* CSoarRoot::d_focusWnd =NULL;
CSoarRoot::CSoarRoot(ISoarEngine* Eng):d_bindingSheet(NULL),d_oldWndProc(NULL),d_bEnableAlphaBlend(false)
{
	d_DrawEngine =Eng;
}

CSoarRoot::~CSoarRoot(void)
{
	//先删除所有的挂接
	LEESHEETMAP::iterator stlink = d_sheetlists.begin();
	while(stlink !=d_sheetlists.end())
	{
		ISoarSheet* pSheet=(*stlink).second;
		pSheet->clearAllLinkWnd();
		++stlink;
	}
	LeeWndMap::iterator it =d_PopupWnds.begin();
	ISoarWnd * pWnd=NULL;
	while(it !=d_PopupWnds.end())
	{
		pWnd=(*it).second;
		d_PopupWnds.erase(it);
		delete pWnd;
		it =d_PopupWnds.begin();
	}
	it =d_Wnds.begin();
	while(it !=d_Wnds.end())
	{
		pWnd=(*it).second;
		d_Wnds.erase(it);
		delete pWnd;
		it =d_Wnds.begin();
	}
	//删除Sheet
	LEESHEETMAP::iterator st = d_sheetlists.begin();
	while(st !=d_sheetlists.end())
	{
		ISoarSheet* pWnd=(*st).second;
		d_sheetlists.erase(st);
		delete pWnd;
		st =d_sheetlists.begin();
	}
	d_DrawEngine->ShutdownRender();
}
bool CSoarRoot::isValidateWnd(ISoarWnd* hWnd)
{
	if(!hWnd)
		return false;
	LeeWndMap::iterator it =d_PopupWnds.begin();
	while(it !=d_PopupWnds.end())
	{
		if(it->second == hWnd )
		return true ;
		++it;
	}
	it =d_Wnds.begin();
	while(it !=d_Wnds.end())
	{
		if(it->second == hWnd )
		return true ;
		++it;
	}
	//重新排列顺序
	return false ;
}
bool CSoarRoot::destroyWnd(ISoarWnd * destroyWnd)
{
	if ( !this->isValidateWnd(destroyWnd) )
	{
		return false;;
	}
	//从父窗口中摘掉
	ISoarWnd* parent = destroyWnd->GetParent();
	if (parent && this->isValidateWnd(parent))
	{
		parent->eraseChildWnd(destroyWnd);
	}
	//先断开Sheet连接
	if(d_theActivateSheet->seekWnd(destroyWnd)){
		//如果是主窗口
		d_theActivateSheet->eraseWndRecursive(destroyWnd);
	}
	else{//先查看堆栈，如有
		LEESHEETMAP::iterator itsheet= d_sheetlists.begin(); 
		while(itsheet != d_sheetlists.end()){
			ISoarSheet* pSheet =itsheet->second;
			if(pSheet->seekWnd(destroyWnd)){
			pSheet->eraseWndRecursive(destroyWnd);}
			++itsheet;
		}
	}
	ISoarWnd * pWnd=NULL;
	if (destroyWnd->isPopupWnd())
	{
		LeeWndMap::iterator it =d_PopupWnds.find(destroyWnd->getWndName());
		if(it !=d_PopupWnds.end())
		{
			pWnd=(*it).second;
			d_PopupWnds.erase(it);
			delete pWnd;
			return true ;
		}
		return false;
	}
	LeeWndMap::iterator it =d_Wnds.find(destroyWnd->getWndName());
	if(it !=d_Wnds.end())
	{
		pWnd=(*it).second;
		d_Wnds.erase(it);
		delete pWnd;
		return true ;
	}
	//重新排列顺序
	return false ;
}
bool CSoarRoot::eraseWnd(ISoarWnd * eraseWnd)
{
	if (eraseWnd->isPopupWnd())
	{
		LeeWndMap::iterator it =d_PopupWnds.find(eraseWnd->getWndName());
		if(it !=d_PopupWnds.end())
		{
			d_PopupWnds.erase(it);
			return true ;
		}
		return false;
	}
	LeeWndMap::iterator it =d_Wnds.find(eraseWnd->getWndName());
	if(it !=d_Wnds.end())
	{
		d_Wnds.erase(it);
		return true ;
	}
	//重新排列顺序
	return false ;
}

bool CSoarRoot::attachMsWindow(HWND hMainWnd)
{
	d_hOwner =hMainWnd;
#ifdef _WIN64
	WNDPROC theRemoteProc = (WNDPROC) ::GetWindowLong(hMainWnd, GWLP_WNDPROC);
#else
	WNDPROC theRemoteProc = (WNDPROC) ::GetWindowLong(hMainWnd, GWL_WNDPROC);
#endif
	d_oldWndProc =theRemoteProc;
	if (theRemoteProc !=CSoar::sLeeRootWndProc)
	{
#ifdef _WIN64
		d_oldWndProc = (WNDPROC) ::SetWindowLong(hMainWnd, GWLP_WNDPROC, (LONG)CSoar::sLeeRootWndProc);
#else
		d_oldWndProc = (WNDPROC) ::SetWindowLong(hMainWnd, GWL_WNDPROC, (LONG) CSoar::sLeeRootWndProc);
#endif
	}
	//创建一个mainSheet
	d_theActivateSheet =new CSoarSheet(d_hOwner);
	//
	d_bindingSheet =new CSoarBindSheetWnd(d_hOwner,d_DrawEngine);
	d_bindingSheet->setWndName("SOARSHEET");
	//获取信息
	RECT rcWind;
	::GetClientRect(hMainWnd,&rcWind);
	int srcW=rcWind.right-rcWind.left;
	int srcH=rcWind.bottom-rcWind.top;
	///-------------
	AbsoluteDim aDimx(0);
		AbsoluteDim aDimy(0);
		AbsoluteDim aDimH(500);
		AbsoluteDim aDimW(100);
	
		Dimension x(aDimx,DT_LEFT_EDGE);
		//Dimension h(aDimH,DT_HEIGHT);
		Dimension y(aDimy,DT_TOP_EDGE);

		UDim dmin(1.0,0);
		UnifiedDim ux(dmin,DT_WIDTH);
		Dimension w(ux,DT_WIDTH);
		UDim ud(1.0,0);
		UnifiedDim ux1(ud,DT_HEIGHT);
		Dimension h(ux1,DT_HEIGHT);
		 ComponentArea rcArea;
		 rcArea.d_left= x;
		rcArea.d_top =y;
		rcArea.d_right_or_width=w;
		rcArea.d_bottom_or_height=h;
		d_bindingSheet->setArea(rcArea);
	//默认为非弹出对话框
	d_bindingSheet->setZindex(0);
	d_theActivateSheet->addWndRecursive(d_bindingSheet);
	d_bindingSheet->InitializeSegments();
	((CSoarBindSheetWnd*)d_bindingSheet)->Bind2Window(hMainWnd);
	d_Wnds[d_bindingSheet->getWndName()] =d_bindingSheet;


	//获取窗口标题
	TCHAR sTitle[MAX_PATH]={0};
	::GetWindowText(hMainWnd,sTitle,MAX_PATH);
	d_bindingSheet->setText(sTitle);
	d_theActivateSheet->setPrevFocusWnd(d_bindingSheet);
	d_theActivateSheet->setMainWnd(d_bindingSheet);
	setFocusWnd(d_bindingSheet);
	d_sheetlists[d_bindingSheet->getWndName()]= d_theActivateSheet;
	return true ;
}
bool CSoarRoot::CreateSoarUI(HWND hMainWnd,Dimension& x,Dimension& y,Dimension& w,Dimension& h)
{
	d_hOwner =hMainWnd;
	d_oldWndProc =NULL;
	//创建一个mainSheet
	d_theActivateSheet =new CSoarSheet(d_hOwner);
	//
	d_bindingSheet =new CSoarUISheetWnd(d_hOwner,d_DrawEngine);
	d_bindingSheet->setWndName("SOARSHEET");
	///-------------
	

	ComponentArea rcArea;
	rcArea.d_left= x;
	rcArea.d_top =y;
	rcArea.d_right_or_width=w;
	rcArea.d_bottom_or_height=h;
	d_bindingSheet->setArea(rcArea);
	//画布有效性检测
	RECT rcWind;
	::GetClientRect(hMainWnd,&rcWind);
	int srcW=rcWind.right-rcWind.left;
	int srcH=rcWind.bottom-rcWind.top;
	RectF rcf;
	rcf.d_left=rcWind.left;
	rcf.d_right=rcWind.right;
	rcf.d_top=rcWind.top;
	rcf.d_bottom=rcWind.bottom;
	RectF Areaf =rcArea.getBoundingRect(rcf);
	if(Areaf.Height()>srcH ||Areaf.Width()>srcW){\
		AbsoluteDim adim(0.0);
	ComponentArea rcArea;
	adim.setValue(0);
	rcArea.d_left.setBaseDimension(adim);
	rcArea.d_left.setDimensionType(DT_LEFT_EDGE);
	adim.setValue(0);
	rcArea.d_top.setBaseDimension(adim);
	rcArea.d_top.setDimensionType(DT_TOP_EDGE);
	adim.setValue(srcW);
	rcArea.d_right_or_width.setBaseDimension(adim);
	rcArea.d_right_or_width.setDimensionType(DT_WIDTH);
	adim.setValue(srcH);
	rcArea.d_bottom_or_height.setBaseDimension(adim);
	rcArea.d_bottom_or_height.setDimensionType(DT_HEIGHT);
		d_bindingSheet->setArea(rcArea);
	}
	//默认为非弹出对话框
	d_bindingSheet->setZindex(0);
	d_theActivateSheet->addWndRecursive(d_bindingSheet);
	d_bindingSheet->InitializeSegments();
	((CSoarUISheetWnd*)d_bindingSheet)->Bind2Window(hMainWnd);
	d_Wnds[d_bindingSheet->getWndName()] =d_bindingSheet;


	//获取窗口标题
	TCHAR sTitle[MAX_PATH]={0};
	::GetWindowText(hMainWnd,sTitle,MAX_PATH);
	d_bindingSheet->setText(sTitle);
	d_theActivateSheet->setPrevFocusWnd(d_bindingSheet);
	d_theActivateSheet->setMainWnd(d_bindingSheet);
	setFocusWnd(d_bindingSheet);
	d_sheetlists[d_bindingSheet->getWndName()]= d_theActivateSheet;
	return true ;
}
bool CSoarRoot::CreateSoarUI(HWND hMainWnd,int x,int y,int W ,int H)
{
	d_hOwner =hMainWnd;
	d_oldWndProc =NULL;
	//创建一个mainSheet
	d_theActivateSheet =new CSoarSheet(d_hOwner);
	//
	d_bindingSheet =new CSoarUISheetWnd(d_hOwner,d_DrawEngine);
	d_bindingSheet->setWndName("SOARSHEET");
	//画布有效性检测
	RECT rcWind;
	::GetClientRect(hMainWnd,&rcWind);
	if(x<rcWind.left)
		x=rcWind.left;
	if(rcWind.top<0)
		rcWind.top=0;
	int srcW=rcWind.right-rcWind.left;
	int srcH=rcWind.bottom-rcWind.top;
	if(W>srcW)
		W=srcW;
	if(H>srcH)
		H=srcH;
	///-------------
	AbsoluteDim adim(0.0);
	ComponentArea rcArea;
	adim.setValue(x);
	rcArea.d_left.setBaseDimension(adim);
	rcArea.d_left.setDimensionType(DT_LEFT_EDGE);
	adim.setValue(y);
	rcArea.d_top.setBaseDimension(adim);
	rcArea.d_top.setDimensionType(DT_TOP_EDGE);
	adim.setValue(W);
	rcArea.d_right_or_width.setBaseDimension(adim);
	rcArea.d_right_or_width.setDimensionType(DT_WIDTH);
	adim.setValue(H);
	rcArea.d_bottom_or_height.setBaseDimension(adim);
	rcArea.d_bottom_or_height.setDimensionType(DT_HEIGHT);
		d_bindingSheet->setArea(rcArea);
	//默认为非弹出对话框
	d_bindingSheet->setZindex(0);
	d_theActivateSheet->addWndRecursive(d_bindingSheet);
	d_bindingSheet->InitializeSegments();
	((CSoarUISheetWnd*)d_bindingSheet)->Bind2Window(hMainWnd);
	d_Wnds[d_bindingSheet->getWndName()] =d_bindingSheet;


	//获取窗口标题
	TCHAR sTitle[MAX_PATH]={0};
	::GetWindowText(hMainWnd,sTitle,MAX_PATH);
	d_bindingSheet->setText(sTitle);
	d_theActivateSheet->setPrevFocusWnd(d_bindingSheet);
	d_theActivateSheet->setMainWnd(d_bindingSheet);
	setFocusWnd(d_bindingSheet);
	d_sheetlists[d_bindingSheet->getWndName()]= d_theActivateSheet;
	return true ;
}
void  CSoarRoot::setMainSheetTitle(CLeeString title)
{
	d_bindingSheet->setText(title);
}
HWND CSoarRoot::getMsWindow(void)  
{
	return d_hOwner;
}
//获取主窗口的大小：即客户区域大)
RECT CSoarRoot::getWindowRect(void)  
{
	return d_bindingSheet->getWindowRect();
}
//获取主窗口的大小：即客户区域大小
RECT CSoarRoot::getClientRect(void)  
{
	return d_bindingSheet->getClientRect();
}
ISoarWnd* CSoarRoot::getWnd(const CLeeString &windowName)  
{
	//从激活的sheet中查找
	ISoarWnd* tmp =NULL;
	tmp =d_theActivateSheet->getWnd(windowName);
	if(tmp)
		return tmp;
	LeeWndMap::iterator it;
	it =d_Wnds.find(windowName);
	if (it !=d_Wnds.end())
	{
		tmp =(*it).second;
		return tmp;
	}
	it =d_PopupWnds.find(windowName);
	if (it !=d_PopupWnds.end())
	{
		tmp =(*it).second;
		return tmp;
	}
	return NULL ;
}
ISoarWnd* CSoarRoot::createWnd(const CLeeString & name,LWNDT classType,ISoarWnd* Parent,INT dwZindex,LWNDSTY dwstyle)
{
	//如果父窗口不为NULL，检测
	if(isValidateWnd(Parent)){
		if(Parent->isPopupWnd())
			return createPopupWnd( name,classType,Parent,dwZindex,dwstyle);
	}
	d_WndCcSection.Enter();
	LeeWndMap::iterator it =d_Wnds.find(name);
	if (it !=d_Wnds.end())
	{
		d_WndCcSection.Leave();
		return (*it).second;
	}
	ISoarWnd * child =NULL;
	child =createWnd(classType,dwstyle);
	d_Wnds[name] =child;
	d_WndCcSection.Leave();
	Parent=Parent?Parent:d_theActivateSheet->getMainWnd();//当parent为NULL时，设置主底板为Parent
	child->setParent(Parent);
	child->setWndName(name);
	if(d_theActivateSheet)
		d_theActivateSheet->addWndRecursive(child);
	//初始化工作
	child->InitializeSegments();
	
	return child ;
}
ISoarWnd* CSoarRoot::createWnd(const CLeeString & name,int x,int y,int w,int h,LWNDT classType,ISoarWnd* Parent,INT dwZindex,LWNDSTY dwstyle) 
{
	//如果父窗口不为NULL，检测
	if(isValidateWnd(Parent)){
		if(Parent->isPopupWnd())
			return createPopupWnd( name,x,y,w,h,classType,Parent,dwZindex,dwstyle);
	}
	d_WndCcSection.Enter();
	LeeWndMap::iterator it =d_Wnds.find(name);
	if (it !=d_Wnds.end())
	{
		d_WndCcSection.Leave();
		return (*it).second;
	}
	ISoarWnd * child =NULL;
	child =createWnd(classType,dwstyle);
	
	d_Wnds[name] =child;
	d_WndCcSection.Leave();
	AbsoluteDim adim(0.0);
	ComponentArea rcArea;
	adim.setValue(x);
	rcArea.d_left.setBaseDimension(adim);
	rcArea.d_left.setDimensionType(DT_LEFT_EDGE);
	adim.setValue(y);
	rcArea.d_top.setBaseDimension(adim);
	rcArea.d_top.setDimensionType(DT_TOP_EDGE);
	adim.setValue(w);
	rcArea.d_right_or_width.setBaseDimension(adim);
	rcArea.d_right_or_width.setDimensionType(DT_WIDTH);
	adim.setValue(h);
	rcArea.d_bottom_or_height.setBaseDimension(adim);
	rcArea.d_bottom_or_height.setDimensionType(DT_HEIGHT);
	child->setArea(rcArea);
	child->setWndName(name);
	Parent=Parent?Parent:d_theActivateSheet->getMainWnd();//当parent为NULL时，设置主底板为Parent
	child->setParent(Parent);
	
	//如果parent是POPUP时,特殊处理
	INT curZindex =0;
	if(Parent->isPopupWnd())
	{
		child->setWndPopup(true);
		child->InitializeSegments();
	}
	else//simple wnd
	{
		child->setWndPopup(false);
	}
	if(d_theActivateSheet)
		d_theActivateSheet->addWndRecursive(child);
	//初始化工作
	child->InitializeSegments();
	
	return child ;
}
//统一坐标
ISoarWnd* CSoarRoot::createWnd(const CLeeString & name,Dimension& x,Dimension& y,Dimension& w,Dimension& h,
							 LWNDT classType ,ISoarWnd* Parent,INT dwZindex,LWNDSTY dwstyle)
{
	//如果父窗口不为NULL，检测
	if(isValidateWnd(Parent)){
		if(Parent->isPopupWnd())
			return createPopupWnd( name,x,y,w,h,classType,Parent,dwZindex,dwstyle);
	}
	d_WndCcSection.Enter();
	LeeWndMap::iterator it =d_Wnds.find(name);
	if (it !=d_Wnds.end())
	{
		d_WndCcSection.Leave();
		return (*it).second;
	}
	ISoarWnd * child =NULL;
	child =createWnd(classType,dwstyle);
	
	d_Wnds[name] =child;
	d_WndCcSection.Leave();
	RECT rc={0,0,0,0};
	AbsoluteDim adim(0.0);
	ComponentArea rcArea;
	rcArea.d_left= x;
	rcArea.d_top =y;
	rcArea.d_right_or_width=w;
	rcArea.d_bottom_or_height=h;
	child->setArea(rcArea);
	child->setWndName(name);
	Parent=Parent?Parent:d_theActivateSheet->getMainWnd();//当parent为NULL时，设置主底板为Parent
	child->setParent(Parent);
	
	//如果parent是POPUP时,特殊处理
	INT curZindex =0;
	if(Parent->isPopupWnd())
	{
		child->setWndPopup(true);
		child->InitializeSegments();
	}
	else//simple wnd
	{
		child->setWndPopup(false);
	}
	if(d_theActivateSheet)
		d_theActivateSheet->addWndRecursive(child);
	//初始化工作
	child->InitializeSegments();
	
	return child ;
}
//////////////////////////////////////////////////////////////////////////
ISoarWnd* CSoarRoot::createPopupWnd(const CLeeString & name,LWNDT classType,ISoarWnd* Parent,INT dwZindex,LWNDSTY dwstyle)
{
	d_PopupWndCcSection.Enter();
	LeeWndMap::iterator it =d_PopupWnds.find(name);
	if (it !=d_PopupWnds.end())
	{
		d_PopupWndCcSection.Leave();
		return (*it).second;
	}
	ISoarWnd * child =NULL;
	child =createWnd(classType,dwstyle);
	d_PopupWnds[name] =child;
	d_PopupWndCcSection.Leave();
	Parent=Parent?Parent:d_theActivateSheet->getMainWnd();//当parent为NULL时，设置主底板为Parent
	child->setParent(Parent);
	child->setWndName(name);
	child->setWndPopup(true);
	
	if(d_theActivateSheet)
		d_theActivateSheet->addWndRecursive(child);
	//初始化工作
	child->InitializeSegments();
	
	return child ;
}
ISoarWnd* CSoarRoot::createPopupWnd(const CLeeString & name,int x,int y,int w,int h,LWNDT classType,
								  ISoarWnd* Parent,INT dwZindex,LWNDSTY dwstyle) 
{
	d_PopupWndCcSection.Enter();
	LeeWndMap::iterator it =d_PopupWnds.find(name);
	if (it !=d_PopupWnds.end())
	{
		d_PopupWndCcSection.Leave();
		return (*it).second;
	}
	ISoarWnd * child =NULL;
	child =createWnd(classType,dwstyle);
	d_PopupWnds[name] =child;
	d_PopupWndCcSection.Leave();
	AbsoluteDim adim(0.0);
	ComponentArea rcArea;
	adim.setValue(x);
	rcArea.d_left.setBaseDimension(adim);
	rcArea.d_left.setDimensionType(DT_LEFT_EDGE);
	adim.setValue(y);
	rcArea.d_top.setBaseDimension(adim);
	rcArea.d_top.setDimensionType(DT_TOP_EDGE);
	adim.setValue(w);
	rcArea.d_right_or_width.setBaseDimension(adim);
	rcArea.d_right_or_width.setDimensionType(DT_WIDTH);
	adim.setValue(h);
	rcArea.d_bottom_or_height.setBaseDimension(adim);
	rcArea.d_bottom_or_height.setDimensionType(DT_HEIGHT);
	child->setArea(rcArea);
	child->setWndName(name);
	child->setWndPopup(true);
	Parent=Parent?Parent:d_theActivateSheet->getMainWnd();//当parent为NULL时，设置主底板为Parent
	child->setParent(Parent);
	
	
	if(d_theActivateSheet)
		d_theActivateSheet->addWndRecursive(child);
	//初始化工作
	child->InitializeSegments();

	return child ;
}
//统一坐标
ISoarWnd* CSoarRoot::createPopupWnd(const CLeeString & name,Dimension& x,Dimension& y,Dimension& w,Dimension& h,LWNDT classType ,
								  ISoarWnd* Parent,INT dwZindex,LWNDSTY dwstyle)
{
	d_PopupWndCcSection.Enter();
	LeeWndMap::iterator it =d_PopupWnds.find(name);
	if (it !=d_PopupWnds.end())
	{
		d_PopupWndCcSection.Leave();
		return (*it).second;
	}
	ISoarWnd * child =NULL;
	child =createWnd(classType,dwstyle);
	d_PopupWnds[name] =child;
	d_PopupWndCcSection.Leave();
	RECT rc={0,0,0,0};
	AbsoluteDim adim(0.0);
	ComponentArea rcArea;
	rcArea.d_left= x;
	rcArea.d_top =y;
	rcArea.d_right_or_width=w;
	rcArea.d_bottom_or_height=h;
	child->setArea(rcArea);
	child->setWndName(name);
	child->setWndPopup(true);
	Parent=Parent?Parent:d_theActivateSheet->getMainWnd();//当parent为NULL时，设置主底板为Parent
	child->setParent(Parent);
	
	//INT curZindex =d_theActivateSheet->generatePopupZindex(Parent);
	//	child->setPopupZindex(curZindex);
	if(d_theActivateSheet)
		d_theActivateSheet->addWndRecursive(child);
	//初始化工作
	child->InitializeSegments();
	
	return child ;
}

void CSoarRoot::NotifyChildZIndexChanged(ISoarWnd* ChildWnd,int newFocusZpos,int oldZindex)
{
	//如有激活的sheet 设置相应的layer层次
	if (d_theActivateSheet &&  ChildWnd != d_theActivateSheet->getMainWnd() )
	{
		d_theActivateSheet->WndZorderChanged(ChildWnd,newFocusZpos,oldZindex);
	}
}
//特殊接口：可以让用自定义绘制
 ISoarWnd* CSoarRoot::attachCustomClassWnd(ISoarWnd* Wnd,ISoarWnd* Parent,INT dwZindex) 
 {
	  LeeWndMap::iterator it;
	   INT curZindex ;
	    ISoarWnd * child =Wnd;
	   Parent=Parent?Parent:d_theActivateSheet->getMainWnd();//当parent为NULL时，设置主底板为Parent
	 if (Wnd->isPopupWnd())
	 {
		 it =d_PopupWnds.find(Wnd->getWndName());
		 if (it!= d_PopupWnds.end())
		 {
			 return NULL;
		 }
		 curZindex =dwZindex>=Parent->getPopupZindex()+1?dwZindex:Parent->getPopupZindex()+1;
		 child->setZindex(curZindex);
		 d_PopupWnds[Wnd->getWndName()]=child;
	 }
	 else
	 {
		 it =d_Wnds.find(Wnd->getWndName());
		 if (it!= d_Wnds.end())
		 {
			 return NULL;
		 }
		 curZindex =Parent?Parent->getZindex()+1:dwZindex;
		 child->setZindex(curZindex);
		 d_Wnds[Wnd->getWndName()] =child;
	 }
	 Wnd->attachRoot(d_hOwner);
	 child->setParent(Parent);
	//如有激活的sheet 设置相应的layer层次
	 if(d_theActivateSheet)
		d_theActivateSheet->addWndRecursive(Wnd);
	
	 return child ;
 }
void CSoarRoot::endPatch(void){
	if(d_DrawEngine){
		d_DrawEngine->endPatch();
	}
}
void CSoarRoot::beginPatch(void){
	if(d_DrawEngine){
		d_DrawEngine->beginPatch();
	}
}
void CSoarRoot::clearScene(void){
	if(d_DrawEngine){
		d_DrawEngine->clearScene();
	}
}
void CSoarRoot::beginScene(void){
	if(d_DrawEngine){
		d_DrawEngine->beginScene();
	}
}
void CSoarRoot::endScene(void){
	if(d_DrawEngine){
		d_DrawEngine->endScene();
	}
}
void CSoarRoot::Present(void) {
	if(d_DrawEngine){
		d_DrawEngine->Present();
	}
}
bool CSoarRoot::enableAlphaBlend(bool bEnable)
 {
	 if(d_bEnableAlphaBlend != bEnable){
		 d_bEnableAlphaBlend =bEnable;
		 return true;
	 }
	 return false;
 }
bool CSoarRoot::SoarUIRenderLoop(float t) 
{
	
	ILeeDrawInterface DrawFuns;
	DrawFuns.hdc =NULL;
	DrawFuns.LeeDrawWidget =&CSoarRoot::CustomDrawSelfT;
	DrawFuns.LeeDrawWidgetImg=&CSoarRoot::CustomDrawSelf;
	DrawFuns.graphic_AlphaBlend=&CSoarRoot::CustomEnableAlphaBlend;
	DrawFuns.graphic_AlphaBlendState=&CSoarRoot::CustomGraphicGetAlphaBlendState;
	bool bPreState =DrawFuns.graphic_AlphaBlendState();
	DrawFuns.graphic_AlphaBlend(d_bEnableAlphaBlend);
	//先渲染主面板（必须最先渲染）
//////////////////////////////////////////////////////////////////////////
	//对于弹出对话框，要处理之前的激活窗口
	LEESHEETSTACK::iterator  it = d_sheetStack.begin();
	while( it !=d_sheetStack.end() )
	{
		(*it)->RenderLoop(&DrawFuns,t);
		++it;
	}
	//////////////////////////////////////////////////////////////////////////
	//渲染激活的Sheet，除了系统级窗口
	if ( d_theActivateSheet )
	{
		d_theActivateSheet->RenderLoop(&DrawFuns,t);
		
	}
	//渲染系统级
	//d_DrawEngine->endPatch();
	DrawFuns.graphic_AlphaBlend(bPreState);
	return true;
}

/************************************************************************/
/* pFocus/d_focusWnd 都可能为NULL                                       */
/************************************************************************/
ISoarWnd* CSoarRoot::setFocusWnd(ISoarWnd* pFocus)
{
	return d_theActivateSheet->setFocusWnd(pFocus) ;
}


//////////////////////////////////////////////////////////////////////////
/*
msg.message==WM_LBUTTONDOWN ||msg.message==WM_LBUTTONDBLCLK||
msg.message == WM_RBUTTONDOWN ||
msg.message == WM_RBUTTONDBLCLK
*/
//////////////////////////////////////////////////////////////////////////

 //直接让ROOT运行即可，意味它只给激活的窗口运行而已
bool CSoarRoot::filter_MouseMessage(UINT uMsg,WPARAM wParam,LPARAM lParam,LRESULT& lr)
 {
	 if (uMsg < WM_MOUSEFIRST ||uMsg>WM_MOUSELAST)
	 {
		 return false;
	 }
	 MSG msg={CSoarRoot::getSingletonPtr()->getMsWindow(),uMsg,wParam,lParam};
	return CSoarRoot::getSingletonPtr()->RunSoarMsg(msg,lr);
 }
bool CSoarRoot::filter_allMessage(UINT uMsg,WPARAM wParam,LPARAM lParam,LRESULT& lr)
{
	if (uMsg == WM_PAINT ||uMsg==WM_MOUSEMOVE )
	{
		return false;
	}
	MSG msg={CSoarRoot::getSingletonPtr()->getMsWindow(),uMsg,wParam,lParam};
	return CSoarRoot::getSingletonPtr()->RunSoarMsg( msg ,lr);
}
//我们只作为UI附加到目标窗口
BOOL CSoarRoot::InJectMsg(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg ==WM_SIZE)
	{
		d_DrawEngine->DisplayReset();
		//更新地板大小
		int nWidth = LOWORD(lParam); // width of client area
		int nHeight = HIWORD(lParam); // height of client area

	}
	LRESULT lr =0;
	if(d_theActivateSheet)//主要处理激活消息
	{
		d_theActivateSheet->SoarUIMsgProc(  uMsg,wParam ,lParam,lr ) ;
	}
	////默认点击在主sheet登记的窗口上
	//MSG msg={getMsWindow(),uMsg ,wParam,lParam};
	//RunTopSheetMsg(msg,lr);
	return true;
}
//我们创建地窗口或者Hook了
 LRESULT CSoarRoot::SoarUIMsgProc ( UINT uMsg,WPARAM wParam ,LPARAM lParam) 
{
	LRESULT lr =0;
	if (uMsg ==WM_SIZE /*&& wParam ==SIZE_MAXIMIZED*/)
	{
		d_DrawEngine->DisplayReset();
	}
	if (uMsg == WM_PAINT)
	 {
		 PAINTSTRUCT lpstr;
		 ::BeginPaint(d_hOwner,&lpstr);
		 ::EndPaint( d_hOwner,&lpstr );
		 return 0;
	 }
	if (uMsg == WM_ERASEBKGND)
	{
		lr =1;
		return TRUE;
	}
	if(d_theActivateSheet)//主要处理激活消息
	{
		if(d_theActivateSheet->SoarUIMsgProc(  uMsg,wParam ,lParam,lr ) )
		{
			return lr;
		}
	}
	//这里处理用户挂接的事件
	return this->SoarDefWndProc(  uMsg , wParam , lParam ) ;//默认处理
}
LRESULT CSoarRoot::SoarDefWndProc ( UINT uMsg,WPARAM wParam ,LPARAM lParam )
{
	//这里处理用户挂接的事件
	if (d_oldWndProc && d_oldWndProc != CSoar::sLeeRootWndProc)
	{
		return ::CallWindowProc( d_oldWndProc ,d_hOwner,uMsg ,wParam ,lParam ) ;//默认处理
	}
	if(d_oldWndProc) //由Soar产生的窗口
		return ::DefWindowProc( d_hOwner, uMsg , wParam , lParam ) ; ;//默认处理
	return 0;
}
/*
模态对话框支持
*/
void CSoarRoot::StartModal(ISoarWnd* hModelWnd)
{
	//创建一个模态
	ISoarSheet* pCur = new CSoarSheet(d_hOwner,true);
	pCur->setPrevFocusWnd(d_theActivateSheet->getFocusWnd() );
	pCur->setModalState(true);
	//设置该层的相关信息
	pCur->setStartPopupZindex(d_theActivateSheet->getMaxPopupZindex()+1);
	pCur->setStartZindex(d_theActivateSheet->getMaxZindex()+1);
	//却换当前激活sheet到模态sheet，先保存现场
	d_sheetStack.push_back(d_theActivateSheet);
	ISoarSheet* pPastSheet =d_theActivateSheet;
	d_theActivateSheet =pCur;
	if(hModelWnd !=NULL){
		hModelWnd->setWndModal(true);
		//先移除连接
	pPastSheet->removeWndRecursive(hModelWnd);
	//设置默认主窗口
	d_theActivateSheet->setMainWnd(hModelWnd);
	//设置默认对话框为激活窗口
	d_theActivateSheet->setFocusWnd(hModelWnd);
	pCur->addWndRecursive(hModelWnd);
	}
	
	
	
}
int CSoarRoot::EndModal(){
	if(d_sheetStack.empty()){
		return 1;
	}
	//恢复之前的激活窗口
	 ISoarWnd *focusWnd =d_theActivateSheet->getPrevFocusWnd();
	//恢复前一个激活的sheet
	ISoarSheet* pCurDlg = d_theActivateSheet;
	ISoarWnd *mainWnd =d_theActivateSheet->getMainWnd();
	if(!mainWnd->isModalWnd()){
		return 1;
	}
	
	//恢复现场
	 ISoarSheet* pCur =d_sheetStack.back();
	 //恢复之前的激活窗口--可以省略因为每个Sheet都保留自己的堆栈
	d_sheetStack.pop_back();
	pCur->setFocusWnd(focusWnd);
	pCur->addWndRecursive(pCurDlg->getMainWnd());
	//
	pCurDlg->removeWndRecursive(pCurDlg->getMainWnd());
	d_theActivateSheet =pCur;
	mainWnd->setWndModal(false);
	delete pCurDlg;
	return 0;
}
int CSoarRoot::DoModal(ISoarWnd* hModelWnd)
{
	//却换当前激活sheet到模态sheet，先保存现场
	d_sheetStack.push_back(d_theActivateSheet);
	//创建一个模态
	ISoarSheet* pCur = new CSoarSheet(d_hOwner,true);
	pCur->setPrevFocusWnd(d_theActivateSheet->getFocusWnd() );
	d_theActivateSheet =pCur;
	//设置默认主窗口
	d_theActivateSheet->setMainWnd(hModelWnd);
	//设置默认对话框为激活窗口
	d_theActivateSheet->setFocusWnd(hModelWnd);
	DWORD dwTick =GetTickCount();
	DWORD dwTickprev =GetTickCount();
	MSG msg;
	msg.message=WM_NULL;
	// 主消息循环:
	if( msg.message !=WM_QUIT)//我们是异步模拟，所以不应该用while，我们还是要经过消息循环的，不应该阻塞
	{
		
		dwTick =GetTickCount();
		if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		if (CSoarRoot::getSingletonPtr())
		{
			CSoarRoot::getSingletonPtr()->RunSoarMessageSytem(); 
		}
		if (CSoarRoot::getSingletonPtr())
			{
				// Do some rendering
				d_DrawEngine->clearScene();
				d_DrawEngine->beginScene();
				d_DrawEngine->beginPatch();
				CSoarRoot::getSingletonPtr()->SoarUIRenderLoop(dwTick-dwTickprev);
				d_DrawEngine->endPatch();
				d_DrawEngine->endScene();
				d_DrawEngine->Present();
			}
		dwTickprev =dwTick ;
	}
	else{

	//恢复之前的激活窗口
	 ISoarWnd *focusWnd =d_theActivateSheet->getPrevFocusWnd();
	//恢复前一个激活的sheet
	ISoarSheet* pCurDlg = d_theActivateSheet;
	//恢复现场
	 ISoarSheet* pCur =d_sheetStack.back();
	 //恢复之前的激活窗口--可以省略因为每个Sheet都保留自己的堆栈
	d_sheetStack.pop_back();
	pCur->setFocusWnd(focusWnd);
	d_theActivateSheet =pCur;
	delete pCurDlg;
	}
	return 0;
}
int  CSoarRoot::DoModal(ISoarWnd* hModelWnd,ISoarLoopRunner* pGameLoop)
{
	//却换当前激活sheet到模态sheet，先保存现场
	d_sheetStack.push_back(d_theActivateSheet);
	//创建一个模态
	ISoarSheet* pCur = new CSoarSheet(d_hOwner,true);
	pCur->setPrevFocusWnd(d_theActivateSheet->getFocusWnd() );
	d_theActivateSheet =pCur;
	d_theActivateSheet->setMainWnd(hModelWnd);
	//设置默认对话框为激活窗口
	d_theActivateSheet->setFocusWnd(hModelWnd);
	DWORD dwTick =GetTickCount();
	DWORD dwTickprev =GetTickCount();
	MSG msg;
	msg.message=WM_NULL;
	// 主消息循环:
	while( msg.message !=WM_QUIT)
	{
		
		dwTick =GetTickCount();
		if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		if (CSoarRoot::getSingletonPtr())
		{
			CSoarRoot::getSingletonPtr()->RunSoarMessageSytem(); 
		}
		DWORD t =dwTick-dwTickprev;
		d_DrawEngine->clearScene();
		d_DrawEngine->beginScene();
		d_DrawEngine->beginPatch();
		if(pGameLoop)
				pGameLoop->RunLoop(t);
		if (CSoarRoot::getSingletonPtr())
		{
			// Do some rendering
			CSoarRoot::getSingletonPtr()->SoarUIRenderLoop(t);
		}
		d_DrawEngine->endPatch();
		d_DrawEngine->endScene();
		d_DrawEngine->Present();
		dwTickprev =dwTick ;
	}
	//恢复之前的激活窗口
	 ISoarWnd *focusWnd =d_theActivateSheet->getPrevFocusWnd();
	//恢复前一个激活的sheet
	ISoarSheet* pCurDlg = d_theActivateSheet;
	//恢复现场
	pCur =d_sheetStack.back();
	 //恢复之前的激活窗口--可以省略因为每个Sheet都保留自己的堆栈
	d_sheetStack.pop_back();
	pCur->setFocusWnd(focusWnd);
	d_theActivateSheet =pCur;
	delete pCurDlg;
	return 0;
}
bool CSoarRoot::CheckIsSoarMainWnd(ISoarWnd* pChkWnd)
{
	ISoarWnd* hactivateWnd=getSoarSheet();
	return pChkWnd == hactivateWnd;
}
////////////////////////////======================//////
//用户类绘制接口
void CSoarRoot::CustomDrawSelfT(const CLeeString& elementName,const CLeeString& elementStatus,const RECT& ControlRect,const CLeeString& text,int vertTextFmt,int horzTextFmt)
{
	RectF rcc=ControlRect;
	if (d_DrawEngine->hasWidgetlook(elementName))
	{
		d_DrawEngine->DrawWidgetlook(elementName,elementStatus,rcc,text,vertTextFmt,horzTextFmt);
	}
}
void CSoarRoot::CustomDrawSelf(const CLeeString& elementName,const CLeeString& elementStatus,const RECT& ControlRect)
{
	RectF rcc=ControlRect;
	if (d_DrawEngine->hasWidgetlook(elementName))
	{
		d_DrawEngine->DrawWidgetlookImage(elementName,elementStatus,rcc);
	}
}
bool CSoarRoot::CustomEnableAlphaBlend(bool bEnable){
	if(d_DrawEngine){
		return d_DrawEngine->EnableAlphaBlend(bEnable);
	}
	return false;
}
bool CSoarRoot::CustomGraphicGetAlphaBlendState(){
	if(d_DrawEngine){
		return d_DrawEngine->GetAlphaBlendState();
	}
	return false;
}
/************************************************************************/
/* 消息系统：                                                            */
/************************************************************************/
bool CSoarRoot::Register(UINT uMsg,ISoarWnd* handler,ISoarWnd* pOwner)
{
	return d_themsgCenter.Register(uMsg,handler,pOwner);
}
bool CSoarRoot::UnRegister(ISoarWnd* pOwner,UINT uMsg)
{
	return d_themsgCenter.UnRegister(pOwner,uMsg);
}
bool CSoarRoot::addOfflineMsg(SOARMSG &msg)
{
	return d_themsgCenter.addOfflineMsg(msg);
}
bool CSoarRoot::RunSoarMsg(MSG &msg,LRESULT& lr)
{
	if( d_theActivateSheet->getFocusWnd() )//激活的窗口处理
	{
		lr = d_themsgCenter.ExcuteDirect(msg,d_theActivateSheet->getFocusWnd() );
		return true;
	}
	return false;
}
bool CSoarRoot::RunTopSheetMsg(MSG &msg,LRESULT& lr)
{
	lr = d_themsgCenter.ExcuteDirect(msg,d_bindingSheet );
	return true;
}
void CSoarRoot::RunSoarMessageSytem(void)
{
	//处理离线信息
	d_themsgCenter.excuteoffline(this);
}
/************************************************************************/
/* UI_Engine mothod                                                     */
/************************************************************************/
void CSoarRoot::ChangTheme(CLeeString & szThemeName)
{
	if(d_DrawEngine)
	d_DrawEngine->engine_loadtheme(szThemeName);
}
///
INT CSoarRoot::translateMouseMessage(MSG &msg,bool bNCMsg)
{
	switch(msg.message)
	{
	case WM_LBUTTONDOWN:
		{
			if(bNCMsg)
				return SOAR_NCLCLICK_DOWN;
			return SOAR_LCLICK_DOWN;
		}
	case WM_LBUTTONUP:
		{
			if(bNCMsg)
				return SOAR_NCLCLICK_DOWN;
			return SOAR_LCLICK_UP;
		}
	case WM_LBUTTONDBLCLK:
		{
			if(bNCMsg)
				return SOAR_NCLDBCLICK;
		return SOAR_LDBCLICK;
		}
	case WM_RBUTTONDOWN:
		{
			if(bNCMsg)
				return SOAR_NCRCLICK_DOWN;
		return SOAR_RCLICK_DOWN;
		}
	case WM_RBUTTONUP:
		{
			if(bNCMsg)
				return SOAR_NCRCLICK_UP;
		return SOAR_RCLICK_UP;
		}
	case WM_RBUTTONDBLCLK:
		{
			if(bNCMsg)
				return SOAR_NCRDBCLICK;
		return SOAR_RDBCLICK;
		}
	case WM_MBUTTONDOWN:
		return SOAR_MCLICK_DOWN;
	case WM_MBUTTONUP:
		return SOAR_MCLICK_UP;
	case WM_MBUTTONDBLCLK:
		return SOAR_MDBCLICK;
	case WM_MOUSEWHEEL:
		return SOAR_XWHEEL;
	case WM_MOUSEMOVE:
		{
			if(bNCMsg)
				return SOAR_NCMOUSEMOVE;
		return SOAR_MOUSEMOVE;
		}
	default:
		return SOAR_START;

	}
}
ISoarWnd* CSoarRoot::createWnd(LWNDT classType,LWNDSTY lstyle)
{
	ISoarWnd * newWnd =NULL;
	switch(classType)
	{
	case LWNDT_SYS_SHEET:
		{
			newWnd =new CSoarBindSheetWnd(d_hOwner,d_DrawEngine);
			break;
		}
	case LWNDT_EDIT:
		{
			newWnd =new CLeeEditWnd(d_hOwner,d_DrawEngine);
			break;
		}
		case LWNDT_STATIC:
		{
			newWnd =new CLeeStaticWnd(d_hOwner,d_DrawEngine);
			break;
		}
	case LWNDT_BUTTON:
		{
			newWnd =new CLeeButtonWnd(d_hOwner,d_DrawEngine);
			break;
		}
	case LWNDT_RADIOBTN:
		{
			newWnd =new CLeeRadioWnd(d_hOwner,d_DrawEngine);
			break;
		}
	case LWNDT_CHECKBTN:
		{
			newWnd =new CLeeCheckButtonWnd(d_hOwner,d_DrawEngine);
			break;
		}
	case LWNDT_GROUPBOX:
		{
			newWnd =new CLeeGroupBox(d_hOwner,d_DrawEngine);
			break;
		}
		case LWNDT_INTERETWND:
		{
			newWnd =new CLeeInternetWnd(d_hOwner,d_DrawEngine);
			break;
		}
		case LWNDT_POPMENU:
		{
			newWnd =new CLeeMenuWnd(d_hOwner,d_DrawEngine);
			break;
		}
		
		case LWNDT_SYS_INFODIALOG:
		{
			newWnd =new CLeeInfoDialog(d_hOwner,d_DrawEngine);
			break;
		}
		case LWNDT_PROGRESSBAR:
		{
			if (lstyle & LWNDSTY_HORZWND)
			{
				newWnd =new CLeeHorzProgress(d_hOwner,d_DrawEngine);
			} 
			else
			{
				newWnd =new CLeeVertProgress(d_hOwner,d_DrawEngine);
			}
			
			break;
		}
		case LWNDT_LISTBOX:
		{
			newWnd =new CLeeListbox(d_hOwner,d_DrawEngine);
			break;
		}
		case LWNDT_COMBOBOX:
		{
			newWnd =new CLeeComboWnd(d_hOwner,d_DrawEngine);
			break;
		}
	
	
	case LWNDT_COMBOEX:
		{
			newWnd =new CLeeComboExWnd(d_hOwner,d_DrawEngine);
			break;
		}
		case LWNDT_POPLISTVIEW:
		{
			newWnd =new CLeePopupListViewWnd(d_hOwner,d_DrawEngine);
			break;
		}
		case LWNDT_TABCTRL:
		{
			newWnd =new CLeeTabWnd(d_hOwner,d_DrawEngine);
			break;
		}
	
		case LWNDT_LISTVIEWWND:
		{
			newWnd =new CLeeListViewWnd(d_hOwner,d_DrawEngine);
			break;
		}
		//////////////////////////////////////////////////////////////////////////CMM controls
	case LWNDT_CMM_FILEDIALOG:
		{
			newWnd =new CLeeFileDialogWnd(d_hOwner,d_DrawEngine);
			break;
		}
		case LWNDT_POPLISTBOX:
		{
			newWnd =new CLeePopupListbox(d_hOwner,d_DrawEngine);
			break;
		}
		case LWNDT_STAICITEMWND:
		{
			newWnd =new CLeeStaticItemWnd(d_hOwner,d_DrawEngine);
			break;
		}
		case LWNDT_SPINWND:
		{
			newWnd =new CLeeSpinWnd(d_hOwner,d_DrawEngine);
			break;
		}
		case LWNDT_SLIDERWND:
		{
			bool bVert =!(lstyle & LWNDSTY_HORZWND)?true:false;
			newWnd =new CSoarSliderWnd(d_hOwner,d_DrawEngine,bVert);
			break;
		}
	default:
		newWnd =new CSoarWnd(d_hOwner,d_DrawEngine);
	}
	if (newWnd)
	{
		newWnd->setWndStyle(lstyle);
	}
	return newWnd;
}