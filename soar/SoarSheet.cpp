#include "SoarSheet.h"
#include"SoarLayer.h"
#include "SoarRoot.h"

CSoarSheet::CSoarSheet(HWND root,bool bAutoDestroy):
d_rootWnd(root),d_bAutoDestroy(bAutoDestroy),d_sheetmainWnd(NULL),d_PrevSheetfocusWnd(NULL),d_SheetfocusWnd(NULL),
	d_baseZindex(0),d_zPopupIndexOld(0),d_zIndexOld(0),d_zIndexMax(1),d_popupzIndexMax(1),
	d_bModalState(false),d_baseTempZindex(0),d_bEnableAlphaBlend(false)//默认为普通层,
{
	d_popuplayers.clear();
	d_layers.clear();
}


CSoarSheet::~CSoarSheet(void)
{
	//销毁
	if(d_bAutoDestroy)
	{
		LayerMap::reverse_iterator it;
		it =d_popuplayers.rbegin();
		while (it != d_popuplayers.rend() )//popup
		{
			ISoarLayer* layer = it->second;
			layer->eraseAllWnd();
			++it ;
		}
		it =d_layers.rbegin();
		while (it != d_layers.rend() )//popup
		{
			ISoarLayer* layer = it->second;
			layer->eraseAllWnd();
			++it ;
		}
	}
	LayerMap::iterator it;
	it =d_popuplayers.begin();
	while (it != d_popuplayers.end() )//popup
	{
		ISoarLayer* layer = it->second;
		delete layer;
		it= d_popuplayers.erase(it) ;
	}
	it =d_layers.begin();
	while (it != d_layers.end() )//popup
	{
		ISoarLayer* layer = it->second;
		delete layer;
		it= d_layers.erase(it) ;
	}
	
}
ISoarWnd* CSoarSheet::getPrevFocusWnd(void)
{
	return d_PrevSheetfocusWnd;
}
ISoarWnd* CSoarSheet::setPrevFocusWnd(ISoarWnd* pFocus)
{
	ISoarWnd * tmp = d_PrevSheetfocusWnd;
	d_PrevSheetfocusWnd = pFocus;
	return tmp;
}
void CSoarSheet::setModalState(bool bModal){
	d_bModalState=bModal;
}
ISoarWnd* CSoarSheet::getFocusWnd(void)
{
	return d_SheetfocusWnd;
}
ISoarWnd* CSoarSheet::setMainWnd(ISoarWnd* mainW)
{
	ISoarWnd * ptmp =d_sheetmainWnd;
	d_sheetmainWnd =mainW;
	if(d_sheetmainWnd->isPopupWnd()){
		
		d_sheetmainWnd->setPopupZindex(d_basePopupZindex);
		d_zIndexOld=d_basePopupZindex;
	}
	else{
		d_sheetmainWnd->setZindex(d_baseZindex);
		d_zIndexOld=d_baseZindex;
	}
	
	return ptmp;
}
ISoarWnd* CSoarSheet::getMainWnd(void)
{
	return d_sheetmainWnd;
}
void CSoarSheet::addWndToZlayer(int zpos,ISoarWnd* pWnd){
	LayerMap::iterator it;
	if (pWnd->isPopupWnd() )//popup
	{
		pWnd->setPopupZindex(zpos);
		it =d_popuplayers.find(zpos);
		if(it != d_popuplayers.end())
		{
			ISoarLayer* layer = it->second;
			layer->addWnd(pWnd);
		}
		else//看看是否已经在其他图层
		{
			ISoarLayer* layer = new CSoarLayer(this);
			layer->addWnd(pWnd);
			d_popuplayers[zpos]=layer;
		}
	}
	else
	{
		pWnd->setZindex(zpos);
		it =d_layers.find(zpos);
		if(it != d_layers.end())
		{
			ISoarLayer* layer = it->second;
			layer->addWnd(pWnd);
		}
		else
		{
			it=d_layers.begin();
			while(it != d_layers.end() )
			{
				if(it->second->seekWnd(pWnd) )
				{	
					it->second->removeWnd(pWnd);
					break;
				}
				++it;
			}
			ISoarLayer* layer = new CSoarLayer(this);
			layer->addWnd(pWnd);
			d_layers[zpos]=layer;
		}

	}
}
void CSoarSheet::addWnd(ISoarWnd* newWnd) {
	int tIndex = 0;
	ISoarWnd* thisParent =newWnd->GetParent();
	//step 1:find if it 's paren in current sheet ,found that use it's parent index +1
	if (newWnd->isPopupWnd() )//popup
	{
		
		if(findWndfast(thisParent)&& thisParent->isPopupWnd()){
			tIndex= ensurePopupZindex(thisParent->getPopupZindex()+1);
		}
		else{
			tIndex=generatePopupZindex();
		}
	}
	else
	{
		if(findWndfast(thisParent)&&!thisParent->isPopupWnd() ){
			tIndex= ensureZindex(thisParent->getZindex()+1);
		}
		else{
			tIndex=generateZindex();
		}
	}
	addWndToZlayer(tIndex,newWnd);
}
void CSoarSheet::removeWnd(ISoarWnd* newWnd){
	LayerMap::iterator it;

	it=d_popuplayers.begin();
	while(it != d_popuplayers.end() )
	{
		if(it->second->seekWnd(newWnd) )
		{	
			it->second->removeWnd(newWnd);
			return;
		}
		++it;
	}
	
	it=d_layers.begin();
	while(it != d_layers.end() )
	{
		if(it->second->seekWnd(newWnd) )
		{	
			it->second->removeWnd(newWnd);
			break;
		}
		++it;
	}
}
void CSoarSheet::eraseWnd(ISoarWnd* pWnd){
	removeWnd(pWnd);
}
void CSoarSheet::addWndRecursive(ISoarWnd* newWnd)
{
	int tIndex = 0;
	ISoarWnd* thisParent =newWnd->GetParent();
	//step 1:find if it 's paren in current sheet ,found that use it's parent index +1
	if (newWnd->isPopupWnd() )//popup
	{
		
		if(findWndfast(thisParent)&& thisParent->isPopupWnd()){
			tIndex= ensurePopupZindex(newWnd->GetParent()->getPopupZindex()+1);
		}
		else{
			tIndex=generatePopupZindex();
		}
	}
	else
	{
		if(findWndfast(thisParent)&&!thisParent->isPopupWnd() ){
			tIndex= ensureZindex(thisParent->getZindex()+1);
		}
		else{
			tIndex=generateZindex();
		}
	}
	addWndToZlayer(tIndex,newWnd);
	//对它的子窗口进行添加处理
		LeeWndPtrMap &childmap=newWnd->getChidWndSet();
		LeeWndPtrMap::iterator itChild=childmap.begin();
		while(itChild != childmap.end()){
			addWndRecursive(itChild->second);
			++itChild;
		}
}
bool CSoarSheet::findWndfast(ISoarWnd* pWnd){
	if(pWnd==NULL)
		return false;
	if (pWnd->isPopupWnd() )//popup
	{
		LayerMap::iterator it =d_popuplayers.find(pWnd->getPopupZindex());
		if(it != d_popuplayers.end())
		{
			ISoarLayer* layer = it->second;
			return layer->seekWnd(pWnd);
		}
	}
	else
	{
		LayerMap::iterator it =d_popuplayers.find(pWnd->getZindex());
		if(it != d_popuplayers.end())
		{
			ISoarLayer* layer = it->second;
			return layer->seekWnd(pWnd);
		}

	}
	return false;
}
BOOL CSoarSheet::seekWnd(ISoarWnd* newWnd) 
{
	LayerMap::iterator it;

	it=d_popuplayers.begin();
	while(it != d_popuplayers.end() )
	{
		if(it->second->seekWnd(newWnd) )
		{	
			return true;
		}
		++it;
	}
	
	it=d_layers.begin();
	while(it != d_layers.end() )
	{
		if(it->second->seekWnd(newWnd) )
		{	
			return true;
		}
		++it;
	}
	
	return false;
}
bool CSoarSheet::seekWnd(const CLeeString & name)
{
	LayerMap::iterator it;
	it=d_popuplayers.begin();
	while(it != d_popuplayers.end() )
	{
		if(it->second->seekWnd(name) )
		{	
			return true;
		}
		++it;
	}
	it=d_layers.begin();
	while(it != d_layers.end() )
	{
		if(it->second->seekWnd(name) )
		{	
			return true;
		}
		++it;
	}
	
	return false;
}
ISoarWnd* CSoarSheet::getWnd(const CLeeString & name){
	LayerMap::iterator it;

	it=d_popuplayers.begin();
	ISoarWnd* pFound =NULL;
	while(it != d_popuplayers.end() )
	{
		pFound=it->second->getWnd(name);
		if(pFound )
		{	
			return pFound;
		}
		++it;
	}
	it=d_layers.begin();
	while(it != d_layers.end() )
	{
		pFound=it->second->getWnd(name);
		if(pFound )
		{	
			return pFound;
		}
		++it;
	}
	
	return NULL;
}
void CSoarSheet::removeWndRecursive(ISoarWnd* newWnd) 
{
	LayerMap::iterator it;

	it=d_popuplayers.begin();
	while(it != d_popuplayers.end() )
	{
		if(it->second->seekWnd(newWnd) )
		{	
			it->second->removeWnd(newWnd);
			goto doremovechild;
		}
		++it;
	}
	
	it=d_layers.begin();
	while(it != d_layers.end() )
	{
		if(it->second->seekWnd(newWnd) )
		{	
			it->second->removeWnd(newWnd);
			break;
		}
		++it;
	}
	doremovechild:
	//对它的子窗口进行添加处理
		LeeWndPtrMap &childmap=newWnd->getChidWndSet();
		LeeWndPtrMap::iterator itChild=childmap.begin();
		while(itChild != childmap.end()){
			removeWndRecursive(itChild->second);
			++itChild;
		}
}
void CSoarSheet::eraseWndRecursive(ISoarWnd* newWnd) 
{
	//当前窗口是否为焦点窗口
	if (d_SheetfocusWnd == newWnd)
	{
		setFocusWnd(d_sheetmainWnd);
	}
	removeWndRecursive(newWnd);
}
void CSoarSheet::clearAllLinkWnd(){
	LayerMap::iterator it;
	it =d_popuplayers.begin();
	while (it != d_popuplayers.end() )//popup
	{
		ISoarLayer* layer = it->second;
		layer->eraseAllWnd();
		++it;
	}
	it =d_layers.begin();
	while (it != d_layers.end() )//popup
	{
		ISoarLayer* layer = it->second;
		layer->eraseAllWnd();
		++it;
	}
}
BOOL CSoarSheet::SoarUIMsgProc ( UINT uMsg,WPARAM wParam ,LPARAM lParam,LRESULT& lr )
{
	//处理WM_PAINT消息
	if (filter_PaintMessage(uMsg,wParam,lParam) )
	{
		lr = 0;
		return TRUE;
	}
	//处理鼠标移动信息更新UI-已经移出到渲染流水线中
	///filter_MouseMoveUI(uMsg,wParam,lParam);
	
	//激活窗口等点击消息处理(预处理)
	if( filter_ClickMsg(uMsg,wParam,lParam,lr) )
	{
		return TRUE;
	}
	//让激活的窗口处理信息
	if( filter_allMessage(uMsg,wParam,lParam,lr) )
	{
		return TRUE;
	}
	return false;
}
bool CSoarSheet::filter_ClickMsg(UINT uMsg ,WPARAM wParam,LPARAM lParam,LRESULT& lr)
{
	UINT tempMsg = uMsg;
#ifdef _WIN64
	if (uMsg == WM_SETCURSOR)
	{
		tempMsg = HIWORD(lParam);
	}
#endif
	if (tempMsg < WM_MOUSEFIRST )
	 {
		 return false;
	 }
	if (tempMsg >WM_MOUSELAST)
	{
		return false;
	}
#ifdef _WIN64
	POINT pt;
	::GetCursorPos(&pt);
	::ScreenToClient(CSoarRoot::getSingletonPtr()->getMsWindow(), &pt);
	lParam = MAKELPARAM(pt.x,pt.y);
#endif
	MSG msg={CSoarRoot::getSingletonPtr()->getMsWindow(),tempMsg ,wParam,lParam};
	  //焦点切换
	if(tempMsg ==WM_LBUTTONDOWN ||
		tempMsg ==WM_LBUTTONDBLCLK||
		tempMsg == WM_RBUTTONDOWN ||
		tempMsg == WM_RBUTTONDBLCLK ||
		tempMsg ==WM_XBUTTONDOWN ||
		tempMsg == WM_XBUTTONDBLCLK )
	{ 
		handlerClickMsg(msg);
      //发送消息
	 fireUIEvent_COMMAND(0.0,msg);
	}
	if(tempMsg ==WM_LBUTTONUP ||
		tempMsg == WM_RBUTTONUP ||
		tempMsg == WM_MBUTTONUP ||
		tempMsg == WM_XBUTTONUP )
	{ 
      //发送消息
	  fireUIEvent_COMMAND(0.0,msg);
	}
	//让激活窗口处理消息
	CSoarRoot::getSingletonPtr()->RunSoarMsg( msg ,lr);
	return true;
}
bool CSoarSheet::handlerClickMsg(MSG msg)
{
	POINT pt;
	::GetCursorPos(&pt);
	::ScreenToClient(CSoarRoot::getSingletonPtr()->getMsWindow(),&pt);
	LayerMap::reverse_iterator it =d_layers.rbegin();
	//优先权限
	it =d_popuplayers.rbegin();
	while( it != d_popuplayers.rend() )
	{
		if( it->second->handlerClickMsg(msg,pt))
			return true;
		++it;
	}
	it =d_layers.rbegin();
	while( it != d_layers.rend() )
	{
		if( it->second->handlerClickMsg(msg,pt))
			return true;
		++it;
	}
	return false;
}
bool CSoarSheet::filter_MouseMoveUI(UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	LayerMap::reverse_iterator it =d_popuplayers.rbegin();
	
	while( it != d_popuplayers.rend() )
	{
		 it->second->filter_MouseMoveUI( uMsg , wParam , lParam ) ;
		++it;
	}
	it =d_layers.rbegin();
	while( it != d_layers.rend() )
	{
		 it->second->filter_MouseMoveUI( uMsg , wParam , lParam ) ;
		++it;
	}
	return false;
}
bool CSoarSheet::filter_PaintMessage(UINT uMessage,WPARAM wParam,LPARAM lParam)
 {
	LayerMap::reverse_iterator it =d_popuplayers.rbegin();
	while( it != d_popuplayers.rend() )
	{
		it->second->filter_PaintMessage( uMessage , wParam , lParam ) ;
		++it;
	}
	it =d_layers.rbegin();
	while( it != d_layers.rend() )
	{
		it->second->filter_PaintMessage( uMessage , wParam , lParam ) ;
		++it;
	}
	return false;
 }
 //直接让ROOT运行即可，意味它只给激活的窗口运行而已
bool CSoarSheet::filter_allMessage(UINT uMsg,WPARAM wParam,LPARAM lParam,LRESULT &lr)
{
	MSG msg={CSoarRoot::getSingletonPtr()->getMsWindow(),uMsg ,wParam,lParam};
	//让激活窗口处理消息
	return CSoarRoot::getSingletonPtr()->RunSoarMsg( msg ,lr);
}
//窗口层次改变
BOOL CSoarSheet::WndZorderChanged(ISoarWnd* FocusWnd,int newFocusZpos,int oldZindex)
{
	//先找到之前的层，从中移除
	if( newFocusZpos==oldZindex)
		return false;
	int tIndex = oldZindex;
	LayerMap::iterator it;
	if (FocusWnd &&FocusWnd->isPopupWnd() )//popup
	{
		it =d_popuplayers.find(tIndex);
		if(it != d_popuplayers.end())
		{
			ISoarLayer* layer = it->second;
			layer->removeWnd(FocusWnd);
		}
		else
		{
			it=d_popuplayers.begin();
			while(it != d_popuplayers.end() )
			{
				if(it->second->seekWnd(FocusWnd) )
				{	
					it->second->removeWnd(FocusWnd);
				}
				++it;
			}
		}
	}
	else
	{
			it =d_layers.find(tIndex);
			if(it != d_layers.end())
			{
				ISoarLayer* layer = it->second;
				layer->removeWnd(FocusWnd);
			}else
			{
				it=d_layers.begin();
				while(it != d_layers.end() )
				{
					if(it->second->seekWnd(FocusWnd) )
					{	
						it->second->removeWnd(FocusWnd);
					}
					++it;
				}
			}
	}
	//增加到新层
	 tIndex = newFocusZpos;
	if (FocusWnd &&FocusWnd->isPopupWnd() )//popup
	{
		it =d_popuplayers.find(tIndex);
		if(it != d_popuplayers.end())
		{
			ISoarLayer* layer = it->second;
			layer->addWnd(FocusWnd);
		}
		else
		{
			ISoarLayer* layer = new CSoarLayer(this);
				layer->addWnd(FocusWnd);
				d_popuplayers[tIndex]=layer;
		}
	}
	else
	{
			it =d_layers.find(tIndex);
			if(it != d_layers.end())
			{
				ISoarLayer* layer = it->second;
				layer->addWnd(FocusWnd);
			}
			else{
				ISoarLayer* layer = new CSoarLayer(this);
				layer->addWnd(FocusWnd);
				d_layers[tIndex]=layer;
			}
	}
	return true;
}
	//窗口样式改变
BOOL CSoarSheet::WndStyleChanged(ISoarWnd* newWnd) 
{
	return true;
}
bool CSoarSheet::RenderLoop(ILeeDrawInterface *DrawFuns,float &t)
{
	//方案 一：由主窗口渲染包含子窗口：Z顺序的准确性需要重新排序
	//方案二：各自渲染，效率高，但是要依托Z顺序的准确性
	LayerMap::iterator it;
	it =d_layers.begin();
	while (it != d_layers.end() )//popup
	{
		ISoarLayer* layer = it->second;
		if(layer)
		layer->RenderLoop(DrawFuns,t);
		++it;
	}
	it =d_popuplayers.begin();
	while (it != d_popuplayers.end() )//popup
	{
		ISoarLayer* layer = it->second;
		if(layer)
		layer->RenderLoop(DrawFuns,t);
		++it;
	}
	return true;
}
 bool CSoarSheet::enableAlphaBlend(bool bEnable)
 {
	 if(d_bEnableAlphaBlend != bEnable){
		 d_bEnableAlphaBlend =bEnable;
		 return true;
	 }
	 return false;
 }
/************************************************************************/
/* pFocus/d_focusWnd 都可能为NULL                                       */
/************************************************************************/
ISoarWnd* CSoarSheet::setFocusWnd(ISoarWnd* pFocus)
{
	//没有改变，直接返回
	if(pFocus == d_SheetfocusWnd)
	{
		//发送激活消息
		fireUIEvent_Focus(NULL);
		return pFocus;
	}
	ISoarWnd * d_prevFocus =d_SheetfocusWnd;
	d_SheetfocusWnd =pFocus;//登记激活该窗口
	//恢复原来层次(底板和Sheet的MainWnd不用设置)
	if (d_prevFocus && 
		d_prevFocus!=d_SheetfocusWnd && 
		(d_prevFocus != d_sheetmainWnd )//&& //失去焦点的不是底板
		///d_SheetfocusWnd !=d_sheetmainWnd //得到焦点的不是底板
		)
	{
		if (d_prevFocus->isPopupWnd() )//popup
		{
			//如有激活的sheet 设置相应的layer层次
			//d_prevFocus->setPopupZindex(d_zPopupIndexOld);
			degradeLevelWnd(d_prevFocus,d_zIndexOld);
		}
		else
		{
			//如有激活的sheet 设置相应的layer层次
			//d_prevFocus->setZindex(d_zIndexOld);
			degradeLevelWnd(d_prevFocus,d_zIndexOld);
		}
		
	}
	//为焦点窗口提升层次
	if (d_SheetfocusWnd && 
		(d_SheetfocusWnd != d_sheetmainWnd )
		)
	{
		bringWndToTopLevel(d_SheetfocusWnd);
		//if (d_SheetfocusWnd->isPopupWnd() )//popup
		//{
		//	//如有激活的sheet 设置相应的layer层次
		//	d_zPopupIndexOld=d_SheetfocusWnd->setPopupZindex(d_popupzIndexMax);
		//	
		//}
		//else
		//{
		//	//如有激活的sheet 设置相应的layer层次
		//	d_zIndexOld=d_SheetfocusWnd->setZindex(d_zIndexMax);
		//}
	}
	//发送激活消息
	fireUIEvent_Focus(d_prevFocus);
	return d_prevFocus;
}


//////////////////////////////////////////////////////////////////////////
/*
msg.message==WM_LBUTTONDOWN ||msg.message==WM_LBUTTONDBLCLK||
msg.message == WM_RBUTTONDOWN ||
msg.message == WM_RBUTTONDBLCLK
*/
//////////////////////////////////////////////////////////////////////////
bool CSoarSheet::fireUIEvent_Focus(const ISoarWnd * d_prevFocus)
{
	//向底板发生激活信息,高亮显示
	if ( d_SheetfocusWnd != d_prevFocus)
	{
		//发送激活消息到激活窗口
		if (d_SheetfocusWnd)
		{
			SOARMSG leeMsg;
			leeMsg.message =SOAR_SETFOCUS;
			leeMsg.mouseEvent =0;
			leeMsg.sourceWnd =(ISoarWnd*)d_prevFocus;
			leeMsg.targetWnd =d_SheetfocusWnd;
			leeMsg.routeWnd=NULL;
			leeMsg.wParam =0;
			leeMsg.lParam =0;
			CSoarRoot::getSingletonPtr()->addOfflineMsg(leeMsg);
		}
		//发送失去焦点消息到非激活窗口
		if (d_prevFocus)
		{
			SOARMSG leeMsg;
			leeMsg.message =SOAR_LOSTFOCUS;
			leeMsg.mouseEvent =0;
			leeMsg.sourceWnd =d_SheetfocusWnd;
			leeMsg.targetWnd =(ISoarWnd*)d_prevFocus;
			leeMsg.routeWnd=NULL;
			leeMsg.wParam =0;
			leeMsg.lParam =0;
			CSoarRoot::getSingletonPtr()->addOfflineMsg(leeMsg);
		}
		return true;
	}
	return false;
}
//处理焦点切换，自动发送SOAR_COM至各个窗口（留在ROOT层）
bool CSoarSheet::fireUIEvent_COMMAND(float t,MSG msg)
{
	//给父窗口发送FYLD_Command消息：WM_LBUTTONDOWN WM_LBUTTONUP等
    if (msg.message !=WM_MOUSEMOVE &&
		msg.message>= WM_MOUSEFIRST && 
		msg.message <= WM_MOUSELAST )
    {
		//顶层窗口不需发
		if (d_SheetfocusWnd &&d_SheetfocusWnd->GetParent())
		{
			bool bNCMsg=false;
			POINT pt;
			::GetCursorPos(&pt);
			::ScreenToClient(CSoarRoot::getSingletonPtr()->getMsWindow(),&pt);
			RECT rcClient =d_SheetfocusWnd->getClientRect();
			if(!::PtInRect(&rcClient,pt))
			{
				bNCMsg=true;
			}
			SOARMSG leeMsg ;
		    leeMsg.sourceWnd =d_SheetfocusWnd;
		    leeMsg.targetWnd =d_SheetfocusWnd->GetParent();
			leeMsg.routeWnd =d_SheetfocusWnd;
		    leeMsg.message =SOAR_COMMAND ;
		    leeMsg.mouseEvent=CSoarRoot::getSingletonPtr()->translateMouseMessage(msg,bNCMsg);
		    leeMsg.wParam =msg.wParam;
		    leeMsg.lParam = msg.lParam;
			leeMsg.Data=0;
		   CSoarRoot::getSingletonPtr()->addOfflineMsg(leeMsg);
		}
    }
	return false;
}
//Z管理
long CSoarSheet::generateZindex()
{
	d_zIndexMax = d_zIndexMax +1;
	return d_zIndexMax;
}
long CSoarSheet::generatePopupZindex()
{
	d_popupzIndexMax = d_popupzIndexMax +1;
	return d_popupzIndexMax;
}
long CSoarSheet::ensureZindex(int nwantedZindex){
	if(d_zIndexMax>nwantedZindex){
		return nwantedZindex;
	}
	return generateZindex();
}
long CSoarSheet::ensurePopupZindex(int nwantedZindex){
	if(d_popupzIndexMax>nwantedZindex){
		return nwantedZindex;
	}
	return generatePopupZindex();
}
void CSoarSheet::setStartZindex(long iStart){
	d_zIndexMax = iStart+1;
	d_baseZindex=iStart;
}
void CSoarSheet::setStartPopupZindex(long iStart){
	d_popupzIndexMax = iStart +1;
	d_basePopupZindex=iStart;
}
long CSoarSheet::getMaxZindex(){
	return d_zIndexMax;
}
long CSoarSheet::getMaxPopupZindex(){
	return d_popupzIndexMax;
}
long CSoarSheet::bringWndToTopLevel(ISoarWnd* pFocusWnd)
{
	//父窗口提升至N
	ISoarWnd * pParent = FindTopLevelParent(pFocusWnd);
	//父窗口是底板不提升
	if(pParent==d_sheetmainWnd){
		return 0;
	}
	int topmax = d_baseTempZindex;
	if(pParent){
		if (d_SheetfocusWnd->isPopupWnd() )//popup
		{
			//d_zPopupIndexOld=pParent->setPopupZindex(d_popupzIndexMax);
		 d_baseTempZindex=d_popupzIndexMax;
		}
		else{
			//d_zIndexOld=pParent->setZindex(d_zIndexMax);
			 d_baseTempZindex=d_zIndexMax;
		}
		topmax=topmax>d_baseTempZindex?topmax:d_baseTempZindex;
	}
	//提升当前窗口至N+1，所有子窗口默认提升相应
	
	//对它的子窗口进行添加处理
	if (pParent->isPopupWnd() )//popup
	{
		d_zIndexOld=pParent->getPopupZindex();
		if(d_popupzIndexMax<=pParent->getPopupZindex()){
			d_baseTempZindex=pParent->getPopupZindex();
			
			return 0;
		}
	}
	else{
		d_zIndexOld=pParent->getZindex();
		if(topmax<=pParent->getZindex()){
			d_baseTempZindex=pParent->getZindex();
			
			return 0;
		}
	}
		modifyTempZindexWnd(pParent,false);
	return 0;
}
long CSoarSheet::degradeLevelWnd(ISoarWnd* pFocusWnd,long targetlevel){
	//父窗口提升至N
	ISoarWnd * pParent =FindTopLevelParent(pFocusWnd);
	//父窗口是底板不提升不降低
	if(pParent==d_sheetmainWnd){
		return 0;
	}
	
	//对它的窗口进行添加处理
	if (pParent->isPopupWnd() )//popup
	{
		if(d_zIndexOld>=pParent->getPopupZindex()){
			return 0;
		}
		d_baseTemppopupZindex=targetlevel;
	}
	else{
		if(d_zIndexOld>=pParent->getZindex()){
			return 0;
		}
		d_baseTempZindex=targetlevel;
	}
	
	modifyTempZindexWnd(pParent,true);
		
	return 0;
}
long CSoarSheet::generateTempZindex()
{
	d_baseTempZindex = d_baseTempZindex +1;
	return d_baseTempZindex;
}
ISoarWnd* CSoarSheet::FindTopLevelParent(ISoarWnd* pChildWnd){
	if(pChildWnd->getWndStyle() & LWNDSTY_POPUP){
			return pChildWnd;
		}
	if(pChildWnd==d_sheetmainWnd ){
		return d_sheetmainWnd;
		}
	ISoarWnd * pParent =pChildWnd->GetParent();
	//父窗口是底板不提升不降低
	while(pParent){
		if(pParent==d_sheetmainWnd ){
		return d_sheetmainWnd;
		}
		if(pParent->getWndStyle() & LWNDSTY_POPUP){
			return pParent;
		}
		//不是
	 pParent =pParent->GetParent();
	}
	if(pParent ==NULL){
		return d_sheetmainWnd;
	}
}
/*
基于只有popup创建的子窗口都是popup，如果父窗口是popup，子是普通的（不可能的）
*/
void CSoarSheet::modifyTempZindexWnd(ISoarWnd* newWnd,bool isDownlevel){
	int tIndex = 0;
	tIndex = d_baseTempZindex;
	LayerMap::iterator it;
	if (newWnd->isPopupWnd() )//popup
	{
		
		//从其他图层删除
		it=d_popuplayers.begin();
			while(it != d_popuplayers.end() )
			{
				if(it->second->seekWnd(newWnd) )
				{	
					it->second->removeWnd(newWnd);
					break;
				}
				++it;
			}
		//设置Zindex
			newWnd->setPopupZindex(tIndex);
			it =d_popuplayers.find(tIndex);
			if(it != d_popuplayers.end())
			{
				ISoarLayer* layer = it->second;
				layer->addWnd(newWnd);
			}
			else//看看是否已经在其他图层
			{
			
				ISoarLayer* layer = new CSoarLayer(this);
				layer->addWnd(newWnd);
				d_popuplayers[tIndex]=layer;
			}
	}
	else
	{
		//从其他图层删除
		it=d_layers.begin();
		while(it != d_layers.end() )
		{
			if(it->second->seekWnd(newWnd) )
			{	
				it->second->removeWnd(newWnd);
				break;
			}
			++it;
		}
		//设置Zindex
		newWnd->setZindex(tIndex);
		it =d_layers.find(tIndex);
		if(it != d_layers.end())
		{
			ISoarLayer* layer = it->second;
			layer->addWnd(newWnd);
		}
		else
		{
			
			ISoarLayer* layer = new CSoarLayer(this);
			layer->addWnd(newWnd);
			d_layers[tIndex]=layer;
		}

	}
	//对它的子窗口进行添加处理
		LeeWndPtrMap &childmap=newWnd->getChidWndSet();
		LeeWndPtrMap::iterator itChild=childmap.begin();
		while(itChild != childmap.end()){
			modifyTempZindexWndRecursive(itChild->second,isDownlevel);
			++itChild;
		}
}
/*
如果parent是普通窗口的话，不要处理popup的顺序
如过是popup的话，要全部处理
*/
void CSoarSheet::modifyTempZindexWndRecursive(ISoarWnd* newWnd,bool isDownlevel){
	int tIndex = 0;
	ISoarWnd* thisParent =newWnd->GetParent();
	//step 1:find if it 's paren in current sheet ,found that use it's parent index +1
	if ( thisParent->isPopupWnd() )//popup
	{
		if(findWndfast(thisParent) && newWnd->isPopupWnd())
		{//同类型
			//if down leve
			if(isDownlevel)
			{
				tIndex = thisParent->getPopupZindex()+1;
			}
			else
			{
				tIndex = thisParent->getPopupZindex()+1;
				if(d_baseTempZindex<tIndex)
				{
					//产生新的层
					d_baseTempZindex++;
					tIndex =d_baseTempZindex;
				}
			}
			removeWnd(newWnd);
			addWndToZlayer(tIndex,newWnd);
			
		}
		else if(newWnd->isPopupWnd())
		{
			if(isDownlevel)
			{
				tIndex = d_baseTempZindex;
			}
			else{
				tIndex = d_baseTempZindex+1;
				if(d_baseTempZindex<tIndex)
				{
					//产生新的层
					d_baseTempZindex++;
					tIndex =d_baseTempZindex;
				}
			}
			removeWnd(newWnd);
			addWndToZlayer(tIndex,newWnd);
		}
		else
		{//must be some error 
			;
		}
		
	}
	else
	{
		if(!newWnd->isPopupWnd())
		{ //只处理非弹出窗口
			if(findWndfast(thisParent))
			{///找到父窗口
				if(isDownlevel)
				{
					tIndex = thisParent->getZindex()+1;
				}
				else
				{
					tIndex = thisParent->getZindex()+1;
					if(d_baseTempZindex<tIndex)
					{
						//产生新的层
						d_baseTempZindex++;
						tIndex =d_baseTempZindex;
					}
				}
			}
			else{
				if(isDownlevel)
				{
				tIndex = d_baseTempZindex;
				}
				else
				{
					d_baseTempZindex++;
					tIndex = d_baseTempZindex;
				}
			}
			removeWnd(newWnd);
			addWndToZlayer(tIndex,newWnd);
		}
		else 
		{
			;
		}
		
	}
	
	//对它的子窗口进行添加处理
		LeeWndPtrMap &childmap=newWnd->getChidWndSet();
		LeeWndPtrMap::iterator itChild=childmap.begin();
		while(itChild != childmap.end()){
			modifyTempZindexWndRecursive(itChild->second,isDownlevel);
			++itChild;
		}
}
